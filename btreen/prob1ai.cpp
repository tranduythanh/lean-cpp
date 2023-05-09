#include "btree.cpp"

int main(int argc, char **argv) {
    auto tree = make_unique<BTreeN<int>>(2);
    // Insert items
    const std::array<int, 14> items = {25, 17, 31, 42, 21, 19, 26, 33, 47, 44, 45, 43, 8, 9};
    for (const auto& item : items) {
        tree->Insert(item);
        tree->Draw();
    }
}