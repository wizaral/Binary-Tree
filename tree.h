#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>

template <class T>
class Tree final {
    struct Node {
        T data;
        Node *prev = nullptr;
        Node *next[2]{nullptr, nullptr}; // [0] next[L], [1] next[R]
    };

    using Fcr = std::function<void(const T &data)>;
    using Fr = std::function<void(T &data)>;

    Node *root_ = nullptr;
    std::size_t size_ = 0;

public:
    enum Mode { L = 0, R = 1 };

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
        if (auto *node = find(root_, data); node) {
            erase(node);
            return true;
        }
        return false;
    }

    void merge(const Tree &tree) {
        tree.preorder([this](const T &data) {
            insert(data);
        });
    }

    bool find(const T &data) const {
        return find(root_, data) != nullptr;
    }

    void preorder(Fcr f, Mode m = L) const {
        preorder(root_, f, m);
    }

    void inorder(Fcr f, Mode m = L) const {
        inorder(root_, f, m);
    }

    void postorder(Fcr f, Mode m = L) const {
        postorder(root_, f, m);
    }

    void leafs(Fcr f, Mode m = L) const {
        leafs(root_, f, m);
    }

    void branch(const T &data, Fcr f) const {
        if (find(root_, data) != nullptr)
            branch(root_, data, f);
    }

    void sub_tree_preorder(const T &data, Fcr f, Mode m = L) const {
        if (const auto *d = find(root_, data); d != nullptr)
            preorder(d, f, m);
    }

    void sub_tree_inorder(const T &data, Fcr f, Mode m = L) const {
        if (const auto *d = find(root_, data); d != nullptr)
            inorder(d, f, m);
    }

    void sub_tree_postorder(const T &data, Fcr f, Mode m = L) const {
        if (const auto *d = find(root_, data); d != nullptr)
            postorder(d, f, m);
    }

    std::ostream &pyramid_print(std::ostream &os) const {
        std::size_t step = 0;
        return pyramid_print(root_, os, step);
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
            auto *max_ = max(node->next[L]);
            std::swap(node->data, max_->data);
            return (erase(max_), void());
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

    void preorder(Node *node, Fr f, Mode m = L) {
        if (node) {
            f(node->data);
            preorder(node->next[m != L], f, m);
            preorder(node->next[m != R], f, m);
        }
    }

    void inorder(Node *node, Fr f, Mode m = L) {
        if (node) {
            inorder(node->next[m != L], f, m);
            f(node->data);
            inorder(node->next[m != R], f, m);
        }
    }

    void postorder(Node *node, Fr f, Mode m = L) {
        if (node) {
            postorder(node->next[m != L], f, m);
            postorder(node->next[m != R], f, m);
            f(node->data);
        }
    }

    void preorder(const Node *node, Fcr f, Mode m = L) const {
        if (node) {
            f(node->data);
            preorder(node->next[m != L], f, m);
            preorder(node->next[m != R], f, m);
        }
    }

    void inorder(const Node *node, Fcr f, Mode m = L) const {
        if (node) {
            inorder(node->next[m != L], f, m);
            f(node->data);
            inorder(node->next[m != R], f, m);
        }
    }

    void postorder(const Node *node, Fcr f, Mode m = L) const {
        if (node) {
            postorder(node->next[m != L], f, m);
            postorder(node->next[m != R], f, m);
            f(node->data);
        }
    }

    void leafs(const Node *node, Fcr f, Mode m = L) const {
        if (node) {
            if (!node->next[L] && !node->next[R])
                f(node->data);
            leafs(node->next[m != L], f, m);
            leafs(node->next[m != R], f, m);
        }
    }

    void branch(const Node *node, const T &data, Fcr f) const {
        if (node) {
            if (f(node->data); node->data != data)
                branch(node->next[node->data < data], data, f);
        }
    }

    std::ostream &pyramid_print(const Node *node, std::ostream &os, std::size_t step) const {
        if (node) {
            pyramid_print(node->next[R], os, step + 1);
            os << std::setw(step * 6) << node->data << '\n';
            pyramid_print(node->next[L], os, step + 1);
        }
        return os;
    }
};

#define print(order)                                                                                                           \
    template <class T>                                                                                                         \
    std::ostream &print_##order(const Tree<T> &t, std::ostream &os, char delim, typename Tree<T>::Mode m = Tree<T>::Mode::L) { \
        t.order([&os, &delim](auto &&data) {                                                                                   \
            os << data << delim;                                                                                               \
        }, m);                                                                                                                 \
        return os;                                                                                                             \
    }

print(preorder)
print(inorder)
print(postorder)
#undef print

#define print_sub_tree(order)                                                                                                                 \
    template <class T>                                                                                                                        \
    std::ostream &print_##order(const Tree<T> &t, std::ostream &os, char delim, const T &data, typename Tree<T>::Mode m = Tree<T>::Mode::L) { \
        t.order(data, [&os, &delim](auto &&data) {                                                                                            \
            os << data << delim;                                                                                                              \
        }, m);                                                                                                                                \
        return os;                                                                                                                            \
    }

print_sub_tree(sub_tree_preorder)
print_sub_tree(sub_tree_inorder)
print_sub_tree(sub_tree_postorder)
#undef print_sub_tree

template <class T>
std::ostream &print_leafs(const Tree<T> &t, std::ostream &os, char delim, typename Tree<T>::Mode m = Tree<T>::Mode::L) {
    t.leafs([&os, &delim](auto &&data) {
        os << data << delim;
    }, m);
    return os;
}

template <class T>
std::ostream &print_branch(const Tree<T> &t, std::ostream &os, char delim, const T &data) {
    t.branch(data, [&os, &delim](auto &&data) {
        os << data << delim;
    });
    return os;
}
