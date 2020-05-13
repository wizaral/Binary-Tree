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
        Node *prev = nullptr;
        Node *next[2]{nullptr, nullptr};    // [0] next[L], [1] next[R]
    };
    enum {L = 0, R = 1};

    using Fcr = const std::function<void(const T &data)>;
    using Fr = const std::function<void(T &data)>;

    Node *root_ = nullptr;
    std::size_t size_ = 0;
public:
    Tree() = default;

    Tree(const Tree &tree) {
        merge(tree);
    }

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
                } else if (bool cmp = fast->data < data; !(fast = fast->next[cmp])) {
                    slow->next[cmp] = new Node{data, slow};
                    break;
                }
            }
        }
        return ++size_;
    }

    bool erase(const T &data) {
        if (Node *node = find(root_, data); node) {
            erase(node);
            return true;
        }
        return false;
    }

    void merge(const Tree &tree) {
        tree.preorder([this](const T &data) {
            this->insert(data);
        });
    }

    bool find(const T &data) const {
        return find(root_, data) != nullptr;
    }

    void preorder(Fcr &f) const {
        preorder(root_, f);
    }

    void inorder(Fcr &f) const {
        inorder(root_, f);
    }

    void postorder(Fcr &f) const {
        postorder(root_, f);
    }

private:
    std::size_t height(const Node *node) const {
        return node ? std::max(height(node->next[L]), height(node->next[R])) + 1 : 0;
    }

    void clear(Node *node) {
        if (node) {
            clear(node->next[L]);
            clear(node->next[R]);
            delete node;
            --size_;
        }
    }

    Node *find(Node *node, const T &data) const {
        return !node || data == node->data ? node : find(node->next[node->data < data], data);
    }

    Node *min(Node *node) {
        while (node->next[L])
            node = node->next[L];
        return node;
    }

    Node *max(Node *node) {
        while (node->next[R])
            node = node->next[R];
        return node;
    }

    void erase(Node *node) {
        if (node->next[L] && node->next[R]) {
            Node *max_ = max(node->next[L]);
            std::swap(node->data, max_->data);
            return (erase(max_), void(0));
        } else if (node->prev) {
            if (node->next[L] || node->next[R])
                node->prev->next[node == node->prev->next[R]] = node->next[node->next[R] != nullptr];
            else
                node->prev->next[node == node->prev->next[R]] = nullptr;
        } else if (size_ > 1u) {
            root_ = node->next[node->next[R] != nullptr];
            root_->prev = nullptr;
        } else {
            root_ = nullptr;
        }
        delete node;
        --size_;
    }

    void preorder(Node *node, Fr &f) {
        if (node) {
            f(node->data);
            preorder(node->next[L], f);
            preorder(node->next[R], f);
        }
    }

    void inorder(Node *node, Fr &f) {
        if (node) {
            inorder(node->next[L], f);
            f(node->data);
            inorder(node->next[R], f);
        }
    }

    void postorder(Node *node, Fr &f) {
        if (node) {
            postorder(node->next[L], f);
            postorder(node->next[R], f);
            f(node->data);
        }
    }

    void preorder(const Node *node, Fcr &f) const {
        if (node) {
            f(node->data);
            preorder(node->next[L], f);
            preorder(node->next[R], f);
        }
    }

    void inorder(const Node *node, Fcr &f) const {
        if (node) {
            inorder(node->next[L], f);
            f(node->data);
            inorder(node->next[R], f);
        }
    }

    void postorder(const Node *node, Fcr &f) const {
        if (node) {
            postorder(node->next[L], f);
            postorder(node->next[R], f);
            f(node->data);
        }
    }
};
