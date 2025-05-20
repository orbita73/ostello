#ifndef PROFILE_H_INCLUDED
#define PROFILE_H_INCLUDED

#define FILE_PATH "data/profile.txt"
#define MAX_LEN 100

typedef struct {
    int id;
    char userId[MAX_LEN];
    char first_name[MAX_LEN];
    char last_name[MAX_LEN];
    char dob[MAX_LEN];
    char institute[MAX_LEN];
    char nid[MAX_LEN];
    char birth_certificate[MAX_LEN];
} Member;

void createProfile(void);
void searchProfileByName(const char *searchName);
int caseInsensitiveCompare(const char *a, const char *b);
int getNextId(void);
int displayProfile(void);


#endif // PROFILE_H_INCLUDED
