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

Position FindNode(LinkedList list, int data)
{
    Position pos = (Position){NULL, NULL};
    if (list.Head == NULL)
        return pos;

    PNode cur = list.Head;

    // search
    while (cur != NULL && cur->Data != data)
    {
        pos.Pre = cur;
        cur = cur->Next;
    }
    if (cur != NULL)
        pos.Cur = cur;
    return pos;
}

void DeleteNode(LinkedList *list, int data)
{
    Position pos = FindNode(*list, data);
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
        if (pos.Cur->Data == data)
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

LinkedList* InitSampleLL(int arr[], int n)
{
    LinkedList* l = InitLL();
    for (int i=0; i<n; i++)
        AddNodeAfterNode(l, l->Tail, NewNode(arr[i]));
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

int Length(LinkedList* l)
{
    if (l == NULL || l->Head == NULL)
        return 0;
    int count = 0;
    for (PNode cur = l->Head; cur != NULL; cur = cur->Next)
        count++;
    return count;
}

int main()
{
    cout << "prob1 ===============================" << endl;
    LinkedList* list = InitLL();

    string rcv = "";
    cout << "add new node" << endl;
    while (true)
    {
        getline(cin, rcv);
        if (rcv.length() == 0)
            break;
        int num = stoi(rcv);

        PNode newNode = NewNode(num);
        AddNodeAfterNode(list, list->Tail, newNode);
        PrintLinkedList(*list);
    };

    cout << "delete node" << endl;
    while (true)
    {
        getline(cin, rcv);
        if (rcv.length() == 0)
            break;
        int num = stoi(rcv);

        DeleteNode(list, num);
        PrintLinkedList(*list);
    };

    cout << "prob2 ===============================" << endl;
    LinkedList* clonedLL = CloneLL(list);
    PrintLinkedList(*clonedLL);

    cout << "prob3 ===============================" << endl;
    int arr1[] = {1,2,3,4};
    int n1 = sizeof(arr1)/sizeof(arr1[0]);
    int arr2[] = { 7,8 };
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    LinkedList* l1 = InitSampleLL(arr1, n1);
    LinkedList* l2 = InitSampleLL(arr2, n2);

    LinkedList* joinedLL = JoinLL(l1, l2);
    PrintLinkedList(*joinedLL);

    cout << "prob4 ===============================" << endl;
    int lengthJoinedLL = Length(joinedLL);
    cout << "length of joined LL = " << lengthJoinedLL << endl;

    return 0;
}
