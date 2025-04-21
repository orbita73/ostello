#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "staff.h"

typedef struct {
    char userId[20];
    char joining[20];
    char type;
    float hourRate;
} Staff;

void addStaff() {
    Staff s;
    FILE *fp = fopen("staff.txt", "a");

    if (!fp) {
        printf("Error opening staff.txt!\n");
        return;
    }

    printf("Enter Staff ID: ");
    scanf("%s", s.userId);
    printf("Enter Joining Date (dd-mm-yyyy): ");
    scanf("%s", s.joining);
    s.type = 'c';  // default
    printf("Enter Hourly Rate: ");
    scanf("%f", &s.hourRate);

    fprintf(fp, "%s %s %c %.2f\n", s.userId, s.joining, s.type, s.hourRate);
    fclose(fp);
    printf("Staff added successfully.\n");
}

void viewStaff() {
    char userId[20];
    Staff s;
    int found = 0;

    printf("Enter Staff ID to view: ");
    scanf("%s", userId);

    FILE *fp = fopen("staff.txt", "r");

    if (!fp) {
        printf("Error opening staff.txt!\n");
        return;
    }

    while (fscanf(fp, "%s %s %c %f", s.userId, s.joining, &s.type, &s.hourRate) != EOF) {
        if (strcmp(userId, s.userId) == 0) {
            printf("Staff ID: %s\nJoining Date: %s\nType: %c\nHourly Rate: %.2f\n",
                   s.userId, s.joining, s.type, s.hourRate);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Staff not found.\n");
    }

    fclose(fp);
}

void listStaff() {
    Staff s;
    FILE *fp = fopen("staff.txt", "r");

    if (!fp) {
        printf("Error opening staff.txt!\n");
        return;
    }

    printf("\n--- List of Staff ---\n");
    while (fscanf(fp, "%s %s %c %f", s.userId, s.joining, &s.type, &s.hourRate) != EOF) {
        printf("Staff ID: %s | Joining: %s | Type: %c | Rate: %.2f\n",
               s.userId, s.joining, s.type, s.hourRate);
    }

    fclose(fp);
}
