#include "btree.cpp"

int main(int argc, char **argv) {
    auto tree = make_unique<BTreeN<int>>(2);
    // Insert items
    const std::array<int, 20> items = {
        20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 
        10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    for (const auto& item : items) {
        tree->Insert(item);
        tree->Draw();
    }
}
