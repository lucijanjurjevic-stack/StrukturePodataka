#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct city* CityPos;
typedef struct city {
    char cityName[50];
    int population;
    CityPos next;
} City;

typedef struct country* CountryPos;
typedef struct country {
    char countryName[50];
    CountryPos left;
    CountryPos right;
    CityPos cityHead;
} Country;


CountryPos createCountry(char* name);
CityPos createCity(char* name, int population);
CountryPos insertCountry(CountryPos root, char* name, char* filename);
CityPos insertCity(CityPos head, char* name, int population);
int readCountryFromFile(CountryPos countryNode, char* filename);
int printTree(CountryPos root);
int printCityList(CityPos head);
int searchCountryTree(CountryPos root, char* searchName, int limit);

int main() {
    char cName[50], searchName[50];
    char cFile[50];
    int limit;
    CountryPos root = NULL;

    FILE* f = fopen("drzave.txt", "r");
    if (!f) {
        printf("drzave.txt se ne moze otvoriti!\n");
        return EXIT_FAILURE;
    }

    while (fscanf(f, "%s %s", cName, cFile) == 2) {
        root = insertCountry(root, cName, cFile);
    }

    printf("--- ISPIS SVIH PODATAKA (Sortirano) ---\n");
    printTree(root);

    printf("\nUnesite ime drzave za pretragu: ");
    scanf("%s", searchName);
    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &limit);

    searchCountryTree(root, searchName, limit);

    fclose(f);
    return 0;
}

CountryPos insertCountry(CountryPos root, char* name, char* filename) {

    if (root == NULL) {
        CountryPos newCountry = createCountry(name);
        if (!newCountry)
            return NULL;

        readCountryFromFile(newCountry, filename);
        return newCountry;
    }
    if (strcmp(name, root->countryName) < 0) {
        root->left = insertCountry(root->left, name, filename);
    }
    else if (strcmp(name, root->countryName) > 0) {
        root->right = insertCountry(root->right, name, filename);
    }
    // ako je duplikat ne radimo nista

    return root;
}

CountryPos createCountry(char* name) {
    CountryPos newCountry = (CountryPos)malloc(sizeof(Country));
    if (!newCountry) {
        printf("Greska alokacije drzave!\n");
        return NULL;
    }
    strcpy(newCountry->countryName, name);
    newCountry->left = NULL;
    newCountry->right = NULL;
    newCountry->cityHead = NULL;

    return newCountry;
}

int readCountryFromFile(CountryPos countryNode, char* filename) {
    int population;
    char cityName[50];

    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Datoteka %s u ReadFromFile se nemoze otvorit.\n", filename);
        return EXIT_FAILURE;
    }

    while (fscanf(f, "%s %d", cityName, &population) == 2) {
        countryNode->cityHead = insertCity(countryNode->cityHead, cityName, population);
    }

    fclose(f);
    return EXIT_SUCCESS;
}

CityPos insertCity(CityPos head, char* name, int population) {
    CityPos newCity = createCity(name, population);
    if (!newCity) return head;

    //ako ubacivamo na prvo mjesto
    if (head == NULL || head->population < population) {
        newCity->next = head;
        return newCity;
    }
    //ako ima isti broj stanovnika kao prvo mjesto ubaci samo ako abecedno ide prije
    if (head->population == population && strcmp(name, head->cityName) < 0) {
        newCity->next = head;
        return newCity;
    }

    //ako nije na prvom mjestu gledaj dakje u petlji
    CityPos temp = head;
    while (temp->next != NULL) {
        if (temp->next->population < population) break;
        if (temp->next->population == population && strcmp(name, temp->next->cityName) < 0) break;

        temp = temp->next;
    }

    //ako je zadnji u petlji zamjeni pokazivace da pokaziva na noi grad i taj polazivac na NULL
    newCity->next = temp->next;
    temp->next = newCity;

    return head;
}

CityPos createCity(char* name, int population) {
    CityPos newCity = (CityPos)malloc(sizeof(City));
    if (!newCity) return NULL;

    strcpy(newCity->cityName, name);
    newCity->population = population;
    newCity->next = NULL;

    return newCity;
}

// InOrder ispis
int printTree(CountryPos root) {
    if (root == NULL) return EXIT_SUCCESS;

    printTree(root->left);

    printf("Drzava: %s\n", root->countryName);
    printCityList(root->cityHead);

    printTree(root->right);

    return EXIT_SUCCESS; // Falio je return na kraju
}

int printCityList(CityPos head) {
    while (head != NULL) {
        printf("\t%s (%d)\n", head->cityName, head->population);
        head = head->next;
    }
    return EXIT_SUCCESS;
}

int searchCountryTree(CountryPos root, char* searchName, int limit) {

    if (root == NULL) {
        printf("Drzava nije pronadena.\n");
        return EXIT_FAILURE;
    }

    int cmp = strcmp(searchName, root->countryName);

    if (cmp == 0) {
        printf("Gradovi u %s veci od %d stanovnika:\n", root->countryName, limit);
        CityPos temp = root->cityHead;
        while (temp != NULL) {
            if (temp->population > limit) {
                printf("\t%s (%d)\n", temp->cityName, temp->population);
            }
            temp = temp->next;
        }
        return EXIT_SUCCESS;
    }
    //ako se ime ne podudara sa imenom drzave dobiva se -1 i ide se dalje kroz stablo
    else if (cmp < 0) {
        return searchCountryTree(root->left, searchName, limit);
    }
    else {
        return searchCountryTree(root->right, searchName, limit);
    }
}