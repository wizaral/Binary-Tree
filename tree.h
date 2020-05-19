#pragma once
#include <functional>
#include <iomanip>
#include <string>

template <class T>
class Tree final {
    struct Node final {
        T data;
        Node *prev = nullptr;
        Node *next[2]{nullptr, nullptr};
    };

    using Fcr = std::function<void(const T &data)>;
    using Fr = std::function<void(T &data)>;

    Node *root_ = nullptr;
    std::size_t size_ = 0;

public:
    enum Mode { L = 0, R = 1, };

    Tree() = default;

    Tree(const Tree &tree) {
        merge(tree);
    }

    ~Tree() {
        clear();
    }

    Tree &operator=(const Tree &tree) {
        if (this != &tree) {
            clear();
            merge(tree);
        }
        return *this;
    }

    bool empty() const;
    std::size_t size() const;
    std::size_t height() const;

    void clear();
    bool insert(const T &data);
    bool erase(const T &data);
    void merge(const Tree &tree);
    bool find(const T &data) const;

    void preorder(Fcr f, Mode m = L) const;
    void inorder(Fcr f, Mode m = L) const;
    void postorder(Fcr f, Mode m = L) const;

    void leafs(Fcr f, Mode m = L) const;
    void branch(const T &data, Fcr f) const;

    void sub_tree_preorder(const T &data, Fcr f, Mode m = L) const;
    void sub_tree_inorder(const T &data, Fcr f, Mode m = L) const;
    void sub_tree_postorder(const T &data, Fcr f, Mode m = L) const;

private:
    bool compare(const T &lhs, const T &rhs) const;

    std::size_t height(const Node *node) const;
    void clear(Node *node);
    void erase(Node *node);

    Node *find(Node *node, const T &data) const;
    Node *min(Node *node);
    Node *max(Node *node);

    void preorder(const Node *node, Fcr f, Mode m = L) const;
    void inorder(const Node *node, Fcr f, Mode m = L) const;
    void postorder(const Node *node, Fcr f, Mode m = L) const;

    void leafs(const Node *node, Fcr f, Mode m = L) const;
    void branch(const Node *node, const T &data, Fcr f) const;
};

template <class T>
bool Tree<T>::empty() const {
    return size_ == 0;
}

template <class T>
std::size_t Tree<T>::size() const {
    return size_;
}

template <class T>
std::size_t Tree<T>::height() const {
    return height(root_);
}

template <class T>
void Tree<T>::clear() {
    clear(root_);
    root_ = nullptr;
}

template <class T>
bool Tree<T>::insert(const T &data) {
    if (root_ == nullptr) {
        root_ = new Node{data};
    } else {
        for (Node *fast = root_, *slow = fast; true; slow = fast) {
            if (compare(data, fast->data)) {
                return false;
            } else if (bool cmp = fast->data < data; !(fast = fast->next[cmp])) {
                slow->next[cmp] = new Node{data, slow};
                break;
            }
        }
    }
    return (++size_, true);
}

template <class T>
bool Tree<T>::erase(const T &data) {
    if (Node *node = find(root_, data); node) {
        erase(node);
        return true;
    }
    return false;
}

template <class T>
void Tree<T>::merge(const Tree &tree) {
    tree.preorder([this](const T &data) {
        insert(data);
    });
}

template <class T>
bool Tree<T>::find(const T &data) const {
    return find(root_, data) != nullptr;
}

template <class T>
void Tree<T>::preorder(Fcr f, Mode m) const {
    preorder(root_, f, m);
}

template <class T>
void Tree<T>::inorder(Fcr f, Mode m) const {
    inorder(root_, f, m);
}

template <class T>
void Tree<T>::postorder(Fcr f, Mode m) const {
    postorder(root_, f, m);
}

template <class T>
void Tree<T>::leafs(Fcr f, Mode m) const {
    leafs(root_, f, m);
}

template <class T>
void Tree<T>::branch(const T &data, Fcr f) const {
    if (find(root_, data) != nullptr)
        branch(root_, data, f);
}

template <class T>
void Tree<T>::sub_tree_preorder(const T &data, Fcr f, Mode m) const {
    if (const Node *pdata = find(root_, data); pdata != nullptr)
        preorder(pdata, f, m);
}

template <class T>
void Tree<T>::sub_tree_inorder(const T &data, Fcr f, Mode m) const {
    if (const Node *pdata = find(root_, data); pdata != nullptr)
        inorder(pdata, f, m);
}

template <class T>
void Tree<T>::sub_tree_postorder(const T &data, Fcr f, Mode m) const {
    if (const Node *pdata = find(root_, data); pdata != nullptr)
        postorder(pdata, f, m);
}

#define CHECK_COMPARE(name, op)                                                                   \
    template <class T, class = void>                                                              \
    struct name : std::false_type {};                                                             \
    template <class T>                                                                            \
    struct name<T, std::void_t<decltype(std::declval<const T &>() op std::declval<const T &>())>> \
        : std::true_type {};                                                                      \
    template <class T>                                                                            \
    inline constexpr bool name##_v = name<T>::value

namespace hidden_____ {
CHECK_COMPARE(has_eq_op, ==);
CHECK_COMPARE(has_ne_op, !=);
CHECK_COMPARE(has_lt_op, <);
CHECK_COMPARE(has_gt_op, >);
CHECK_COMPARE(has_le_op, <=);
CHECK_COMPARE(has_ge_op, >=);
} // namespace hidden_____
#undef CHECK_COMPARE

template <class T>
bool Tree<T>::compare(const T &lhs, const T &rhs) const {
    using namespace hidden_____;
    if constexpr (has_eq_op_v<T>) {
        return lhs == rhs;
    } else if constexpr (has_ne_op_v<T>) {
        return !(lhs != rhs);
    } else if constexpr (has_lt_op_v<T>) {
        return !(lhs < rhs) && !(rhs < lhs);
    } else if constexpr (has_gt_op_v<T>) {
        return !(lhs > rhs) && !(rhs > lhs);
    } else if constexpr (has_le_op_v<T>) {
        return lhs <= rhs && rhs <= lhs;
    } else if constexpr (has_ge_op_v<T>) {
        return lhs >= rhs && rhs >= lhs;
    } else {
        constexpr auto has_any_comparison_operator = has_eq_op_v<T> || has_ne_op_v<T> || has_lt_op_v<T> || has_gt_op_v<T> || has_le_op_v<T> || has_ge_op_v<T>;
        static_assert(has_any_comparison_operator, "No comparison operator for this operands!");
        return false;
    }
}

template <class T>
std::size_t Tree<T>::height(const Node *node) const {
    return node ? std::max(height(node->next[L]), height(node->next[R])) + 1 : 0;
}

template <class T>
void Tree<T>::clear(Node *node) {
    if (node) {
        clear(node->next[L]);
        clear(node->next[R]);
        delete node;
        --size_;
    }
}

template <class T>
typename Tree<T>::Node *Tree<T>::find(Node *node, const T &data) const {
    return !node || compare(data, node->data) ? node : find(node->next[node->data < data], data);
}

template <class T>
typename Tree<T>::Node *Tree<T>::min(Node *node) {
    while (node->next[L])
        node = node->next[L];
    return node;
}

template <class T>
typename Tree<T>::Node *Tree<T>::max(Node *node) {
    while (node->next[R])
        node = node->next[R];
    return node;
}

template <class T>
void Tree<T>::erase(Node *node) {
    if (node->next[L] && node->next[R]) {
        Node *max_ = max(node->next[L]);
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

template <class T>
void Tree<T>::preorder(const Node *node, Fcr f, Mode m) const {
    if (node) {
        f(node->data);
        preorder(node->next[m != L], f, m);
        preorder(node->next[m != R], f, m);
    }
}

template <class T>
void Tree<T>::inorder(const Node *node, Fcr f, Mode m) const {
    if (node) {
        inorder(node->next[m != L], f, m);
        f(node->data);
        inorder(node->next[m != R], f, m);
    }
}

template <class T>
void Tree<T>::postorder(const Node *node, Fcr f, Mode m) const {
    if (node) {
        postorder(node->next[m != L], f, m);
        postorder(node->next[m != R], f, m);
        f(node->data);
    }
}

template <class T>
void Tree<T>::leafs(const Node *node, Fcr f, Mode m) const {
    if (node) {
        if (!node->next[L] && !node->next[R])
            f(node->data);
        leafs(node->next[m != L], f, m);
        leafs(node->next[m != R], f, m);
    }
}

template <class T>
void Tree<T>::branch(const Node *node, const T &data, Fcr f) const {
    if (node) {
        if (f(node->data); node->data != data)
            branch(node->next[node->data < data], data, f);
    }
}

template <class T>
std::ostream &print_preorder(const Tree<T> &lhs, std::ostream &os, const std::string &delim, typename Tree<T>::Mode m = Tree<T>::Mode::L) {
    lhs.preorder([&os, &delim](const auto &data) {
        os << data << delim;
    }, m);
    return os;
}

template <class T>
std::ostream &print_inorder(const Tree<T> &lhs, std::ostream &os, const std::string &delim, typename Tree<T>::Mode m = Tree<T>::Mode::L) {
    lhs.inorder([&os, &delim](const auto &data) {
        os << data << delim;
    }, m);
    return os;
}

template <class T>
std::ostream &print_postorder(const Tree<T> &lhs, std::ostream &os, const std::string &delim, typename Tree<T>::Mode m = Tree<T>::Mode::L) {
    lhs.postorder([&os, &delim](const auto &data) {
        os << data << delim;
    }, m);
    return os;
}

template <class T>
std::ostream &print_sub_tree_preorder(const Tree<T> &lhs, std::ostream &os, const std::string &delim, const T &data, typename Tree<T>::Mode m = Tree<T>::Mode::L) {
    lhs.sub_tree_preorder(data, [&os, &delim](const auto &data) {
        os << data << delim;
    }, m);
    return os;
}

template <class T>
std::ostream &print_sub_tree_inorder(const Tree<T> &lhs, std::ostream &os, const std::string &delim, const T &data, typename Tree<T>::Mode m = Tree<T>::Mode::L) {
    lhs.sub_tree_inorder(data, [&os, &delim](const auto &data) {
        os << data << delim;
    }, m);
    return os;
}

template <class T>
std::ostream &print_sub_tree_postorder(const Tree<T> &lhs, std::ostream &os, const std::string &delim, const T &data, typename Tree<T>::Mode m = Tree<T>::Mode::L) {
    lhs.sub_tree_postorder(data, [&os, &delim](const auto &data) {
        os << data << delim;
    }, m);
    return os;
}

template <class T>
std::ostream &print_leafs(const Tree<T> &lhs, std::ostream &os, const std::string &delim, typename Tree<T>::Mode m = Tree<T>::Mode::L) {
    lhs.leafs([&os, &delim](const auto &data) {
        os << data << delim;
    }, m);
    return os;
}

template <class T>
std::ostream &print_branch(const Tree<T> &lhs, std::ostream &os, const std::string &delim, const T &data) {
    lhs.branch(data, [&os, &delim](const auto &data) {
        os << data << delim;
    });
    return os;
}
