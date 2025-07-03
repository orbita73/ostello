#ifndef PROFILE_H
#define PROFILE_H

#include "info.h"

int displayProfile();
void viewOwnProfile();
void createProfile();
void searchProfileByName(const char *searchName);
int caseInsensitiveCompare(const char *a, const char *b);
int getNextId();

#endif