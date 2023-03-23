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
    void migrateRootPage(Page* left, Item* middleItem, Page* right);
    void migrateNonRootPage(Page* left, Item* middleItem, Page* right);

public:
    int Order;
    Page* LeftPage;
    vector<Item*> Elems;
    Page* Parent;

    Page(int order, int data, Page* parent);
    ~Page();
    int FirstData();
    int LastData();
    Item* Insert(Item* item);
    void AssignPage(Page* page);
    void Break();
    void CheckToBreak();
    Item* SearchToInsert(int data);
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
    this->Freq = 1;
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
Page::Page(int order, int data, Page* parent) {
    this->Order = order;
    this->LeftPage = NULL;
    this->Parent = parent;
    this->Elems.push_back(new Item(data, NULL));
}
Page::~Page() {
    if (this->LeftPage != NULL)
        delete this->LeftPage;

    for (int i=0; i<this->Elems.size(); i++) 
        delete this->Elems[i];
}
void Page::migrateRootPage(Page* left, Item* middleItem, Page* right) {
    this->Elems.clear();
    this->Elems.push_back(middleItem);
    this->Elems.at(0)->RightPage = right;
    this->LeftPage = left;
}
void Page::AssignPage(Page* page) {
    if (page->LastData() < this->FirstData()) {
        this->LeftPage = page;
        return;
    }
    for (int i=0; i<page->Elems.size(); i++) {
        if (page->LastData() > this->Elems.at(i)->Data)
            continue;
        this->Elems.at(i-1)->RightPage = page;
        return;
    }
    this->Elems.back()->RightPage = page;
}
void Page::migrateNonRootPage(Page* left, Item* middleItem, Page* right) {
    Item* parentItem = this->Parent->Insert(middleItem);
    parentItem->RightPage = right;
    this->Parent->AssignPage(left);
}
void Page::Break() {
    Page* pageLeft = new Page(this->Order, 0, this);
    Page* pageRight = new Page(this->Order, 0, this);
    pageLeft->Elems.assign(
        this->Elems.begin(), 
        this->Elems.begin()+this->Order
    );
    pageRight->Elems.assign(
        this->Elems.end()-this->Order,
        this->Elems.end()
    );
    Item* middleItem = this->Elems.at(this->Order);
    if (this->Parent == NULL) {
        cout << "migrate root" << endl;
        this->migrateRootPage(pageLeft, middleItem, pageRight);
        return;
    }
    this->migrateNonRootPage(pageLeft, middleItem, pageRight);
}
void Page::CheckToBreak() {
    if (this->Elems.size() > 2*this->Order) {
        cout << "let's break" << endl;
        this->Break();
    }
}
Item* Page::SearchToInsert(int data) {
    Item* newItem = new Item(data, NULL);
    if (data < this->FirstData()) {
        if (this->LeftPage!= NULL) {
            // continue searching on the leff branch
            return this->LeftPage->SearchToInsert(data);
        }
        return this->Insert(newItem);
    }
    // Check on every item
    for (int i=0; i<this->Elems.size(); i++) {
        Item* item = this->Elems.at(i);

        // data is alreay in page
        if (data == item->Data) {
            item->Freq++;
            return item;
        }

        // skip if data is larger than item
        if (data > item->Data) 
            continue;

        // Continue searching on the right branch of pre-item
        Item* preItem = this->Elems.at(i-1);
        if (preItem->RightPage != NULL) {
            return preItem->RightPage->SearchToInsert(data);
        }

        // Insert to this page
        return this->Insert(newItem);
    }
    // if data is larger than all items
    // Continue searching on the right branch
    if (this->Elems.back()->RightPage != NULL) {
        return this->Elems.back()->RightPage->SearchToInsert(data);
    }
    
    // Or insert to this page
    return this->Insert(newItem);
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
int Page::LastData() {
    return this->Elems.back()->Data;
}
Item* Page::Insert(Item* newItem) {
    for (int i=0; i<this->Elems.size(); i++) {
        Item* item = this->Elems.at(i);
        if (newItem->Data > item->Data) {
            continue;
        }
        if (newItem->Data == item->Data) {
            item->Freq++;
            return item;
        }

        this->Elems.insert(Elems.begin()+i, newItem);
        this->CheckToBreak();
        return newItem;
    }

    this->Elems.push_back(newItem);
    this->CheckToBreak();
    return newItem;
};



// ----------------------------------
BTreeN::BTreeN(int order, int data) {
    this->Order = order;
    this->Root = new Page(order, data, NULL);
    this->Root->Parent = NULL;
}
BTreeN::~BTreeN() {
    this->Order = 0;
    if (this->Root != NULL)
        delete this->Root;
}
void BTreeN::Insert(int data) {
    this->Root->SearchToInsert(data);
}
void BTreeN::Draw() {
    cout << "-------------------" << endl;
    this->Root->Draw(0);
}

int main () {
    // Page* page010 = new Page(2, 10); page010->Insert(20);
    // Page* page040 = new Page(2, 40); page040->Insert(50);
    // Page* page070 = new Page(2, 70); page070->Insert(80); page070->Insert(90);
    // Page* page035 = new Page(2, 35); page035->Insert(65);
    // page035->LeftPage = page010;
    // page035->Elems.at(0)->RightPage = page040;
    // page035->Elems.at(1)->RightPage = page070;

    // Page* page110 = new Page(2, 110); page110->Insert(120);
    // Page* page140 = new Page(2, 140); page140->Insert(160);
    // Page* page190 = new Page(2, 190); page190->Insert(240); page190->Insert(260);
    // Page* page130 = new Page(2, 130); page130->Insert(180);
    // page130->LeftPage = page110;
    // page130->Elems.at(0)->RightPage = page140;
    // page130->Elems.at(1)->RightPage = page190;

    // Page* page100 = new Page(2, 100);
    // page100->LeftPage = page035;
    // page100->Elems.at(0)->RightPage = page130;
    
    // page100->Draw(0);

    

    BTreeN* tree = new BTreeN(2, 20);
    tree->Draw();
    tree->Insert(40);
    tree->Draw();
    tree->Insert(10);
    tree->Draw();
    tree->Insert(30);
    tree->Draw();
    tree->Insert(15);
    tree->Draw();
    tree->Insert(35);
    tree->Draw();
    tree->Insert(7);
    tree->Draw();
    tree->Insert(26);
    tree->Draw();
    tree->Insert(18);
    tree->Draw();
    tree->Insert(22);
    tree->Draw();
    tree->Insert(5);
    tree->Draw();
    tree->Insert(42);
    tree->Draw();
    tree->Insert(13);
    tree->Draw();
    tree->Insert(46);
    tree->Draw();
    tree->Insert(27);
    tree->Draw();
    tree->Insert(8);
    tree->Draw();
    tree->Insert(32);
    // tree->Insert(38);
    // tree->Insert(24);
    // tree->Insert(45);
    // tree->Insert(25);
    tree->Draw();
}
