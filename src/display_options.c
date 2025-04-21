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
void main_options()
{
    int i;
    char *options[] = {"1) Student's Information", "2) Billing", "3) Mess Management", "4) Room Allocation", "5) Security and Access Control", "6) Others", "7) Exit"};

    printf("\t\tWelcome to Ostello\n\n");
    printf("Available Options: \n");
    bool isValid(int x) {
        return !(i>0 && i<8);
    }

    for (int j = 0; j < 7; j++) {
        printf("\t%s\n", options[j]);
    }

    do {
        printf("\nSelect Option: ");
        scanf("%d", &i);
        if(isValid(i)) {
            printf("Invalid options! try again\n");
        }
    } while(isValid(i));
    switch (i) {
    case 1:
        info();
        break;
    case 2:
        seeBill();
        break;
    case 3:
        mess();
        break;
    case 4:
        room();
        break;
    case 5:
        security();
        break;
    case 6:
        others();
        break;
    case 7:
        selectExit();
        break;
    default:
        nothingExists();
    }
}
