#include "btree.cpp"

int main(int argc, char **argv) {
    auto tree = make_unique<BTreeN<string>>(2);
    // Insert items
    const std::array<string, 21> items = {
        "ca", "ea", "ba", "da", "bf", "df", "ah", "cg", "bi", "cc", "af", 
        "eg", "bd", "ec", "ch", "ai", "dc", "di", "ce", "ef", "cf"};
    for (const auto& item : items) {
        tree->Insert(item);
        tree->Draw();
    }
}