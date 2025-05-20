#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/security.h"
#include "../include/display_options.h"

#define ENTRY_LOG_FILE "./data/security/entry.txt"
#define EXIT_LOG_FILE  "./data/security/exit.txt"

void security() {
    char* options[] = {
        "Add an entry",
        "View Last N number of entry",
        "Add an exit",
        "View Last N Number of exit",
        "Back to Main Menu"
    };
    int is_security_selected = 1;
    while (is_security_selected) {
        for (int i = 0; i < 5; ++i) {
            printf("\t%i) %s\n", i + 1, options[i]);
        }
        int choice;
        printf("Enter choice: ");
        scanf("%i", &choice);
        getchar();
        switch (choice) {
            case 1: addEntry(); break;
            case 2: viewLastNEntry(); break;
            case 3: addExit(); break;
            case 4: viewLastNExit(); break;
            case 5:
                is_security_selected = 0;
                main_options();
                break;
            default: printf("Invalid Option selected!\n");
        }
    }
}

void logToFile(const char *filepath, const char *message) {
    FILE *file = fopen(filepath, "a");
    if (!file) {
        perror("Error opening log file");
        return;
    }
    fprintf(file, "%s\n", message);
    fclose(file);
}

void logEntry(const char *userId, const char *logFilePath) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[20], logMessage[256];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
    snprintf(logMessage, sizeof(logMessage), "%s %s", userId, timestamp);
    logToFile(logFilePath, logMessage);
}

void logExit(const char *userId, const char *logFilePath) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[20], logMessage[256];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
    snprintf(logMessage, sizeof(logMessage), "%s %s", userId, timestamp);
    logToFile(logFilePath, logMessage);
}

void addEntry() {
    char userId[100];
    printf("Enter User ID for entry: ");
    fgets(userId, sizeof(userId), stdin);
    userId[strcspn(userId, "\n")] = '\0';
    logEntry(userId, ENTRY_LOG_FILE);
    printf("Entry logged for user %s\n", userId);
}

void addExit() {
    char userId[100];
    printf("Enter User ID for exit: ");
    fgets(userId, sizeof(userId), stdin);
    userId[strcspn(userId, "\n")] = '\0';
    logExit(userId, EXIT_LOG_FILE);
    printf("Exit logged for user %s\n", userId);
}

static void printLogTableHeader() {
    printf("----------------------------------------\n");
    printf("| %-10s | %-19s |\n", "User ID", "Timestamp");
    printf("----------------------------------------\n");
}

static void printLogTableFooter() {
    printf("----------------------------------------\n");
}

static void viewLastNLines(const char *filepath, int n, const char *title) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Error opening log file");
        return;
    }

    char **lines = NULL;
    size_t capacity = 0;
    size_t count = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file)) {
        if (count >= capacity) {
            size_t new_capacity = capacity == 0 ? 16 : capacity * 2;
            char **new_lines = realloc(lines, new_capacity * sizeof(char*));
            if (!new_lines) {
                perror("Memory allocation failed");
                for (size_t i = 0; i < count; i++) free(lines[i]);
                free(lines);
                fclose(file);
                return;
            }
            lines = new_lines;
            capacity = new_capacity;
        }
        lines[count] = strdup(buffer);
        if (!lines[count]) {
            perror("Memory allocation failed");
            for (size_t i = 0; i < count; i++) free(lines[i]);
            free(lines);
            fclose(file);
            return;
        }
        count++;
    }
    fclose(file);

    printf("%s (last %d lines):\n", title, n);
    printLogTableHeader();

    size_t start = (count > (size_t)n) ? count - n : 0;
    for (size_t i = start; i < count; i++) {
        char userId[20], timestamp[20];
        if (sscanf(lines[i], "%19s %19[^\n]", userId, timestamp) == 2) {
            printf("| %-10s | %-19s |\n", userId, timestamp);
        }
        free(lines[i]);
    }
    printLogTableFooter();
    free(lines);
}

void viewLastNEntry() {
    int n;
    printf("Enter number of last entries to view: ");
    scanf("%d", &n);
    getchar();
    viewLastNLines(ENTRY_LOG_FILE, n, "Last Entries");
}

void viewLastNExit() {
    int n;
    printf("Enter number of last exits to view: ");
    scanf("%d", &n);
    getchar();
    viewLastNLines(EXIT_LOG_FILE, n, "Last Exits");
}
