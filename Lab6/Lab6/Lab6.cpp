#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 128
#define LINE_LEN 256

typedef struct Item {
    char name[MAX_NAME];
    int quantity;
    double price;
    struct Item* next;
} Item;

typedef struct Bill {
    char date[11];
    Item* items;
    struct Bill* next;
} Bill;

Bill* loadAllBills();
Bill* loadSingleBill(char* filename);
void insertBillSorted(Bill** head, Bill* newBill);
void insertItemSorted(Item** head, Item* newItem);
void queryItem(Bill* head, const char* soughtItemName, const char* dateFrom, const char* dateTo);
void freeItems(Item* head);
void freeBills(Bill* head);

int main() {
    char itemName[MAX_NAME];
    char dateFrom[11];
    char dateTo[11];

    Bill* bills = loadAllBills();
    if (!bills) {
        printf("Nisu racuni ucitani");
        return 1;
    }

    printf("Unesite ime proizvoda: ");
    fgets(itemName, MAX_NAME, stdin);

    itemName[strcspn(itemName, "\r\n")] = '\0';

    printf("Enter the start date (YYYY-MM-DD): ");
    scanf(" %s", dateFrom);

    printf("Enter the end date (YYYY-MM-DD): ");
    scanf(" %s", dateTo);

    queryItem(bills, itemName, dateFrom, dateTo);

    freeBills(bills);

    return 0;
}

//ucitavnje svih racuna(patha)
Bill* loadAllBills() {
    Bill* head = NULL;
    char line[LINE_LEN];

    FILE* fp = fopen("racuni.txt", "r");
    if (!fp) {
        printf("Nije moguce otvorit datoteku");
        return NULL;
    }

    while (fgets(line, LINE_LEN, fp)) {

        line[strcspn(line, "\r\n")] = '\0';

        Bill* bill = loadSingleBill(line);

        if (bill != NULL) {
            insertBillSorted(&head, bill);
        }
    }

    fclose(fp);
    return head;
}

//jedan racun ucitavanje
Bill* loadSingleBill(char* filename) {

    char date[LINE_LEN], line[LINE_LEN], name[MAX_NAME];;
    int quantity;
    double price;

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Datoteka se nemoze otvorit");
        return NULL;
    }

    Bill* bill = (Bill*)malloc(sizeof(Bill));
    if (!bill) {
        printf("Error allocating memory for bill.\n");
        fclose(fp);
        return NULL;
    }

    //datum
    fgets(date, LINE_LEN, fp);
    date[strcspn(date, "\r\n")] = '\0';
    strncpy(bill->date, date, sizeof(bill->date));
    bill->items = NULL;
    bill->next = NULL;

    //sve ostalo
    while (fgets(line, LINE_LEN, fp)) {
        if (sscanf(line, " %[^,], %d, %lf", name, &quantity, &price) == 3) {

            Item* item = (Item*)malloc(sizeof(Item));
            if (!item) {
                printf("Error allocating memory for item.\n");
                fclose(fp);
                return bill;
            }
            strncpy(item->name, name, MAX_NAME);
            item->name[MAX_NAME - 1] = '\0';
            item->quantity = quantity;
            item->price = price;
            item->next = NULL;

            insertItemSorted(&(bill->items), item);
        }
    }

    fclose(fp);

    return bill;
}

//abecedno slaganje artikala
void insertBillSorted(Bill** head, Bill* newBill) {
    Bill* current = *head;
    Bill* previous = NULL;

    while (current != NULL && strcmp(current->date, newBill->date) < 0) {
        previous = current;
        current = current->next;
    }

    if (previous == NULL) {
        newBill->next = *head;
        *head = newBill;
    }
    else {
        newBill->next = current;
        previous->next = newBill;
    }
}


void insertItemSorted(Item** head, Item* newItem) {
    Item* current = *head;
    Item* previous = NULL;

    while (current != NULL && strcmp(current->name, newItem->name) < 0) {
        previous = current;
        current = current->next;
    }

    if (previous == NULL) {
        // Insertion at the beginning
        newItem->next = *head;
        *head = newItem;
    }
    else {
        newItem->next = current;
        previous->next = newItem;
    }
}

//ispis rezultata kroz filtere
void queryItem(Bill* head, const char* soughtItemName, const char* dateFrom, const char* dateTo) {

    double totalMoneySpent = 0.0;
    int totalQuantity = 0;

    Bill* currentBill = head;

    while (currentBill != NULL) {

        if (strcmp(currentBill->date, dateFrom) >= 0 && strcmp(currentBill->date, dateTo) <= 0) {

            Item* currentItem = currentBill->items;

            while (currentItem != NULL) {

                if (strcmp(currentItem->name, soughtItemName) == 0) {

                    totalQuantity += currentItem->quantity;
                    totalMoneySpent += currentItem->quantity * currentItem->price;
                }
                currentItem = currentItem->next;
            }
        }

        currentBill = currentBill->next;
    }

    printf("Item: %s\n", soughtItemName);
    printf("Period: %s - %s\n", dateFrom, dateTo);
    printf("Total quantity: %d\n", totalQuantity);
    printf("Total money spent: %.2f\n", totalMoneySpent);
}

//ciscenje memroije
void freeItems(Item* head) {
    Item* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void freeBills(Bill* head) {
    Bill* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        freeItems(temp->items);
        free(temp);
    }
}