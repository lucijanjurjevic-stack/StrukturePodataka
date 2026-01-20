#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 11
#define MAX_LENGTH 100

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

int hashFunction(char* name);

Position createCountry(char* name);
int insertCountry(Position* hashTable, char* name, char* file);
int readCountryFromFile(Position countryNode, char* filename);
int printHashTable(Position* hashTable);
int searchCities(Position* hashTable, char* searchCountry, int limit);
int freeHashTable(Position* hashTable);

treePos createCity(char* name, int population);
treePos insertCity(treePos root, char* name, int population);
void printTree(treePos root);
void searchTree(treePos root, int limit);
void freeTree(treePos root);

int main() {

    Position hashTable[TABLE_SIZE];
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    char cName[50], cFile[50];
    char searchName[50];
    int limit;

    FILE* f = fopen("drzave.txt", "r");
    if (!f) {
        printf("Greska: drzave.txt se ne moze otvoriti!\n");
        return EXIT_FAILURE;
    }

    // Čitanje država i ubacivanje u hash tablicu
    while (fscanf(f, "%s %s", cName, cFile) == 2) {
        insertCountry(hashTable, cName, cFile);
    }
    fclose(f);

    printf("--- SVI PODACI ---\n");
    printHashTable(hashTable);

    printf("\n--- PRETRAZIVANJE ---\n");
    printf("Unesite ime drzave za pretragu: ");
    scanf("%s", searchName);
    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &limit);

    searchCities(hashTable, searchName, limit);

    freeHashTable(hashTable);

    return 0;
}

int hashFunction(char* name) {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        if (name[i] == '\0') break; 
        sum += name[i];
    }
    return sum % TABLE_SIZE;
}

int insertCountry(Position* hashTable, char* name, char* filename) {
    int index = hashFunction(name);

    Position newCountry = createCountry(name);
    if (!newCountry) return EXIT_FAILURE;

    readCountryFromFile(newCountry, filename);

    //Pointer na hesh redak
    Position current = hashTable[index];
    Position prev = NULL;

    // Tražimo mjesto za unos (sortirano po abecedi)
    while (current != NULL && strcmp(current->countryName, name) < 0) {
        prev = current;
        current = current->next;
    }

    // Ubacivanje novog čvora ili na pocetak liste u hash redu
    if (prev == NULL) {

        newCountry->next = hashTable[index];
        hashTable[index] = newCountry;
    }
    else {
        // Unos poslije
        prev->next = newCountry;
        newCountry->next = current;
    }

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
    char cityName[50];
    int population;

    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Datoteka %s nije pronadena.\n", filename);
        return EXIT_FAILURE;
    }

    while (fscanf(f, "%s %d", cityName, &population) == 2) {
        countryNode->root = insertCity(countryNode->root, cityName, population);
    }

    fclose(f);
    return EXIT_SUCCESS;
}

int printHashTable(Position* hashTable) {
    int i;

    for (i = 0; i < TABLE_SIZE; i++) {

        if (hashTable[i] != NULL) {
            printf("\nRedak %d:\n", i);

            Position current = hashTable[i];
            while (current != NULL) {
                printf("Drzava: %s\n", current->countryName);
                printTree(current->root);
                current = current->next;
            }
            printf("\n");
        }
    }
    return EXIT_SUCCESS;
}

int freeHashTable(Position* hashTable) {

    for (int i = 0; i < TABLE_SIZE; i++) {
        Position current = hashTable[i];

        while (current != NULL) {
            Position temp = current;
            current = current->next;

            //Oslobodi stablo gradova
            freeTree(temp->root);

            //Oslobodi državu
            free(temp);
        }
        hashTable[i] = NULL;
    }
    return EXIT_SUCCESS;
}

int searchCities(Position* hashTable, char* searchCountry, int limit) {

    int index = hashFunction(searchCountry);

    Position current = hashTable[index];

    while (current != NULL) {

        if (strcmp(current->countryName, searchCountry) == 0) {
            printf("Gradovi u %s sa vise od %d stanovnika:\n", searchCountry, limit);

            searchTree(current->root, limit);
            return EXIT_SUCCESS;
        }
        current = current->next;
    }

    printf("Drzava '%s' nije pronadena\n", searchCountry);
    return EXIT_FAILURE;
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
        // Ako je broj stanovnika isti, sortiraj po imenu
        if (strcmp(name, root->cityName) < 0)
            root->left = insertCity(root->left, name, population);
        else if (strcmp(name, root->cityName) > 0)
            root->right = insertCity(root->right, name, population);
    }

    return root;
}

void printTree(treePos root) {
    if (root == NULL) return;
    printTree(root->left);
    printf("\tGrad: %s, Stanovnika: %d\n", root->cityName, root->population);
    printTree(root->right);
}

void searchTree(treePos root, int limit) {
    if (root == NULL) return;

    searchTree(root->left, limit);
    if (root->population > limit) {
        printf("\t%s (%d)\n", root->cityName, root->population);
    }
    searchTree(root->right, limit);
}

void freeTree(treePos root) {
    if (root == NULL) return;

    freeTree(root->left);
    freeTree(root->right);

    free(root);
}
