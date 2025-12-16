#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50

typedef struct Dir {
    char name[MAX_NAME];
    struct Dir* child;      
    struct Dir* sibling;    // next
    struct Dir* parent;     
} Dir;

Dir* createDir(char* name, Dir* parent);
void md(Dir* current);
void cd(Dir** current);
void cdBack(Dir** current);
void dir(Dir* current);
void printPath(Dir* current);
void freeDir(Dir* dir);

int main() {
    int choice;

    Dir* root = createDir("C", NULL);
    Dir* current = root;

    while (1) {
        printf("\n");
        printPath(current);
        printf("\n\n");
        printf("1 - MD (napravi direktorij)\n");
        printf("2 - CD DIR (promijeni direktorij)\n");
        printf("3 - CD.. (vrati se nazad)\n");
        printf("4 - DIR (prikazi sadrzaj)\n");
        printf("5 - IZLAZ\n");
        printf("\nOdaberite opciju: ");

        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
        case 1:
            md(current);
            break;
        case 2:
            cd(&current);
            break;
        case 3:
            cdBack(&current);
            break;
        case 4:
            dir(current);
            break;
        case 5:
            printf("Izlak iz programa\n");
            freeDir(root);
            return 0;
        default:
            printf("Krivi odabir. Pokusajte ponovo.\n");
        }
    }

    return 0;
}

Dir* createDir(char* name, Dir* parent) {
    Dir* newDir = (Dir*)malloc(sizeof(Dir));
    if (newDir == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }

    strcpy(newDir->name, name);
    newDir->child = NULL;
    newDir->sibling = NULL;
    newDir->parent = parent;

    return newDir;
}

//kreiranje novog direktorija
void md(Dir* current) {
    char name[MAX_NAME];

    printf("Unesite ime direktorija: ");
    scanf("%49s", name);

    // Provjera da li vec postoji
    Dir* temp = current->child;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            printf("Direktorij '%s' vec postoji!\n", name);
            return;
        }
        temp = temp->sibling;
    }

    Dir* newDir = createDir(name, current);

    if (current->child == NULL) {
        current->child = newDir;
    }
    else {
        temp = current->child;
        while (temp->sibling != NULL) {
            temp = temp->sibling;
        }
        temp->sibling = newDir;
    }

    printf("Direktorij '%s' je uspjesno kreiran.\n", name);
}

//Promjena direktorija
void cd(Dir** current) {
    char name[MAX_NAME];

    printf("Unesite ime direktorija: ");
    scanf("%s", name);

    // Traženje direktorija među djecom trenutnog direktorija
    Dir* temp = (*current)->child;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            *current = temp;
            printf("Promijenjen direktorij na: %s\n", name);
            return;
        }
        temp = temp->sibling;
    }

    printf("Direktorij '%s' ne postoji\n", name);
}

//Povratak u direktorij
void cdBack(Dir** current) {
    if ((*current)->parent != NULL) {
        *current = (*current)->parent;
        printf("Povratak u direktorij: %s\n", (*current)->name);
    }
    else {
        printf("Vec ste u root direktoriju!\n");
    }
}

//Ispis sadržaja trenutnog direktorija
void dir(Dir* current) {
    printf("\nSadrzaj direktorija: %s\n", current->name);

    if (current->child == NULL) {
        printf("Direktorij je prazan.\n");
    }
    else {
        Dir* temp = current->child;
        int count = 0;
        while (temp != NULL) {
            printf("[DIR]  %s\n", temp->name);
            temp = temp->sibling;
            count++;
        }
        printf("\nUkupno direktorija: %d\n", count);
    }
}

// Funkcija za ispis trenutne putanje
void printPath(Dir* current) {
    char path[500] = "";
    Dir* temp = current;
    char tempPath[500] = "";

    // Gradimo putanju od trenutnog do root direktorija
    while (temp != NULL) {
        strcpy(tempPath, temp->name);
        if (temp->parent != NULL) {
            strcat(tempPath, "\\");
        }
        strcat(tempPath, path);
        strcpy(path, tempPath);
        temp = temp->parent;
    }

    printf("%s> ", path);
}

// Funkcija za oslobađanje memorije (rekurzivno)
void freeDir(Dir* dir) {
    if (dir == NULL) return;

    freeDir(dir->child);
    freeDir(dir->sibling);
    free(dir);
}