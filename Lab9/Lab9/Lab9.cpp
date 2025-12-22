#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct bTree {
    struct bTree* left;
    struct bTree* right;
    int tValue;
} treeNode;


treeNode* insert(treeNode* root, int value);
int freeNode(treeNode* root);
int inOrder(treeNode* root, int level);
int inOrderToFile(treeNode* root, FILE* file); 
int replace(treeNode* root);
int treeSum(treeNode* root);

int main() {
    treeNode* root = NULL;
    treeNode* rootRand = NULL;
    int array[] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };
    int i, randomNum;
    FILE* file;

    for (i = 0; i < 10; i++) {
        root = insert(root, array[i]);
    }
    printf("Originalno stablo:\n");
    inOrder(root, 0);

    printf("\nSumirano stablo:\n");
    replace(root);
    inOrder(root, 0);

    //       -----     ZADATAK C       -------  
    printf("\nSada zadatak c\n");
    srand(time(NULL));

    printf("Generirani brojevi: ");
    for (i = 0; i < 10; i++) {
        randomNum = rand() % 81 + 10;
        printf("%d ", randomNum);
        rootRand = insert(rootRand, randomNum);
    }

    file = fopen("rezultati.txt", "w");
    if (file == NULL) {
        printf("Greska pri otvaranju datoteke\n");
        return 1;
    }

    //     ----------       NAKON A ZADATKA       --------
    printf("Originalno stablo:\n");
    inOrder(rootRand, 0);
    fprintf(file, "Nakon funkcije insert:\n");
    inOrderToFile(rootRand, file);
    fprintf(file, "\n\n");

    //         ------    NAKON B ZADATKA     --------
    replace(rootRand);
    printf("\nSumirano stablo:\n");
    inOrder(rootRand, 0);
    fprintf(file, "Nakon funkcije replace:\n");
    inOrderToFile(rootRand, file);
    fprintf(file, "\n");

    fclose(file);
    printf("\nRezultati su zapisani u datoteku 'rezultati.txt'\n");


    freeNode(root);
    freeNode(rootRand); 

    return 0;
}

treeNode* insert(treeNode* root, int value) {
    //              ----------   AKO JE ROOT PRAZAN      --------
    if (root == NULL) {
        treeNode* newNode = (treeNode*)malloc(sizeof(treeNode));
        newNode->tValue = value;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }
    //           ------       SORTIRANI UNOS    ----------
    else {
        if (value > root->tValue) {
            root->right = insert(root->right, value);
        }
        else {
            root->left = insert(root->left, value);
        }
        return root;
    }
}

int freeNode(treeNode* root) {
    if (root == NULL)
        return 0;
    freeNode(root->left);
    freeNode(root->right);
    free(root);
    return 0;
}

int inOrder(treeNode* root, int level) {
    if (root == NULL)
        return 1;
    else {
        inOrder(root->left, level + 1);
        printf("Level %d: %d\n", level, root->tValue);
        inOrder(root->right, level + 1);
    }
    return 0;
}

int replace(treeNode* root) {
    int leftSum, rightSum;
    if (root == NULL) {
        return 1;
    }

    replace(root->left);
    replace(root->right);

    leftSum = treeSum(root->left);
    rightSum = treeSum(root->right);
    root->tValue = leftSum + rightSum;

    return 0;
}

int treeSum(treeNode* root) {
    if (root == NULL) {
        return 0;
    }
    return root->tValue + treeSum(root->left) + treeSum(root->right);
}

int inOrderToFile(treeNode* root, FILE* file) {
    if (root == NULL)
        return 1;
    else {
        inOrderToFile(root->left, file);
        fprintf(file, "%d ", root->tValue);
        inOrderToFile(root->right, file);
    }
    return 0;
}