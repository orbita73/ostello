#include <stdio.h>
#include <stdbool.h>
#include "../include/info.h"
#include "../include/others.h"
#include "../include/mess.h"
#include "../include/room.h"
#include "../include/security.h"
#include "../include/exit.h"
#include "../include/invalid.h"
#include "../include/bill.h"
#include "../include/staff.h"
#include "../include/profile.h"
#include "../include/auth.h"

void main_options()
{
    int choice;
    char *options[] = {"Student's Information", "Billing", "Mess Management", "Room Allocation", "Security and Access Control", "Staff", "Exit"};
    printf("\t\tMain Menu\n");

    printf("Available Options: \n");
    bool isValid(int x) {
        return !(choice>0 && choice<8);
    }

    for (int i = 0; i < 7; i++) {
        printf("\t%d) %s\n",i+1, options[i]);
    }

    do {
        printf("\nSelect Option: ");
        scanf("%d", &choice);
        if(isValid(choice)) {
            printf("Invalid options! try again\n");
        }
    } while(isValid(choice));
    switch (choice) {
    case 1:
        displayProfile();
        break;
    case 2:
        displayBillOptions();
        break;
    case 3:
        mess();
        break;
    case 4:
        displayRoomOptions();
        break;
    case 5:
        security();
        break;
    case 6:
        displayStaff();
        break;
    case 7:
        displayAuth();
        break;
    default:
        nothingExists();
    }
}
