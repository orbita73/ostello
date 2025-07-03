#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "../include/room.h"
#include "../include/display_options.h"
#include "../include/auth.h"

#define MAX_LEN 100
#define DATA_DIR "./data"
#define ROOM_DIR "./data/rooms"

typedef struct {
    int id;
    char userId[MAX_LEN];
    char first_name[MAX_LEN];
    char last_name[MAX_LEN];
    char dob[MAX_LEN];
    char institute[MAX_LEN];
    char nid[MAX_LEN];
    char birth_certificate[MAX_LEN];
} Member;

typedef struct {
    int building_num;
    char name[MAX_LEN];
    char address[MAX_LEN];
} Building;

typedef struct {
    int floor_num;
    char description[MAX_LEN];
} Floor;

typedef struct {
    int flat_num;
    char flat_name[MAX_LEN];
    int is_ac_available;
} Flat;

typedef struct {
    int seat_num;
    int status;
} Seat;

typedef struct {
    int seat_num;
    char occupant_name[MAX_LEN];
    char occupant_id[MAX_LEN];
    char occupant_contact[MAX_LEN];
    char allocation_date[20];
} Allocation;

void ensureDataDirectories()
{
    struct stat st = {0};
    if (stat(DATA_DIR, &st) == -1) mkdir(DATA_DIR, 0700);
    if (stat(ROOM_DIR, &st) == -1) mkdir(ROOM_DIR, 0700);
}

static void readLine(char *buffer, int size)
{
    if (fgets(buffer, size, stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;
    }
}

static void getCurrentDate(char *buffer, int size)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buffer, size, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

void saveBuilding(Building *b)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/building.txt", ROOM_DIR);
    FILE *fp = fopen(filename, "a");
    if (!fp) {
        printf("Error opening building.txt for writing.\n");
        return;
    }
    fprintf(fp, "%d,%s,%s\n", b->building_num, b->name, b->address);
    fclose(fp);

    snprintf(filename, sizeof(filename), "%s/building_%d.txt", ROOM_DIR, b->building_num);
    fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening building details file.\n");
        return;
    }
    fprintf(fp, "Building Number: %d\nName: %s\nAddress: %s\n", b->building_num, b->name, b->address);
    fclose(fp);
}

void saveFloors(int building_num, Floor *floors, int count)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/floor_building_%d.txt", ROOM_DIR, building_num);
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening floor file for writing.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s\n", floors[i].floor_num, floors[i].description);
    }
    fclose(fp);
}

void saveFlats(int building_num, int floor_num, Flat *flats, int count)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/flat_building_%d_floor_%d.txt", ROOM_DIR, building_num, floor_num);
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening flat file for writing.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%d\n", flats[i].flat_num, flats[i].flat_name, flats[i].is_ac_available);
    }
    fclose(fp);
}

void saveSeats(int building_num, int floor_num, int flat_num, Seat *seats, int count)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/seats_building_%d_floor_%d_flat_%d.txt", ROOM_DIR, building_num, floor_num, flat_num);
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening seats file for writing.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%d\n", seats[i].seat_num, seats[i].status);
    }
    fclose(fp);
}

void viewBuildings()
{
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/building.txt", ROOM_DIR);
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("No buildings found.\n");
        return;
    }
    printf("\nBuildings:\n");
    printf("----------------------------------------------------------\n");
    printf("%-10s | %-20s | %-25s\n", "Building#", "Name", "Address");
    printf("----------------------------------------------------------\n");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int num;
        char name[MAX_LEN], addr[MAX_LEN];
        if (sscanf(line, "%d,%[^,],%[^\n]", &num, name, addr) == 3) {
            printf("%-10d | %-20s | %-25s\n", num, name, addr);
        }
    }
    fclose(fp);
}

void viewFloors(int building_num)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/floor_building_%d.txt", ROOM_DIR, building_num);
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("No floors found for building %d.\n", building_num);
        return;
    }
    printf("\nFloors in Building %d:\n", building_num);
    printf("-----------------------------------------\n");
    printf("%-10s | %-30s\n", "Floor#", "Description");
    printf("-----------------------------------------\n");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int floor_num;
        char desc[MAX_LEN];
        if (sscanf(line, "%d,%[^\n]", &floor_num, desc) == 2) {
            printf("%-10d | %-30s\n", floor_num, desc);
        }
    }
    fclose(fp);
}

void viewFlats(int building_num, int floor_num)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/flat_building_%d_floor_%d.txt", ROOM_DIR, building_num, floor_num);
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("No flats found for building %d floor %d.\n", building_num, floor_num);
        return;
    }
    printf("\nFlats in Building %d Floor %d:\n", building_num, floor_num);
    printf("---------------------------------------------\n");
    printf("%-10s | %-20s | %-15s\n", "Flat#", "Flat Name", "AC Available");
    printf("---------------------------------------------\n");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int flat_num, is_ac_available;
        char flat_name[MAX_LEN];
        if (sscanf(line, "%d,%[^,],%d", &flat_num, flat_name, &is_ac_available) == 3) {
            printf("%-10d | %-20s | %-15s\n", flat_num, flat_name, is_ac_available ? "Yes" : "No");
        }
    }
    fclose(fp);
}

void viewSeats(int building_num, int floor_num, int flat_num)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/seats_building_%d_floor_%d_flat_%d.txt", ROOM_DIR, building_num, floor_num, flat_num);
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("No seats found for building %d floor %d flat %d.\n", building_num, floor_num, flat_num);
        return;
    }
    printf("\nSeats in Building %d Floor %d Flat %d:\n", building_num, floor_num, flat_num);
    printf("---------------------------------------------\n");
    printf("%-10s | %-10s\n", "Seat#", "Status");
    printf("---------------------------------------------\n");
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int seat_num, status;
        if (sscanf(line, "%d,%d", &seat_num, &status) == 2) {
            printf("%-10d | %-10s\n", seat_num, status ? "Available" : "Occupied");
        }
    }
    fclose(fp);
}

int isValidProfileId(const char *id)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/profile.txt", DATA_DIR);
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Could not open profile.txt\n");
        return 0;
    }
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        int member_id;
        if (sscanf(line, "%d,", &member_id) == 1) {
            char member_id_str[20];
            sprintf(member_id_str, "%d", member_id);
            if (strcmp(member_id_str, id) == 0) {
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);
    return 0;
}

int getMemberNameById(const char *id, char *name_out)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/profile.txt", DATA_DIR);
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        int member_id;
        char userId[MAX_LEN], first_name[MAX_LEN], last_name[MAX_LEN];
        if (sscanf(line, "%d,%[^,],%[^,],%[^,],", &member_id, userId, first_name, last_name) == 4) {
            char member_id_str[20];
            sprintf(member_id_str, "%d", member_id);
            if (strcmp(member_id_str, id) == 0) {
                snprintf(name_out, MAX_LEN, "%s %s", first_name, last_name);
                fclose(fp);
                return 1;
            }
        }
    }
    fclose(fp);
    return 0;
}

void allocateSeat(int building_num, int floor_num, int flat_num, int seat_num)
{
    char seatFile[256];
    snprintf(seatFile, sizeof(seatFile), "%s/seats_building_%d_floor_%d_flat_%d.txt", ROOM_DIR, building_num, floor_num, flat_num);
    FILE *fpSeat = fopen(seatFile, "r");
    if (!fpSeat) {
        printf("Seat file not found.\n");
        return;
    }
    Seat seats[100];
    int count = 0;
    char line[256];
    int found = 0, seat_index = -1;
    while (fgets(line, sizeof(line), fpSeat)) {
        int sNum, sStatus;
        if (sscanf(line, "%d,%d", &sNum, &sStatus) == 2) {
            seats[count].seat_num = sNum;
            seats[count].status = sStatus;
            if (sNum == seat_num) {
                found = 1;
                seat_index = count;
            }
            count++;
        }
    }
    fclose(fpSeat);
    if (!found) {
        printf("Seat %d not found.\n", seat_num);
        return;
    }
    if (seats[seat_index].status != 1) {
        printf("Seat %d is not available.\n", seat_num);
        return;
    }
    Allocation alloc;
    alloc.seat_num = seat_num;
    while (1) {
        printf("Enter occupant ID (profile id): ");
        readLine(alloc.occupant_id, MAX_LEN);
        if (isValidProfileId(alloc.occupant_id)) {
            break;
        } else {
            printf("Invalid profile ID! Please enter a valid profile id from profile.txt.\n");
        }
    }
    if (getMemberNameById(alloc.occupant_id, alloc.occupant_name)) {
        printf("Occupant name auto-filled: %s\n", alloc.occupant_name);
    } else {
        printf("Error: Could not fetch name for the given profile id.\n");
        return;
    }
    printf("Enter occupant contact: ");
    readLine(alloc.occupant_contact, MAX_LEN);
    getCurrentDate(alloc.allocation_date, sizeof(alloc.allocation_date));
    char allocFile[256];
    snprintf(allocFile, sizeof(allocFile), "%s/allocation_building_%d_floor_%d_flat_%d.txt", ROOM_DIR, building_num, floor_num, flat_num);
    FILE *fpAlloc = fopen(allocFile, "a");
    if (!fpAlloc) {
        printf("Error opening allocation file.\n");
        return;
    }
    fprintf(fpAlloc, "%d,%s,%s,%s,%s\n", alloc.seat_num, alloc.occupant_name, alloc.occupant_id, alloc.occupant_contact, alloc.allocation_date);
    fclose(fpAlloc);
    seats[seat_index].status = 0;
    fpSeat = fopen(seatFile, "w");
    if (!fpSeat) {
        printf("Error updating seat file.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fpSeat, "%d,%d\n", seats[i].seat_num, seats[i].status);
    }
    fclose(fpSeat);
    printf("Seat %d allocated to %s successfully.\n", seat_num, alloc.occupant_name);
}

void deallocateSeat(int building_num, int floor_num, int flat_num, int seat_num)
{
    char seatFile[256];
    snprintf(seatFile, sizeof(seatFile), "%s/seats_building_%d_floor_%d_flat_%d.txt", ROOM_DIR, building_num, floor_num, flat_num);
    FILE *fpSeat = fopen(seatFile, "r");
    if (!fpSeat) {
        printf("Seat file not found.\n");
        return;
    }
    Seat seats[100];
    int count = 0;
    char line[256];
    int found = 0, seat_index = -1;
    while (fgets(line, sizeof(line), fpSeat)) {
        int sNum, sStatus;
        if (sscanf(line, "%d,%d", &sNum, &sStatus) == 2) {
            seats[count].seat_num = sNum;
            seats[count].status = sStatus;
            if (sNum == seat_num) {
                found = 1;
                seat_index = count;
            }
            count++;
        }
    }
    fclose(fpSeat);
    if (!found) {
        printf("Seat %d not found.\n", seat_num);
        return;
    }
    if (seats[seat_index].status != 0) {
        printf("Seat %d is already unoccupied.\n", seat_num);
        return;
    }
    char allocFile[256];
    snprintf(allocFile, sizeof(allocFile), "%s/allocation_building_%d_floor_%d_flat_%d.txt", ROOM_DIR, building_num, floor_num, flat_num);
    FILE *fpAlloc = fopen(allocFile, "r");
    if (!fpAlloc) {
        printf("Allocation file not found.\n");
        return;
    }
    FILE *temp = fopen(ROOM_DIR "/temp_alloc.txt", "w");
    if (!temp) {
        printf("Error creating temp file.\n");
        fclose(fpAlloc);
        return;
    }
    int removed = 0;
    while (fgets(line, sizeof(line), fpAlloc)) {
        int sNum;
        char occupant_name[MAX_LEN], occupant_id[MAX_LEN], occupant_contact[MAX_LEN], allocation_date[20];
        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &sNum, occupant_name, occupant_id, occupant_contact, allocation_date) == 5) {
            if (sNum == seat_num) {
                removed = 1;
                continue;
            }
        }
        fputs(line, temp);
    }
    fclose(fpAlloc);
    fclose(temp);
    if (!removed) {
        printf("Allocation for seat %d not found.\n", seat_num);
        remove(ROOM_DIR "/temp_alloc.txt");
        return;
    }
    remove(allocFile);
    rename(ROOM_DIR "/temp_alloc.txt", allocFile);
    seats[seat_index].status = 1;
    fpSeat = fopen(seatFile, "w");
    if (!fpSeat) {
        printf("Error updating seat file.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fpSeat, "%d,%d\n", seats[i].seat_num, seats[i].status);
    }
    fclose(fpSeat);
    printf("Seat %d deallocated successfully.\n", seat_num);
}

void searchOccupant(int building_num, int floor_num, int flat_num)
{
    char allocFile[256];
    snprintf(allocFile, sizeof(allocFile), "%s/allocation_building_%d_floor_%d_flat_%d.txt", ROOM_DIR, building_num, floor_num, flat_num);
    FILE *fpAlloc = fopen(allocFile, "r");
    if (!fpAlloc) {
        printf("No allocations found.\n");
        return;
    }
    char occupant_id[MAX_LEN];
    printf("Enter occupant ID to search: ");
    readLine(occupant_id, MAX_LEN);
    char line[256];
    int found = 0;
    printf("\nAllocations for occupant ID %s:\n", occupant_id);
    printf("-----------------------------------------------------------\n");
    printf("%-10s | %-25s | %-15s | %-12s\n", "Seat#", "Name", "Contact", "Date");
    printf("-----------------------------------------------------------\n");
    while (fgets(line, sizeof(line), fpAlloc)) {
        int seat_num;
        char occupant_name[MAX_LEN], occupant_id_file[MAX_LEN], occupant_contact[MAX_LEN], allocation_date[20];
        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &seat_num, occupant_name, occupant_id_file, occupant_contact, allocation_date) == 5) {
            if (strcmp(occupant_id_file, occupant_id) == 0) {
                printf("%-10d | %-25s | %-15s | %-12s\n", seat_num, occupant_name, occupant_contact, allocation_date);
                found = 1;
            }
        }
    }
    if (!found) {
        printf("No allocation found for occupant ID %s.\n", occupant_id);
    }
    fclose(fpAlloc);
}

void searchOccupantByName(int building_num, int floor_num, int flat_num)
{
    char allocFile[256];
    snprintf(allocFile, sizeof(allocFile), "%s/allocation_building_%d_floor_%d_flat_%d.txt", ROOM_DIR, building_num, floor_num, flat_num);
    FILE *fpAlloc = fopen(allocFile, "r");
    if (!fpAlloc) {
        printf("No allocations found.\n");
        return;
    }
    char search_name[MAX_LEN];
    printf("Enter occupant first name, last name, or full name to search: ");
    readLine(search_name, MAX_LEN);
    char search_name_lower[MAX_LEN];
    for (int i = 0; search_name[i]; i++)
        search_name_lower[i] = tolower(search_name[i]);
    search_name_lower[strlen(search_name)] = '\0';
    char line[256];
    int found = 0;
    printf("\nAllocations matching name \"%s\":\n", search_name);
    printf("-----------------------------------------------------------\n");
    printf("%-10s | %-25s | %-15s | %-12s\n", "Seat#", "Name", "Contact", "Date");
    printf("-----------------------------------------------------------\n");
    while (fgets(line, sizeof(line), fpAlloc)) {
        int seat_num;
        char occupant_name[MAX_LEN], occupant_id[MAX_LEN], occupant_contact[MAX_LEN], allocation_date[20];
        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%s", &seat_num, occupant_name, occupant_id, occupant_contact, allocation_date) == 5) {
            char occupant_name_lower[MAX_LEN];
            for (int i = 0; occupant_name[i]; i++)
                occupant_name_lower[i] = tolower(occupant_name[i]);
            occupant_name_lower[strlen(occupant_name)] = '\0';
            if (strstr(occupant_name_lower, search_name_lower) != NULL) {
                printf("%-10d | %-25s | %-15s | %-12s\n", seat_num, occupant_name, occupant_contact, allocation_date);
                found = 1;
            }
        }
    }
    if (!found) {
        printf("No allocation found for occupant name \"%s\".\n", search_name);
    }
    fclose(fpAlloc);
}

void viewMyAllocation()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    char user_id[20];
    sprintf(user_id, "%d", current_user.id);
    char buildingFile[256];
    snprintf(buildingFile, sizeof(buildingFile), "%s/building.txt", ROOM_DIR);
    FILE *fpBuildings = fopen(buildingFile, "r");
    if (!fpBuildings) {
        printf("No buildings found\n");
        return;
    }
    char building_line[256];
    int found = 0;
    printf("\nYour Room Allocations:\n");
    printf("------------------------------------\n");
    while (fgets(building_line, sizeof(building_line), fpBuildings)) {
        int building_num;
        char building_name[MAX_LEN], building_addr[MAX_LEN];
        if (sscanf(building_line, "%d,%[^,],%[^\n]", &building_num, building_name, building_addr) != 3)
            continue;
        char floorFile[256];
        snprintf(floorFile, sizeof(floorFile), "%s/floor_building_%d.txt", ROOM_DIR, building_num);
        FILE *fpFloors = fopen(floorFile, "r");
        if (!fpFloors) continue;
        char floor_line[256];
        while (fgets(floor_line, sizeof(floor_line), fpFloors)) {
            int floor_num;
            char floor_desc[MAX_LEN];
            if (sscanf(floor_line, "%d,%[^\n]", &floor_num, floor_desc) != 2)
                continue;
            char flatFile[256];
            snprintf(flatFile, sizeof(flatFile), "%s/flat_building_%d_floor_%d.txt", ROOM_DIR, building_num, floor_num);
            FILE *fpFlats = fopen(flatFile, "r");
            if (!fpFlats) continue;
            char flat_line[256];
            while (fgets(flat_line, sizeof(flat_line), fpFlats)) {
                int flat_num, ac;
                char flat_name[MAX_LEN];
                if (sscanf(flat_line, "%d,%[^,],%d", &flat_num, flat_name, &ac) != 3)
                    continue;
                char allocFile[256];
                snprintf(allocFile, sizeof(allocFile), "%s/allocation_building_%d_floor_%d_flat_%d.txt", ROOM_DIR, building_num, floor_num, flat_num);
                FILE *fpAlloc = fopen(allocFile, "r");
                if (!fpAlloc) continue;
                char alloc_line[256];
                while (fgets(alloc_line, sizeof(alloc_line), fpAlloc)) {
                    int seat_num;
                    char occupant_name[MAX_LEN], occupant_id[MAX_LEN], occupant_contact[MAX_LEN], allocation_date[20];
                    if (sscanf(alloc_line, "%d,%[^,],%[^,],%[^,],%s", &seat_num, occupant_name, occupant_id, occupant_contact, allocation_date) == 5) {
                        if (strcmp(occupant_id, user_id) == 0) {
                            printf("%s: Floor %d, Flat %s, Seat %d\n", building_name, floor_num, flat_name, seat_num);
                            found = 1;
                        }
                    }
                }
                fclose(fpAlloc);
            }
            fclose(fpFlats);
        }
        fclose(fpFloors);
    }
    fclose(fpBuildings);
    if (!found) {
        printf("No allocations found for User ID %s\n", user_id);
    }
}

void displayRoomOptions()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    ensureDataDirectories();
    User current_user = getCurrentUser();
    int choice;
    int building_num, floor_num, flat_num, seat_num;
    int is_room_selected = 1;
    while (is_room_selected) {
        if (current_user.user_type == 'A') {
            printf("\nRoom Management Menu (Admin):\n");
            printf("\t1) Add Building\n");
            printf("\t2) Add Floors to Building\n");
            printf("\t3) Add Flats to Floor\n");
            printf("\t4) Add Seats to Flat\n");
            printf("\t5) View Buildings\n");
            printf("\t6) View Floors of Building\n");
            printf("\t7) View Flats of Floor\n");
            printf("\t8) View Seats of Flat\n");
            printf("\t9) Allocate Seat\n");
            printf("\t10) Deallocate Seat\n");
            printf("\t11) Search Occupant by ID\n");
            printf("\t12) Search Occupant by Name\n");
            printf("\t13) Back To Main Menu\n");
            printf("Enter choice: ");
            scanf("%d", &choice);
            getchar();
            switch (choice) {
                case 1: {
                    Building b;
                    printf("Enter building number: ");
                    scanf("%d", &b.building_num);
                    getchar();
                    printf("Enter building name: ");
                    readLine(b.name, MAX_LEN);
                    printf("Enter building address: ");
                    readLine(b.address, MAX_LEN);
                    saveBuilding(&b);
                    printf("Building added successfully.\n");
                    break;
                }
                case 2: {
                    printf("Enter building number to add floors: ");
                    scanf("%d", &building_num);
                    getchar();
                    int floorCount;
                    printf("Enter number of floors to add: ");
                    scanf("%d", &floorCount);
                    getchar();
                    Floor *floors = malloc(floorCount * sizeof(Floor));
                    for (int i = 0; i < floorCount; i++) {
                        printf("Enter floor number: ");
                        scanf("%d", &floors[i].floor_num);
                        getchar();
                        printf("Enter floor description: ");
                        readLine(floors[i].description, MAX_LEN);
                    }
                    saveFloors(building_num, floors, floorCount);
                    free(floors);
                    printf("Floors added successfully.\n");
                    break;
                }
                case 3: {
                    printf("Enter building number: ");
                    scanf("%d", &building_num);
                    getchar();
                    printf("Enter floor number: ");
                    scanf("%d", &floor_num);
                    getchar();
                    int flatCount;
                    printf("Enter number of flats to add: ");
                    scanf("%d", &flatCount);
                    getchar();
                    Flat *flats = malloc(flatCount * sizeof(Flat));
                    for (int i = 0; i < flatCount; i++) {
                        printf("Enter flat number: ");
                        scanf("%d", &flats[i].flat_num);
                        getchar();
                        printf("Enter flat name: ");
                        readLine(flats[i].flat_name, MAX_LEN);
                        printf("Is AC available (1 for yes, 0 for no): ");
                        scanf("%d", &flats[i].is_ac_available);
                        getchar();
                    }
                    saveFlats(building_num, floor_num, flats, flatCount);
                    free(flats);
                    printf("Flats added successfully.\n");
                    break;
                }
                case 4: {
                    printf("Enter building number: ");
                    scanf("%d", &building_num);
                    getchar();
                    printf("Enter floor number: ");
                    scanf("%d", &floor_num);
                    getchar();
                    printf("Enter flat number: ");
                    scanf("%d", &flat_num);
                    getchar();
                    int seatCount;
                    printf("Enter number of seats to add: ");
                    scanf("%d", &seatCount);
                    getchar();
                    Seat *seats = malloc(seatCount * sizeof(Seat));
                    for (int i = 0; i < seatCount; i++) {
                        printf("Enter seat number: ");
                        scanf("%d", &seats[i].seat_num);
                        getchar();
                        seats[i].status = 1;
                    }
                    saveSeats(building_num, floor_num, flat_num, seats, seatCount);
                    free(seats);
                    printf("Seats added successfully.\n");
                    break;
                }
                case 5:
                    viewBuildings();
                    break;
                case 6:
                    printf("Enter building number to view floors: ");
                    scanf("%d", &building_num);
                    getchar();
                    viewFloors(building_num);
                    break;
                case 7:
                    printf("Enter building number: ");
                    scanf("%d", &building_num);
                    getchar();
                    printf("Enter floor number: ");
                    scanf("%d", &floor_num);
                    getchar();
                    viewFlats(building_num, floor_num);
                    break;
                case 8:
                    printf("Enter building number: ");
                    scanf("%d", &building_num);
                    getchar();
                    printf("Enter floor number: ");
                    scanf("%d", &floor_num);
                    getchar();
                    printf("Enter flat number: ");
                    scanf("%d", &flat_num);
                    getchar();
                    viewSeats(building_num, floor_num, flat_num);
                    break;
                case 9:
                    printf("Enter building number: ");
                    scanf("%d", &building_num);
                    getchar();
                    printf("Enter floor number: ");
                    scanf("%d", &floor_num);
                    getchar();
                    printf("Enter flat number: ");
                    scanf("%d", &flat_num);
                    getchar();
                    printf("Enter seat number to allocate: ");
                    scanf("%d", &seat_num);
                    getchar();
                    allocateSeat(building_num, floor_num, flat_num, seat_num);
                    break;
                case 10:
                    printf("Enter building number: ");
                    scanf("%d", &building_num);
                    getchar();
                    printf("Enter floor number: ");
                    scanf("%d", &floor_num);
                    getchar();
                    printf("Enter flat number: ");
                    scanf("%d", &flat_num);
                    getchar();
                    printf("Enter seat number to deallocate: ");
                    scanf("%d", &seat_num);
                    getchar();
                    deallocateSeat(building_num, floor_num, flat_num, seat_num);
                    break;
                case 11:
                    printf("Enter building number: ");
                    scanf("%d", &building_num);
                    getchar();
                    printf("Enter floor number: ");
                    scanf("%d", &floor_num);
                    getchar();
                    printf("Enter flat number: ");
                    scanf("%d", &flat_num);
                    getchar();
                    searchOccupant(building_num, floor_num, flat_num);
                    break;
                case 12:
                    printf("Enter building number: ");
                    scanf("%d", &building_num);
                    getchar();
                    printf("Enter floor number: ");
                    scanf("%d", &floor_num);
                    getchar();
                    printf("Enter flat number: ");
                    scanf("%d", &flat_num);
                    getchar();
                    searchOccupantByName(building_num, floor_num, flat_num);
                    break;
                case 13:
                    is_room_selected = 0;
                    main_options();
                    break;
                default:
                    printf("Invalid choice. Try again.\n");
            }
        } else {
            printf("\nRoom Management Menu (User):\n");
            printf("\t1) View My Allocation\n");
            printf("\t2) View Buildings\n");
            printf("\t3) View Floors of Building\n");
            printf("\t4) View Flats of Floor\n");
            printf("\t5) View Seats of Flat\n");
            printf("\t6) Search Occupant by ID\n");
            printf("\t7) Search Occupant by Name\n");
            printf("\t8) Back To Main Menu\n");
            printf("Enter choice: ");
            scanf("%d", &choice);
            getchar();
            switch (choice) {
                case 1:
                    viewMyAllocation();
                    break;
                case 2:
                    viewBuildings();
                    break;
                case 3:
                    printf("Enter building number to view floors: ");
                    scanf("%d", &building_num);
                    getchar();
                    viewFloors(building_num);
                    break;
                case 4:
                    printf("Enter building number: ");
                    scanf("%d", &building_num);
                    getchar();
                    printf("Enter floor number: ");
                    scanf("%d", &floor_num);
                    getchar();
                    viewFlats(building_num, floor_num);
                    break;
                case 5:
                    printf("Enter building number: ");
                    scanf("%d", &building_num);
                    getchar();
                    printf("Enter floor number: ");
                    scanf("%d", &floor_num);
                    getchar();
                    printf("Enter flat number: ");
                    scanf("%d", &flat_num);
                    getchar();
                    viewSeats(building_num, floor_num, flat_num);
                    break;
                case 6:
                    printf("Enter building number: ");
                    scanf("%d", &building_num);
                    getchar();
                    printf("Enter floor number: ");
                    scanf("%d", &floor_num);
                    getchar();
                    printf("Enter flat number: ");
                    scanf("%d", &flat_num);
                    getchar();
                    searchOccupant(building_num, floor_num, flat_num);
                    break;
                case 7:
                    printf("Enter building number: ");
                    scanf("%d", &building_num);
                    getchar();
                    printf("Enter floor number: ");
                    scanf("%d", &floor_num);
                    getchar();
                    printf("Enter flat number: ");
                    scanf("%d", &flat_num);
                    getchar();
                    searchOccupantByName(building_num, floor_num, flat_num);
                    break;
                case 8:
                    is_room_selected = 0;
                    main_options();
                    break;
                default:
                    printf("Invalid choice. Try again.\n");
            }
        }
    }
}
