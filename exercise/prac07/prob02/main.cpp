#include "stdio.h"
#include "math.h"
#include <iostream>
#include <string>
using namespace std;


typedef struct node
{
    int Data;
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
        cout << "[" << cur->Data << "]"
        // << "(" << cur << ")"
        ;
        if (cur->Next != NULL)
        {
            cout << "-->";
            cur = cur->Next;
        }
        else
            break;
    }
    cout << endl
         << endl;
}

PNode NewNode(int data)
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

Position FindNode(LinkedList *list, int num)
{
    Position pos = (Position){NULL, NULL};
    if (list->Head == NULL)
        return pos;

    PNode cur = list->Head;

    // search
    while (cur != NULL && cur->Data != num)
    {
        pos.Pre = cur;
        cur = cur->Next;
    }
    if (cur != NULL)
        pos.Cur = cur;
    return pos;
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

LinkedList* InitSampleLL(int num[], int n)
{
    LinkedList* l = InitLL();
    for (int i=0; i<n; i++)
        AddNodeAfterNode(l, l->Tail, NewNode(num[i]));
    return l;
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

    for (PNode cur = l->Head; cur != NULL && cur->Next != NULL; )
    {
        // get a value from unsorted array
        PNode unsortedNode = cur->Next;


        PNode insertPoint = NULL;
        for (PNode tmp = l->Head; tmp != unsortedNode; tmp = tmp->Next)
            if (unsortedNode->Data > tmp->Data)
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


        // correct list tail
        while (l->Tail->Next != NULL)
            l->Tail = l->Tail->Next;

        if (insertPoint == cur)
            cur = cur->Next;
    }
    return;
}

bool isLocalMinimum(PNode pre, PNode cur)
{
    if (pre == NULL)
    {
        if (cur->Next == NULL)
            return true;
        return cur->Data < cur->Next->Data;
    }

    if (cur->Next == NULL)
        return pre->Data > cur->Data;

    return  (pre->Data > cur->Data) &&
            (cur->Data < cur->Next->Data);
}

bool isLocalMaximum(PNode pre, PNode cur)
{
    if (pre == NULL)
    {
        if (cur->Next == NULL)
            return true;
        return cur->Data > cur->Next->Data;
    }

    if (cur->Next == NULL)
        return pre->Data < cur->Data;

    return (pre->Data < cur->Data) &&
           (cur->Data > cur->Next->Data);
}

void printLocalOptimums(LinkedList* l)
{
    if (l == NULL || l->Head == NULL)
    {
        printf("empty list ==> nothing to print");
        return;
    }

    string maxElem = "";
    string minElem = "";

    PNode pre = NULL;
    PNode cur = l->Head;
    while (cur != NULL)
    {
        if (isLocalMaximum(pre, cur))
            maxElem = maxElem.append("\t").append(to_string(cur->Data));

        if (isLocalMinimum(pre, cur))
            minElem = minElem.append("\t").append(to_string(cur->Data));

        pre = cur;
        cur = cur->Next;
    }
    cout << "Local maximum: " << maxElem << endl;
    cout << "Local minimum: " << minElem << endl;
}

void DeleteNodeAfterNode(LinkedList* l, PNode pn)
{
    if (l == NULL || l->Head == NULL || pn->Next == NULL)
        return;

    // case: delete head node
    if (pn == NULL)
    {
        delete l->Head;
        l->Head = l->Head->Next;
    }

    // case: delete a node that is not a head
    PNode tmp = pn->Next;
    pn->Next = pn->Next->Next;
    if (l->Tail == pn->Next)
        l->Tail = pn;
    delete tmp;
}

void unique(LinkedList* l)
{
    if (l == NULL || l->Head == NULL)
    {
        printf("empty list ==> nothing to do");
        return;
    }

    for (PNode anchor = l->Head; anchor != NULL; anchor = anchor->Next)
        for (PNode tmp = anchor; tmp != NULL && tmp->Next != NULL; tmp = tmp->Next)
            if (tmp->Next->Data == anchor->Data)
                DeleteNodeAfterNode(l, tmp);
}

bool isPrime(int x)
{
    if (x <= 0)
        return false;

    for (int i=2; i<x/2+1; i++)
        if (x%i==0)
            return false;

    return true;
}

int nextPrimeOf(int x)
{
    for (x++;!isPrime(x);x++);
    return x;
}

void gameOfPrime(LinkedList* l)
{
    if (l == NULL || l->Head == NULL)
    {
        printf("empty list ==> nothing to do");
        return;
    }

    for (PNode tmp = l->Head; tmp != NULL; tmp = tmp->Next)
        if (isPrime(tmp->Data))
            tmp->Data = nextPrimeOf(tmp->Data);
}

Position minNodeValueAfterNode(LinkedList* l, PNode node)
{
    if (l == NULL)
        return (Position){NULL, NULL};

    if (l->Head == l->Tail)
        return (Position){NULL, l->Head};

    if (node->Next == NULL)
        return (Position){node, NULL};

    int minValue = node->Data;

    PNode pre = node;
    node = node->Next;
    while (node != NULL)
        if (node->Data < pre->Data)
        {
            minValue = node->Data;
            pre = node;
            node = node->Next;
        }
    return (Position) {pre, node};
}

void selectionSort(LinkedList* l)
{
    if (l == NULL || l->Head == NULL)
        return;

    PNode sorted = l->Head;
    while (true) {
        Position pos = minNodeValueAfterNode(l, sorted);
        if (pos.Cur == NULL)
            break;

        if (pos.Cur == l->Tail)
            l->Tail = pos.Pre;

        pos.Pre->Next = pos.Cur->Next;
        pos.Cur->Next= sorted->Next;
        sorted->Next = pos.Cur;
    }
}

void AddNodeToListDesc(LinkedList* l, int data)
{
    if (l == NULL)
        return;
    if (l->Head == NULL)
        AddNodeAfterNode(l, l->Tail, NewNode(data));

    PNode cur = l->Head;
    while (cur != NULL)
    {
        if (data <= cur->Data)
            AddNodeAfterNode(l, cur, NewNode(data));
        cur = cur->Next;
    }
}

void buildList()
{
    LinkedList* l = InitLL();
    int n = 0;
    scanf("n = %d", &n);

    for (int i=0; i<n; i++)
    {
        int s = 0;
        scanf("element %d", &s);
        AddNodeToListDesc(l, s);
    }
}

int main()
{
    int arr[] = {9, 1, 2, 8, 9, 75, 65, 9, 81, 36, 47, 61, 9, 55, 63, 9};
    LinkedList *l = InitSampleLL(arr, sizeof(arr)/sizeof(arr[0]));

    // find local max, local min
    printf("a. Print local optimums =====================================\n");
    printLocalOptimums(l);

    printf("\n\n");
    printf("b. Remove duplicated items ==================================\n");
    PrintLinkedList(*l);
    unique(l);
    PrintLinkedList(*l);

    printf("\n\n");
    printf("c. Game of Prime ============================================\n");
    gameOfPrime(l);
    PrintLinkedList(*l);

    printf("\n\n");
    printf("d. Insertion sort ===========================================\n");
    insertSort(l);
    PrintLinkedList(*l);

    printf("\n\n");
    printf("e. Selection sort ===========================================\n");
    selectionSort(l);
    PrintLinkedList(*l);

    printf("\n\n");
    printf("f. Build list ===============================================\n");
    buildList();
    return 0;
}
