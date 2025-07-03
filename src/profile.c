#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/info.h"
#include "../include/profile.h"
#include "../include/display_options.h"
#include "../include/auth.h"

#define MAX_LEN 100

void viewOwnProfile()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    char line[512];
    int found = 0;
    printf("\n%-5s %-15s %-15s %-15s %-12s %-20s %-15s %-20s\n",
           "ID", "User ID", "First Name", "Last Name", "DOB",
           "Institute", "NID", "Birth Certificate");
    printf("-------------------------------------------------------------------------------------------------------------\n");
    while (fgets(line, sizeof(line), file)) {
        Member m;
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
               &m.id, m.userId, m.first_name, m.last_name, m.dob,
               m.institute, m.nid, m.birth_certificate);
        if (m.id == current_user.id) {
            printf("%-5d %-15s %-15s %-15s %-12s %-20s %-15s %-20s\n",
                   m.id, m.userId, m.first_name, m.last_name, m.dob,
                   m.institute, m.nid, m.birth_certificate);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("No profile found for your ID\n");
    }
    fclose(file);
}

int displayProfile()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return 0;
    }
    int choice;
    char name[MAX_LEN];
    User current_user = getCurrentUser();
    char *admin_options[] = {"Create Profile", "Search Profile by Name", "Back to main menu"};
    char *user_options[] = {"View My Profile", "Back to main menu"};
    int max_options = (current_user.user_type == 'A') ? 3 : 2;
    while (1) {
        printf("\nProfile\n");
        if (current_user.user_type == 'A') {
            for (int i = 0; i < 3; i++) {
                printf("%i) %s\n", i+1, admin_options[i]);
            }
        } else {
            for (int i = 0; i < 2; i++) {
                printf("%i) %s\n", i+1, user_options[i]);
            }
        }
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        if (choice < 1 || choice > max_options) {
            printf("Invalid choice.\n");
            continue;
        }
        if (current_user.user_type == 'A') {
            switch (choice) {
                case 1:
                    createProfile();
                    break;
                case 2:
                    printf("Enter first or last name to search: ");
                    fgets(name, MAX_LEN, stdin);
                    name[strcspn(name, "\n")] = '\0';
                    searchProfileByName(name);
                    break;
                case 3:
                    main_options();
                    return 0;
            }
        } else {
            switch (choice) {
                case 1:
                    viewOwnProfile();
                    break;
                case 2:
                    main_options();
                    return 0;
            }
        }
    }
}

void createProfile()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    if (current_user.user_type != 'A') {
        printf("Access denied: Only admins can create profiles\n");
        return;
    }
    Member m;
    m.id = getNextId();
    printf("Enter userId: ");
    fgets(m.userId, MAX_LEN, stdin);
    m.userId[strcspn(m.userId, "\n")] = '\0';
    printf("Enter first name: ");
    fgets(m.first_name, MAX_LEN, stdin);
    m.first_name[strcspn(m.first_name, "\n")] = '\0';
    printf("Enter last name: ");
    fgets(m.last_name, MAX_LEN, stdin);
    m.last_name[strcspn(m.last_name, "\n")] = '\0';
    printf("Enter date of birth (dd-mm-yyyy): ");
    fgets(m.dob, MAX_LEN, stdin);
    m.dob[strcspn(m.dob, "\n")] = '\0';
    printf("Enter institute: ");
    fgets(m.institute, MAX_LEN, stdin);
    m.institute[strcspn(m.institute, "\n")] = '\0';
    printf("Enter NID: ");
    fgets(m.nid, MAX_LEN, stdin);
    m.nid[strcspn(m.nid, "\n")] = '\0';
    printf("Enter birth certificate number: ");
    fgets(m.birth_certificate, MAX_LEN, stdin);
    m.birth_certificate[strcspn(m.birth_certificate, "\n")] = '\0';
    FILE *file = fopen(FILE_PATH, "a");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    fprintf(file, "%d,%s,%s,%s,%s,%s,%s,%s\n",
            m.id, m.userId, m.first_name, m.last_name, m.dob,
            m.institute, m.nid, m.birth_certificate);
    fclose(file);
    printf("Profile created with ID: %d\n", m.id);
}

void searchProfileByName(const char *searchName)
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    if (current_user.user_type != 'A') {
        printf("Access denied: Only admins can search profiles\n");
        return;
    }
    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }
    char line[512];
    int found = 0;
    printf("\n%-5s %-15s %-15s %-15s %-12s %-20s %-15s %-20s\n",
           "ID", "User ID", "First Name", "Last Name", "DOB",
           "Institute", "NID", "Birth Certificate");
    printf("-------------------------------------------------------------------------------------------------------------\n");
    while (fgets(line, sizeof(line), file)) {
        Member m;
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
               &m.id, m.userId, m.first_name, m.last_name, m.dob,
               m.institute, m.nid, m.birth_certificate);
        if (caseInsensitiveCompare(m.first_name, searchName) == 0 ||
            caseInsensitiveCompare(m.last_name, searchName) == 0) {
            printf("%-5d %-15s %-15s %-15s %-12s %-20s %-15s %-20s\n",
                   m.id, m.userId, m.first_name, m.last_name, m.dob,
                   m.institute, m.nid, m.birth_certificate);
            found = 1;
        }
    }
    if (!found) {
        printf("No profile found with the name '%s'\n", searchName);
    }
    fclose(file);
}

int caseInsensitiveCompare(const char *a, const char *b)
{
    for (; *a && *b; ++a, ++b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return tolower((unsigned char)*a) - tolower((unsigned char)*b);
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

int getNextId()
{
    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL) return 1;
    int maxId = 0;
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        int currentId;
        sscanf(line, "%d,", &currentId);
        if (currentId > maxId) maxId = currentId;
    }
    fclose(file);
    return maxId + 1;
}