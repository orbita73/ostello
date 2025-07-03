#ifndef INFO_H
#define INFO_H

#define FILE_PATH "./data/profiles.txt"

typedef struct {
    int id;
    char userId[100];
    char first_name[100];
    char last_name[100];
    char dob[12];
    char institute[100];
    char nid[20];
    char birth_certificate[20];
} Member;

#endif