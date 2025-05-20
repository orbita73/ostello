#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/staff.h"
#include "../include/display_options.h"

typedef struct {
    char userId[20];
    char joining[20];
    char type;
    float hourRate;
} Staff;

void displayStaffOptions()
{
    char *options[] = {"Add Staff", "Search staff by id","list staff", "Back"};
    for(int i = 0; i<4; ++i) {
        printf("%i) %s\n",i+1, options[i]);
    }
}

void displayStaff()
{
    int is_staff_options_selected = 1;
    while(is_staff_options_selected) {
        displayStaffOptions();
        int choice;
        printf("Enter staff option: ");
        scanf("%d",&choice);
        switch (choice) {
        case 1:
            addStaff();
            break;
        case 2:
            viewStaff();
            break;
        case 3:
            listStaff();
            break;
        case 4:
            printf("Exiting program.\n");
            is_staff_options_selected = 0;
            main_options();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

void addStaff()
{
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

void viewStaff()
{
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

void listStaff()
{
    Staff s;
    FILE *fp = fopen("staff.txt", "r");

    if (!fp) {
        printf("Error opening staff.txt!\n");
        return;
    }

    printf("--------------------------------------------------------------\n");
    printf("%-15s %-15s %-10s %-10s\n", "Staff ID", "Joining", "Type", "Hourly Rate");
    printf("--------------------------------------------------------------\n");

    while (fscanf(fp, "%s %s %c %f", s.userId, s.joining, &s.type, &s.hourRate) != EOF) {
        printf("%-15s %-15s %-10c %-10.2f\n", s.userId, s.joining, s.type, s.hourRate);
        printf("--------------------------------------------------------------\n");
    }

    fclose(fp);
}
