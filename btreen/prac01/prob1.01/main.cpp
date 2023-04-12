#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cassert>

using namespace std;

// forward declarations
class Item;
class Page;
class SearchResult;

class Item {
private:
    bool isLeaf();

public:
    int Data;
    Page* WrapperPage;
    Page* RightPage;
    int Freq;
    Item(int data, Page* rightPage, Page* wrapperPage);
    ~Item();
    void Draw(int level);
};

class Page {
private:
    void migrateRootPage(Page* left, Item* middleItem, Page* right);
    void migrateNonRootPage(Page* left, Item* middleItem, Page* right);
    void eraseItem(int data);

public:
    int Order;
    Page* LeftPage;
    vector<Item*> Elems;
    Page* Parent;

    Page(int order, int data, Page* parent);
    ~Page();
    int FirstData();
    int LastData();
    bool IsDeficiency();
    Item* InsertToItems(Item* item);
    void SetRightPage(int elemIdx, Page* page);
    void SetLeftPage(Page* page);
    Page* GetSibling();
    SearchResult* Search(int data, bool increase);
    Page* SearchPotentialPage(int data);
    void SearchToAssignPage(Page* page);
    void Break();
    void CheckToBreak();
    Page* Delete(Item* item);
    void Draw(int level);
    void DebugItems();
};


class SearchResult {
private:
public:
    Page* P;
    int ItemIdx;
    SearchResult(Page* page, int itemIndex){
        this->P = page;
        this->ItemIdx = itemIndex;
    };
    ~SearchResult();
    Item* GetItem(){
        if (this->ItemIdx < 0) {
            return NULL;
        }
        return this->P->Elems[this->ItemIdx];
    };
};


class BTreeN {
private:
public:
    int Order;
    Page* Root;
    BTreeN(int order, int data);
    ~BTreeN();
    void Insert(int data);
    void Delete(int data);
    void Draw();
};


// ==============================================================
// Implement methods
// ==============================================================

Item::Item(int data, Page* rightPage, Page* wrapperPage) {
    this->Data = data;
    this->Freq = 1;
    this->RightPage = rightPage;
    this->WrapperPage = wrapperPage;
}
Item::~Item() {
    if (this->RightPage != NULL)
        delete this->RightPage;
}
bool Item::isLeaf() {
    return this->RightPage == NULL;
}
void Item::Draw(int level) {
    for (int i=0; i<level; i++) cout << "\t\t\t";
    cout << this->Data << endl;
    for (int i=0; i<level; i++) cout << "\t\t\t";
    cout << "  |" << "wrapper   =" << "\x1B[31m[" << this->WrapperPage << "]\033[0m" << endl;
    for (int i=0; i<level; i++) cout << "\t\t\t";
    cout << "  |" << "right_page=" << "\x1B[31m[" << this->RightPage << "]\033[0m" << endl;
}



// ----------------------------------
Page::Page(int order, int data, Page* parent) {
    this->Order = order;
    this->LeftPage = NULL;
    this->Parent = parent;
    this->Elems.push_back(new Item(data, NULL, this));
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
    middleItem->WrapperPage = this;
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
bool Page::IsDeficiency() {
    return this->Elems.size() < this->Order;
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

    Item* parentItem = this->Parent->InsertToItems(middleItem);
    
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
        left->Elems.at(i)->WrapperPage = left;
        if (left->Elems.at(i)->RightPage != NULL) {
            left->Elems.at(i)->RightPage->Parent = left;
        }
    }

    for (int i=0; i<right->Elems.size(); i++) {
        right->Elems.at(i)->WrapperPage = right;
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
SearchResult* Page::Search(int data, bool increase) {
    if (data < this->FirstData()) {
        if (this->LeftPage!= NULL) {
            // continue searching on the leff branch
            return this->LeftPage->Search(data, increase);
        }
        return NULL;
    }
    // Check on every item
    for (int i=0; i<this->Elems.size(); i++) {
        Item* item = this->Elems.at(i);

        // data is alreay in page
        if (data == item->Data) {
            if (increase) item->Freq++;
            return new SearchResult(this, i);
        }

        // skip if data is larger than item
        if (data > item->Data) 
            continue;

        // Continue searching on the right branch of pre-item
        Item* preItem = this->Elems.at(i-1);
        if (preItem->RightPage != NULL) {
            return preItem->RightPage->Search(data, increase);
        }
    }
    
    // if data is larger than all items
    // Continue searching on the right branch
    if (this->Elems.back()->RightPage != NULL) {
        return this->Elems.back()->RightPage->Search(data, increase);
    }

    return NULL;
}
Page* Page::SearchPotentialPage(int data) {
    if (data < this->FirstData()) {
        if (this->LeftPage!= NULL) {
            // continue searching on the leff branch
            return this->LeftPage->SearchPotentialPage(data);
        }
        return this;
    }
    // Check on every item
    for (int i=0; i<this->Elems.size(); i++) {
        Item* item = this->Elems.at(i);

        // data is alreay in page
        if (data == item->Data) {
            return this;
        }

        // skip if data is larger than item
        if (data > item->Data) 
            continue;

        // Continue searching on the right branch of pre-item
        Item* preItem = this->Elems.at(i-1);
        if (preItem->RightPage != NULL) {
            return preItem->RightPage->SearchPotentialPage(data);
        }
    }
    
    // if data is larger than all items
    // Continue searching on the right branch
    if (this->Elems.back()->RightPage != NULL) {
        return this->Elems.back()->RightPage->SearchPotentialPage(data);
    }

    return this;
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
Item* Page::InsertToItems(Item* newItem) {
    newItem->WrapperPage = this;
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
Page* Page::GetSibling() {
    Page* parent = this->Parent;
}
Page* Page::Delete(Item* item) {
    for (int i=0; i<this->Elems.size(); i++) {
        if (this->Elems.at(i) != item) continue;
        
        this->Elems.erase(Elems.begin()+i);
        return this;
    }
}



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
    Item* newItem = new Item(data, NULL, NULL);
    Page* potentialPage = this->Root->SearchPotentialPage(data);
    if (potentialPage == NULL) return;
    potentialPage->InsertToItems(newItem);
    potentialPage->CheckToBreak();
}
void BTreeN::Delete(int data) {
    SearchResult* sr = this->Root->Search(data, false);
    Item* item = sr->GetItem();
    
    if (item == NULL) return;
    
    Page* page = item->WrapperPage->Delete(item);
    delete(item);

    if (!page->IsDeficiency()) return;


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
    vector<int> vect{
        20,
        40, 10, 30, 15,
        35, 7, 26, 18, 22,
        5, 42, 13, 46, 27, 8, 32,
        38, 24, 45, 25
    };
    for (int x: vect) {
        tree->Insert(x);
    }
    tree->Draw();

    tree->Delete(46);
    tree->Delete(45);
    
    tree->Draw();
    
    // Page* p = new Page(2, 20, NULL);
    // Item* newItem1 = p->Insert(new Item(30, NULL));
    // Item* newItem2 = p->Insert(new Item(10, NULL));
    // p->Draw(0);
    // cout << newItem1->Data << "should be 30" << endl;
    // cout << newItem2->Data << "should be 10" << endl;
}
