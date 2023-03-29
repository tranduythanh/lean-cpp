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
    void SetLeftPage(Page* page);
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
    for (int i=0; i<level; i++) cout << "\t\t\t";
    cout << this->Data << endl;
    for (int i=0; i<level; i++) cout << "\t\t\t";
    cout << "  |" << "right_page=" << "\x1B[31m[" << this->RightPage << "]\033[0m" << endl;
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
    this->SetLeftPage(left);
}
void Page::SetRightPage(int elemIdx, Page* page) {
    this->Elems.at(elemIdx)->RightPage = page;
    page->Parent = this;
}
void Page::SetLeftPage(Page* page) {
    this->LeftPage = page;
    page->Parent = this;
}
void Page::SearchToAssignPage(Page* page) {
    cout << "first data: " << this->FirstData() << endl;
    cout << "last  data: " << page->LastData() << endl;

    if (page->LastData() < this->FirstData()) {
        this->SetLeftPage(page);
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

    cout << "\n\n====================" << endl;
    cout << "middle_item.data              : " << middleItem->Data << endl;
    cout << "middle_item.right_page.elem[0]: " << middleItem->RightPage->Elems.at(0)->Data << endl;
    cout << "middle_item.right_page.parent : " << middleItem->RightPage->Parent->FirstData() << endl;

    Item* parentItem = this->Parent->Insert(middleItem);
    
    assert(right->Elems.size() == 2);
    assert(left->Elems.size() == 2);

    cout << "parent_item.data              : " << parentItem->Data << endl;
    cout << "parent_item.right_page.elem[0]: " << parentItem->RightPage->Elems.at(0)->Data << endl;
    cout << "parent_item.right_page.parent : " << parentItem->RightPage->Parent->FirstData() << endl;
    cout << "====================\n\n" << endl;
    
    parentItem->RightPage = right;
    right->Parent = this->Parent;

    assert(right->Elems.size() == 2);
    assert(left->Elems.size() == 2);

    cout << "left  "; left->DebugItems();
    cout << "right "; right->DebugItems();

    this->Parent->SearchToAssignPage(left);

    this->Parent->CheckToBreak();
}
void Page::Break() {
    Page* left = new Page(this->Order, 0, this);
    Page* right = new Page(this->Order, 0, this);
    left->Elems.assign(
        this->Elems.begin(), 
        this->Elems.begin()+this->Order
    );
    right->Elems.assign(
        this->Elems.end()-this->Order,
        this->Elems.end()
    );

    for (int i=0; i<left->Elems.size(); i++) {
        if (left->Elems.at(i)->RightPage != NULL) {
            left->Elems.at(i)->RightPage->Parent = left;
        }
    }

    for (int i=0; i<right->Elems.size(); i++) {
        if (right->Elems.at(i)->RightPage != NULL) {
            right->Elems.at(i)->RightPage->Parent = right;
        }
    }

    Item* middleItem = this->Elems.at(this->Order);

    // Inheritance ========================
    // "left" must be inherit the LeftPage of original page
    left->LeftPage = this->LeftPage;
    if (left->LeftPage!= NULL) {
        left->LeftPage->Parent = left;
    }

    //    [ 10    20      25      30     40 ]
    //     |  |     |       |       |      |
    // [...]  [...] [...] [26,27] [...]  [...]
    // 
    //                 25
    //                |   |  |
    //    [ 10    20 ]    |  [30    40]
    //     |  |     |     |
    // [...]  [...] [...] [26,27] 
    // 
    //                 25
    //                |   |
    //    [ 10    20 ]    [ 30    40]
    //     |  |     |      |
    // [...]  [...] [...]  [26,27] 
    // delegate the middle item's right page to the left of new right-page
    right->LeftPage = middleItem->RightPage;
    if (right->LeftPage!= NULL) {
        right->LeftPage->Parent = right;
    }
    // new RightPage of middle item will be the "right"
    middleItem->RightPage = right;
    // ====================================
    

    if (this->Parent == NULL) {
        cout << "migrate root" << endl;
        this->migrateRootPage(left, middleItem, right);
        return;
    }

    cout << "migrate non root" << endl;
    cout << "left" << endl;
    left->Draw(0);
    cout << "middle" << endl;
    middleItem->Draw(0);
    cout << "right" << endl;
    right->Draw(0);
    
    cout << endl;
    cout << "middle_item.data              : " << middleItem->Data << endl;
    cout << "middle_item.right_page.elem[0]: " << middleItem->RightPage->Elems.at(0)->Data << endl;
    cout << "middle_item.right_page.parent : " << middleItem->RightPage->Parent->FirstData() << endl;
    cout << endl;
    
    this->migrateNonRootPage(left, middleItem, right);
}
void Page::CheckToBreak() {
    cout << "######### check to break" << endl;
    this->DebugItems();
    if (this->Elems.size() > 2*this->Order) {
        this->Draw(0);
        cout << "######### let's break" << endl;
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

        Item* _item = this->Insert(newItem);
        this->CheckToBreak();
        return _item;
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
        Item* _item = this->Insert(newItem);
        this->CheckToBreak();
        return _item;
    }
    // if data is larger than all items
    // Continue searching on the right branch
    if (this->Elems.back()->RightPage != NULL) {
        return this->Elems.back()->RightPage->SearchToInsert(data);
    }
    
    // Or insert to this page
    Item* _item = this->Insert(newItem);
    this->CheckToBreak();
    return _item;
}
void Page::Draw(int level) {
    if (this->LeftPage!= NULL)
        this->LeftPage->Draw(level+1);
    
    // print "--" at the begin of each level
    for (int i=0; i<level+1; i++) cout << "\t\t\t";
    cout << "--" << endl;
    for (int i=0; i<level+1; i++) cout << "\t\t\t";
    cout << "  | left  : " << "\x1b[32m" << this->LeftPage << "\033[0m" << endl;
    for (int i=0; i<level+1; i++) cout << "\t\t\t";
    cout << "  | parent: " << "\x1b[32m" << this->Parent << "\033[0m" << endl;
    for (int i=0; i<level+1; i++) cout << "\t\t\t";
    cout << "  | this  : " << "\x1b[32m" << this << "\033[0m" << endl;

    for (int i=0; i<this->Elems.size(); i++) {
        this->Elems[i]->Draw(level+1);
        
        // print "--" at the end of each level
        if (i==this->Elems.size()-1) {
            for (int i=0; i<level+1; i++)
                cout << "\t\t\t";
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
        return newItem;
    }

    this->Elems.push_back(newItem);
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
    
    printf("\n");
    printf("\x1B[31mTexting\033[0m\t\t");
    printf("\x1B[32mTexting\033[0m\t\t");
    printf("\x1B[33mTexting\033[0m\t\t");
    printf("\x1B[34mTexting\033[0m\t\t");
    printf("\x1B[35mTexting\033[0m\n");
    
    printf("\x1B[36mTexting\033[0m\t\t");
    printf("\x1B[36mTexting\033[0m\t\t");
    printf("\x1B[36mTexting\033[0m\t\t");
    printf("\x1B[37mTexting\033[0m\t\t");
    printf("\x1B[93mTexting\033[0m\n");
    
    printf("\033[3;42;30mTexting\033[0m\t\t");
    printf("\033[3;43;30mTexting\033[0m\t\t");
    printf("\033[3;44;30mTexting\033[0m\t\t");
    printf("\033[3;104;30mTexting\033[0m\t\t");
    printf("\033[3;100;30mTexting\033[0m\n");

    printf("\033[3;47;35mTexting\033[0m\t\t");
    printf("\033[2;47;35mTexting\033[0m\t\t");
    printf("\033[1;47;35mTexting\033[0m\t\t");
    printf("\t\t");
    printf("\n");


    BTreeN* tree = new BTreeN(2, 20);
    tree->Insert(40);
    tree->Insert(10);
    tree->Insert(30);
    tree->Insert(15);
    tree->Insert(35);
    tree->Insert(7);
    tree->Insert(26);
    tree->Insert(18);
    tree->Insert(22);
    tree->Insert(5);
    tree->Insert(42);
    tree->Insert(13);
    tree->Insert(46);
    tree->Insert(27);
    tree->Insert(8);
    tree->Insert(32);
    tree->Insert(38);
    tree->Insert(24);
    tree->Insert(45);
    tree->Draw();
    tree->Insert(25);
    tree->Insert(21);
    tree->Insert(28);
    tree->Insert(29);
    tree->Insert(31);
    tree->Insert(23);
    tree->Insert(6);
    tree->Insert(12);
    tree->Insert(41);
    tree->Insert(4);
    tree->Insert(33);
    tree->Insert(34);
    tree->Insert(36);
    tree->Insert(37);
    tree->Insert(39);
    tree->Insert(43);
    tree->Draw();
    
    // Page* p = new Page(2, 20, NULL);
    // Item* newItem1 = p->Insert(new Item(30, NULL));
    // Item* newItem2 = p->Insert(new Item(10, NULL));
    // p->Draw(0);
    // cout << newItem1->Data << "should be 30" << endl;
    // cout << newItem2->Data << "should be 10" << endl;
}
