#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/payment.h"
#include "../include/display_options.h"
#include "../include/auth.h"

#define MAX_LEN 100
#define PAYMENT_FILE "./data/payments.txt"

typedef struct {
    char userId[20];
    float amount;
    char date[20];
    char description[MAX_LEN];
    char status[20];
} Payment;

void ensurePaymentDirectoryExists()
{
    struct stat st = {0};
    if (stat("./data", &st) == -1) {
        mkdir("./data", 0700);
    }
}

static void readLine(char *buffer, int size)
{
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void getCurrentDate(char *buffer, int size)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buffer, size, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

void recordPayment()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    Payment p;
    if (current_user.user_type == 'A') {
        printf("Enter User ID for payment: ");
        scanf("%s", p.userId);
        getchar();
    } else {
        sprintf(p.userId, "%d", current_user.id);
        printf("Recording payment for User ID: %s\n", p.userId);
    }
    printf("Enter payment amount: ");
    scanf("%f", &p.amount);
    getchar();
    printf("Enter payment description: ");
    readLine(p.description, MAX_LEN);
    getCurrentDate(p.date, sizeof(p.date));
    strcpy(p.status, "Pending");
    ensurePaymentDirectoryExists();
    FILE *file = fopen(PAYMENT_FILE, "a");
    if (!file) {
        printf("Error opening payments.txt\n");
        return;
    }
    fprintf(file, "%s,%.2f,%s,%s,%s\n", p.userId, p.amount, p.date, p.description, p.status);
    fclose(file);
    printf("Payment recorded successfully with status Pending\n");
}

void viewOwnPaymentHistory()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    FILE *file = fopen(PAYMENT_FILE, "r");
    if (!file) {
        printf("Error opening payments.txt\n");
        return;
    }
    char line[256];
    int found = 0;
    char user_id_str[20];
    sprintf(user_id_str, "%d", current_user.id);
    printf("\nYour Payment History:\n");
    printf("-----------------------------------------------------------------------\n");
    printf("%-10s %-10s %-12s %-30s %-10s\n", "User ID", "Amount", "Date", "Description", "Status");
    printf("-----------------------------------------------------------------------\n");
    while (fgets(line, sizeof(line), file)) {
        Payment p;
        if (sscanf(line, "%[^,],%f,%[^,],%[^,],%s", p.userId, &p.amount, p.date, p.description, p.status) == 5) {
            if (strcmp(p.userId, user_id_str) == 0) {
                printf("%-10s %-10.2f %-12s %-30s %-10s\n", p.userId, p.amount, p.date, p.description, p.status);
                found = 1;
            }
        }
    }
    if (!found) {
        printf("No payment history found for your ID\n");
    }
    printf("-----------------------------------------------------------------------\n");
    fclose(file);
}

void viewAllPayments()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    if (current_user.user_type != 'A') {
        printf("Access denied: Only admins can view all payments\n");
        return;
    }
    FILE *file = fopen(PAYMENT_FILE, "r");
    if (!file) {
        printf("Error opening payments.txt\n");
        return;
    }
    char line[256];
    printf("\nAll Payments:\n");
    printf("-----------------------------------------------------------------------\n");
    printf("%-10s %-10s %-12s %-30s %-10s\n", "User ID", "Amount", "Date", "Description", "Status");
    printf("-----------------------------------------------------------------------\n");
    while (fgets(line, sizeof(line), file)) {
        Payment p;
        if (sscanf(line, "%[^,],%f,%[^,],%[^,],%s", p.userId, &p.amount, p.date, p.description, p.status) == 5) {
            printf("%-10s %-10.2f %-12s %-30s %-10s\n", p.userId, p.amount, p.date, p.description, p.status);
        }
    }
    printf("-----------------------------------------------------------------------\n");
    fclose(file);
}

void deletePayment()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    if (current_user.user_type != 'A') {
        printf("Access denied: Only admins can delete payments\n");
        return;
    }
    char userId[20], date[20];
    printf("Enter User ID of payment to delete: ");
    scanf("%s", userId);
    getchar();
    printf("Enter payment date (yyyy-mm-dd): ");
    readLine(date, 20);
    FILE *file = fopen(PAYMENT_FILE, "r");
    if (!file) {
        printf("Error opening payments.txt\n");
        return;
    }
    FILE *temp = fopen("./data/payments_temp.txt", "w");
    if (!temp) {
        printf("Error creating temporary file\n");
        fclose(file);
        return;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        Payment p;
        if (sscanf(line, "%[^,],%f,%[^,],%[^,],%s", p.userId, &p.amount, p.date, p.description, p.status) == 5) {
            if (strcmp(p.userId, userId) == 0 && strcmp(p.date, date) == 0) {
                found = 1;
                continue;
            }
        }
        fputs(line, temp);
    }
    fclose(file);
    fclose(temp);
    if (found) {
        remove(PAYMENT_FILE);
        rename("./data/payments_temp.txt", PAYMENT_FILE);
        printf("Payment for User ID %s on %s deleted successfully\n", userId, date);
    } else {
        remove("./data/payments_temp.txt");
        printf("No payment found for User ID %s on %s\n", userId, date);
    }
}

void viewPaymentSummary()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    if (current_user.user_type != 'A') {
        printf("Access denied: Only admins can view payment summary\n");
        return;
    }
    FILE *file = fopen(PAYMENT_FILE, "r");
    if (!file) {
        printf("Error opening payments.txt\n");
        return;
    }
    Payment payments[1000];
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%[^,],%f,%[^,],%[^,],%s", payments[count].userId, &payments[count].amount, payments[count].date, payments[count].description, payments[count].status) == 5) {
            count++;
        }
    }
    fclose(file);
    printf("\nPayment Summary by User:\n");
    printf("------------------------------------\n");
    printf("%-10s %-10s\n", "User ID", "Total Amount");
    printf("------------------------------------\n");
    for (int i = 0; i < count; i++) {
        if (strcmp(payments[i].status, "Completed") == 0) {
            float total = payments[i].amount;
            char *userId = payments[i].userId;
            for (int j = i + 1; j < count; j++) {
                if (strcmp(payments[j].userId, userId) == 0 && strcmp(payments[j].status, "Completed") == 0) {
                    total += payments[j].amount;
                    strcpy(payments[j].userId, "");
                }
            }
            if (strlen(userId) > 0) {
                printf("%-10s %-10.2f\n", userId, total);
            }
        }
    }
    printf("------------------------------------\n");
}

void displayPaymentOptions()
{
    if (!isLoggedIn()) {
        printf("Please log in first\n");
        displayAuth();
        return;
    }
    User current_user = getCurrentUser();
    char *admin_options[] = {"Record Payment", "View All Payments", "Delete Payment", "View Payment Summary", "Back to Main Menu"};
    char *user_options[] = {"Make Payment", "View My Payment History", "Back to Main Menu"};
    int max_options = (current_user.user_type == 'A') ? 5 : 3;
    int is_payment_selected = 1;
    while (is_payment_selected) {
        printf("\nPayment Management Menu:\n");
        if (current_user.user_type == 'A') {
            for (int i = 0; i < 5; ++i) {
                printf("\t%i) %s\n", i+1, admin_options[i]);
            }
        } else {
            for (int i = 0; i < 3; ++i) {
                printf("\t%i) %s\n", i+1, user_options[i]);
            }
        }
        int choice;
        printf("Enter choice: ");
        scanf("%i", &choice);
        getchar();
        if (choice < 1 || choice > max_options) {
            printf("Invalid option selected\n");
            continue;
        }
        if (current_user.user_type == 'A') {
            switch (choice) {
                case 1:
                    recordPayment();
                    break;
                case 2:
                    viewAllPayments();
                    break;
                case 3:
                    deletePayment();
                    break;
                case 4:
                    viewPaymentSummary();
                    break;
                case 5:
                    is_payment_selected = 0;
                    main_options();
                    break;
            }
        } else {
            switch (choice) {
                case 1:
                    recordPayment();
                    break;
                case 2:
                    viewOwnPaymentHistory();
                    break;
                case 3:
                    is_payment_selected = 0;
                    main_options();
                    break;
            }
        }
    }
}
