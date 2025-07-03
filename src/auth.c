#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/auth.h"
#include "../include/display_options.h"

#define MAX_USERS 1000
#define USER_FILE "./data/users.txt"

static User current_user;
static int logged_in = 0;

void readLine(char *buffer, int size)
{
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void signup()
{
    User users[MAX_USERS];
    int user_count = 0;
    FILE *file = fopen(USER_FILE, "r");
    if (file) {
        while (fscanf(file, "%d,%c,%[^,],%[^\n]\n", &users[user_count].id, &users[user_count].user_type, users[user_count].username, users[user_count].password) == 4) {
            user_count++;
        }
        fclose(file);
    }
    User new_user;
    new_user.id = user_count + 1;
    printf("Enter user type (A for Admin, U for User): ");
    scanf(" %c", &new_user.user_type);
    getchar();
    if (new_user.user_type != 'A' && new_user.user_type != 'U') {
        printf("Invalid user type! Must be A or U.\n");
        return;
    }
    printf("Enter username: ");
    readLine(new_user.username, sizeof(new_user.username));
    for (int i = 0; i < user_count; i++) {
        if (strcmp(new_user.username, users[i].username) == 0) {
            printf("Username already exists!\n");
            return;
        }
    }
    printf("Enter password: ");
    readLine(new_user.password, sizeof(new_user.password));
    file = fopen(USER_FILE, "a");
    if (!file) {
        printf("Error opening users.txt\n");
        return;
    }
    fprintf(file, "%d,%c,%s,%s\n", new_user.id, new_user.user_type, new_user.username, new_user.password);
    fclose(file);
    printf("Signup successful! User ID: %d\n", new_user.id);
}

void login()
{
    char username[50], password[50];
    printf("Enter username: ");
    readLine(username, sizeof(username));
    printf("Enter password: ");
    readLine(password, sizeof(password));
    FILE *file = fopen(USER_FILE, "r");
    if (!file) {
        printf("Error opening users.txt\n");
        return;
    }
    User user;
    while (fscanf(file, "%d,%c,%[^,],%[^\n]\n", &user.id, &user.user_type, user.username, user.password) == 4) {
        if (strcmp(username, user.username) == 0 && strcmp(password, user.password) == 0) {
            current_user = user;
            logged_in = 1;
            printf("Login successful! Welcome, %s\n", user.username);
            fclose(file);
            main_options();
            return;
        }
    }
    fclose(file);
    printf("Invalid username or password\n");
}

void logout()
{
    if (!logged_in) {
        printf("No user is currently logged in\n");
        return;
    }
    logged_in = 0;
    memset(&current_user, 0, sizeof(User));
    printf("Logged out successfully\n");
}

int isLoggedIn()
{
    return logged_in;
}

User getCurrentUser()
{
    return current_user;
}

void displayAuth()
{
    int choice;
    while (1) {
        printf("\nHostel Management System\n");
        printf("1) Login\n");
        printf("2) Signup\n");
        printf("3) Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                signup();
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }
}
