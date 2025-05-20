#ifndef AUTH_H_INCLUDED
#define AUTH_H_INCLUDED

#define MAX_USERS 10
#define PASSWORD_LENGTH 50

struct User {
    int id;
    char first_name[50];
    char last_name[50];
    char user_type;
    char password[50];
};

struct User getCurrentUser(void);
void showChoices(void);
void displaySignin(void);
void displaySignup(void);
void saveUserToFile(struct User new_user);
void signUp(void);
void logIn(void);
void displayAuth(void);

#endif // AUTH_H_INCLUDED
