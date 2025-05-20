#ifndef ROOM_MANAGEMENT_H
#define ROOM_MANAGEMENT_H

#define MAX_LEN 100

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
    char flat_type[MAX_LEN];
} Flat;

typedef struct {
    int seat_num;
    char seat_type[MAX_LEN];
    char status[MAX_LEN];
} Seat;

typedef struct {
    int seat_num;
    char occupant_name[MAX_LEN];
    char occupant_id[MAX_LEN];
    char occupant_contact[MAX_LEN];
    char allocation_date[20];
} Allocation;

void readLine(char *buffer, int size);
void getCurrentDate(char *buffer, int size);

void saveBuilding(Building *b);
void saveBuildingDetails(Building *b);
void saveFloors(int building_num, Floor *floors, int count);
void saveFlats(int building_num, int floor_num, Flat *flats, int count);
void saveSeats(int building_num, int floor_num, int flat_num, Seat *seats, int count);

void viewBuildings();
void viewFloors(int building_num);
void viewFlats(int building_num, int floor_num);
void viewSeats(int building_num, int floor_num, int flat_num);

void allocateSeat(int building_num, int floor_num, int flat_num, int seat_num);
void searchOccupant(int building_num, const char *name);

void displayRoomOptions();
#endif
