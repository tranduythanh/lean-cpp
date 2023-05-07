#include <gtest/gtest.h>
#include "btree.cpp"


TEST(BTreeTest, GetSibling) {
    auto tree = new BTreeN<int>(2, 20);
    ASSERT_TRUE(tree->Root != NULL);
    
    tree->Insert(20);
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
    tree->Insert(25);

    // Sibling of page contains 42 is page contains 32
    Page<int>* sibling;
    SearchResult<int>* sr;
    sr = tree->Root->Search(42, false);
    ASSERT_TRUE(sr->P != NULL);
    sibling = sr->P->GetSibling();
    ASSERT_TRUE(sibling == tree->Root->Search(32, false)->P);
    
    // Sibling of page contains 32 is page contains 42
    sr = tree->Root->Search(32, false);
    ASSERT_TRUE(sr->P != NULL);
    sibling = sr->P->GetSibling();
    ASSERT_TRUE(sibling == tree->Root->Search(42, false)->P);

    // Sibling of page contains 27 is page contains 32
    sr = tree->Root->Search(27, false);
    ASSERT_TRUE(sr->P != NULL);
    sibling = sr->P->GetSibling();
    ASSERT_TRUE(sibling == tree->Root->Search(32, false)->P);

    // Sibling of page contains 26 is page contains 35
    sr = tree->Root->Search(26, false);
    ASSERT_TRUE(sr->P != NULL);
    sibling = sr->P->GetSibling();
    ASSERT_TRUE(sibling == tree->Root->Search(35, false)->P);

    // Sibling of page contains 38 is page contains 46
    sr = tree->Root->Search(38, false);
    ASSERT_TRUE(sr->P != NULL);
    sibling = sr->P->GetSibling();
    ASSERT_TRUE(sibling == tree->Root->Search(46, false)->P);

    // Sibling of page contains 30 is page contains 20
    sr = tree->Root->Search(30, false);
    ASSERT_TRUE(sr->P != NULL);
    sibling = sr->P->GetSibling();
    ASSERT_TRUE(sibling == tree->Root->Search(20, false)->P);

    // Sibling of page contains 10 is page contains 40
    sr = tree->Root->Search(10, false);
    ASSERT_TRUE(sr->P != NULL);
    sibling = sr->P->GetSibling();
    ASSERT_TRUE(sibling == tree->Root->Search(40, false)->P);

    // Sibling of page contains 5 is page contains 13
    sr = tree->Root->Search(5, false);
    ASSERT_TRUE(sr->P != NULL);
    sibling = sr->P->GetSibling();
    ASSERT_TRUE(sibling == tree->Root->Search(13, false)->P);

    // Sibling of page contains 13 is page contains 24
    sr = tree->Root->Search(13, false);
    ASSERT_TRUE(sr->P != NULL);
    sibling = sr->P->GetSibling();
    ASSERT_TRUE(sibling == tree->Root->Search(24, false)->P);

    // Sibling of page contains 25 is NULL
    sr = tree->Root->Search(25, false);
    ASSERT_TRUE(sr->P != NULL);
    sibling = sr->P->GetSibling();
    ASSERT_TRUE(sibling == NULL);
}

TEST(BTreeTest, SimpleTree) {
    auto tree = new BTreeN<int>(2, 20);
    ASSERT_TRUE(tree->Root != NULL);
    
    tree->Insert(20);
    ASSERT_TRUE(tree->Root->Elems.size() == 1);
    ASSERT_TRUE(tree->Root->Elems.at(0)->IsLeaf() == true);
    tree->Insert(40);
    ASSERT_TRUE(tree->Root->Elems.size() == 2);
    ASSERT_TRUE(tree->Root->Elems.at(0)->IsLeaf() == true);
    tree->Insert(10);
    ASSERT_TRUE(tree->Root->Elems.size() == 3);
    ASSERT_TRUE(tree->Root->Elems.at(0)->IsLeaf() == true);
    tree->Insert(30);
    ASSERT_TRUE(tree->Root->Elems.size() == 4);
    ASSERT_TRUE(tree->Root->Elems.at(0)->IsLeaf() == true);
    ASSERT_TRUE(tree->Root->Elems.at(1)->IsLeaf() == true);
    ASSERT_TRUE(tree->Root->Elems.at(2)->IsLeaf() == true);
    ASSERT_TRUE(tree->Root->Elems.at(3)->IsLeaf() == true);
    tree->Insert(15);
    ASSERT_TRUE(tree->Root->Elems.size() == 1);
    ASSERT_TRUE(tree->Root->Elems.at(0)->Data == 20);
    ASSERT_TRUE(tree->Root->Elems.at(0)->IsLeaf() == false);
    ASSERT_TRUE(tree->Root->LeftPage != NULL);
    ASSERT_TRUE(tree->Root->Elems.at(0)->RightPage != NULL);
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
    tree->Insert(25);
    ASSERT_TRUE(tree->Root->Elems.size() == 1);
    ASSERT_TRUE(tree->Root->Elems.at(0)->Data == 25);
    // check left of root
    auto rl = tree->Root->LeftPage;
    ASSERT_TRUE(rl != NULL);
    ASSERT_TRUE(rl->Parent == tree->Root);
    ASSERT_TRUE(rl->Elems.size() == 2);
    ASSERT_TRUE(rl->Elems.at(0)->Data == 10);
    ASSERT_TRUE(rl->Elems.at(1)->Data == 20);

    // check right of elem 0 of root
    auto r0 = tree->Root->Elems.at(0)->RightPage;
    ASSERT_TRUE(r0 != NULL);
    ASSERT_TRUE(r0->Parent == tree->Root);
    ASSERT_TRUE(r0->Elems.size() == 2);
    ASSERT_TRUE(r0->Elems.at(0)->Data == 30);
    ASSERT_TRUE(r0->Elems.at(1)->Data == 40);

    auto rll = rl->LeftPage;
    ASSERT_TRUE(rll != NULL);
    ASSERT_TRUE(rll->Parent == rl);
    ASSERT_TRUE(rll->Elems.size() == 3);
    ASSERT_TRUE(rll->Elems.at(0)->Data == 5);
    ASSERT_TRUE(rll->Elems.at(1)->Data == 7);
    ASSERT_TRUE(rll->Elems.at(2)->Data == 8);
    ASSERT_TRUE(rll->Elems.at(0)->RightPage == NULL);
    ASSERT_TRUE(rll->Elems.at(1)->RightPage == NULL);
    ASSERT_TRUE(rll->Elems.at(2)->RightPage == NULL);
    ASSERT_TRUE(rll->Elems.at(0)->WrapperPage == rll);
    ASSERT_TRUE(rll->Elems.at(1)->WrapperPage == rll);
    ASSERT_TRUE(rll->Elems.at(2)->WrapperPage == rll);

    auto rl0r = rl->Elems.at(0)->RightPage;
    ASSERT_TRUE(rl0r != NULL);
    ASSERT_TRUE(rl0r->Parent == rl);
    ASSERT_TRUE(rl0r->Elems.size() == 3);
    ASSERT_TRUE(rl0r->Elems.at(0)->Data == 13);
    ASSERT_TRUE(rl0r->Elems.at(1)->Data == 15);
    ASSERT_TRUE(rl0r->Elems.at(2)->Data == 18);
    ASSERT_TRUE(rl0r->Elems.at(0)->RightPage == NULL);
    ASSERT_TRUE(rl0r->Elems.at(1)->RightPage == NULL);
    ASSERT_TRUE(rl0r->Elems.at(2)->RightPage == NULL);
    ASSERT_TRUE(rl0r->Elems.at(0)->WrapperPage == rl0r);
    ASSERT_TRUE(rl0r->Elems.at(1)->WrapperPage == rl0r);
    ASSERT_TRUE(rl0r->Elems.at(2)->WrapperPage == rl0r);

    auto rl1r = rl->Elems.at(1)->RightPage;
    ASSERT_TRUE(rl1r != NULL);
    ASSERT_TRUE(rl1r->Parent == rl);
    ASSERT_TRUE(rl1r->Elems.size() == 2);
    ASSERT_TRUE(rl1r->Elems.at(0)->Data == 22);
    ASSERT_TRUE(rl1r->Elems.at(1)->Data == 24);
    ASSERT_TRUE(rl1r->Elems.at(0)->RightPage == NULL);
    ASSERT_TRUE(rl1r->Elems.at(1)->RightPage == NULL);
    ASSERT_TRUE(rl1r->Elems.at(0)->WrapperPage == rl1r);
    ASSERT_TRUE(rl1r->Elems.at(1)->WrapperPage == rl1r);

    auto rr = tree->Root->Elems.at(0)->RightPage;
    ASSERT_TRUE(rr != NULL);
    ASSERT_TRUE(rr->Elems.size() == 2);
    ASSERT_TRUE(rr->Elems.at(0)->Data == 30);
    ASSERT_TRUE(rr->Elems.at(1)->Data == 40);

    auto rrl = rr->LeftPage;
    ASSERT_TRUE(rrl != NULL);
    ASSERT_TRUE(rrl->Parent == rr);
    ASSERT_TRUE(rrl->Elems.size() == 2);
    ASSERT_TRUE(rrl->Elems.at(0)->Data == 26);
    ASSERT_TRUE(rrl->Elems.at(1)->Data == 27);
    ASSERT_TRUE(rrl->Elems.at(0)->RightPage == NULL);
    ASSERT_TRUE(rrl->Elems.at(1)->RightPage == NULL);
    ASSERT_TRUE(rrl->Elems.at(0)->WrapperPage == rrl);
    ASSERT_TRUE(rrl->Elems.at(1)->WrapperPage == rrl);

    auto rr0r = rr->Elems.at(0)->RightPage;
    ASSERT_TRUE(rr0r != NULL);
    ASSERT_TRUE(rr0r->Parent == rr);
    ASSERT_TRUE(rr0r->Elems.size() == 3);
    ASSERT_TRUE(rr0r->Elems.at(0)->Data == 32);
    ASSERT_TRUE(rr0r->Elems.at(1)->Data == 35);
    ASSERT_TRUE(rr0r->Elems.at(2)->Data == 38);
    ASSERT_TRUE(rr0r->Elems.at(0)->RightPage == NULL);
    ASSERT_TRUE(rr0r->Elems.at(1)->RightPage == NULL);
    ASSERT_TRUE(rr0r->Elems.at(2)->RightPage == NULL);
    ASSERT_TRUE(rr0r->Elems.at(0)->WrapperPage == rr0r);
    ASSERT_TRUE(rr0r->Elems.at(1)->WrapperPage == rr0r);
    ASSERT_TRUE(rr0r->Elems.at(2)->WrapperPage == rr0r);

    auto rr1r = rr->Elems.at(1)->RightPage;
    ASSERT_TRUE(rr1r != NULL);
    ASSERT_TRUE(rr1r->Parent == rr);
    ASSERT_TRUE(rr1r->Elems.size() == 3);
    ASSERT_TRUE(rr1r->Elems.at(0)->Data == 42);
    ASSERT_TRUE(rr1r->Elems.at(1)->Data == 45);
    ASSERT_TRUE(rr1r->Elems.at(2)->Data == 46);
    ASSERT_TRUE(rr1r->Elems.at(0)->RightPage == NULL);
    ASSERT_TRUE(rr1r->Elems.at(1)->RightPage == NULL);
    ASSERT_TRUE(rr1r->Elems.at(2)->RightPage == NULL);
    ASSERT_TRUE(rr1r->Elems.at(0)->WrapperPage == rr1r);
    ASSERT_TRUE(rr1r->Elems.at(1)->WrapperPage == rr1r);
    ASSERT_TRUE(rr1r->Elems.at(2)->WrapperPage == rr1r);
}

TEST(BTreeTest, DeleteLeafItem) {
    auto tree = make_unique<BTreeN<int>>(2, 20);
    ASSERT_TRUE(tree->Root != nullptr);

    // Insert items
    const std::array<int, 21> items = {20, 40, 10, 30, 15, 35, 7, 26, 18, 22, 5, 42, 13, 46, 27, 8, 32, 38, 24, 45, 25};
    for (const auto& item : items) {
        tree->Insert(item);
    }

    // Delete items
    tree->Delete(46);
    tree->Delete(45);

    // Check expected results
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.at(1)->Data, 38);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.at(1)->RightPage->Elems.at(0)->Data, 40);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.at(1)->RightPage->Elems.at(1)->Data, 42);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.at(0)->RightPage->Elems.at(0)->Data, 32);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.at(0)->RightPage->Elems.at(1)->Data, 35);

    // Check sibling of page containing 30 is page containing 20
    auto sr = tree->Root->Search(30, false);
    ASSERT_TRUE(sr->P != nullptr);
    auto sibling = sr->P->GetSibling();
    ASSERT_TRUE(sibling == tree->Root->Search(20, false)->P);
    tree->Draw();

    tree->Delete(35);
    ASSERT_EQ(tree->Root->Elems.size(), 4);
    ASSERT_EQ(tree->Root->Elems.at(3)->RightPage->Parent, tree->Root);
    tree->Draw();
}


TEST(BTreeTest, DeleteNonLeafItem) {
    auto tree = make_unique<BTreeN<int>>(2, 20);
    ASSERT_TRUE(tree->Root != nullptr);

    // Insert items
    const std::array<int, 21> items = {20, 40, 10, 30, 15, 35, 7, 26, 18, 22, 5, 42, 13, 46, 27, 8, 32, 38, 24, 45, 25};
    for (const auto& item : items) {
        tree->Insert(item);
    }
    
    cout << "try to delete 25" << endl;
    tree->Delete(25);
    tree->Draw();
    ASSERT_EQ(tree->Root->Elems.size(), 1);
    ASSERT_EQ(tree->Root->LeftPage->Elems.size(), 2);
    ASSERT_EQ(tree->Root->LeftPage->Elems.at(0)->Data, 10);
    ASSERT_EQ(tree->Root->LeftPage->Elems.at(1)->Data, 18);
    ASSERT_EQ(tree->Root->LeftPage->Elems.at(1)->RightPage->Elems.size(), 2);
    ASSERT_EQ(tree->Root->LeftPage->Elems.at(1)->RightPage->Elems.at(0)->Data, 20);
    ASSERT_EQ(tree->Root->LeftPage->Elems.at(1)->RightPage->Elems.at(1)->Data, 22);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.size(), 2);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.at(0)->Data, 30);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.at(0)->RightPage->Elems.at(0)->Data, 32);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.at(0)->RightPage->Elems.at(1)->Data, 35);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.at(0)->RightPage->Elems.at(2)->Data, 38);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.at(1)->Data, 40);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.at(1)->RightPage->Elems.at(0)->Data, 42);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.at(1)->RightPage->Elems.at(1)->Data, 45);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.at(1)->RightPage->Elems.at(2)->Data, 46);

    tree->Delete(45);
    tree->Delete(24);
    tree->Draw();
    ASSERT_EQ(tree->Root->Elems.size(), 4);

    tree->Delete(38);
    tree->Delete(32);
    tree->Draw();
    ASSERT_EQ(tree->Root->Elems.size(), 3);
    ASSERT_EQ(tree->Root->Elems.at(2)->Data, 30);
    ASSERT_EQ(tree->Root->Elems.at(2)->RightPage->Elems.size(), 4);

    tree->Delete(8);
    tree->Delete(27);
    ASSERT_EQ(tree->Root->Elems.size(), 3);
    ASSERT_EQ(tree->Root->Elems.at(2)->Data, 35);
    ASSERT_EQ(tree->Root->Elems.at(2)->RightPage->Elems.size(), 3);
    tree->Draw();

    tree->Delete(46);
    tree->Delete(13);
    tree->Delete(42);
    ASSERT_EQ(tree->Root->Elems.size(), 2);
    ASSERT_EQ(tree->Root->Elems.at(1)->Data, 22);
    ASSERT_EQ(tree->Root->Elems.at(1)->RightPage->Elems.size(), 4);
    tree->Draw();

    tree->Delete(5);
    tree->Delete(22);
    ASSERT_EQ(tree->Root->Elems.size(), 2);
    ASSERT_EQ(tree->Root->Elems.at(0)->Data, 15);
    ASSERT_EQ(tree->Root->Elems.at(1)->Data, 26);
    ASSERT_EQ(tree->Root->Elems.at(1)->RightPage->Elems.size(), 3);
    tree->Draw();

    tree->Delete(18);
    tree->Delete(26);
    ASSERT_EQ(tree->Root->Elems.size(), 1);
    ASSERT_EQ(tree->Root->Elems.at(0)->Data, 15);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.size(), 4);
    tree->Draw();

    tree->Delete(7);
    tree->Delete(35);
    ASSERT_EQ(tree->Root->Elems.size(), 1);
    ASSERT_EQ(tree->Root->Elems.at(0)->Data, 20);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage->Elems.size(), 2);
    tree->Draw();

    tree->Delete(15);
    ASSERT_EQ(tree->Root->Elems.size(), 4);
    ASSERT_EQ(tree->Root->Elems.at(0)->Data, 10);
    ASSERT_EQ(tree->Root->Elems.at(0)->RightPage, nullptr);
    tree->Draw();

    tree->Delete(10);
    tree->Delete(20);
    tree->Delete(30);
    tree->Delete(40);
    ASSERT_EQ(tree->Root->Elems.size(), 0);
    tree->Draw();

    tree->Delete(70);
    ASSERT_EQ(tree->Root->Elems.size(), 0);
    tree->Draw();

    tree->Insert(1000);
    tree->Insert(2000);
    tree->Insert(3000);
    tree->Insert(4000);
    tree->Insert(5000);
    ASSERT_EQ(tree->Root->Elems.size(), 1);
    ASSERT_EQ(tree->Root->Elems.at(0)->Data, 3000);
    tree->Draw();
}
