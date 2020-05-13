#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <numeric>

// template <class T>
using T = int;

class Tree final {
    struct Node {
        T data;
        Node *parent = nullptr;
        std::array<Node*, 2> nodes{nullptr, nullptr};    // [0] nodes[0], [1] nodes[1]
    };

    Node *root_ = nullptr;
    std::size_t size_ = 0;
public:
    Tree() = default;

    // Tree(T &&...args) {
    //     // std::forward<Args>(args)...
    // }

    Tree(const Tree &tree) {
        merge(tree);
    }

    // Tree(Tree &&tree) {
    //     merge(std::move(tree));
    //     tree.clear();
    // }

    ~Tree() {
        clear();
    }

    Tree &operator=(const Tree &tree) {
        clear();
        Tree temp(tree);
        root_ = temp.root_;
        size_ = temp.size_;
        return *this;
    }

    // Tree &operator=(Tree &&tree) {
    //     clear();
    //     root_ = tree.root_;
    //     size_ = tree.size_;

    //     tree.root_ = nullptr;
    //     tree.size_ = 0;
    //     return *this;
    // }

    bool empty() const {
        return size_ == 0;
    }

    std::size_t size() const {
        return size_;
    }

    std::size_t height() const {
        return height(root_);
    }

    void clear() {
        clear(root_);
        root_ = nullptr;
    }

    bool insert(const T &data) {
        if (root_ == nullptr) {
            root_ = new Node{data};
        } else {
            for (Node *fast = root_, *slow = fast; true; slow = fast) {
                if (data == fast->data) {
                    return false;
                } else if (bool cmp = fast->data < data; !(fast = fast->nodes[cmp])) {
                    slow->nodes[cmp] = new Node{data, slow};
                    break;
                }
            }
        }
        ++size_;
        return true;
    }
    // bool insert(T &&data) {
    // }

    bool erase(const T &data) {
        return false;
    }

    void merge(const Tree &tree) {
        tree.preorder([this](const T &data) {
            this->insert(data);
        });
    }
    // void merge(Tree &&tree) {
    //     tree.preorder([this](Node *node) {
    //         this->insert(node->data);
    //     });
    // }

    bool find(const T &data) const {
        return find(root_, data) != nullptr;
    }

    void preorder(const std::function<void(const T &data)> &f) const {
        preorder(root_, f);
    }

    void inorder(const std::function<void(const T &data)> &f) const {
        inorder(root_, f);
    }

    void postorder(const std::function<void(const T &data)> &f) const {
        postorder(root_, f);
    }

private:
    std::size_t height(const Node *node) const {
        return node ? std::max(height(node->nodes[0]), height(node->nodes[1])) + 1 : 0;
    }

    void clear(Node *node) {
        if (node) {
            clear(node->nodes[0]);
            clear(node->nodes[1]);
            delete node;
            --size_;
        }
    }

    Node *find(Node *node, const T &data) const {
        if (node) {
            if (data == node->data) {
                return node;
            } else {
                return find(node->nodes[node->data < data], data);
            }
        }
        return nullptr;
    }

    void preorder(Node *node, const std::function<void(T &data)> &f) {
        if (node) {
            f(node->data);
            preorder(node->nodes[0], f);
            preorder(node->nodes[1], f);
        }
    }

    void inorder(Node *node, const std::function<void(T &data)> &f) {
        if (node) {
            inorder(node->nodes[0], f);
            f(node->data);
            inorder(node->nodes[1], f);
        }
    }

    void postorder(Node *node, const std::function<void(T &data)> &f) {
        if (node) {
            postorder(node->nodes[0], f);
            postorder(node->nodes[1], f);
            f(node->data);
        }
    }

    void preorder(const Node *node, const std::function<void(const T &data)> &f) const {
        if (node) {
            f(node->data);
            preorder(node->nodes[0], f);
            preorder(node->nodes[1], f);
        }
    }

    void inorder(const Node *node, const std::function<void(const T &data)> &f) const {
        if (node) {
            inorder(node->nodes[0], f);
            f(node->data);
            inorder(node->nodes[1], f);
        }
    }

    void postorder(const Node *node, const std::function<void(const T &data)> &f) const {
        if (node) {
            postorder(node->nodes[0], f);
            postorder(node->nodes[1], f);
            f(node->data);
        }
    }
};
