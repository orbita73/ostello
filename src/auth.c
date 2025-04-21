#include <stdio.h>
#include <string.h>
#include "../include/display_options.h"

#define MAX_USERS 10
#define PASSWORD_LENGTH 50

struct User {
    int id;
    char first_name[50];
    char last_name[50];
    char user_type;
    char password[50];
};

void displaySignin()
{
    printf("Sign in\n");
}

void displaySignup()
{
    printf("Sign up view\n");
}

void saveUserToFile(struct User new_user)
{
    FILE *file = fopen("users.txt", "a");
    if (file == NULL) {
        printf("Error opening file for saving user data.\n");
        return;
    }
    fprintf(file, "%d,%s,%s,%c,%s\n", new_user.id, new_user.first_name, new_user.last_name, new_user.user_type, new_user.password);
    fclose(file);
}

void signUp()
{
    struct User new_user;
    printf("Enter first name: ");
    scanf("%s", new_user.first_name);
    printf("Enter last name: ");
    scanf("%s", new_user.last_name);
    printf("Enter user type (A/B/C): ");
    scanf(" %c", &new_user.user_type);
    printf("Enter password: ");
    scanf("%s", new_user.password);

    FILE *file = fopen("users.txt", "r");
    if (file != NULL) {
        int max_id = 0;
        char line[200];
        while (fgets(line, sizeof(line), file)) {
            int id;
            sscanf(line, "%d", &id);
            if (id > max_id) {
                max_id = id;
            }
        }
        fclose(file);
        new_user.id = max_id + 1;
    } else {
        new_user.id = 1;
    }

    saveUserToFile(new_user);
    printf("User %s %s signed up successfully!\n", new_user.first_name, new_user.last_name);
}

void logIn()
{
    int id;
    char password[50];
    printf("Enter ID: ");
    scanf("%d", &id);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Error opening file for login.\n");
        return;
    }

    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        struct User temp_user;
        sscanf(line, "%d,%49[^,],%49[^,],%c,%49[^\n]", &temp_user.id, temp_user.first_name, temp_user.last_name, &temp_user.user_type, temp_user.password);
        if (temp_user.id == id &&
            strcmp(temp_user.password, password) == 0) {
            printf("Welcome back, %s %s!\n", temp_user.first_name, temp_user.last_name);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Login failed. Incorrect credentials.\n");
    } else {
        main_options();

    }
    fclose(file);


}

void displayAuth()
{
    int choice;
    printf("\nMenu:\n");
    printf("1. Sign Up\n");
    printf("2. Log In\n");
    printf("3. Exit\n");

    while (1) {
    printf("Enter your choice: ");
    scanf("%d", &choice);
        switch (choice) {
        case 1:
            displaySignup();
            signUp();
            break;
        case 2:
            displaySignin();
            logIn();
            break;
        case 3:
            printf("Exiting program.\n");
            return;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}
