#pragma once
#include "tree.h"
#include <iostream>

inline void print_menu() {
    std::cout << std::endl;
    std::cout << "1. Insert value in tree." << std::endl;
    std::cout << "2. Erase value from tree." << std::endl;
    std::cout << "3. Find value in tree." << std::endl;
    std::cout << "4. Print tree." << std::endl;
    std::cout << "5. Print subtree by value." << std::endl;
    std::cout << "6. Print branch by value." << std::endl;
    std::cout << "7. Print leafs." << std::endl;
    std::cout << "8. Print tree height." << std::endl;
    std::cout << "9. Print tree size." << std::endl;
    std::cout << "0. Clear tree." << std::endl;
    std::cout << "10. Exit." << std::endl;
    std::cout << "Enter: ";
}

template <class T>
void insert(Tree<T> &tree) {
    T data;
    std::cout << "Enter element: ";
    std::cin >> data;

    if (tree.insert(data))
        std::cout << "Success insertion." << std::endl;
    else
        std::cout << "This element already exists." << std::endl;
}

template <class T>
void erase(Tree<T> &tree) {
    T data;
    std::cout << "Enter element: ";
    std::cin >> data;

    if (tree.erase(data))
        std::cout << "Success deletion." << std::endl;
    else
        std::cout << "This element does not exist." << std::endl;
}

template <class T>
void find(Tree<T> &tree) {
    T data;
    std::cout << "Enter element: ";
    std::cin >> data;

    if (tree.find(data))
        std::cout << "This element already exists." << std::endl;
    else
        std::cout << "This element does not exist." << std::endl;
}

template <class T>
void print_tree(Tree<T> &tree) {
    int type, direct;

    do {
        std::cout << "Choose type of printing:" << std::endl;
        std::cout << "1. preorder" << std::endl;
        std::cout << "2. inorder" << std::endl;
        std::cout << "3. postorder" << std::endl;
        std::cout << "Enter: ";
        std::cin >> type;
    } while (type < 1 || type > 3);

    std::cout << "Choose direction of printing: " << std::endl;
    std::cout << "1. direct" << std::endl;
    std::cout << "2. reversed" << std::endl;
    std::cout << "Enter: ";

    std::cin >> direct;
    std::cout << "Tree: ";
    typename Tree<T>::Mode m = direct == 1 ? Tree<T>::L : Tree<T>::R;

    if (type == 1)
        print_preorder(tree, std::cout, " ", m) << std::endl;
    else if (type == 2)
        print_inorder(tree, std::cout, " ", m) << std::endl;
    else
        print_postorder(tree, std::cout, " ", m) << std::endl;
}

template <class T>
void print_sub(Tree<T> &tree) {
    T data;
    std::cout << "Enter element: ";
    std::cin >> data;

    int type, direct;
    do {
        std::cout << "Choose type of printing:" << std::endl;
        std::cout << "1. preorder" << std::endl;
        std::cout << "2. inorder" << std::endl;
        std::cout << "3. postorder" << std::endl;
        std::cout << "Enter: ";
        std::cin >> type;
    } while (type < 1 || type > 3);

    std::cout << "Choose direction of printing: " << std::endl;
    std::cout << "1. direct" << std::endl;
    std::cout << "2. reversed" << std::endl;
    std::cout << "Enter: ";

    std::cin >> direct;
    std::cout << "Subtree: ";
    typename Tree<T>::Mode m = direct == 1 ? Tree<T>::L : Tree<T>::R;

    if (type == 1)
        print_sub_tree_preorder(tree, std::cout, " ", data, m) << std::endl;
    else if (type == 2)
        print_sub_tree_inorder(tree, std::cout, " ", data, m) << std::endl;
    else
        print_sub_tree_postorder(tree, std::cout, " ", data, m) << std::endl;
}

template <class T>
void print_leafs(Tree<T> &tree) {
    std::cout << "Choose direction of printing: " << std::endl;
    std::cout << "1. direct" << std::endl;
    std::cout << "2. reversed" << std::endl;
    std::cout << "Enter: ";

    int direct;
    std::cin >> direct;
    std::cout << "Leafs: ";

    typename Tree<T>::Mode m = direct == 1 ? Tree<T>::L : Tree<T>::R;
    print_leafs(tree, std::cout, " ", m) << std::endl;
}

template <class T>
void print_branch(Tree<T> &tree) {
    T data;
    std::cout << "Enter element: ";
    std::cin >> data;

    std::cout << "Branch: ";
    print_branch(tree, std::cout, " ", data) << std::endl;
}

template <class T>
void print_height(Tree<T> &tree) {
    std::cout << "Height: " << tree.height() << std::endl;
}

template <class T>
void print_size(Tree<T> &tree) {
    std::cout << "Size: " << tree.size() << std::endl;
}

template <class T>
void menu(Tree<T> &tree) {
    bool exit_ = false;
    int choice;

    while (!exit_) {
        print_menu();
        std::cin >> choice;

        switch (choice) {
        case 1:
            insert(tree);
            break;
        case 2:
            erase(tree);
            break;
        case 3:
            find(tree);
            break;
        case 4:
            print_tree(tree);
            break;
        case 5:
            print_sub(tree);
            break;
        case 6:
            print_branch(tree);
            break;
        case 7:
            print_leafs(tree);
            break;
        case 8:
            print_height(tree);
            break;
        case 9:
            print_size(tree);
            break;
        case 0:
            tree.clear();
            break;
        case 10:
            exit_ = true;
            break;
        default:
            std::cout << "Wrong key!" << std::endl;
        }
    }
}
