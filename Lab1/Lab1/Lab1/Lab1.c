#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_LENGTH 50
#define GRESKA_PRI_ALOKACIJI_MEMORIJE_ILI_OTVARANJU_DATOTEKE -1

typedef struct _Student {
    char ime[MAX_LENGTH];
    char prezime[MAX_LENGTH];
    int bodovi;
} Student;

int numStudents(FILE* dat);
Student* dinAlocArray(int n);
Student* readStudent(FILE* dat, int n, Student* students);
int maxIRelativni(Student* students, int n);

int main() {
    int brojac = 0;
    FILE* dat;
    Student* students = NULL;
    int i;
    int maxBodova;
    double relativan;

    dat = fopen("studenti.txt", "r");
    if (!dat) {
        printf("Greška pri otvaranju datoteke!\n");
        return GRESKA_PRI_ALOKACIJI_MEMORIJE_ILI_OTVARANJU_DATOTEKE;
    }

    //brojanje studenata
    brojac = numStudents(dat);
    if (brojac <= 0) {
        printf("Datoteka je prazna ili nema valjanih zapisa.\n");
        fclose(dat);
        return 0;
    }

    rewind(dat);

    //dinamicko ucitavanje niza
    students = dinAlocArray(brojac);
    if (!students) {
        printf("Greška pri alokaciji memorije!\n");
        fclose(dat);
        return GRESKA_PRI_ALOKACIJI_MEMORIJE_ILI_OTVARANJU_DATOTEKE;
    }

    //ucitavanje elemenata
    if (!readStudent(dat, brojac, students)) {
        printf("Greška pri čitanju podataka!\n");
        free(students);
        fclose(dat);
        return GRESKA_PRI_ALOKACIJI_MEMORIJE_ILI_OTVARANJU_DATOTEKE;
    }

    //racunanjane apsolutnih i relativnih bodova
    maxBodova = maxIRelativni(students, brojac);
    if (maxBodova <= 0) {
        printf("Nevaljani maksimum bodova (<= 0).\n");
        free(students);
        return 0;
    }

    //ispis
    printf("Broj redaka u datoteci: %d\n", brojac);
    for (i = 0; i < brojac; i++) {
        relativan = (double)students[i].bodovi / (double)maxBodova * 100.0;
        printf("%s %s %d %.2f\n", students[i].ime, students[i].prezime, students[i].bodovi,relativan);
    }

    free(students);
    fclose(dat);
    return 0;
}

//funkcija za dinamicko alociranje niza
Student* dinAlocArray(int n) {
    Student* students = (Student*)malloc((size_t)n * sizeof(Student));
    return students; 
}


//funkcija za ucitavanaje studenata
Student* readStudent(FILE* dat, int n,Student* students) {
    int i;

    for (i = 0; i < n; i++) {
        fscanf(dat,"%s %s %d",students[i].ime, students[i].prezime, &students[i].bodovi);
    }
    
    return students;
}

//funkcija za brojanje studenata
int numStudents(FILE* dat) {
    int brojac = 0;
    char c;

    while ((c = fgetc(dat)) != EOF) {
        if (c == '\n')
            brojac++;
    }

    return brojac;
}

//funkcija za pronalazak absolutnog i relativnog broja bodova
int maxIRelativni(Student* students, int n) {
    int i;
    int max = students[0].bodovi;

    for (i = 1; i < n; i++) {
        if (students[i].bodovi > max)
            max = students[i].bodovi;
    }
    return max;
}