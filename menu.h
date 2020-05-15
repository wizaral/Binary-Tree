#include "tree.h"

void print_menu() {
    std::cout << std::endl;
    std::cout << "1. Insert value in tree." << std::endl;
    std::cout << "2. Erase value from tree." << std::endl;
    std::cout << "3. Find value in tree." << std::endl;
    std::cout << "4. Print tree." << std::endl;
    std::cout << "5. Print subtree." << std::endl;
    std::cout << "6. Print leafs." << std::endl;
    std::cout << "7. Print branch." << std::endl;
    std::cout << "8. Exit." << std::endl;
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
    int type;
    bool direct;

    do {
        std::cout << "Choose type of printing:" << std::endl;
        std::cout << "1. preorder" << std::endl;
        std::cout << "2. inorder" << std::endl;
        std::cout << "3. postorder" << std::endl;
        std::cout << "4. pyramid" << std::endl;
        std::cout << "Enter: ";
        std::cin >> type;
    } while (type < 1 || type > 4);

    if (type != 4) {
        std::cout << "Choose direction of printing: " << std::endl;
        std::cout << "0. direct" << std::endl;
        std::cout << "1. reversed" << std::endl;
        std::cout << "Enter: ";
        std::cin >> direct;
    }
    typename Tree<T>::Mode m = !direct ? Tree<T>::L : Tree<T>::R;

    std::cout << "Tree: ";
    if (type == 1)
        print_preorder(tree, std::cout, ' ', m) << std::endl;
    else if (type == 2)
        print_inorder(tree, std::cout, ' ', m) << std::endl;
    else if (type == 3)
        print_postorder(tree, std::cout, ' ', m) << std::endl;
    else
        tree.pyramid_print(std::cout) << std::endl;
}

template <class T>
void print_sub(Tree<T> &tree) {
    T data;
    std::cout << "Enter element: ";
    std::cin >> data;

    int type;
    bool direct;

    do {
        std::cout << "Choose type of printing:" << std::endl;
        std::cout << "1. preorder" << std::endl;
        std::cout << "2. inorder" << std::endl;
        std::cout << "3. postorder" << std::endl;
        std::cout << "Enter: ";
        std::cin >> type;
    } while (type < 1 || type > 3);

    std::cout << "Choose direction of printing: " << std::endl;
    std::cout << "0. direct" << std::endl;
    std::cout << "1. reversed" << std::endl;
    std::cout << "Enter: ";
    std::cin >> direct;
    typename Tree<T>::Mode m = !direct ? Tree<T>::L : Tree<T>::R;

    std::cout << "Subtree: ";
    if (type == 1)
        print_sub_tree_preorder(tree, std::cout, ' ', data, m) << std::endl;
    else if (type == 2)
        print_sub_tree_inorder(tree, std::cout, ' ', data, m) << std::endl;
    else
        print_sub_tree_postorder(tree, std::cout, ' ', data, m) << std::endl;
}

template <class T>
void print_leafs(Tree<T> &tree) {
    print_leafs(tree, std::cout, ' ') << std::endl;
}

template <class T>
void print_branch(Tree<T> &tree) {
    T data;
    std::cout << "Enter element: ";
    std::cin >> data;

    std::cout << "Branch: ";
    print_branch(tree, std::cout, ' ', data) << std::endl;
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
            print_leafs(tree);
            break;
        case 7:
            print_branch(tree);
            break;
        case 8:
            exit_ = true;
        }
    }
}
