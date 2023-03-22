#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

// forward declaration
class Item;

class Page
{
private:
    void naivelyRemoveItem(int data);
    void migrateRootPage(Page* left, int data, Page* right);
    void migrateNonRootPage(Page* left, int data, Page* right);

public:
    int Order;
    Page* LeftPage;
    vector<Item*> Elems;
    Page* Parent;

    Page(int order, int data);
    ~Page();
    int FirstData();
    void InsertItem(int data);
    void Break();
    void CheckToBreak();
    void Draw(int level);
};


class Item
{
private:
public:
    int   Data;
    Page* RightPage;
    int   Freq;
    Item(int data, Page* rightPage);
    ~Item();
    void Draw(int level);
};

class BTreeN
{
private:
public:
    int   Order;
    Page* Root;
    BTreeN(int order, int data);
    ~BTreeN();
    void Insert(int data);
    void Draw();
};

// ==============================================================
// Implement methods
// ==============================================================

Item::Item(int data, Page* rightPage) {
    this->Data = data;
    this->Freq = 0;
    this->RightPage = rightPage;
}
Item::~Item() {
    if (this->RightPage != NULL)
        delete this->RightPage;
}
void Item::Draw(int level) {
    for (int i=0; i<level; i++)
        cout << "\t";
    cout << this->Data << endl;
}



// ----------------------------------
Page::Page(int order, int data) {
    this->Order = order;
    this->LeftPage = NULL;
    this->Elems.push_back(new Item(data, NULL));
}
Page::~Page() {
    if (this->LeftPage != NULL)
        delete this->LeftPage;

    for (int i=0; i<this->Elems.size(); i++) 
        delete this->Elems[i];
}
void Page::naivelyRemoveItem(int data) {
    for (int i=0; i<this->Elems.size(); i++) {
        if (this->Elems.at(i)->Data != data)
            continue;
        
        delete this->Elems[i];
        this->Elems.erase(this->Elems.begin() + i);
        return;
    }
}
void Page::Break() {
    Page* page1st = new Page(this->Order, 0);
    Page* page2nd = new Page(this->Order, 0);
    page1st->Elems.assign(
        this->Elems.begin(), 
        this->Elems.begin()+this->Order
    );
    page2nd->Elems.assign(
        this->Elems.end()-this->Order,
        this->Elems.end()
    );    
    cout << "page1st, page2nd " << endl;
    page1st->Draw(0);
    page2nd->Draw(0);
}
void Page::CheckToBreak() {
    if (this->Elems.size() > this->Order)
        this->Break();
}
void Page::Draw(int level) {
    if (this->LeftPage!= NULL)
        this->LeftPage->Draw(level+1);
    
    // print "--" at the begin of each level
    for (int i=0; i<level+1; i++)
        cout << "\t";
    cout << "--" << endl;

    for (int i=0; i<this->Elems.size(); i++) {
        this->Elems[i]->Draw(level+1);
        
        // print "--" at the end of each level
        if (i==this->Elems.size()-1) {
            for (int i=0; i<level+1; i++)
                cout << "\t";
            cout << "--" << endl;
        }

        if (this->Elems[i]->RightPage != NULL)
            this->Elems[i]->RightPage->Draw(level+1);
    }
}
int Page::FirstData() {
    return this->Elems.at(0)->Data;
}
void Page::InsertItem(int data) {
    Item* newItem = new Item(data, NULL);
    newItem->Freq = 1;

    for (int i=0; i<this->Elems.size(); i++) {
        if (data > this->Elems.at(i)->Data) {
            continue;
        }
        if (data == this->Elems.at(i)->Data) {
            this->Elems.at(i)->Freq++;
            return;
        }

        this->Elems.insert(Elems.begin()+i, newItem);
        this->CheckToBreak();
        return;
    }

    this->Elems.push_back(newItem);
    this->CheckToBreak();
    return;
};



// ----------------------------------
BTreeN::BTreeN(int order, int data) {
    this->Order = order;
    this->Root = new Page(order, data);
    this->Root->Parent = NULL;
}
BTreeN::~BTreeN() {
    this->Order = 0;
    if (this->Root != NULL)
        delete this->Root;
}
void BTreeN::Insert(int data) {
    this->Root->InsertItem(data);
}
void BTreeN::Draw() {
    this->Root->Draw(0);
}

int main () {
    Page* page010 = new Page(2, 10); page010->InsertItem(20);
    Page* page040 = new Page(2, 40); page040->InsertItem(50);
    Page* page070 = new Page(2, 70); page070->InsertItem(80); page070->InsertItem(90);
    Page* page035 = new Page(2, 35); page035->InsertItem(65);
    page035->LeftPage = page010;
    page035->Elems.at(0)->RightPage = page040;
    page035->Elems.at(1)->RightPage = page070;

    Page* page110 = new Page(2, 110); page110->InsertItem(120);
    Page* page140 = new Page(2, 140); page140->InsertItem(160);
    Page* page190 = new Page(2, 190); page190->InsertItem(240); page190->InsertItem(260);
    Page* page130 = new Page(2, 130); page130->InsertItem(180);
    page130->LeftPage = page110;
    page130->Elems.at(0)->RightPage = page140;
    page130->Elems.at(1)->RightPage = page190;

    Page* page100 = new Page(2, 100);
    page100->LeftPage = page035;
    page100->Elems.at(0)->RightPage = page130;
    
    page100->Draw(0);



    BTreeN* tree = new BTreeN(2, 100);
    tree->Insert(35);
    tree->Insert(65);
    tree->Insert(130);
    tree->Insert(180);
    tree->Draw();
}
