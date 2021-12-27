#include <math.h>
#include <stdio.h>

#include <iostream>
#include <string>

using namespace std;

struct Node
{
    int Data;
    Node* Next;
};

typedef Node* PNode;

typedef struct
{
    PNode Head;
    PNode Tail;
} LinkedList;

PNode NewNode(int x)
{
    PNode pn = NULL;
    while (pn == NULL)
        pn = new Node();
        pn->Data = x;
    return pn;
}

LinkedList *InitLL()
{
    LinkedList *l = NULL;
    while (l == NULL)
        l = new LinkedList();
    l->Head = NULL;
    l->Tail = NULL;
    return l;
}

void View(LinkedList list)
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
        cout << "[" << cur->Data << "](" << cur << ")";
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

void InsertHead(LinkedList* l, int x)
{
    PNode newNode = NewNode(x);
    if (l->Head == NULL) {
        l->Head =l->Tail = newNode;
        return;
    }
    newNode->Next = l->Head;
    l->Head = newNode;
}

LinkedList* CreateList()
{
    int num = 0;
    int n = 0;
    printf("total number of elements: n = ");
    scanf("%d", &n);

    LinkedList* l = InitLL();

    for (int i=0; i<n; i++) {
        cout << "number #" << i+1 << " = ";
        scanf("%d", &num);
        InsertHead(l, num);
    }
    return l;
}

void EditList(LinkedList* l, int from, int to)
{
    PNode cur = l->Head;
    for ( ;cur != NULL; cur = cur->Next)
        if (cur->Data == from)
            cur->Data = to;
}

void SplitList(
    LinkedList* l,
    LinkedList* gt,
    LinkedList* lte,
    int x)
{
    PNode cur = l->Head;
    for ( ; cur != NULL; cur = cur->Next)
        if (cur->Data <= x)
            InsertHead(lte, cur->Data);
        else
            InsertHead(gt, cur->Data);
}

void AddNodeAfterNode(LinkedList *l, PNode pn, PNode newNode)
{
    if (pn == NULL)
    {
        l->Head = newNode;
        l->Tail = newNode;
        return;
    }
    newNode->Next = pn->Next;
    pn->Next = newNode;

    if (l->Tail == pn)
        l->Tail = newNode;
}

LinkedList *InitSampleLL(int num[], int n)
{
    LinkedList *l = InitLL();
    for (int i = 0; i < n; i++)
        AddNodeAfterNode(l, l->Tail, NewNode(num[i]));
    return l;
}

PNode FindLastX(LinkedList* l, int x)
{
    PNode pre = NULL;
    PNode ret = NULL;
    PNode cur = l->Head;

    for (; cur != NULL; cur = cur->Next)
    {
        if (cur->Data ==  x)
            ret = pre;
        pre = cur;
    }

    return ret;
}

void DeleteLastX(LinkedList* l, int x)
{
    // check to ignore empty list
    if (l == NULL || l->Head == NULL)
        return;

    if (l->Head == l->Tail && l->Head->Data == x) {
        l->Head = l->Tail = NULL;
    }

    PNode pre = FindLastX(l, x);

    // node is at head
    if (pre == NULL)
    {
        PNode tmp = l->Head;
        l->Head = l->Head->Next;
        delete tmp;
        return;
    }

    // not found
    if (pre->Next == NULL)
        return;

    // node is in middle
    pre->Next = pre->Next->Next;

    // node is at tail
    if (pre->Next == l->Tail)
        l->Tail = pre;

    delete pre->Next;
}

int main()
{
    // run

    printf("\n\nb/ Delete last node that has given value: 3\n");
    int a[] = {6, 8, 4, 4, 7};
    LinkedList* sample = InitSampleLL(a, sizeof(a)/sizeof(a[0]));
    printf("List before deleting:\n");
    View(*sample);

    DeleteLastX(sample, 3);
    printf("List after deleting:\n");
    View(*sample);
    return 0;
}