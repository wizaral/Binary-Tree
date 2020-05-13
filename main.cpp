#include <string>

#include "tree.h"
using std::cin, std::cout, std::endl;

int main() {
    Tree tree;

    tree.insert(1);
    tree.insert(11);
    tree.insert(111);

    tree.inorder([] (auto &&data) {
        std::cout << data << std::endl;
    });

    Tree temp;
    temp.insert(2);
    temp.insert(22);
    temp.insert(222);

    tree.merge(temp);
    tree.inorder([] (auto &&data) {
        std::cout << data << std::endl;
    });

    
}
