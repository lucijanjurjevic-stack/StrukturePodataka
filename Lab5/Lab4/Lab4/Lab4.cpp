#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct _postFix {
    double value;
    struct _postFix* next;
} postFix;


postFix* push(postFix* top, double value);
postFix* pop(postFix* top, double* outValue);
void freeStack(postFix* top);
double evaluatePostFix(const char* expr);

int main(void) {
    FILE* f = fopen("postfiks.txt", "r");
    if (!f) {
        printf("Greska pri otvaranju datoteke!\n");
        return 1;
    }

    char expr[256];
    int i = 0; char c;
    while ((c = fgetc(f)) != EOF) {
        if (isspace((unsigned char)c)) continue;
        expr[i++] = c;
    }
    expr[i] = '\0';
    fclose(f);

    double result = evaluatePostFix(expr);
    printf("Konacan rezultat: %.2f\n", result);
    return 0;
}

//Push 
postFix* push(postFix* top, double value) {
    postFix* n = (postFix*)malloc(sizeof(postFix));
    if (!n) {
        printf("Greska pri alokaciji memorije!\n");
        exit(1);
    }
    n->value = value;
    n->next = top;     // stari vrh postaje drugi
    return n;          // novi vrh
}

//Pop
postFix* pop(postFix* top, double* outValue) {
    if (!top) {
        printf("Greska: stog je prazan!\n");
        exit(1);
    }
    *outValue = top->value;
    postFix* newTop = top->next;
    free(top);
    return newTop;
}

void freeStack(postFix* top) {
    double dummy;
    while (top) {
        top = pop(top, &dummy);
    }
}

double evaluatePostFix(const char* expr) {
    postFix* stack = NULL;
    double a, b, r;

    for (int i = 0; expr[i] != '\0'; ++i) {
        char c = expr[i];
        if (isdigit((char)c)) {
            stack = push(stack, (double)(c - '0'));
        }
        else {
            stack = pop(stack, &b);
            stack = pop(stack, &a);

            switch (c) {
            case '+': r = a + b; break;
            case '-': r = a - b; break;
            case '*': r = a * b; break;
            case '/':
                if (b == 0) {
                    printf("Greska: dijeljenje s nulom!\n");
                    freeStack(stack);
                    exit(1);
                }
                r = a / b;
                break;
            default:
                printf("Nepoznat operator '%c'\n", c);
                freeStack(stack);
                exit(1);
            }
            stack = push(stack, r);
        }
    }

    double result;
    stack = pop(stack, &result);

    if (stack != NULL) {
        printf("Upozorenje: preostali elementi na stogu!\n");
        freeStack(stack);
    }

    return result;
}
