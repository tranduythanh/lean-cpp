#include "stdio.h"
#include "math.h"
#include <iostream>
#include <string>
using namespace std;

typedef struct node {
    int Data;
    node *Next;
} Node;

typedef Node* PNode;

typedef struct {
    PNode Head;
    PNode Tail;
} LinkedList;

typedef struct {
    PNode Pre;
    PNode Cur;
} Position;

void PrintLinkedList(LinkedList list)
{
    if (list.Head == NULL || list.Tail == NULL)
    {
        cout << "[]" << endl;
        return;
    }

    PNode cur = list.Head;
    
    cout << endl;
    while (true)
    {
        cout << "[" << cur->Data << "]";
        if (cur->Next != NULL)
        {
            cout << "-->";
            cur = cur->Next;
        } else
            break;
    }
    cout << endl << endl;
}

PNode NewNode(int data)
{
    PNode pn = new Node();
    pn->Data = data;
    pn->Next = NULL;
    return pn;
}

void AddNodeAfterNode(LinkedList* list, PNode pn, PNode newNode)
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

void DeleteNode(LinkedList* list, int data)
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

int main()
{

    LinkedList list = (LinkedList){NULL, NULL};
    
    string rcv = "";
    cout << "add new node" << endl;
    while (true)
    {
        getline(cin, rcv);
        if (rcv.length() == 0)
            break;
        int num = stoi(rcv);

        PNode newNode = NewNode(num);
        AddNodeAfterNode(&list, list.Tail, newNode);
        PrintLinkedList(list);
    };

    cout << "delete node" << endl;
    while (true)
    {
        getline(cin, rcv);
        if (rcv.length() == 0)
            break;
        int num = stoi(rcv);

        DeleteNode(&list, num);
        PrintLinkedList(list);
    };

    cout << "------------end------------" << endl;
}

