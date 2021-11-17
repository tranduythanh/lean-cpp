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

void DeleteNode(LinkedList* list, int data)
{
    if (list->Head == NULL)
        return;
    
    if (list->Head == list->Tail)
        if (list->Head->Data == data)
        {
            *list = (LinkedList) { NULL, NULL };
            return;
        }


    PNode pre = NULL;
    PNode cur = list->Head;

    // Check case node deleted is the head
    if (cur->Data == data)
    {
        list->Head = cur->Next;
        delete cur;
        return;
    }

    // search
    while (cur->Data != data)
    {
        pre = cur;
        cur = cur->Next;

        if (cur == NULL)
            return; // node not exist, just return
    }
    // delete
    pre->Next = cur->Next;
    delete cur;
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

