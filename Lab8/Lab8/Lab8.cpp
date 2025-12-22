#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct bTree {
	struct bTree *left;
	struct bTree *right;
	int tValue;
}treeNode;

treeNode* createNode(treeNode* adress, int value);
int freeNode(treeNode* root);
int inOrder(treeNode* root,int level);
int postOrder(treeNode* root, int level);
int preOrder(treeNode* root,int level);
//int levelOrder(treeNode* root,int level);
bool findElement(treeNode* root, int value, int level);
treeNode* deleteElement(treeNode* root, int value);

int main() {
	int choice,sortchoice,value,number;
    int loop = 1, sortloop = 1;
    treeNode* root = NULL;
	
    while (loop) {
        printf("\n--- IZBORNIK BINARNOG STABLA ---\n");
        printf("1. Unos novog elementa\n");
        printf("2. Ispis\n");
        printf("3. Pronalazenje elementa\n");
        printf("4. Brisanje elementa\n");
        printf("0. Izlaz\n");
        printf("Izabreite: ");

        scanf("%d", &choice);
        switch (choice) {

        case 1:
            printf("Unesite vrijednost\n");

            scanf("%d",&value);
            root = createNode(root, value);
            break;

        case 2:
            sortloop = 1;
                while (sortloop) {
                    printf("\n--- IZBORNIK SORTIRANJA ISPISA ---\n");
                    printf("1. Sortiraj Preorder\n");
                    printf("2. Sortiraj Postorder\n");
                    printf("3. Sortiraj Inorder\n");
                    printf("4. Sortiraj Level order\n");
                    printf("0. Povratak u glavni meni\n");

                    scanf("%d", &sortchoice);
                    switch (sortchoice) {
                    case 1:
                        printf("Odabrali ste Preorder\n");
                        preOrder(root,0);
                        break;
                    case 2:
                        printf("Odabrali ste Postorder\n");
                        postOrder(root, 0);
                        break;
                    case 3:
                        printf("Odabrali ste Inorder\n");
                        inOrder(root, 0);
                        break;
                    case 4:
                        printf("Odabrali ste Level order\n");
                        //levelOrder(root, 0);
                        break;
                  
                     case 0:
                        printf("Povratak u glavni meni\n");
                        sortloop = 0;
                        break;
                     default:
                        printf("Nevazeci izbor, pokusajte ponovo.\n");
                        }
                }
            break;

        case 3:
            printf("Unesite vrijednost\n");
            scanf("%d", &number);

            if (findElement(root, number,0)) 
                printf("%d se nalazi u stablu", number);
            else 
                printf("%d se ne nalazi u stablu.\n", number);
            break;
        case 4:
            printf("Unesite vrijednost\n");
            scanf("%d", &number);
            root = deleteElement(root, number);
            break;

        case 0:
            printf("Izlaz iz programa\n");
            loop = 0; 
            break;
        default:
            printf("Nevazeci izbor, pokusajte ponovo.\n");
        }
    }

    freeNode(root);

	return 0;
}

treeNode* createNode(treeNode* adress, int value) {
    treeNode* temp = adress;

    //  ---------   AKO JE ROOT PRAZAN --------
    if (temp == NULL) {
        treeNode* newNode = (treeNode*)malloc(sizeof(treeNode));
        newNode->tValue = value;
        newNode->left = NULL;
        newNode->right = NULL;

        return newNode;
    }

    //  -------- SORTIRANI UNOS -----------
    else {
        if (value > temp->tValue) {
            temp->right = createNode(temp->right, value);
        }
        else if (value < temp->tValue) {
            temp->left = createNode(temp->left, value);
        }
        else {
            printf("Vrijednost vec postoji\n");
        }
        return adress;
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

int preOrder(treeNode* root,int level) {
    treeNode* temp = root;

    if (root == NULL)
        return 1;

    else {
        for (int i = 0; i < level; i++)
            printf("  ");

        printf("Level %d: %d\n", level, root->tValue);
        preOrder(root->left,level+1);
        preOrder(root->right,level+1);
    }

    return 0;
}

int inOrder(treeNode* root, int level) {
    treeNode* temp = root;

    if (root == NULL)
        return 1;

    else {
        inOrder(root->left, level + 1);
        printf("Level %d: %d\n", level, root->tValue);
        inOrder(root->right, level + 1);
    }

    return 0;
}

/*int levelOrder(treeNode* root, int level) {
    treeNode* temp = root;

    if (root == NULL)
        return 1;

    else {
        while ((root->left || root->right) != NULL) {
            printf("Level %d: %d, %d\n", level, root->left->tValue, root->right->tValue);
            levelOrder(root->left, level + 1);
            levelOrder(root->right, level + 1);
        }
    }

    return 0;
}*/

int postOrder(treeNode* root, int level) {
    treeNode* temp = root;

    if (root == NULL)
        return 1;

    else {
        postOrder(root->left, level + 1);
        postOrder(root->right, level + 1);
        printf("Level %d: %d\n", level, root->tValue);
    }

    return 0;
}

bool findElement(treeNode* root, int value, int level) {
    if (root == NULL)
        return false;

    if (value == root->tValue) {
        printf("Level:%d ",level);
        return true;
    }

    if (value < root->tValue)
        findElement(root->left, value, level + 1);
    else 
        findElement(root->right, value, level + 1);
}

treeNode* deleteElement(treeNode* root, int value) {
    treeNode* temp = NULL;

    if (root == NULL)
        return root;

    //           -------   PROLAZENJE KROZ PETLJU   ------------
    if (value < root->tValue)
        root->left = deleteElement(root->left, value);
    else if (value > root->tValue)
        root->right = deleteElement(root->right, value);

    //          -------     AKO IMA 1 ILI 0 DJECE    ----------
    else {
        if (root->left == NULL) {
            temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            temp = root->left;
            free(root);
            return temp;
        }

        //         ------   AKO IMA 2 DJECE -------
        else {
            temp = root->right;
            while (temp && temp->left != NULL) 
                temp = temp->left; 
            root->tValue = temp->tValue;
            root->right = deleteElement(root->right, temp->tValue);
        }
    }
    return root;
}