#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Polinom {
    int koef;
    int exp;
    struct Polinom* next;
} POL;

POL* createHead();
POL* createNode(int k, int e);
void insertSorted(POL* head, int k, int e);
void readPolyFromFile(const char* filename, POL* head);
void printPoly(POL* head);
POL* addPoly(POL* A, POL* B);
POL* mulPoly(POL* A, POL* B);
void freePoly(POL* head);


int main() {
    POL* A = createHead();
    POL* B = createHead();

    readPolyFromFile("polinom1.txt", A);
    readPolyFromFile("polinom2.txt", B);

    printf("Polinom A: ");
    printPoly(A);

    printf("Polinom B: ");
    printPoly(B);

    POL* Z = addPoly(A, B);
    POL* M = mulPoly(A, B);

    printf("\nZbroj A + B = ");
    printPoly(Z);

    printf("Umnozak A * B = ");
    printPoly(M);

    freePoly(A);
    freePoly(B);
    freePoly(Z);
    freePoly(M);

    return 0;
}

POL* createHead() {
    POL* h = (POL*)malloc(sizeof(POL));
    if (!h) {
        printf("Greska pri alokaciji!\n");
        exit(1);
    }
    h->koef = 0;
    h->exp = 0;
    h->next = NULL;
    return h;
}

POL* createNode(int k, int e) {
    POL* p = (POL*)malloc(sizeof(POL));
    if (!p) {
        printf("Greska pri alokaciji!\n");
        exit(1);
    }
    p->koef = k;
    p->exp = e;
    p->next = NULL;
    return p;
}

void insertSorted(POL* head, int k, int e) {
    if (k == 0) return;

    POL* p = head;

    while (p->next != NULL && p->next->exp > e)
        p = p->next;

    if (p->next != NULL && p->next->exp == e) {
        p->next->koef += k;

        if (p->next->koef == 0) {
            POL* del = p->next;
            p->next = del->next;
            free(del);
        }
        return;
    }

    POL* novi = createNode(k, e);
    novi->next = p->next;
    p->next = novi;
}

void readPolyFromFile(char* filename, POL* head) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Ne mogu otvoriti %s!\n", filename);
        exit(1);
    }

    int k, e;

    while (fscanf(f, "%d %d", &k, &e) == 2)
        insertSorted(head, k, e);

    fclose(f);
}

void printPoly(POL* head) {
    POL* p = head->next;

    if (!p) {
        printf("0\n");
        return;
    }

    while (p) {
        printf("%d", p->koef);

        if (p->exp != 0) {
            printf("x");
            if (p->exp != 1)
                printf("^%d", p->exp);
        }

        if (p->next && p->next->koef > 0)
            printf(" + ");
        else if (p->next && p->next->koef < 0)
            printf(" ");

        p = p->next;
    }

    printf("\n");
}

POL* addPoly(POL* A, POL* B) {
    POL* R = createHead();

    for (POL* p = A->next; p; p = p->next)
        insertSorted(R, p->koef, p->exp);

    for (POL* q = B->next; q; q = q->next)
        insertSorted(R, q->koef, q->exp);

    return R;
}

POL* mulPoly(POL* A, POL* B) {
    POL* R = createHead();

    for (POL* p = A->next; p; p = p->next)
        for (POL* q = B->next; q; q = q->next)
            insertSorted(R, p->koef * q->koef, p->exp + q->exp);

    return R;
}

void freePoly(POL* head) {
    POL* p = head;
    while (p) {
        POL* next = p->next;
        free(p);
        p = next;
    }
}

