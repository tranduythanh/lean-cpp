#include "stdio.h"
#include "math.h"
#include <iostream>
#include <string>
using namespace std;

typedef struct {
    string Name;
    int Amount;
} Product;

typedef Product* PProduct;

typedef struct node
{
    Product *Data;
    node *Next;
} Node;

typedef Node *PNode;

typedef struct
{
    PNode Head;
    PNode Tail;
} LinkedList;

typedef struct
{
    PNode Pre;
    PNode Cur;
} Position;

LinkedList* InitLL()
{
    LinkedList* l = NULL;
    while (l == NULL)
        l = new LinkedList();
    l->Head = NULL;
    l->Tail = NULL;
    return l;
}

void PrintLinkedList(LinkedList list)
{
    if (list.Head == NULL || list.Tail == NULL)
    {
        cout << "[](" << &list << ")" << endl;
        return;
    }

    PNode cur = list.Head;

    cout << endl;
    while (true)
    {
        cout << "[" << cur->Data->Name << "\t" << cur->Data->Amount << "]\t(" << cur << ")";
        if (cur->Next != NULL)
        {
            cout << "\n";
            cur = cur->Next;
        }
        else
            break;
    }
    cout << endl
         << endl;
}

PProduct NewProduct(string name, int amount)
{
    PProduct pp = NULL;
    while (pp==NULL)
        pp = new Product();
    pp->Amount = amount;
    pp->Name = name;
    return pp;
}

PNode NewNode(PProduct data)
{
    PNode pn = NULL;
    while (pn == NULL)
        pn = new Node();
    pn->Data = data;
    pn->Next = NULL;
    return pn;
}

void AddNodeAfterNode(LinkedList *list, PNode pn, PNode newNode)
{
    cout << "add new value: " << newNode->Data << endl;

    if (pn != NULL)
        cout << pn->Data << endl
             << pn->Next << endl;

    if (pn == NULL)
    {
        list->Head = newNode;
        list->Tail = newNode;
        return;
    }
    newNode->Next = pn->Next;
    pn->Next = newNode;

    if (list->Tail == pn)
        list->Tail = newNode;
}

Position FindProductByName(LinkedList *list, string name)
{
    Position pos = (Position){NULL, NULL};
    if (list->Head == NULL)
        return pos;

    PNode cur = list->Head;

    // search
    while (cur != NULL && cur->Data->Name.compare(name) != 0)
    {
        pos.Pre = cur;
        cur = cur->Next;
    }
    if (cur != NULL)
        pos.Cur = cur;
    return pos;
}

void DeleteProductByName(LinkedList *list, string name)
{
    Position pos = FindProductByName(list, name);
    cout << pos.Pre << " " << pos.Cur << endl;
    if (pos.Cur == NULL)
        return;

    if (pos.Pre == NULL)
    {
        if (pos.Cur == list->Tail)
        {
            *list = (LinkedList){NULL, NULL};
            delete pos.Cur;
            return;
        }
        if (pos.Cur->Data->Name.compare(name) == 0)
        {
            list->Head = pos.Cur->Next;
            delete pos.Cur;
            return;
        }
    }

    // delete
    pos.Pre->Next = pos.Cur->Next;
    if (pos.Cur == list->Tail)
        list->Tail = pos.Pre;
    delete pos.Cur;
}

LinkedList* CloneLL(LinkedList *list)
{
    if (list == NULL) {
        return NULL;
    }

    LinkedList* newList = InitLL();

    PNode cur = list->Head;
    PNode pre = NULL;
    while (cur != NULL)
    {
        PNode newNode = NewNode(cur->Data);
        if (pre != NULL)
            pre->Next = newNode;
        else
            newList->Head = newNode;
        
        pre = newNode;
        cur = cur->Next;
    }
    newList->Tail = pre;
    return newList;
}

LinkedList* InitSampleLL(string names[], int amount[], int n)
{
    LinkedList* l = InitLL();
    for (int i=0; i<n; i++)
    {
        PProduct pp = NewProduct(names[i], amount[i]);
        AddNodeAfterNode(l, l->Tail, NewNode(pp));
    }
    return l;
}

LinkedList *JoinLL(LinkedList* orgL1, LinkedList* orgL2)
{
    LinkedList* l1 = CloneLL(orgL1);
    LinkedList* l2 = CloneLL(orgL2);

    if (l1 == NULL || l1->Head == NULL)
        return l2;
    if (l2 == NULL || l2->Head == NULL)
        return l1;
    l1->Tail->Next = l2->Head;
    l1->Tail = l2->Tail;
    return l1;
}

PProduct InputProduct()
{
    string name;
    int amount = 0;
    printf("name  : ");
    cin >> name;
    printf("amount: ");
    scanf("%d", &amount);
    PProduct pp = NewProduct((string)name, amount);
    return pp;
}

int Length(LinkedList *l)
{
    if (l == NULL || l->Head == NULL)
        return 0;
    int count = 0;
    for (PNode cur = l->Head; cur != NULL; cur = cur->Next)
        count++;
    return count;
}

void insertSort(LinkedList *l)
{
    int n = Length(l);
    if (n <= 1)
        return;

    for (PNode cur = l->Head; cur != NULL && cur->Next != NULL; cur = cur->Next)
    {
        // get a value from unsorted array
        PNode unsortedNode = cur->Next;


        PNode insertPoint = NULL;
        for (PNode tmp = l->Head; tmp != unsortedNode; tmp = tmp->Next)
            if (unsortedNode->Data->Name.compare(tmp->Data->Name) > 0)
                insertPoint = tmp;
            else
                break;


        // move unsortedNode to after insertPoint
        cur->Next = unsortedNode->Next;

        if (insertPoint == NULL)
        {
            unsortedNode->Next = l->Head;
            l->Head = unsortedNode;
        }
        else
        {
            unsortedNode->Next = insertPoint->Next;
            insertPoint->Next = unsortedNode;
        }
        while (l->Tail->Next != NULL)
            l->Tail = l->Tail->Next;
    }
    return;
}

void SortByNameAsc(LinkedList* l)
{
    insertSort(l);
}

void AddProductToStorage(LinkedList* l)
{
    printf("Please provide info of product:\n");
    PProduct pp = InputProduct();
    Position pos = FindProductByName(l, pp->Name);
    // case1: product not exist, just add to list
    if (pos.Cur == NULL)
        return AddNodeAfterNode(l, l->Tail, NewNode(pp));
    
    // case 2: product has been existing, update amount
    pos.Cur->Data->Amount += pp->Amount;
    return;
}

void GetProductFromStorage(LinkedList *l)
{
    printf("Which product do you wanna get?\n");
    PProduct pp = InputProduct();
    Position pos = FindProductByName(l, pp->Name);
    // case1: product not exist, just add to list
    if (pos.Cur == NULL)
    {
        printf("Product not found\n");
        return;
    }
    // case 2: product has been existing, update amount
    pos.Cur->Data->Amount -= pp->Amount;
    if (pos.Cur->Data->Amount < 0)
        pos.Cur->Data->Amount = 0;
    return;
}

void StopSellingProduct(LinkedList *l)
{
    printf("Which product do you wanna stop selling?\n");
    string name = "";
    printf("name: ");
    cin >> name;
    Position pos = FindProductByName(l, name);
    // case1: product not exist, just add to list
    if (pos.Cur == NULL)
    {
        printf("Product not found\n");
        return;
    }
    // case 2: product has been existing, update amount
    DeleteProductByName(l, name);
    return;
}

int main()
{
    LinkedList* l = InitLL();

    int choice = -1;
    while (choice != 0)
    {
        printf("What do you want to do?\n");
        printf("[0] Exit\n");
        printf("[1] Print list of products order by name asc\n");
        printf("[2] Add product to storage\n");
        printf("[3] Get product from storage\n");
        printf("[4] Stop selling a product\n");
        
        scanf("%d", &choice);

        switch (choice)
        {
        case 0:
            return 0;
            break;

        case 1:
            PrintLinkedList(*l);
            SortByNameAsc(l);
            PrintLinkedList(*l);
            break;

        case 2:
            AddProductToStorage(l);
            break;

        case 3:
            GetProductFromStorage(l);
            break;

        case 4:
            StopSellingProduct(l);
            break;

        default:
            // do nothing, just loop again
            break;
        }
        printf("\n\n");
    }
}
