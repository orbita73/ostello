#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include "../include/security.h"
#include "../include/display_options.h"
#include "../include/auth.h"

#define MAX_LOG 1000
#define MAX_LEN 100

typedef struct {
    char user_id[20];
    char type[10];
    char date_time[20];
} Log;

void ensureSecurityDirectoryExists()
{
    struct stat st = {0};
    if (stat("./data/security", &st) == -1) {
        mkdir("./data/security", 0700);
    }
}

static void readLine(char *buffer, int size)
{
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void getCurrentDateTime(char *buffer, int size)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buffer, size, "%04d-%02d-%02d %02d:%02d:%02d",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void logEntry()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    char user_id[20];
    if (current_user.user_type == 'A') {
        printf("Enter User ID: ");
        readLine(user_id, sizeof(user_id));
    } else {
        sprintf(user_id, "%d", current_user.id);
        printf("Logging entry for User ID: %s\n", user_id);
    }
    Log log;
    strcpy(log.user_id, user_id);
    strcpy(log.type, "Entry");
    getCurrentDateTime(log.date_time, sizeof(log.date_time));
    ensureSecurityDirectoryExists();
    FILE *file = fopen("./data/security/entry.txt", "a");
    if (!file) {
        printf("Error opening entry.txt\n");
        return;
    }
    fprintf(file, "%s,%s,%s\n", log.user_id, log.type, log.date_time);
    fclose(file);
    printf("Entry logged for User ID %s at %s\n", log.user_id, log.date_time);
}

void logExit()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    char user_id[20];
    if (current_user.user_type == 'A') {
        printf("Enter User ID: ");
        readLine(user_id, sizeof(user_id));
    } else {
        sprintf(user_id, "%d", current_user.id);
        printf("Logging exit for User ID: %s\n", user_id);
    }
    Log log;
    strcpy(log.user_id, user_id);
    strcpy(log.type, "Exit");
    getCurrentDateTime(log.date_time, sizeof(log.date_time));
    ensureSecurityDirectoryExists();
    FILE *file = fopen("./data/security/exit.txt", "a");
    if (!file) {
        printf("Error opening exit.txt\n");
        return;
    }
    fprintf(file, "%s,%s,%s\n", log.user_id, log.type, log.date_time);
    fclose(file);
    printf("Exit logged for User ID %s at %s\n", log.user_id, log.date_time);
}

void viewOwnLog()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    char user_id[20];
    sprintf(user_id, "%d", current_user.id);
    FILE *entry_file = fopen("./data/security/entry.txt", "r");
    FILE *exit_file = fopen("./data/security/exit.txt", "r");
    if (!entry_file || !exit_file) {
        printf("Error opening log files\n");
        if (entry_file) fclose(entry_file);
        if (exit_file) fclose(exit_file);
        return;
    }
    char line[256];
    int found = 0;
    printf("\nYour Access Logs:\n");
    printf("----------------------------------------\n");
    printf("%-10s %-10s %-20s\n", "User ID", "Type", "Date & Time");
    printf("----------------------------------------\n");
    while (fgets(line, sizeof(line), entry_file)) {
        Log log;
        sscanf(line, "%[^,],%[^,],%[^\n]", log.user_id, log.type, log.date_time);
        if (strcmp(log.user_id, user_id) == 0) {
            printf("%-10s %-10s %-20s\n", log.user_id, log.type, log.date_time);
            found = 1;
        }
    }
    while (fgets(line, sizeof(line), exit_file)) {
        Log log;
        sscanf(line, "%[^,],%[^,],%[^\n]", log.user_id, log.type, log.date_time);
        if (strcmp(log.user_id, user_id) == 0) {
            printf("%-10s %-10s %-20s\n", log.user_id, log.type, log.date_time);
            found = 1;
        }
    }
    if (!found) {
        printf("No logs found for your ID\n");
    }
    fclose(entry_file);
    fclose(exit_file);
}

void viewAllLogs()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    if (current_user.user_type != 'A') {
        printf("Access denied: Only admins can view all logs\n");
        return;
    }
    FILE *entry_file = fopen("./data/security/entry.txt", "r");
    FILE *exit_file = fopen("./data/security/exit.txt", "r");
    if (!entry_file || !exit_file) {
        printf("Error opening log files\n");
        if (entry_file) fclose(entry_file);
        if (exit_file) fclose(exit_file);
        return;
    }
    char line[256];
    printf("\nAll Access Logs:\n");
    printf("----------------------------------------\n");
    printf("%-10s %-10s %-20s\n", "User ID", "Type", "Date & Time");
    printf("----------------------------------------\n");
    while (fgets(line, sizeof(line), entry_file)) {
        Log log;
        sscanf(line, "%[^,],%[^,],%[^\n]", log.user_id, log.type, log.date_time);
        printf("%-10s %-10s %-20s\n", log.user_id, log.type, log.date_time);
    }
    while (fgets(line, sizeof(line), exit_file)) {
        Log log;
        sscanf(line, "%[^,],%[^,],%[^\n]", log.user_id, log.type, log.date_time);
        printf("%-10s %-10s %-20s\n", log.user_id, log.type, log.date_time);
    }
    fclose(entry_file);
    fclose(exit_file);
}

void security()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    char *admin_options[] = {"Log Entry", "Log Exit", "View All Logs", "Back to Main Menu"};
    char *user_options[] = {"Log Entry", "Log Exit", "View My Log", "Back to Main Menu"};
    int max_options = 4;
    int is_security_selected = 1;
    while (is_security_selected) {
        printf("\nSecurity Management Menu:\n");
        if (current_user.user_type == 'A') {
            for (int i = 0; i < 4; ++i) {
                printf("\t%i) %s\n", i+1, admin_options[i]);
            }
        } else {
            for (int i = 0; i < 4; ++i) {
                printf("\t%i) %s\n", i+1, user_options[i]);
            }
        }
        int choice;
        printf("Enter choice: ");
        scanf("%i", &choice);
        getchar();
        if (choice < 1 || choice > max_options) {
            printf("Invalid option selected\n");
            continue;
        }
        if (current_user.user_type == 'A') {
            switch (choice) {
                case 1:
                    logEntry();
                    break;
                case 2:
                    logExit();
                    break;
                case 3:
                    viewAllLogs();
                    break;
                case 4:
                    is_security_selected = 0;
                    main_options();
                    break;
            }
        } else {
            switch (choice) {
                case 1:
                    logEntry();
                    break;
                case 2:
                    logExit();
                    break;
                case 3:
                    viewOwnLog();
                    break;
                case 4:
                    is_security_selected = 0;
                    main_options();
                    break;
            }
        }
    }
}
