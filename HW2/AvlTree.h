#ifndef _AVLTREE_H
#define _AVLTREE_H

//Code Taken From Cs300 Trees pp (elemetUpdate function added and remove function updated)

template <class Comparable>
class AvlTree;

template <class Comparable>
class AvlNode
{
    Comparable 	element;
    AvlNode* left;
    AvlNode* right;
    int height;

    AvlNode(const Comparable& theElement,
        AvlNode* lt, AvlNode* rt, int h = 0)
        : element(theElement), left(lt), right(rt), height(h) { }

    friend class AvlTree<Comparable>;
};

template <class Comparable>
class AvlTree {

public:

    explicit AvlTree(const Comparable& notFound);
    AvlTree(const AvlTree& rhs);
    ~AvlTree();

    void makeEmpty();

    void insert(const Comparable& x);
    const Comparable& find(const Comparable& x) const;
    const Comparable& findMin() const;
    void remove(const Comparable& x);
    Comparable* elementUpdate(const Comparable& x) const;

private:

    AvlNode<Comparable>* root;
    const Comparable ITEM_NOT_FOUND;

    void makeEmpty(AvlNode<Comparable>*& t) const;

    const Comparable& elementAt(AvlNode<Comparable>* t) const;

    void insert(const Comparable& x, AvlNode<Comparable>*& t) const;
    int max(int lhs, int rhs) const;
    void rotateWithLeftChild(AvlNode<Comparable>*& k2) const;
    void doubleWithLeftChild(AvlNode<Comparable>*& k3) const;
    void rotateWithRightChild(AvlNode<Comparable>*& k1) const;
    void doubleWithRightChild(AvlNode<Comparable>*& k1) const;

    AvlNode<Comparable>* find(const Comparable& x, AvlNode<Comparable>* t) const;
    int height(AvlNode<Comparable>* t) const;
    AvlNode<Comparable>* findMin(AvlNode<Comparable>* t) const;
    void remove(const Comparable& x, AvlNode<Comparable>*& t) const;

};

#endif //_AVLTREE_H
