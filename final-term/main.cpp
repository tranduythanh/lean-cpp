#include <math.h>
#include <stdio.h>

#include <iostream>
#include <string>

using namespace std;

struct Node
{
    int Data;
    Node* Left;
    Node* Right;
};

typedef Node* PNode;

typedef struct
{
    PNode Root;
} BST;

PNode NewNode(int x)
{
    PNode pn = NULL;
    while (pn == NULL)
        pn = new Node();
        pn->Data = x;
    return pn;
}

BST *InitBST()
{
    BST *bst = NULL;
    while (bst == NULL)
        bst = new BST();
    return bst;
}

void InOrder(PNode node) {
    if (node == NULL)
    {
        return;
    }
    InOrder(node->Left);
    cout << node->Data << "   ";
    InOrder(node->Right);
}

void debug(PNode node) {
    InOrder(node);
    printf("\n");
}

void InsertRecursive(PNode node, int x)
{
    if (node->Data == x)
        return;
    if (node->Data < x)
    {
        if (node->Right == NULL)
        {
            node->Right = NewNode(x);
            return;
        }
        InsertRecursive(node->Right, x);
        return;
    }

    if (node->Left == NULL)
    {
        node->Left = NewNode(x);
        return;
    }
    InsertRecursive(node->Left, x);
}

void Insert(BST* bst, int x) {
    if (bst->Root == NULL) {
        bst->Root = NewNode(x);
        return;
    }
    InsertRecursive(bst->Root, x);
}

BST* InputBST() {
    int total = 0;
    BST *bst = InitBST();

    printf("Total node: ");
    scanf("%d", &total);

    for (int i=0; i<total; i++) {
        int num = 0;
        printf("node #%d = ", i);
        scanf("%d", &num);

        Insert(bst, num);
    }
    return bst;
}

void CountEvenNodesGreaterThanRecursive(PNode node, int y, int* count) {
    if (node == NULL) {
        return;
    }
    CountEvenNodesGreaterThanRecursive(node->Left, y, count);
    if (node->Data%2 == 0 && node->Data > y)
        (*count)++;
    CountEvenNodesGreaterThanRecursive(node->Right, y, count);
}

int CountEvenNodesGreaterThan(BST* bst, int y) {
    if (bst->Root == NULL) {
        return 0;
    }
    int count = 0;
    CountEvenNodesGreaterThanRecursive(bst->Root, y, &count);
    return count;
}

PNode SearchNodeHas1Child(PNode node) {
    if (node == NULL)
        return NULL;
    if (node->Left == NULL && node->Right != NULL)
        return node;
    if (node->Left != NULL && node->Right == NULL)
        return node;
    return SearchNodeHas1Child(node->Right);
    return SearchNodeHas1Child(node->Left);
}

PNode Search(PNode node, int x) {
    if (node == NULL || node->Data == x)
        return node;
    if (node->Data < x)
        return Search(node->Right, x);
    return Search(node->Left, x);
}

void CountNodeHasChildRecursive(PNode node, int *count) {
    if (node == NULL)
        return;
    CountNodeHasChildRecursive(node->Left, count);
    if (node->Left != NULL || node->Right != NULL)
        (*count)++;
    CountNodeHasChildRecursive(node->Right, count);
}

int CountNodeHasChild(BST *bst) {
    if (bst->Root == NULL)
        return 0;
    int count = 0;
    CountNodeHasChildRecursive(bst->Root, &count);
    return count;
}

void DeleteNodeHas1Child(PNode node) {
    if (node == NULL)
        return;
    if (node->Left != NULL and node->Right != NULL)
        return;
    if (node->Left == NULL and node->Right == NULL)
        return;

    PNode tmp;

    if (node->Left != NULL)
        tmp = node->Left;
    else
        tmp = node->Right;

    node->Data = tmp->Data;
    node->Left = tmp->Left;
    node->Right = tmp->Right;

    delete tmp;
}

void PrintNodeAtLevelRecursive(PNode node, int level, int curLevel) {
    if (node == NULL)
        return;

    curLevel ++;

    if (level==curLevel) {
        cout << node << "\t" << node->Data << endl;
        return;
    }
    PrintNodeAtLevelRecursive(node->Left, level, curLevel);
    PrintNodeAtLevelRecursive(node->Right, level, curLevel);
}

void PrintNodeAtLevel(BST* bst, int level) {
    if (level<=0 || bst->Root == NULL) {
        printf("no node\n");
        return;
    }
    PrintNodeAtLevelRecursive(bst->Root, level, 0);
}

int main() {
    printf("prob a,b ================\n");
    BST* bst = InputBST();

    // BST* bst = InitBST();
    // Insert(bst, 50);
    // Insert(bst, 40);
    // Insert(bst, 60);
    // Insert(bst, 45);
    // Insert(bst, 35);
    // Insert(bst, 55);
    // Insert(bst, 75);
    // Insert(bst, 0);
    // Insert(bst, 1);
    // Insert(bst, -3);
    // Insert(bst, 74);

    printf("BST in order: \n");
    debug(bst->Root);

    printf("\nprob c ==================\n");

    cout << "Count event nodes greater than 2: " <<
        CountEvenNodesGreaterThan(bst, 2) << endl;

    printf("\nprob d ==================\n");
    printf("BST in order before deleting a node has 1 child\n");
    debug(bst->Root);
    PNode it = SearchNodeHas1Child(bst->Root);
    if (it == NULL)
        printf("No node has only 1 child\n");
    else
        cout << "Node has 1 child " << it << "\t" << it->Data << endl;

    DeleteNodeHas1Child(it);
    printf("BST in order after deleting a node has 1 child\n");
    debug(bst->Root);

    printf("\nprob e ==================\n");
    printf("BST in order\n");
    debug(bst->Root);
    cout << "Count nodes has 1-2 children: "
        << CountNodeHasChild(bst) << endl;

    printf("\nprob f ==================\n");
    printf("Print BST in level\n");
    for (int level=0; level < 7; level++) {
        printf("\nAt level %d:\n", level);
        PrintNodeAtLevel(bst, level);
    }
}