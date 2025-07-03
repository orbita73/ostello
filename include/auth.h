#ifndef AUTH_H
#define AUTH_H

typedef struct {
    int id;
    char user_type;
    char username[50];
    char password[50];
} User;

User getCurrentUser();
void displayAuth();
void login();
void signup();
int isLoggedIn();

#endif
