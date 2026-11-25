#include <stdio.h>
#include <stdlib.h>
#include <string.h>     
#include <ctype.h>      
#define MAX 50 
struct Consumer {
    int id;
    char name[50],address[100],phone[11];
};
struct Bill {
    float previous,current,units,amount,penalty,total;     
    int readingEntered,calculated,paid;           
};
struct Consumer consumers[MAX];
struct Bill bills[MAX];
int count = 0;          
void clearBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}
int readInt(char message[]) {
    int n;
    while (1) {
        printf("%s", message);
        if (scanf("%d", &n) == 1) {        
            clearBuffer();
            return n;
        }
        if (feof(stdin)) {exit(0);}
        printf("Invalid input! Please enter a number.\n");
        clearBuffer();
    }
}

float readFloat(char message[]) {
    float n;
    while (1) {
        printf("%s", message);
        if (scanf("%f", &n) == 1) {
            clearBuffer();
            return n;
        }
        if (feof(stdin)) {exit(0);}
        printf("Invalid input! Please enter a number.\n");
        clearBuffer();
    }
}

void readText(char message[], char text[], int size){
    int len, ch;
    while (1) {
        printf("%s", message);
        if (fgets(text, size, stdin) == NULL){exit(0);}
        len = strlen(text);
        if (len > 0 && text[len - 1] == '\n') {text[len - 1] = '\0';}
        else {
            ch = getchar();
            if (ch != '\n' && ch != EOF){
                clearBuffer();
                printf("Input is too long! Try again.\n");
                continue; 
            }
        }
        if (strlen(text) == 0) {printf("Input cannot be empty!\n");}
        else if (strchr(text, '|') != NULL) {printf("The character '|' is not allowed!\n");}
        else{return;}
    }
}

int isValidPhone(char phone[]) {
    int i;
    if (strlen(phone) != 10) {return 0;}
    for (i = 0; i < 10; i++) {if (!isdigit(phone[i])){return 0;}}
    return 1;
}

void loadData(){
    FILE *fp;
    int i;
    fp = fopen("consumers.txt", "r");       
    if (fp == NULL) {
        return;                             
    }
    while (count < MAX &&
           fscanf(fp, " %d|%49[^|]|%99[^|]|%10s",
                  &consumers[count].id,
                  consumers[count].name,
                  consumers[count].address,
                  consumers[count].phone) == 4) {
        count++;
    }
    fclose(fp);
    fp = fopen("bills.txt", "r");
    if (fp == NULL) {
        printf("Warning: bills.txt not found. Bill records are empty.\n");
        return;
    }
    for (i = 0; i < count; i++) {
        if (fscanf(fp, "%f %f %f %f %f %f %d %d %d",
                   &bills[i].previous, &bills[i].current, &bills[i].units,
                   &bills[i].amount, &bills[i].penalty, &bills[i].total,
                   &bills[i].readingEntered, &bills[i].calculated,
                   &bills[i].paid) != 9) {
            break;
        }
    }
    fclose(fp);
}

void saveData() {
    FILE *fp;
    int i;
    fp = fopen("consumers.txt", "w");   
    if (fp == NULL) {
        printf("Error: cannot open consumers.txt\n");
        return;
    }
    for (i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%s|%s\n", consumers[i].id, consumers[i].name,
                consumers[i].address, consumers[i].phone);
    }
    fclose(fp);
    fp = fopen("bills.txt", "w");
    if (fp == NULL) {
        printf("Error: cannot open bills.txt\n");
        return;
    }
    for (i = 0; i < count; i++) {
        fprintf(fp, "%.2f %.2f %.2f %.2f %.2f %.2f %d %d %d\n",
                bills[i].previous, bills[i].current, bills[i].units,
                bills[i].amount, bills[i].penalty, bills[i].total,
                bills[i].readingEntered, bills[i].calculated,
                bills[i].paid);
    }
    fclose(fp);
}

int findConsumer(int id) {
    int i;
    for (i = 0; i < count; i++) {
        if (consumers[i].id == id) {
            return i;
        }
    }
    return -1;
}
void registerConsumer() {
    int id;
    if (count == MAX) {
        printf("Consumer list is full!\n");
        return;
    }
    id = readInt("Enter Consumer ID: ");
    if (id <= 0) {
        printf("Consumer ID must be a positive number!\n");
        return;
    }
    if (findConsumer(id) != -1) {
        printf("This Consumer ID already exists!\n");
        return;
    }
    consumers[count].id = id;
    readText("Enter Name: ", consumers[count].name, 50);
    readText("Enter Address: ", consumers[count].address, 100);
    do {
        readText("Enter Phone Number (10 digits): ", consumers[count].phone, 11);
        if (!isValidPhone(consumers[count].phone)) {
            printf("Invalid phone number! Enter exactly 10 digits.\n");
        }
    } while (!isValidPhone(consumers[count].phone));
    bills[count].previous = 0;
    bills[count].current = 0;
    bills[count].units = 0;
    bills[count].amount = 0;
    bills[count].penalty = 0;
    bills[count].total = 0;
    bills[count].readingEntered = 0;
    bills[count].calculated = 0;
    bills[count].paid = 0;
    count++;
    saveData();
    printf("Consumer registered successfully!\n");
}
void enterMeterReading() {
    int id, i;
    float prev, curr;
    id = readInt("Enter Consumer ID: ");
    i = findConsumer(id);
    if (i == -1) {
        printf("Consumer not found!\n");
        return;
    }
    prev = readFloat("Enter Previous Reading: ");
    if (prev < 0) {
        printf("Reading cannot be negative!\n");
        return;
    }
    curr = readFloat("Enter Current Reading: ");
    if (curr < prev) {
        printf("Current reading cannot be less than previous reading!\n");
        return;
    }
    bills[i].previous = prev;
    bills[i].current = curr;
    bills[i].units = curr - prev;
    bills[i].readingEntered = 1;
    bills[i].amount = 0;
    bills[i].penalty = 0;
    bills[i].total = 0;
    bills[i].calculated = 0;
    bills[i].paid = 0;
    saveData();
    printf("Meter reading saved. Units consumed = %.2f\n", bills[i].units);
}
void calculateBill() {
    int id, i;
    float units, amount;
    id = readInt("Enter Consumer ID: ");
    i = findConsumer(id);
    if (i == -1) {
        printf("Consumer not found!\n");
        return;}
    if (bills[i].readingEntered == 0) {
        printf("Please enter the meter reading first!\n");
        return;}
    if (bills[i].paid == 1) {
        printf("This bill is already paid!\n");
        return;}
    units = bills[i].units;
    if (units <= 100) {amount = units * 1.5;}
    else if (units <= 200) {amount = 100 * 1.5 + (units - 100) * 3;}
    else {amount = 100 * 1.5 + 100 * 3 + (units - 200) * 5;}
    bills[i].amount = amount;
    bills[i].total = amount + bills[i].penalty;
    bills[i].calculated = 1;
    saveData();
    printf("Bill calculated. Bill Amount = Rs. %.2f\n", bills[i].amount);
}

void calculatePenalty() {
    int id, i, days;
    id = readInt("Enter Consumer ID: ");
    i = findConsumer(id);
    if (i == -1) {
        printf("Consumer not found!\n");
        return;}
    if (bills[i].calculated == 0) {
        printf("Please calculate the bill first!\n");
        return;}
    if (bills[i].paid == 1) {
        printf("This bill is already paid!\n");
        return;
    }
    days = readInt("Enter number of days overdue (0 if not late): ");
    if (days < 0) {
        printf("Days cannot be negative!\n");
        return;
    }
    bills[i].penalty = days * 5;
    bills[i].total = bills[i].amount + bills[i].penalty;
    saveData();
    printf("Penalty = Rs. %.2f\n", bills[i].penalty);
    printf("Total Amount = Rs. %.2f\n", bills[i].total);
}
void printBill() {
    int id, i;
    id = readInt("Enter Consumer ID: ");
    i = findConsumer(id);
    if (i == -1) {
        printf("Consumer not found!\n");
        return;}
    if (bills[i].calculated == 0){
        printf("Bill is not calculated yet!\n");
        return;}
    printf("\n==================================\n");
    printf("         ELECTRICITY BILL\n");
    printf("==================================\n");
    printf("Consumer ID     : %d\n", consumers[i].id);
    printf("Name            : %s\n", consumers[i].name);
    printf("Address         : %s\n", consumers[i].address);
    printf("Phone           : %s\n", consumers[i].phone);
    printf("----------------------------------\n");
    printf("Previous Reading: %.2f\n", bills[i].previous);
    printf("Current Reading : %.2f\n", bills[i].current);
    printf("Units Consumed  : %.2f\n", bills[i].units);
    printf("----------------------------------\n");
    printf("Bill Amount     : Rs. %.2f\n", bills[i].amount);
    printf("Penalty         : Rs. %.2f\n", bills[i].penalty);
    printf("Total Amount    : Rs. %.2f\n", bills[i].total);
    printf("----------------------------------\n");
    if (bills[i].paid == 1) {printf("Payment Status  : PAID\n");}
    else {printf("Payment Status  : UNPAID\n");}
    printf("==================================\n");
}
void paymentStatus() {
    int id, i, choice;
    id = readInt("Enter Consumer ID: ");
    i = findConsumer(id);
    if (i == -1) {
        printf("Consumer not found!\n");
        return;}
    if (bills[i].calculated == 0) {
        printf("Bill is not calculated yet!\n");
        return; }

    if (bills[i].paid == 1) {
        printf("Status: PAID\n");
        return;}
    printf("Status: UNPAID. Total due = Rs. %.2f\n", bills[i].total);
    choice = readInt("Mark as paid? (1 = Yes, 2 = No): ");
    if (choice == 1) {
        bills[i].paid = 1;
        saveData();
        printf("Payment recorded. Status: PAID\n"); }
    else if (choice == 2) {printf("Status remains UNPAID.\n");}
    else {printf("Invalid choice! Status remains UNPAID.\n");}
}
int main() {
    int choice;
    loadData();
    do {
        printf("\n====== ELECTRICITY BILL GENERATION SYSTEM ======\n");
        printf("1. Consumer Registration\n");
        printf("2. Meter Reading Entry\n");
        printf("3. Bill Calculation\n");
        printf("4. Penalty Calculation\n");
        printf("5. Print Bill\n");
        printf("6. Payment Status\n");
        printf("7. Exit\n");
        choice = readInt("Enter your choice: ");
        switch (choice) {
            case 1: registerConsumer();     break;
            case 2: enterMeterReading();    break;
            case 3: calculateBill();        break;
            case 4: calculatePenalty();     break;
            case 5: printBill();            break;
            case 6: paymentStatus();        break;
            case 7: printf("Thank you!\n"); break;
            default: printf("Invalid choice! Enter a number from 1 to 7.\n");
        }
    } while (choice != 7);
    return 0;
}