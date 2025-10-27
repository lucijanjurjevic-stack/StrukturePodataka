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

Person* addBefore(Person* head, char* lastName);
Person* addAfter(Person* head, char* lastName);
Person* sortList(Person* head);
void writeToFile(Person* head);
Person* readFromFile();

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

    //Ispis liste
    printf("\nTrenutna lista:\n");
    printList(head);

    //Brisanje po prezimenu
    printf("\nUnesite prezime osobe za brisanje: \n");
    fgets(deleteLastName, MAX_LENGTH, stdin);
    deleteLastName[strcspn(deleteLastName, "\n")] = '\0';

    head = deleteByLastName(head, deleteLastName);

    //Ispis liste
    printf("\nTrenutna lista:\n");
    printList(head);

    //dodavanje osobe prije trazenog prezimena
    printf("\nUnesite prezime osobe prije koje zelite dodati element:\n");
    fgets(searchLastName, MAX_LENGTH, stdin);
    searchLastName[strcspn(searchLastName, "\n")] = '\0';
    head = addBefore(head, searchLastName);

    //dodavanje osobe iza trazenog prezimena
    printf("\nUnesite prezime osobe nakon koje zelite dodati element:\n");
    fgets(searchLastName, MAX_LENGTH, stdin);
    searchLastName[strcspn(searchLastName, "\n")] = '\0';
    head = addAfter(head,searchLastName);

    //Ispis liste
    printf("\nTrenutna lista:\n");
    printList(head);


    //sortiranje liste po prezimenu
    printf("\nSortirana lista:\n");
    head = sortList(head);
    printList(head);

    //upisanje u datoteku
    writeToFile(head);

    //ispisivanje iz datoteke
    head = readFromFile();
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

// funkcija za kreaciju osobe
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

//funkcija za dodavanje osobe iza trazenog prezimena
Person* addAfter(Person* head, char lastName[MAX_LENGTH]) {
    Person* newPerson = createPerson();
    Person* target = findByLastName(head, lastName);
    if (!target) return head;        //sigurnosna provjera

    newPerson->next = target->next;
    target->next = newPerson;

    return head;
}

//funkcija za dodavanje osobe prije trazenog prezimena
Person* addBefore(Person* head, char lastName[MAX_LENGTH]) {
    Person* curr = findByLastName(head, lastName);
    if (!curr) return head;         //sigurnosna provjera

    Person* newPerson = createPerson();

    if (curr == head) {             //ako je prezime prvi element
        newPerson->next = head;
        return newPerson;
    }

    Person* temp = head;
    while (temp->next != curr)
        temp = temp->next;

    temp->next = newPerson;
    newPerson->next = curr;

    return head;
}

//funkcija za sortiranje liste
Person* sortList(Person* head) {

    if (!head) return NULL;
    Person* i;
    Person* j;
    char tempName[MAX_LENGTH];
    int tempBirthYear;

    for (i = head; i->next != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (strcmp(i->lastName, j->lastName) > 0) {
                strcpy(tempName, i->lastName);
                strcpy(i->lastName, j->lastName);
                strcpy(j->lastName, tempName);

                strcpy(tempName, i->firstName);
                strcpy(i->firstName, j->firstName);
                strcpy(j->firstName, tempName);

                tempBirthYear = i->birthYear;
                i->birthYear = j->birthYear;
                j->birthYear = tempBirthYear;
            }
        }
    }
    return head;
}

//funkcija za upisivanje u datoteku
void writeToFile(Person* head) {
    FILE* f = fopen("upisivanje_osoba.txt", "w");
    if (!f) {
        printf("Greska pri otvaranju datoteke za pisanje!\n");
        return;
    }

    while (head) {
        fprintf(f, "%s %s %d\n", head->firstName, head->lastName, head->birthYear);
        head = head->next;
    }

    fclose(f);
    printf("Lista je uspjesno upisana u datoteku 'upisivanje_osobe.txt'.\n");
}

// funkcija za citanje iz datoteke
Person* readFromFile() {
    FILE* f = fopen("osobe.txt", "r");
    if (!f) {
        printf("Greska pri otvaranju datoteke za citanje!\n");
        return NULL;
    }

    Person* head = NULL;
    Person* tail = NULL;

    while (1) {
        Person* newPerson = (Person*)malloc(sizeof(Person));
        if (!newPerson) {
            printf("Greska pri alokaciji memorije!\n");
            break;
        }

        if (fscanf(f, "%s %s %d", newPerson->firstName, newPerson->lastName, &newPerson->birthYear) != 3) {
            free(newPerson);
            break;
        }

        newPerson->next = NULL;

        if (!head)
            head = newPerson;
        else
            tail->next = newPerson;

        tail = newPerson;
    }

    fclose(f);
    printf("Lista uspjesno ucitana iz datoteke 'osobe.txt'.\n");

    return head;
}


