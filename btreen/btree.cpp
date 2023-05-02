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

// Phương thức di chuyển trang "root" đến trang mới
// khi trang "root" đã đầy. Nó nhận ba đối số
// - left
// - middleItem
// - right
// đó là các trang được gán cho
// LeftPage, giữa và phải của trang gốc,
// tương ứng. Phương thức đặt middleItem
// là item duy nhất của trang root, đặt
// trang bao bọc của mục giữa là trang gốc, và
// đặt các trang bên trái và phải của mục ở giữa.
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
    // dữ liệu nhỏ hơn cả Item đầu tiên trong Elems,
    // vậy ta sẽ gán page làm LeftPage
    if (page->LastData() < this->FirstData()) {
        this->SetLeftPage(page);
        return;
    }

    // Duyệt qua toàn bộ Item trong Elems và gán page
    // làm RightPage của Item phù hợp
    for (int i=0; i<this->Elems.size(); i++) {
        if (page->LastData() > this->Elems.at(i)->Data)
            continue;
        
        this->SetRightPage(i-1, page);
        return;
    }

    // Dữ liệu lớn hơn mọi Item trong Elems, vậy thì
    // ta sẽ gán page làm RightPage của Item cuối cùng
    // trong Elems
    this->SetRightPage(this->Elems.size()-1, page);
}

// This method checks whether the number of 
// elements in the page is less than the order of the B-Tree.
bool Page::IsDeficiency() {
    return this->Elems.size() < this->Order;
}

// Phương thức sẽ chèn middleItem vào Parent của
// Page hiện tại, gán RightPage cho Item mới chèn vào.
// Kiểm tra và gán left vào chỗ phù hợp trong Parent.
// Tương tự, Sau khi chèn Item mới vào Parent thì Page
// này cũng có thể xày ra tình huống tràn, do vậy mà ta
// sẽ (đệ quy) kiểm tra để tách Page. Trường hợp tệ nhất,
// sẽ gọi ngược lên tận root
void Page::migrateNonRootPage(Page* left, Item* middleItem, Page* right) {
    Item* parentItem = this->Parent->InsertToItems(middleItem);
    parentItem->RightPage = right;
    right->Parent = this->Parent;
    this->Parent->SearchToAssignPage(left);
    this->Parent->CheckToBreak();
}

// Phương thức sẽ tách Page hiện tại làm 3 phần:
// 1. Page mới tên là left, chứa nửa Item bên trái trong Elems
// 2. Page mới tên là right, chứa nửa Item bên phải trong Elems
// 3. Item mới tên là middleItem, chính là Item nằm chính giữa trong Elems
// Ngoài ra:
// - right sẽ thừa kế RightPage của middleItem, đặt làm LeftPage của mình
// - toàn bộ right sẽ thành RightPage của middleItem
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
    
    // "left" sẽ kế thừa LeftPage của Page hiện tại
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
    // 
    // "right" sẽ thừa kế RightPage của middleItem và
    // đặt làm LeftPage của mình
    right->LeftPage = middleItem->RightPage;
    if (right->LeftPage) {
        right->LeftPage->Parent = right;
    }

    // RightPage của middleItem sẽ là "right"
    middleItem->RightPage = right;
    

    if (this->Parent == nullptr) {
        this->migrateRootPage(left, middleItem, right);
        return;
    }

    this->migrateNonRootPage(left, middleItem, right);
}

// Nếu Page hiện tại có số lượng Item lớn hơn 2n thì
// tiến hành tách Page này.
void Page::CheckToBreak() {
    if (this->Elems.size() > 2*this->Order) {
        this->Break();
    }
}

// tìm kiếm dữ liệu cho trước xem nó thuộc Item nào
// và trà về thông tin của Item đó thông qua SearchResult
SearchResult* Page::Search(int data, bool increase) {
    if (this->Elems.size() > 0 && data < this->FirstData()) {
        // Nếu dữ liệu nhỏ hơn mọi Item trong Elems thì 
        // tiếp tục tìm kiếm (đệ quy) trên LeftPage của 
        // Page hiện tại
        if (this->LeftPage) {
            return this->LeftPage->Search(data, increase);
        }

        // LeftPage không tồn tại, vậy ta sẽ trả ra nullptr
        // để báo hiệu là không tìm được Item mong muốn
        return nullptr;
    }

    // Page hiện tại không có Elems nào, khả năng cao đây 
    // là root khi B-Cây mới khởi tạo. Ta dĩ nhiên sẽ trả
    // nullptr để báo hiệu không tìm được Item mong muốn
    if (this->Elems.size() == 0) return nullptr;

    // Duyệt qua tất cả Item trong Elems.
    for (int i=0; i<this->Elems.size(); i++) {
        auto item = this->Elems.at(i);

        // Dữ liệu có tồn tại, ta trả về thông tin của Item
        // chứa dữ liệu này
        if (data == item->Data) {
            if (increase) item->Freq++;
            return new SearchResult(this, i);
        }

        // Dữ liệu lớn hơn Item hiện tại, ta tiếp tục tìm
        // kiếm trên Item kế tiếp trong Elems
        if (data > item->Data) 
            continue;

        // Dữ llieuej nhỏ hơn Item hiện tại, vậy ta tiếp tục
        // tìm kiếm trên RightPage của Item liền trước
        auto preItem = this->Elems.at(i-1);
        if (preItem->RightPage ) {
            return preItem->RightPage->Search(data, increase);
        }
    }
    
    // Nếu dữ liệu lớn hơn mọi Item trong Elems thì ta 
    // tiếp tục tìm kiếm (đệ quy) trong RightPage của 
    // Item này
    if (this->Elems.back()->RightPage ) {
        return this->Elems.back()->RightPage->Search(data, increase);
    }

    return nullptr;
}

// Phương thức này cố gắng tìm kiếm Page tiềm năng mà
// ta sẽ chèn thêm Item vào.
// Nếu dữ liệu đã tồn tại trên Page này, thì ta trả
// địa chỉ Page này về.
// Nếu dữ liệu chưa tồn tại thì nó sẽ cố gắng tìm
// một trang lá phù hợp và trả về địa chỉ của trang lá này
Page* Page::SearchPotentialPage(int data) {
    if (this->Elems.size() > 0 && data < this->FirstData()) {
        // Dữ liệu cần chèn có giá trị nhỏ hơn tất cả các
        // Item đang có trên page này, do đó ta sẽ cố gắng
        // tìm Page tiềm năng trên LeftPage
        if (this->LeftPage)
            return this->LeftPage->SearchPotentialPage(data);

        // LeftPage không tồn tại, thì Page hiện tại chính
        // là Page tiềm năng
        return this;
    }

    // Page này không có Item nào, khả năng cao đây chính là
    // Page root khi mới khởi tạo (chưa có dữ liệu). Vậy đây
    // chính là Page tiềm năng.
    if (this->Elems.size() == 0) return this;

    // Duyệt qua toàn bộ Item trong Page.
    for (int i=0; i<this->Elems.size(); i++) {
        auto item = this->Elems.at(i);

        // Dữ liệu đã tồn tại trên Page.
        if (data == item->Data) {
            return this;
        }

        // Dữ liệu lớn hơn Item hiện tại, vậy
        // ta sẽ tiếp tục đối sánh với Item
        // kế tiếp
        if (data > item->Data) 
            continue;

        // Dữ liệu nhỏ hơn Item hiện tại, vậy thì
        // ta trở lui về Item liền trước, và tiếp
        // tục tìm kiếm (đệ quy) Page tiềm năng
        // trên RightPage của Item liền trước đó.
        auto preItem = this->Elems.at(i-1);
        if (preItem->RightPage ) {
            return preItem->RightPage->SearchPotentialPage(data);
        }
    }
    
    // Dữ liệu lớn hơn tất cả Item trong Page.
    // Ta sẽ tiếp tục tìm kiếm (đệ quy) Page
    // tiềm năng trên RightPage của Item cuối cùng.
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
        cout << "  | left  : "  << "\x1b[32m"    
                                << this->LeftPage << "\033[0m" << endl;
        cout << string((level + 1)*3, '\t');
        cout << "  | parent: "  << "\x1b[32m" 
                                << this->Parent << "\033[0m" << endl;
        cout << string((level + 1)*3, '\t');
        cout << "  | this  : "  << "\x1b[32m" 
                                << this << "\033[0m" << endl;
    }

        // Traverse the items of the page
    for (const auto& item : this->Elems) {
        // Draw the current item
        item->Draw(level + 1);

        // Print a horizontal line at the end of each level
        if (&item == &this->Elems.back()) {
            cout    << string((level+1) * 3, '\t') 
                    << "--" << '\n';
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

// Phương thức sẽ chèn Item mới vào vector Elems
// của Page hiện tại theo cách giữ nguyên thứ tự
// của các giá trị Data trong vector. Đầu tiên, 
// nó đặt thuộc tính WrapperPage của Item mới là 
// Page hiện tại. Sau đó, nó sử dụng hàm lower_bound 
// từ thư viện <algorithm> để tìm vị trí đầu tiên
// trong vector Elems mà giá trị Data của Item mới nên
// chèn vào trong khi vẫn giữ nguyên thứ tự của Item 
// trong Elems. Nếu 1 Item có cùng giá trị Data đã 
// tồn tại, nó tăng giá trị thuộc tính Freq của Item 
// đó và trả về một con trỏ đến Item đó. Ngược lại, 
// nó chèn Item mới vào vị trí do lower_bound trả về
// và trả về một con trỏ đến Item mới.
Item* Page::InsertToItems(Item* newItem) {
    newItem->WrapperPage = this;
    auto it = lower_bound(
        Elems.begin(), Elems.end(), 
        newItem, 
        [](Item* a, Item* b) { return a->Data < b->Data; }
    );
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
    auto it = find(Elems.begin(), Elems.end(), item);
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
        throw runtime_error("BTreeN::Delete: No parentItemIdx");
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
    // Nếu Page hiện tại không cạn thì không cần xử lí gì luôn
    if (!this->IsDeficiency()) return nullptr;

    // Nếu không có Parent thì cũng chả cần phải xú lí Parent bị cạn
    // làm gì nữa
    if (this->Parent == nullptr) return nullptr;

    // Tìm Page ngang cấp là anh em của Page hiện tại
    auto sibling = this->GetSibling();
    if (sibling == nullptr) 
        throw runtime_error("BTreeN::Delete: No sibling");

    // Page anh em ngang cấp có thể nằm bên trái hoặc phải,
    // vậy nên để tiện thực thi các bước sau, ta sẽ kiểm tra
    // và gán đúng vị trí vào 2 biến mới là left và right
    auto left = sibling;
    auto right = this;
    if (sibling->FirstData() > this->FirstData()) {
        left = this;
        right = sibling;
    }

    // Tiến hành gộp Page right vào Page left. Nhớ là phải 
    // cập nhật lại Parent của đống Item trong right sau khi
    // gộp vào left
    right->MergeTo(left);
    for (auto& item : left->Elems)
        if (item->RightPage) item->RightPage->Parent = left;

    // Kiểm tra coi left có bị tràn hay không, nếu tràn thì 
    // tiến hành tách
    left->CheckToBreak();
    
    // Nế Parent của left mà là root thì ta sẽ gán kiểm tra
    // và gán lại root cho đúng. Nếu không thì ta tiến hành
    // kiểm tra (đệ quuy) và xử lí nếu Parent bị c
    if (left->Parent) {
        if (left->Parent->Parent == nullptr) {
            if (left->Parent->Elems.size() == 0) {
                left->Parent = nullptr;
                return left;
            }
        }

        return left->Parent->HandleDeficiency();
    }

    throw runtime_error(
        "BTreeN::Delete: left sibling is NULL after merge and break!!!");
        
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

SearchResult* BTreeN::SearchMaxDataInBranchOfItem(Item* item) {
    if (item->RightPage) {
        auto newItem = item->RightPage->Elems.back();
        return this->SearchMaxDataInBranchOfItem(newItem);
    }
    
    // RightPage không tồn tại, vậy Item này chính là Item
    // cần tìm, ta sẽ lợi dụng phương thức Search để 
    // tìm kiếm chính nó trong WrapperPage và trả về 
    // SearchResult mong muốn.
    return item->WrapperPage->Search(item->Data, false);
}

SearchResult* BTreeN::SearchLeftNeighborOfItem(Item* item) {
    auto sr = this->Root->Search(item->Data, false);
    
    // Nếu đây không phải Item đầu tiên trong Elems thì
    // trả về Item liền trước nó.
    if (sr->ItemIdx > 0) {
        return new SearchResult{
            sr->P,
            sr->ItemIdx-1,
        };
    }

    // Đây là Item đầu tiên trong Elems, nên chả có
    // Item nào trước nó cả
    return nullptr;
}

void BTreeN::DeleteLeaf(Item* item) {
    auto page = item->WrapperPage->Delete(item);
    delete(item);
    auto newRoot = page->HandleDeficiency();
    if (newRoot) {
        this->Root = newRoot;
    }
    return;
}

// Phương thức sẽ tìm kiếm Item (gọi là B) có giá trị lớn nhất 
// mà vẫn nhỏ hơn giá trị của Item cần xóa (gọi là A) thông qua 
// SearchLeftNeighborOfItem và SearchMaxDataInBranchOfItem. 
// Vì B là Leaf nên có thể xóa dễ dàng bằng phương thức DeleteLeaf
// Cuối cùng, ta thay dữ liệu của A bằng dữ liệu của B.
void BTreeN::DeleteNonLeaf(Item* item) {
    auto parent = item->WrapperPage->Parent;

    // Tìm kiếm Item liền trước Item hiện tại
    auto sr1 = this->SearchLeftNeighborOfItem(item);
    if (sr1 == nullptr) {
        if (item->WrapperPage->LeftPage == nullptr) {
            throw runtime_error("BTreeN::DeleteNonLeaf: invalid BTree");
        }

        // Không có item nào liền trước Item hiện tại,
        // vậy ta sẽ trả ra Item cuối cùng của LeftPage
        // của Page hiện tại
        sr1 = new SearchResult{
            item->WrapperPage->LeftPage,
            int(item->WrapperPage->LeftPage->Elems.size()-1)
        };
    }

    // Tìm Item có dữ liệu lớn nhất trong Page hiện tại và trong
    // tất cả Page con cháu chắt chút chít... của nó
    auto sr2 = this->SearchMaxDataInBranchOfItem(sr1->GetItem());
    if (sr2 == nullptr) {
        throw runtime_error("BTreeN::Delete: No max data in branch");
    }

    // Xóa Item thế thân tìm được.
    auto maxLeftItem = sr2->GetItem();
    auto maxValue = maxLeftItem->Data;
    this->DeleteLeaf(maxLeftItem);

    // lấy dữ liệu của Item thế thân làm dữ liệu của Item cần xóa
    item->Data = maxValue;

    return;
}

// Ý tưởng:
// - Tìm xem dữ liệu có tồn tại trên cây hay không.
// - Nếu có thì tiến hành xóa Item tương ứng 
//   với dữ liệu đó. Có 2 khả năng. Nếu  Item 
//   là Leaf thì dễ, còn nếu không phải là 
//   Leaf thì ta sẽ dùng một chút thủ thuật.
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
