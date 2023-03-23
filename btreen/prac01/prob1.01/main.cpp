#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cassert> 

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
    void SetRightPage(int elemIdx, Page* page);
    void SearchToAssignPage(Page* page);
    void Break();
    void CheckToBreak();
    Item* SearchToInsert(int data);
    void Draw(int level);
    void DebugItems();
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
    
    this->SetRightPage(0, right);
    
    this->LeftPage = left;
    left->Parent=this;
}
void Page::SetRightPage(int elemIdx, Page* page) {
    this->Elems.at(elemIdx)->RightPage = page;
    page->Parent = this;
}
void Page::SearchToAssignPage(Page* page) {
    cout << "first data: " << this->FirstData() << endl;
    cout << "last  data: " << page->LastData() << endl;

    if (page->LastData() < this->FirstData()) {
        this->LeftPage = page;
        return;
    }

    cout << "elems size: " << this->Elems.size() << endl;
    for (int i=0; i<this->Elems.size(); i++) {
        if (page->LastData() > this->Elems.at(i)->Data) {
            cout << "** elem data: " << this->Elems.at(i)->Data << endl;
            cout << "   last data: " << page->LastData() << endl;
            cout << "continue" << endl;
            continue;
        }
        
        this->SetRightPage(i-1, page);
        return;
    }
    this->SetRightPage(this->Elems.size()-1, page);
}
void Page::migrateNonRootPage(Page* left, Item* middleItem, Page* right) {
    assert(this->Parent != NULL);
    assert(this->Elems.size() == 2*this->Order+1);

    cout << "Draw relative Parent" << endl;
    this->Parent->DebugItems();
    cout << "====================" << endl;

    Item* parentItem = this->Parent->Insert(middleItem);
    
    assert(right->Elems.size() == 2);
    assert(left->Elems.size() == 2);

    
    parentItem->RightPage = right;
    right->Parent = this->Parent;

    assert(right->Elems.size() == 2);
    assert(left->Elems.size() == 2);

    cout << "left  "; left->DebugItems();
    cout << "right "; right->DebugItems();

    this->Parent->SearchToAssignPage(left);
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
    cout << "migrate non root" << endl;
    cout << "left" << endl;
    pageLeft->Draw(0);
    cout << "middle" << endl;
    middleItem->Draw(0);
    cout << "right" << endl;
    pageRight->Draw(0);
    this->migrateNonRootPage(pageLeft, middleItem, pageRight);
}
void Page::CheckToBreak() {
    if (this->Elems.size() > 2*this->Order) {
        this->Draw(0);
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
void Page::DebugItems() {
    for (int i=0; i<this->Elems.size(); i++) {
        cout << this->Elems.at(i)->Data << ",";
    }
    cout << endl;
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
