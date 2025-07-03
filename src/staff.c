#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/staff.h"
#include "../include/display_options.h"
#include "../include/auth.h"

#define MAX_STAFF 1000
#define STAFF_FILE "./data/staff.txt"

typedef struct {
    char name[100];
    char role[100];
    char contact[100];
} Staff;

void ensureStaffDirectoryExists()
{
    struct stat st = {0};
    if (stat("./data", &st) == -1) {
        mkdir("./data", 0700);
    }
}

static void readLine(char *buffer, int size)
{
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void addStaff()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    if (current_user.user_type != 'A') {
        printf("Access denied: Only admins can add staff\n");
        return;
    }
    Staff s;
    printf("Enter staff name: ");
    readLine(s.name, sizeof(s.name));
    printf("Enter staff role: ");
    readLine(s.role, sizeof(s.role));
    printf("Enter staff contact: ");
    readLine(s.contact, sizeof(s.contact));
    ensureStaffDirectoryExists();
    FILE *file = fopen(STAFF_FILE, "a");
    if (!file) {
        printf("Error opening staff.txt\n");
        return;
    }
    fprintf(file, "%s,%s,%s\n", s.name, s.role, s.contact);
    fclose(file);
    printf("Staff %s added successfully\n", s.name);
}

void viewStaff()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    if (current_user.user_type != 'A') {
        printf("Access denied: Only admins can view staff\n");
        return;
    }
    FILE *file = fopen(STAFF_FILE, "r");
    if (!file) {
        printf("Error opening staff.txt\n");
        return;
    }
    char line[256];
    printf("\nStaff List:\n");
    printf("------------------------------------\n");
    printf("%-20s %-20s %-20s\n", "Name", "Role", "Contact");
    printf("------------------------------------\n");
    while (fgets(line, sizeof(line), file)) {
        Staff s;
        sscanf(line, "%[^,],%[^,],%[^\n]", s.name, s.role, s.contact);
        printf("%-20s %-20s %-20s\n", s.name, s.role, s.contact);
    }
    fclose(file);
}

void displayStaff()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    if (current_user.user_type != 'A') {
        printf("Access denied: Only admins can access staff management\n");
        return;
    }
    char *options[] = {"Add Staff", "View Staff", "Back to Main Menu"};
    int is_staff_selected = 1;
    while (is_staff_selected) {
        printf("\nStaff Management Menu:\n");
        for (int i = 0; i < 3; ++i) {
            printf("\t%i) %s\n", i+1, options[i]);
        }
        int choice;
        printf("Enter choice: ");
        scanf("%i", &choice);
        getchar();
        if (choice < 1 || choice > 3) {
            printf("Invalid option selected\n");
            continue;
        }
        switch (choice) {
            case 1:
                addStaff();
                break;
            case 2:
                viewStaff();
                break;
            case 3:
                is_staff_selected = 0;
                main_options();
                break;
        }
    }
}
