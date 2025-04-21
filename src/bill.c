#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bill.h"

typedef struct {
    char userId[20];
    float hours;
} WorkLog;

typedef struct {
    char userId[20];
    char joining[20];
    char type;
    float hourRate;
} Staff;

void createMonthlyBill() {
    char month[20], filename[50];
    WorkLog log;
    Staff s;
    FILE *logFp, *staffFp;
    float total;

    printf("Enter Month (e.g., April): ");
    scanf("%s", month);
    sprintf(filename, "work-logs-%s.txt", month);

    logFp = fopen(filename, "r");
    if (!logFp) {
        printf("Log file for %s not found.\n", month);
        return;
    }

    printf("\n--- Monthly Bill for %s ---\n", month);

    while (fscanf(logFp, "%s %f", log.userId, &log.hours) != EOF) {
        staffFp = fopen("staff.txt", "r");
        int found = 0;
        while (fscanf(staffFp, "%s %s %c %f", s.userId, s.joining, &s.type, &s.hourRate) != EOF) {
            if (strcmp(s.userId, log.userId) == 0) {
                total = log.hours * s.hourRate;
                printf("Staff: %s | Hours: %.2f | Rate: %.2f | Total: %.2f\n",
                       s.userId, log.hours, s.hourRate, total);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Staff ID %s not found in staff records.\n", log.userId);
        }
        fclose(staffFp);
    }

    fclose(logFp);
}
