#include "btree.cpp"

int main(int argc, char **argv) {
    auto tree = make_unique<BTreeN<int>>(3);
    // Insert items
    const std::array<int, 40> items = {
        40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 
        30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 
        20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 
        10,  9,  8,  7,  6,  5,  4,  3,  2,  1
    };
    for (const auto& item : items) {
        tree->Insert(item);
    }
    tree->Draw();

    tree->Destroy();

    if (tree == nullptr) {
        cout << "deallocated success" << endl;
    }
}