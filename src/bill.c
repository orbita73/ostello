#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/bill.h"
#include "../include/display_options.h"


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

void displayBillOptions()
{

    int choice;
    int is_bill_menu_selected = 1;
    while(is_bill_menu_selected) {
        char* options[] = {"create monthly bill","view montly bill", "back to main menu"};
        for(int i = 0; i<3; ++i) {
            printf("%i) %s\n",i+1,options[i]);
        }
        printf("Enter your choice: ");
        scanf("%i",&choice);
        switch(choice) {
        case 1:
            createMonthlyBill();
            break;
        case 2:
            viewMonthlyBill();
            break;
        case 3:
            printf("Back to main menu.\n");
            main_options();
            is_bill_menu_selected = 0;
            break;
        default:
            printf("Invalid option selected. Try again!\n");
        }
    }
}

void viewMonthlyBill()
{
    char month[20];
    printf("Enter month to read the bill (e.g., april23): ");
    scanf("%s", month);

    char billFilename[50], line[256];
    FILE *billFp;

    sprintf(billFilename, "./data/bill/%s.txt", month);

    billFp = fopen(billFilename, "r");
    if (!billFp) {
        printf("Could not open bill file for %s.\n", month);
        return;
    }

    printf("----------------------------------------------\n");
    printf("%-15s %-10s %-10s %-10s\n", "Staff", "Hours", "Rate", "Total");
    printf("----------------------------------------------\n");
    while (fgets(line, sizeof(line), billFp)) {
        char staff[20];
        float hours, rate, total;
        if (sscanf(line, "%s %f %f %f", staff, &hours, &rate, &total) == 4) {
            printf("%-15s %-10.2f %-10.2f %-10.2f\n", staff, hours, rate, total);
            printf("----------------------------------------------\n");
        }
    }


    fclose(billFp);
}


void createMonthlyBill()
{
    char month[20], logFilename[50], billFilename[50];
    WorkLog log;
    Staff s;
    FILE *logFp, *staffFp, *billFp;
    float total;

    printf("Enter Month (e.g., april23): ");
    scanf("%s", month);

    sprintf(logFilename, "./data/work-logs/%s.txt", month);
    sprintf(billFilename, "./data/bill/%s.txt", month);

    logFp = fopen(logFilename, "r");
    if (!logFp) {
        printf("Log file for %s not found.\n", month);
        return;
    }

    billFp = fopen(billFilename, "w");
    if (!billFp) {
        printf("Could not create bill file for %s.\n", month);
        fclose(logFp);
        return;
    }

    while (fscanf(logFp, "%s %f", log.userId, &log.hours) != EOF) {
        staffFp = fopen("./data/staff.txt", "r");
        while (fscanf(staffFp, "%s %s %c %f", s.userId, s.joining, &s.type, &s.hourRate) != EOF) {
            if (strcmp(s.userId, log.userId) == 0) {
                total = log.hours * s.hourRate;
                fprintf(billFp, "%s %.2f %.2f %.2f\n", s.userId, log.hours, s.hourRate, total);
                break;
            }
        }
        fclose(staffFp);
    }

    fclose(logFp);
    fclose(billFp);

    printf("Monthly bill for %s has been saved to %s\n", month, billFilename);
}

