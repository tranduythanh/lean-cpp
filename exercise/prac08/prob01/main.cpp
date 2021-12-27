#include <math.h>
#include <stdio.h>

#include <iostream>
#include <string>

using namespace std;

struct Node {
    char data;
    Node* Next;
    Node* Pre;
};

typedef Node* PNode;

typedef struct {
    PNode Head;
    PNode Tail;
} Stack;

Stack* InitEmptyStack() {
    Stack* ret = NULL;
    while (ret == NULL)
        ret = new Stack();
    return ret;
}

PNode NewNode(char data) {
    PNode ret = NULL;
    while (ret == NULL)
    {
        ret = new Node();
        ret->data = data;
    }
    return ret;
}

void Push(Stack* s, char data) {
    PNode newNode = NewNode(data);
    if (s->Head == NULL) {
        s->Head = newNode;
        s->Tail = newNode;
        return;
    }
    newNode->Pre = s->Tail;
    s->Tail->Next = newNode;
    s->Tail = newNode;
}

char Pop(Stack* s) {
    if (s->Head == NULL)
        return '\0';
    PNode tail = s->Tail;
    s->Tail = s->Tail->Pre;
    return tail->data;
}

void PrintStack(Stack* s) {
    if (s == NULL || s->Head == NULL)
        return;
    PNode tail = s->Tail;
        cout << "stack:";
    while (tail != NULL) {
        cout << tail->data;
        tail = tail->Pre;
    }
    cout << endl;
}

int main() {
    // input
    Stack* s = InitEmptyStack();
    printf("input characters\n");
    char in = '\0';
    while (true) {
        scanf("%c", &in);
        // in = 'A';
        if (in == '.')
            break;
        if (in == '\n')
            continue;
        if (in == '*')
        {
            Pop(s);
            PrintStack(s);
        }
        else
        {
            Push(s, in);
            PrintStack(s);
        }
    }

    // calculate & print
    PrintStack(s);
    return 0;
}