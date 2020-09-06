#include "BTree.hpp"

#include <cstdint>

int main()
{
    BTree<int64_t, int64_t, 3> tree;
    tree.insert(128, 128);
    std::cout <<"----\n";
    tree.print();
    tree.insert(45, 18);
    std::cout <<"----\n";
    tree.print();
    tree.insert(18, 12);
    std::cout <<"----\n";
    tree.print();
    tree.insert(5, 1);
    std::cout <<"----\n";
    tree.print();
    tree.insert(16, 7);
    std::cout <<"----\n";
    tree.print();
    tree.insert(32, 47);
    std::cout <<"----\n";
    tree.print();
    tree.insert(3, 7);
    std::cout <<"----\n";
    tree.print();
    tree.insert(30, 27);
    std::cout <<"----\n";
    tree.print();
    tree.insert(0, 9);
    std::cout <<"----\n";
    tree.print();
    return 0;
}
