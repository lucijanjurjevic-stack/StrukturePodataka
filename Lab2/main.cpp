#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 50

typedef struct _person {
    char firstName[MAX_LENGTH];
    char lastName[MAX_LENGTH];
    int birthYear;
    struct _person* next;
} Person;

Person* addAtBeginning(Person* head);
Person* addAtEnd(Person* head);
Person* findByLastName(Person* head, char* lastName);
Person* deleteByLastName(Person* head, char* lastName);
Person* createPerson();
void printList(Person* head);

int main() {
    Person* head = NULL;
    Person* newNode = NULL;
    Person* temp = NULL;
    char searchLastName[MAX_LENGTH];
    char deleteLastName[MAX_LENGTH];

    //Dodavanje osoba na poèetak
    printf("Dodajte osobe na pocetak:\n");
    head = addAtBeginning(head);
    head = addAtBeginning(head);

    //Dodavanje osobe na kraj
    printf("\nDodajte osobu na kraj:\n");
    head = addAtEnd(head);

    //Trazenje osobe po prezimenu
    printf("\nUnesite prezime za pretragu: ");
    fgets(searchLastName, MAX_LENGTH, stdin);
    searchLastName[strcspn(searchLastName, "\n")] = '\0'; 

    Person* found = findByLastName(head, searchLastName);
    if (found)
        printf("Pronadena osoba: %s %s (%d)\n", found->firstName, found->lastName, found->birthYear);
    else
        printf("Osoba s prezimenom '%s' nije pronadena.\n", searchLastName);

    //Brisanje po prezimenu
    printf("\nUnesite prezime osobe za brisanje: ");
    fgets(deleteLastName, MAX_LENGTH, stdin);
    deleteLastName[strcspn(deleteLastName, "\n")] = '\0';

    head = deleteByLastName(head, deleteLastName);

    //Ispis liste
    printf("\nTrenutna lista:\n");
    printList(head);

    //Oslobadanje memorije
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}

// funkcija ja kreaciju osobe
Person* createPerson() {
    Person* result = (Person*)malloc(sizeof(Person));
    if (!result) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    printf("Unesite ime: ");
    fgets(result->firstName, MAX_LENGTH, stdin);
    result->firstName[strcspn(result->firstName, "\n")] = '\0';

    printf("Unesite prezime: ");
    fgets(result->lastName, MAX_LENGTH, stdin);
    result->lastName[strcspn(result->lastName, "\n")] = '\0';

    printf("Unesite godinu rodenja: ");
    scanf("%d", &result->birthYear);
    getchar(); 

    result->next = NULL;
    return result;
}

//funckija za dodavanje osoba na pocetak liste
Person* addAtBeginning(Person* head) {
    Person* newPerson = createPerson();
    if (!newPerson)
        return head;

    newPerson->next = head;
    return newPerson;
}

//funckija za dodavanje osoba na kraj liste
Person* addAtEnd(Person* head) {
    Person* newPerson = createPerson();
    if (!newPerson)
        return head;

    if (head == NULL)
        return newPerson;

    Person* temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newPerson;
    return head;
}

//funckija za trazenje osobe u listi po prezimenu
Person* findByLastName(Person* head, char* lastName) {
    Person* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->lastName, lastName) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

//funckija za brisanje osobe iz liste po prezimenu
Person* deleteByLastName(Person* head, char* lastName) {
    if (head == NULL)
        return NULL;

    Person* temp = head;
    Person* prev = NULL;

    while (temp != NULL && strcmp(temp->lastName, lastName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Osoba s prezimenom '%s' nije pronadena.\n", lastName);
        return head;
    }

    if (prev == NULL)
        head = temp->next; 
    else
        prev->next = temp->next;

    printf("Osoba '%s' je obrisana iz liste.\n", lastName);
    free(temp);
    return head;
}

//funckija za printanje liste
void printList(Person* head) {
    if (head == NULL) {
        printf("Lista je prazna.\n");
        return;
    }

    int i = 1;
    Person* temp = head;
    while (temp != NULL) {
        printf("%d. %s %s (%d)\n", i, temp->firstName, temp->lastName, temp->birthYear);
        temp = temp->next;
        i++;
    }
}
