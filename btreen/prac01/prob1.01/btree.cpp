#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cassert>
#include <stdexcept>

using namespace std;

bool debug = true;

// forward declarations
class Item;
class Page;
class SearchResult;

class Item {
private:

public:
    int Data;
    Page* WrapperPage;
    Page* RightPage;
    int Freq;
    Item(int data, Page* rightPage, Page* wrapperPage);
    ~Item();
    bool IsLeaf();
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
    int GetParentItemIndex() const;
    Page* GetSiblingRight() const;
    Page* GetSiblingLeft() const;
    Page* GetSibling() const;
    void MergeTo(Page* leftSibling);
    SearchResult* Search(int data, bool increase);
    Page* SearchPotentialPage(int data);
    void SearchToAssignPage(Page* page);
    void Break();
    void CheckToBreak();
    Page* HandleDeficiency();
    Page* Delete(Item* item);
    void Draw(int level) const;
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
            return nullptr;
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
    SearchResult* SearchLeftNeighborOfItem(Item* item);
    SearchResult* SearchMaxDataInBranchOfItem(Item* item);
    void DeleteLeaf(Item* item);
    void DeleteNonLeaf(Item* item);
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
    if (this->RightPage )
        delete this->RightPage;
}
bool Item::IsLeaf() {
    return this->RightPage == nullptr;
}
void Item::Draw(int level) {
    for (int i=0; i<level; i++) cout << "\t\t\t";
    cout << this->Data << endl;
    if (!debug) return;

    for (int i=0; i<level; i++) cout << "\t\t\t";
    cout << "  |" << "wrapper   =" << "\x1B[31m[" << this->WrapperPage << "]\033[0m" << endl;
    for (int i=0; i<level; i++) cout << "\t\t\t";
    cout << "  |" << "right_page=" << "\x1B[31m[" << this->RightPage << "]\033[0m" << endl;
}



// ----------------------------------
Page::Page(int order, int data, Page* parent) {
    this->Order = order;
    this->LeftPage = nullptr;
    this->Parent = parent;
    this->Elems.push_back(new Item(data, nullptr, this));
}
Page::~Page() {
    if (this->LeftPage )
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
    // cout << "first data: " << this->FirstData() << endl;
    // cout << "last  data: " << page->LastData() << endl;

    if (page->LastData() < this->FirstData()) {
        this->SetLeftPage(page);
        return;
    }

    // cout << "elems size: " << this->Elems.size() << endl;
    for (int i=0; i<this->Elems.size(); i++) {
        if (page->LastData() > this->Elems.at(i)->Data) {
            // cout << "** elem data: " << this->Elems.at(i)->Data << endl;
            // cout << "   last data: " << page->LastData() << endl;
            // cout << "continue" << endl;
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
    assert(this->Parent );

    // cout << "Draw relative Parent" << endl;
    // this->Parent->DebugItems();
    // cout << "====================" << endl;

    // cout << "\n\n====================" << endl;
    // cout << "middle_item.data              : " << middleItem->Data << endl;
    // cout << "middle_item.right_page.elem[0]: " << middleItem->RightPage->Elems.at(0)->Data << endl;
    // cout << "middle_item.right_page.parent : " << middleItem->RightPage->Parent->FirstData() << endl;

    Item* parentItem = this->Parent->InsertToItems(middleItem);
    
    assert(left->Elems.size() == 2);
    assert(right->Elems.size() == 2 || right->Elems.size() == 3);

    // cout << "parent_item.data              : " << parentItem->Data << endl;
    // cout << "parent_item.right_page.elem[0]: " << parentItem->RightPage->Elems.at(0)->Data << endl;
    // cout << "parent_item.right_page.parent : " << parentItem->RightPage->Parent->FirstData() << endl;
    // cout << "====================\n\n" << endl;
    
    parentItem->RightPage = right;
    right->Parent = this->Parent;

    assert(right->Elems.size() == 2 || right->Elems.size() == 3);
    assert(left->Elems.size() == 2);

    // cout << "left  "; left->DebugItems();
    // cout << "right "; right->DebugItems();

    this->Parent->SearchToAssignPage(left);

    this->Parent->CheckToBreak();
}
void Page::Break() {
    auto left = new Page(this->Order, 0, this);
    auto right = new Page(this->Order, 0, this);

    int leftN = this->Elems.size()/2;
    int rightN = leftN;
    if (this->Elems.size() % 2 == 0) {
        leftN--;
    }
    
    left->Elems.assign(
        this->Elems.begin(), 
        this->Elems.begin()+leftN
    );
    right->Elems.assign(
        this->Elems.end()-rightN, 
        this->Elems.end()
    );


    cout << "left:  "; left->DebugItems();
    cout << "right: "; right->DebugItems();

    for (const auto& item : left->Elems) {
        item->WrapperPage = left;
        if (item->RightPage) {
            item->RightPage->Parent = left;
        }
    }

    for (const auto& item : right->Elems) {
        item->WrapperPage = right;
        if (item->RightPage) {
            item->RightPage->Parent = right;
        }
    }

    auto middleItem = this->Elems.at(this->Order);

    // Inheritance ========================
    // "left" must be inherit the LeftPage of original page
    left->LeftPage = this->LeftPage;
    if (left->LeftPage) {
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
    if (right->LeftPage) {
        right->LeftPage->Parent = right;
    }
    // new RightPage of middle item will be the "right"
    middleItem->RightPage = right;
    // ====================================
    

    if (this->Parent == nullptr) {
        // cout << "migrate root" << endl;
        this->migrateRootPage(left, middleItem, right);
        return;
    }

    // cout << "migrate non root" << endl;
    // cout << "left" << endl;
    // left->Draw(0);
    // cout << "middle" << endl;
    // middleItem->Draw(0);
    // cout << "right" << endl;
    // right->Draw(0);
    
    // cout << endl;
    // cout << "middle_item.data              : " << middleItem->Data << endl;
    // cout << "middle_item.right_page.elem[0]: " << middleItem->RightPage->Elems.at(0)->Data << endl;
    // cout << "middle_item.right_page.parent : " << middleItem->RightPage->Parent->FirstData() << endl;
    // cout << endl;
    
    this->migrateNonRootPage(left, middleItem, right);
}
void Page::CheckToBreak() {
    cout << "######### check to break" << endl;
    this->DebugItems();
    if (this->Elems.size() > 2*this->Order) {
        this->Break();
    }
}
SearchResult* Page::Search(int data, bool increase) {
    if (this->Elems.size() > 0 && data < this->FirstData()) {
        if (this->LeftPage) {
            // continue searching on the leff branch
            return this->LeftPage->Search(data, increase);
        }
        return nullptr;
    }

    if (this->Elems.size() == 0) return nullptr;

    // Check on every item
    for (int i=0; i<this->Elems.size(); i++) {
        auto item = this->Elems.at(i);

        // data is alreay in page
        if (data == item->Data) {
            if (increase) item->Freq++;
            return new SearchResult(this, i);
        }

        // skip if data is larger than item
        if (data > item->Data) 
            continue;

        // Continue searching on the right branch of pre-item
        auto preItem = this->Elems.at(i-1);
        if (preItem->RightPage ) {
            return preItem->RightPage->Search(data, increase);
        }
    }
    
    // if data is larger than all items
    // Continue searching on the right branch
    if (this->Elems.back()->RightPage ) {
        return this->Elems.back()->RightPage->Search(data, increase);
    }

    return nullptr;
}
Page* Page::SearchPotentialPage(int data) {
    if (this->Elems.size() > 0 && data < this->FirstData()) {
        if (this->LeftPage) {
            // continue searching on the leff branch
            return this->LeftPage->SearchPotentialPage(data);
        }
        return this;
    }

    if (this->Elems.size() == 0) return this;

    // Check on every item
    for (int i=0; i<this->Elems.size(); i++) {
        auto item = this->Elems.at(i);

        // data is alreay in page
        if (data == item->Data) {
            return this;
        }

        // skip if data is larger than item
        if (data > item->Data) 
            continue;

        // Continue searching on the right branch of pre-item
        auto preItem = this->Elems.at(i-1);
        if (preItem->RightPage ) {
            return preItem->RightPage->SearchPotentialPage(data);
        }
    }
    
    // if data is larger than all items
    // Continue searching on the right branch
    if (this->Elems.back()->RightPage ) {
        return this->Elems.back()->RightPage->SearchPotentialPage(data);
    }

    return this;
}
void Page::Draw(int level) const {
    if (this->LeftPage)
        this->LeftPage->Draw(level+1);
    
    // print "--" at the begin of each level
    cout << string((level + 1)*3, '\t');
    cout << "--" << endl;

    if (debug) {
        cout << string((level + 1)*3, '\t');
        cout << "  | left  : " << "\x1b[32m" << this->LeftPage << "\033[0m" << endl;
        cout << string((level + 1)*3, '\t');
        cout << "  | parent: " << "\x1b[32m" << this->Parent << "\033[0m" << endl;
        cout << string((level + 1)*3, '\t');
        cout << "  | this  : " << "\x1b[32m" << this << "\033[0m" << endl;
    }

        // Traverse the items of the page
    for (const auto& item : this->Elems) {
        // Draw the current item
        item->Draw(level + 1);

        // Print a horizontal line at the end of each level
        if (&item == &this->Elems.back()) {
            std::cout << string((level+1) * 3, '\t') << "--" << '\n';
        }

        // Recursively draw the right page of the current item
        if (item->RightPage) {
            item->RightPage->Draw(level + 1);
        }
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
    auto it = std::lower_bound(Elems.begin(), Elems.end(), newItem, [](Item* a, Item* b) { return a->Data < b->Data; });
    if (it != Elems.end() && (*it)->Data == newItem->Data) {
        (*it)->Freq++;
        return *it;
    }
    Elems.insert(it, newItem);
    return newItem;
}
int Page::GetParentItemIndex() const {
    if (!Parent) {
        return -1;
    }
    for (const auto& elem : Parent->Elems) {
        if (elem->RightPage == this) {
            return &elem - &Parent->Elems.front();
        }
    }
    return -1;
}

Page* Page::GetSiblingRight() const {
    if (!Parent) {
        return nullptr;
    }

    // Get right sibling for left page
    if (Parent->LeftPage == this) {
        for (const auto& elem : Parent->Elems) {
            if (elem->RightPage) {
                return elem->RightPage;
            }
        }
        return nullptr;
    }

    // Get right sibling for right page
    const int i = GetParentItemIndex() + 1;
    if (i == Parent->Elems.size()) {
        return nullptr;
    }
    return Parent->Elems[i]->RightPage;
}

Page* Page::GetSiblingLeft() const {
    if (!Parent) {
        return nullptr;
    }
    
    auto sibling = Parent->LeftPage;
    for (const auto& elem : Parent->Elems) {
        // Try to get LEFT sibling first
        if (elem->RightPage != this) {
            sibling = elem->RightPage ? elem->RightPage : sibling;
        } else {
            break;
        }
    }
    return sibling;
}
Page* Page::GetSibling() const {
    auto sibling = this->GetSiblingRight();
    if (sibling ) return sibling;
    return this->GetSiblingLeft();
}
Page* Page::Delete(Item* item) {
    auto it = std::find(Elems.begin(), Elems.end(), item);
    if (it != Elems.end()) {
        Elems.erase(it);
    }
    return this;
}
void Page::MergeTo(Page* leftSibling) {
    int parentItemIdx = this->GetParentItemIndex();
    if (parentItemIdx == -1) {
        throw std::runtime_error("BTreeN::Delete: No parentItemIdx");
    }
    auto parentItem = this->Parent->Elems.at(parentItemIdx);
    
    parentItem->WrapperPage = leftSibling;
    parentItem->RightPage = this->LeftPage;
    leftSibling->InsertToItems(parentItem);

    for (auto& item : this->Elems) {
        item->WrapperPage = leftSibling;
        leftSibling->InsertToItems(item);
    }
    
    this->Parent->Delete(parentItem);
}
Page* Page::HandleDeficiency() {
    if (!this->IsDeficiency()) return nullptr;

    // Skip if this page is root page
    if (this->Parent == nullptr) return nullptr;

    auto sibling = this->GetSibling();
    if (sibling == nullptr) throw runtime_error("BTreeN::Delete: No sibling");

    auto left = sibling;
    auto right = this;

    if (sibling->FirstData() > this->FirstData()) {
        left = this;
        right = sibling;
    }

    cout << "-----------------------------------------)))))))" << endl;
    left->DebugItems();
    right->DebugItems();
    
    right->MergeTo(left);
    cout << "-----------------------------------------)))))))" << endl;

    for (auto& item : left->Elems)
        if (item->RightPage) item->RightPage->Parent = left;

    cout << "Page::new left after merge: ";
    left->DebugItems();

    left->CheckToBreak();

    cout << "left after check to break   ";
    left->DebugItems();
    
    if (left->Parent) {
        if (left->Parent->Parent == nullptr) {
            cout << "parrent items ++: ";
            left->Parent->DebugItems();

            if (left->Parent->Elems.size() == 0) {
                cout << "set parrent to root" << endl;
                left->Parent = nullptr;
                return left;
            }
        }

        cout << "Page::HandleDeficiency() left->Parent is NOT NULL: ";
        left->Parent->DebugItems();
        
        // if (left->Parent->FirstData() == 30) {
        //     cout << "Page::HandleDeficiency() left->Parent->FirstData() == 30 ";
        cout << "Page::HandleDeficiency() continue on parent level" << endl;
        return left->Parent->HandleDeficiency();
        // }
    }

    throw runtime_error("BTreeN::Delete: left sibling is NULL after merge and break!!!");
    return nullptr;
}



// ----------------------------------
BTreeN::BTreeN(int order, int data) {
    this->Order = order;
    this->Root = new Page(order, data, nullptr);
    this->Root->Parent = nullptr;
}
BTreeN::~BTreeN() {
    this->Order = 0;
    if (this->Root ) delete this->Root;
}
void BTreeN::Insert(int data) {
    auto newItem = new Item(data, nullptr, nullptr);
    
    auto potentialPage = this->Root->SearchPotentialPage(data);
    
    cout << "here" << endl;

    
    if (potentialPage == nullptr) return;
    potentialPage->InsertToItems(newItem);
    potentialPage->CheckToBreak();
}
SearchResult* BTreeN::SearchMaxDataInBranchOfItem(Item* item) {
    if (item->RightPage) {
        auto newItem = item->RightPage->Elems.back();
        return this->SearchMaxDataInBranchOfItem(newItem);
    }
    // this item is what we want
    return item->WrapperPage->Search(item->Data, false);
}
SearchResult* BTreeN::SearchLeftNeighborOfItem(Item* item) {
    auto sr = this->Root->Search(item->Data, false);
    
    // if there is an item left of this item, return it
    if (sr->ItemIdx > 0) {
        return new SearchResult{
            sr->P,
            sr->ItemIdx-1,
        };
    }

    // well, there is no item left of this item
    return nullptr;
}

void BTreeN::DeleteLeaf(Item* item) {
    cout << "BTreeN::Delete() item is leaf" << endl;
    auto page = item->WrapperPage->Delete(item);
    delete(item);
    auto newRoot = page->HandleDeficiency();
    if (newRoot) {
        cout << "trying to set new root" << endl;
        this->Root = newRoot;
    }
    return;
}
void BTreeN::DeleteNonLeaf(Item* item) {
    auto parent = item->WrapperPage->Parent;

    // search for max item that less that current item
    auto sr1 = this->SearchLeftNeighborOfItem(item);
    if (sr1 == nullptr) {
        if (item->WrapperPage->LeftPage == nullptr) {
            cout << "============================================" << endl;
            cout << "Vậy thì đành kiếm bên phải rồi thay thế thôi" << endl;
            cout << "============================================" << endl;
            return;
        }

        // so, get last item of left page
        sr1 = new SearchResult{
            item->WrapperPage->LeftPage,
            int(item->WrapperPage->LeftPage->Elems.size()-1)
        };
    }

    auto sr2 = this->SearchMaxDataInBranchOfItem(sr1->GetItem());
    if (sr2 == nullptr) {
        throw std::runtime_error("BTreeN::Delete: No max data in branch");
    }

    // delete this max item. this item must be leaf,
    // so deleting it is an easy job
    auto maxLeftItem = sr2->GetItem();
    auto maxValue = maxLeftItem->Data;
    this->DeleteLeaf(maxLeftItem);

    // replace value of current item by max item value
    item->Data = maxValue;

    // handle special case: item is in root, and it is the only item
    return;
}
void BTreeN::Delete(int data) {
    cout << this->Root << endl;
    auto sr0 = this->Root->Search(data, false);
    if (sr0 == nullptr) return;
    
    cout << "sr0 is not null" << endl;

    auto item = sr0->GetItem();
    if (item == nullptr) return;
    
    if (item->IsLeaf()) return this->DeleteLeaf(item);

    return this->DeleteNonLeaf(item);
}
void BTreeN::Draw() {
    cout << "-------------------" << endl;
    this->Root->Draw(0);
}

