#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct city* treePos;
typedef struct city {
    char cityName[50];
    int population;
    treePos left;
    treePos right;
} City;

typedef struct country* Position;
typedef struct country {
    char countryName[50];
    Position next;
    treePos root;
} Country;

Position createCountry(char* name);
treePos createCity(char* name, int population);
int insertCountry(Position head, char* name, char* file);
treePos insertCity(treePos root, char* name, int population);
int readCountryFromFile(Position countryNode, char* filename);
int printCountriesAndCities(Position first);
int printTree(treePos root);
int searchCities(Position first, char* searchCountry, int limit);
int searchTree(treePos root, int limit);

int main() {
    char cName[50], searchName[50]; 
    char cFile[50];
    int limit;

    Country head = { .countryName = {0}, .next = NULL, .root = NULL };

    FILE* f = fopen("drzave.txt", "r");
    if (!f) {
        printf("drzave.txt se ne moze otvoriti!\n");
        return EXIT_FAILURE;
    }

    while (fscanf(f, "%s %s", cName, cFile) == 2) {
     
        insertCountry(&head, cName, cFile);
    }
    fclose(f);

    printf("--- ISPIS SVIH PODATAKA ---\n");
    printCountriesAndCities(head.next); 

    printf("\nUnesite ime drzave za pretragu: ");
    scanf("%s", searchName);
    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &limit);

    searchCities(head.next, searchName, limit);

    return 0;
}

int insertCountry(Position head, char* name, char* filename) {
    Position newCountry = createCountry(name);
    if (!newCountry) return EXIT_FAILURE;

    readCountryFromFile(newCountry, filename);

    Position temp = head;
    while (temp->next != NULL && strcmp(temp->next->countryName, name) < 0) {
        temp = temp->next;
    }

    newCountry->next = temp->next;
    temp->next = newCountry;

    return EXIT_SUCCESS;
}

Position createCountry(char* name) {
    Position tempCountry = (Position)malloc(sizeof(Country));
    if (!tempCountry) {
        printf("Greska pri alokaciji memorije za drzavu!\n");
        return NULL;
    }
    strcpy(tempCountry->countryName, name);
    tempCountry->next = NULL;
    tempCountry->root = NULL;

    return tempCountry;
}

int readCountryFromFile(Position countryNode, char* filename) {
    int population;
    char cityName[50];

    FILE* f = fopen(filename, "r");
    if (!f) {
      
        printf("Datoteka %s u ReadFromFile se nemoze otvorit.\n\n", filename);
        return EXIT_FAILURE;
    }

    while (fscanf(f, "%s %d", cityName, &population) == 2) {
        countryNode->root = insertCity(countryNode->root, cityName, population);
    }

    fclose(f);
    return EXIT_SUCCESS;
}

treePos insertCity(treePos root, char* name, int population) {
    if (root == NULL) {
        return createCity(name, population);
    }

    if (population < root->population) {
        root->left = insertCity(root->left, name, population);
    }
    else if (population > root->population) {
        root->right = insertCity(root->right, name, population);
    }
    else {
        if (strcmp(name, root->cityName) < 0)
            root->left = insertCity(root->left, name, population);
        else if (strcmp(name, root->cityName) > 0)
            root->right = insertCity(root->right, name, population);
        // Duplikat, neradimo nista
    }

    return root;
}

treePos createCity(char* name, int population) {
    treePos tempCity = (treePos)malloc(sizeof(City));
    if (!tempCity) {
        printf("Greska alokacije grada!\n");
        return NULL;
    }
    strcpy(tempCity->cityName, name);
    tempCity->population = population;
    tempCity->left = NULL;
    tempCity->right = NULL;

    return tempCity;
}

int printCountriesAndCities(Position first) {
    while (first != NULL) {
        printf("Drzava: %s\n", first->countryName);
        printTree(first->root);
        first = first->next;
    }
    return EXIT_SUCCESS;
}

int printTree(treePos root) {
    if (root == NULL) return 0;
    printTree(root->left);
    printf("\tGrad: %s, Stanovnika: %d\n", root->cityName, root->population);
    printTree(root->right);
    return EXIT_SUCCESS;
}

int searchCities(Position first, char* searchCountry, int limit) {
    Position temp = first;
    while (temp != NULL) {
        if (strcmp(temp->countryName, searchCountry) == 0) {
            printf("Gradovi u %s sa vise od %d stanovnika:\n", searchCountry, limit);
            searchTree(temp->root, limit);
            return EXIT_SUCCESS;
        }
        temp = temp->next;
    }
    printf("Drzava '%s' nije pronadena.\n", searchCountry);
    return EXIT_FAILURE;
}

int searchTree(treePos root, int limit) {
    if (root == NULL) return EXIT_SUCCESS;

    searchTree(root->left, limit);

    if (root->population > limit) {
        printf("\t%s (%d)\n", root->cityName, root->population);
    }

    searchTree(root->right, limit);
}