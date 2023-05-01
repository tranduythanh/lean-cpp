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

// method migrates the root page to new pages 
// when the root page is full. It takes three arguments 
// - left
// - middleItem
// - right
// which are the pages to be assigned to the 
// left, middle, and right of the root page, 
// respectively. The method sets the middle item 
// as the only element of the root page, sets the 
// middle item's wrapper page to the root page, and 
// sets the left and right pages of the middle item.
void Page::migrateRootPage(Page* left, Item* middleItem, Page* right) {
    this->Elems.clear();
    this->Elems.push_back(middleItem);
    middleItem->WrapperPage = this;
    this->SetRightPage(0, right);
    this->SetLeftPage(left);
}

// method sets the right page of an element in the 
// current page. It takes two arguments - elemIdx 
// and page - which are the index of the element 
// and the page to be set as the right page of the 
// element, respectively. The method sets the right 
// page of the element at the specified index as the 
// given page and sets the parent of the given page 
// as the current page.
void Page::SetRightPage(int elemIdx, Page* page) {
    this->Elems.at(elemIdx)->RightPage = page;
    page->Parent = this;
}

// method sets the left page of the current page. 
// It takes one argument - page - which is the page 
// to be set as the left page of the current page. 
// The method sets the left page of the current page 
// as the given page and sets the parent of the 
// given page as the current page.
void Page::SetLeftPage(Page* page) {
    this->LeftPage = page;
    page->Parent = this;
}

// This method searches for the correct position 
// of page to be assigned to the current page's 
// child pages. If page is less than the first 
// element of the current page, then page is set 
// as the left child of the current page. If page 
// is greater than an element in the current page, 
// then page is set as the right child of the 
// previous element.
void Page::SearchToAssignPage(Page* page) {
    if (page->LastData() < this->FirstData()) {
        this->SetLeftPage(page);
        return;
    }

    for (int i=0; i<this->Elems.size(); i++) {
        if (page->LastData() > this->Elems.at(i)->Data) {
            continue;
        }
        
        this->SetRightPage(i-1, page);
        return;
    }
    this->SetRightPage(this->Elems.size()-1, page);
}

// This method checks whether the number of 
// elements in the page is less than the order of the B-Tree.
bool Page::IsDeficiency() {
    return this->Elems.size() < this->Order;
}

// This method is called when the current page is not 
// a root page and needs to be split into two pages. 
// It creates a new item in the parent page containing 
// middleItem and assigns the right page to it. Then it 
// assigns left page to the correct position in the 
// parent page.
void Page::migrateNonRootPage(Page* left, Item* middleItem, Page* right) {
    Item* parentItem = this->Parent->InsertToItems(middleItem);
    parentItem->RightPage = right;
    right->Parent = this->Parent;
    this->Parent->SearchToAssignPage(left);
    this->Parent->CheckToBreak();
}

// This method is called when a page needs to be split 
// into two pages. It creates two new pages, left and 
// right, and copies half of the elements from the current 
// page to each of them. It also delegates the 
// middle element's right page to the left of the new 
// right page.
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
    

    if (this->Parent == nullptr) {
        this->migrateRootPage(left, middleItem, right);
        return;
    }

    this->migrateNonRootPage(left, middleItem, right);
}

// This method checks whether the current page needs 
// to be split into two pages. If the number of elements 
// in the page is greater than twice the order of the 
// B-Tree, then it calls the Break method.
void Page::CheckToBreak() {
    if (this->Elems.size() > 2*this->Order) {
        this->Break();
    }
}

// searches for a specific data value in the tree, 
// and returns a pointer to a SearchResult object 
// that contains a reference to the page that contains 
// the data and the index of the item within that page.
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

//  is used to search for the page in which the data 
// should be inserted or the page from which data should 
// be removed. If the data is already in the tree, it returns 
// the page that contains the data. If the data is not in 
// the tree, it returns the leaf page where the data should 
// be inserted.
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

// method inserts a new Item object into the Elems vector 
// of the current Page object in a way that maintains the 
// order of Data values in the vector. It first sets the 
// WrapperPage attribute of the new item to the current 
// page. It then uses the lower_bound function from the 
// <algorithm> library to find the first position in the 
// Elems vector where the new item's Data value could be 
// inserted while preserving the order of the elements. 
// If an element with the same Data value already exists, 
// it increments the Freq attribute of that element and 
// returns a pointer to that element. Otherwise, it inserts 
// the new item at the position returned by lower_bound 
// and returns a pointer to the new item.
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

// method returns the index of the Item object in the 
// Parent page's Elems vector whose RightPage attribute 
// points to the current page. If the current page has 
// no parent, it returns -1. It searches the Parent page's
// Elems vector for the first element whose RightPage 
// attribute is equal to the current page. It then 
// calculates the index of that element in the Elems 
// vector using pointer arithmetic and returns it.
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

// Returns the right sibling of the current page. 
// If the current page is the leftmost child of 
// the parent, it returns the right sibling of 
// the page to the right of the current page.
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

// Returns the left sibling of the current page. 
// If the current page is the rightmost child of 
// the parent, it returns the left sibling of 
// the page to the left of the current page.
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

// Returns either the left or the right sibling 
// of the current page. If there is no left or 
// right sibling, it returns nullptr. This method 
// first tries to get the right sibling of the 
// current page, and if it fails, it tries to 
// get the left sibling.
Page* Page::GetSibling() const {
    auto sibling = this->GetSiblingRight();
    if (sibling ) return sibling;
    return this->GetSiblingLeft();
}

// method deletes an item from the page's 
// elements vector, given the item pointer. 
// If the item is found in the vector, it is 
// erased, and the updated page is returned.
Page* Page::Delete(Item* item) {
    auto it = std::find(Elems.begin(), Elems.end(), item);
    if (it != Elems.end()) {
        Elems.erase(it);
    }
    return this;
}

// method merges the current page with its 
// left sibling page. The left sibling is 
// passed as an argument, and the current page 
// is merged to its left. The parent item that 
// separates the two pages is moved from the 
// current page to the left sibling. Then, all 
// the items from the current page are moved 
// to the left sibling.
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

// method checks if the current page is deficient 
// (i.e., has less than Order elements). If it is, 
// it tries to borrow an item from its sibling or 
// merge with a sibling. If a merge happens, the 
// parent item is moved from the parent to the left 
// sibling, and all the items from the current page 
// are moved to the left sibling. The method then 
// calls CheckToBreak() to check if the resulting 
// page is still oversized and possibly breaks the 
// page again. Finally, the method calls itself 
// recursively on the parent's page, if necessary.
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

    right->MergeTo(left);
    
    for (auto& item : left->Elems)
        if (item->RightPage) item->RightPage->Parent = left;

    left->CheckToBreak();

    
    if (left->Parent) {
        if (left->Parent->Parent == nullptr) {
            if (left->Parent->Elems.size() == 0) {
                left->Parent = nullptr;
                return left;
            }
        }

        return left->Parent->HandleDeficiency();
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

// method takes an integer value and creates 
// a new item with that value. It then searches 
// for the potential page where the new item 
// should be inserted using the SearchPotentialPage 
// method of the root node. If a potential page is 
// found, the new item is inserted into that page 
// using the InsertToItems method. Finally, the 
// CheckToBreak method of the potential page is 
// called to check if the page needs to be split.
void BTreeN::Insert(int data) {
    auto newItem = new Item(data, nullptr, nullptr);
    auto potentialPage = this->Root->SearchPotentialPage(data);
    if (potentialPage == nullptr) return;
    potentialPage->InsertToItems(newItem);
    potentialPage->CheckToBreak();
}

// method takes an item as input and recursively 
// searches for the right-most element in its right 
// child. The result is a SearchResult object that 
// contains a pointer to the page containing the 
// right-most element and the index of that element 
// in the page's element vector.
SearchResult* BTreeN::SearchMaxDataInBranchOfItem(Item* item) {
    if (item->RightPage) {
        auto newItem = item->RightPage->Elems.back();
        return this->SearchMaxDataInBranchOfItem(newItem);
    }
    // this item is what we want
    return item->WrapperPage->Search(item->Data, false);
}

// method takes an item as input and searches for 
// its left neighbor in the tree. If a left neighbor 
// is found, a SearchResult object is returned that 
// contains a pointer to the page containing the 
// left neighbor and the index of that element in 
// the page's element vector. If there is no left 
// neighbor, nullptr is returned.
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

// method takes an item as input, deletes the item 
// from the tree using the Delete method of the 
// item's wrapper page, and then calls the 
// HandleDeficiency method of the page to handle 
// any deficiencies that may arise due to the 
// deletion. If the page becomes the new root of 
// the tree, the Root pointer is updated accordingly.
void BTreeN::DeleteLeaf(Item* item) {
    auto page = item->WrapperPage->Delete(item);
    delete(item);
    auto newRoot = page->HandleDeficiency();
    if (newRoot) {
        this->Root = newRoot;
    }
    return;
}

// method takes an item as input and performs 
// a special deletion procedure for non-leaf nodes. 
// It searches for the right-most element in the 
// left child of the item using the 
// SearchLeftNeighborOfItem and 
// SearchMaxDataInBranchOfItem methods. The right-most 
// element is then deleted using the DeleteLeaf 
// method. Finally, the value of the deleted 
// element is replaced with the value of the 
// right-most element.
void BTreeN::DeleteNonLeaf(Item* item) {
    auto parent = item->WrapperPage->Parent;

    // search for max item that less that current item
    auto sr1 = this->SearchLeftNeighborOfItem(item);
    if (sr1 == nullptr) {
        if (item->WrapperPage->LeftPage == nullptr) {
            throw std::runtime_error("BTreeN::DeleteNonLeaf: invalid BTree");
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

// method takes an integer value as input and 
// first searches for an item in the tree with 
// that value using the Search method of the root 
// node. If the item is found and it is a leaf, 
// the DeleteLeaf method is called to delete it. 
// If the item is found and it is not a leaf, 
// the DeleteNonLeaf method is called.
void BTreeN::Delete(int data) {
    auto sr0 = this->Root->Search(data, false);
    if (sr0 == nullptr) return;
    
    
    auto item = sr0->GetItem();
    if (item == nullptr) return;
    
    if (item->IsLeaf()) return this->DeleteLeaf(item);

    return this->DeleteNonLeaf(item);
}

void BTreeN::Draw() {
    cout << "-------------------" << endl;
    this->Root->Draw(0);
}
