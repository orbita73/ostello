#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../include/display_options.h"

#define MAX_LEN 100

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

void ensureDataDirectoryExists()
{
    struct stat st = {0};
    if (stat("./data", &st) == -1) {
        mkdir("./data", 0700);
    }
    if (stat("./data/rooms", &st) == -1) {
        mkdir("./data/rooms", 0700);
    }
}

void readLine(char *buffer, int size)
{
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void getCurrentDate(char *buffer, int size)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buffer, size, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

void saveBuilding(Building *b)
{
    FILE *fp = fopen("./data/rooms/building.txt", "a");
    if (!fp) {
        printf("Error opening building.txt for writing.\n");
        return;
    }
    fprintf(fp, "%d,%s,%s\n", b->building_num, b->name, b->address);
    fclose(fp);
}

void saveBuildingDetails(Building *b)
{
    char filename[80];
    sprintf(filename, "./data/rooms/building_%d.txt", b->building_num);
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening %s for writing.\n", filename);
        return;
    }
    fprintf(fp, "Building Number: %d\nName: %s\nAddress: %s\n", b->building_num, b->name, b->address);
    fclose(fp);
}

void saveFloors(int building_num, Floor *floors, int count)
{
    char filename[80];
    sprintf(filename, "./data/rooms/floor_building_%d.txt", building_num);
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening %s for writing.\n", filename);
        return;
    }
    for (int i = 0; i < count; i++)
        fprintf(fp, "%d,%s\n", floors[i].floor_num, floors[i].description);
    fclose(fp);
}

void saveFlats(int building_num, int floor_num, Flat *flats, int count)
{
    char filename[100];
    sprintf(filename, "./data/rooms/flat_building_%d_floor_%d.txt", building_num, floor_num);
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening %s for writing.\n", filename);
        return;
    }
    for (int i = 0; i < count; i++)
        fprintf(fp, "%d,%s,%d\n", flats[i].flat_num, flats[i].flat_name, flats[i].is_ac_available);
    fclose(fp);
}

void saveSeats(int building_num, int floor_num, int flat_num, Seat *seats, int count)
{
    char filename[120];
    sprintf(filename, "./data/rooms/seats_building_%d_floor_%d_flat_%d.txt", building_num, floor_num, flat_num);
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening %s for writing.\n", filename);
        return;
    }
    for (int i = 0; i < count; i++)
        fprintf(fp, "%d,%d\n", seats[i].seat_num, seats[i].status);
    fclose(fp);
}

void viewBuildings()
{
    FILE *fp = fopen("./data/rooms/building.txt", "r");
    if (!fp) {
        printf("No buildings found.\n");
        return;
    }
    char line[256];
    printf("\nBuildings:\n");
    printf("----------------------------------------------------------\n");
    printf("%-10s | %-20s | %-25s\n", "Building#", "Name", "Address");
    printf("----------------------------------------------------------\n");
    while (fgets(line, sizeof(line), fp)) {
        int num;
        char name[MAX_LEN], addr[MAX_LEN];
        if (sscanf(line, "%d,%[^,],%[^\n]", &num, name, addr) == 3)
            printf("%-10d | %-20s | %-25s\n", num, name, addr);
    }
    fclose(fp);
}

void viewFloors(int building_num)
{
    char filename[80];
    sprintf(filename, "./data/rooms/floor_building_%d.txt", building_num);
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("No floors found for building %d.\n", building_num);
        return;
    }
    char line[256];
    printf("\nFloors in Building %d:\n", building_num);
    printf("-----------------------------------------\n");
    printf("%-10s | %-30s\n", "Floor#", "Description");
    printf("-----------------------------------------\n");
    while (fgets(line, sizeof(line), fp)) {
        int floor_num;
        char desc[MAX_LEN];
        if (sscanf(line, "%d,%[^\n]", &floor_num, desc) == 2)
            printf("%-10d | %-30s\n", floor_num, desc);
    }
    fclose(fp);
}

void viewFlats(int building_num, int floor_num)
{
    char filename[100];
    sprintf(filename, "./data/rooms/flat_building_%d_floor_%d.txt", building_num, floor_num);
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("No flats found for building %d floor %d.\n", building_num, floor_num);
        return;
    }
    char line[256];
    printf("\nFlats in Building %d Floor %d:\n", building_num, floor_num);
    printf("---------------------------------------------\n");
    printf("%-10s | %-20s | %-15s\n", "Flat#", "Flat Name", "AC Available");
    printf("---------------------------------------------\n");
    while (fgets(line, sizeof(line), fp)) {
        int flat_num, is_ac_available;
        char flat_name[MAX_LEN];
        if (sscanf(line, "%d,%[^,],%d", &flat_num, flat_name, &is_ac_available) == 3)
            printf("%-10d | %-20s | %-15s\n", flat_num, flat_name, is_ac_available ? "Yes" : "No");
    }
    fclose(fp);
}

void viewSeats(int building_num, int floor_num, int flat_num)
{
    char filename[120];
    sprintf(filename, "./data/rooms/seats_building_%d_floor_%d_flat_%d.txt", building_num, floor_num, flat_num);
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("No seats found for building %d floor %d flat %d.\n", building_num, floor_num, flat_num);
        return;
    }
    char line[256];
    printf("\nSeats in Building %d Floor %d Flat %d:\n", building_num, floor_num, flat_num);
    printf("---------------------------------------------\n");
    printf("%-10s | %-10s\n", "Seat#", "Status");
    printf("---------------------------------------------\n");
    while (fgets(line, sizeof(line), fp)) {
        int seat_num, status;
        if (sscanf(line, "%d,%d", &seat_num, &status) == 2)
            printf("%-10d | %-10s\n", seat_num, status ? "Available" : "Occupied");
    }
    fclose(fp);
}

int isValidProfileId(const char *id)
{
    FILE *fp = fopen("./data/profile.txt", "r");
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
    FILE *fp = fopen("./data/profile.txt", "r");
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
    char seatFile[120];
    sprintf(seatFile, "./data/rooms/seats_building_%d_floor_%d_flat_%d.txt", building_num, floor_num, flat_num);
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

    char allocFile[140];
    sprintf(allocFile, "./data/rooms/allocation_building_%d_floor_%d_flat_%d.txt", building_num, floor_num, flat_num);
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
    for (int i = 0; i < count; i++)
        fprintf(fpSeat, "%d,%d\n", seats[i].seat_num, seats[i].status);
    fclose(fpSeat);

    printf("Seat %d allocated to %s successfully.\n", seat_num, alloc.occupant_name);
}

void searchOccupant(int building_num, int floor_num, int flat_num)
{
    char allocFile[140];
    sprintf(allocFile, "./data/rooms/allocation_building_%d_floor_%d_flat_%d.txt", building_num, floor_num, flat_num);
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
    char allocFile[140];
    sprintf(allocFile, "./data/rooms/allocation_building_%d_floor_%d_flat_%d.txt", building_num, floor_num, flat_num);
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
            // Convert occupant_name to lowercase
            char occupant_name_lower[MAX_LEN];
            for (int i = 0; occupant_name[i]; i++)
                occupant_name_lower[i] = tolower(occupant_name[i]);
            occupant_name_lower[strlen(occupant_name)] = '\0';

            if (strstr(occupant_name_lower, search_name_lower) != NULL) { // substring match
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

void displayRoomOptions()
{
    int choice;
    int building_num, floor_num, flat_num, seat_num;
    int is_room_selected = 1;

    while (is_room_selected) {
        char* options[] = {
            "Add Building","Add Floors to Building","Add Flats to Floor",
            "Add Seats to Flat","View Buildings","View Floors of Building",
            "View Flats of Floor","View Seats of Flat","Allocate Seat",
            "Search Occupant","Search Occupant by Name","Back To main menu"
        };
        printf("\nRoom Management Menu:\n");
        for(int i = 0; i < 12; ++i) {
            printf("\t%i) %s\n", i+1, options[i]);
        }
        printf("\nEnter choice: ");
        scanf("%d",&choice);
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
            saveBuildingDetails(&b);
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
            searchOccupant(building_num, floor_num, flat_num);
            break;
        case 11: {
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
        }
        case 12:
            main_options();
            is_room_selected = 0;
            break;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
}
