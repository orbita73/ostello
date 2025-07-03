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
#include "../include/payment.h"

void main_options()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    int choice;
    User current_user = getCurrentUser();
    char *admin_options[] = {"Student's Information", "Billing", "Mess Management", "Room Allocation", "Security and Access Control", "Staff", "Payment Management", "Exit"};
    char *user_options[] = {"Student's Information", "Payment Management", "Exit"};
    int max_options = (current_user.user_type == 'A') ? 8 : 3;
    printf("\t\tMain Menu\n");
    printf("Available Options: \n");
    bool isValid(int x, int max) {
        return (x < 1 || x > max);
    }
    if (current_user.user_type == 'A') {
        for (int i = 0; i < 8; i++) {
            printf("\t%d) %s\n", i+1, admin_options[i]);
        }
    } else {
        for (int i = 0; i < 3; i++) {
            printf("\t%d) %s\n", i+1, user_options[i]);
        }
    }
    do {
        printf("\nSelect Option: ");
        scanf("%d", &choice);
        if (isValid(choice, max_options)) {
            printf("Invalid option! Try again\n");
        }
    } while (isValid(choice, max_options));
    if (current_user.user_type == 'A') {
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
            displayPaymentOptions();
            break;
        case 8:
            displayAuth();
            break;
        default:
            nothingExists();
        }
    } else {
        switch (choice) {
        case 1:
            displayProfile();
            break;
        case 2:
            displayPaymentOptions();
            break;
        case 3:
            displayAuth();
            break;
        default:
            nothingExists();
        }
    }
}