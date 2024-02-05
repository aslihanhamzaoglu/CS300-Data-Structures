#include <iostream>
#include "AvlTree.h"
#pragma  once

using namespace std;

template <class Comparable>
AvlTree<Comparable>::AvlTree(const Comparable& notFound) : ITEM_NOT_FOUND(notFound), root(NULL) {}

template <class Comparable>
AvlTree<Comparable>::AvlTree(const AvlTree<Comparable>& rhs) : root(NULL), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND)
{
    *this = rhs;
}

template <class Comparable>
AvlTree<Comparable>::~AvlTree()
{
    makeEmpty();
}

template <class Comparable>
void AvlTree<Comparable>::makeEmpty()
{
    makeEmpty(root);
}

template <class Comparable>
void AvlTree<Comparable>::makeEmpty(AvlNode<Comparable>*& t) const
{
    if (t != NULL)
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = NULL;
}

template <class Comparable>
void AvlTree<Comparable>::insert(const Comparable& x)
{
    insert(x, root);
}

template <class Comparable>
void AvlTree<Comparable>::insert(const Comparable& x, AvlNode<Comparable>*& t) const
{
    if (t == NULL)
        t = new AvlNode<Comparable>(x, NULL, NULL);

    else if (x < t->element) {

        insert(x, t->left);

        if (height(t->left) - height(t->right) == 2)
            if (x < t->left->element)
                rotateWithLeftChild(t);
            else
                doubleWithLeftChild(t);
    }
    else if (t->element < x)
    {    // Otherwise X is inserted to the right subtree
        insert(x, t->right);
        if (height(t->right) - height(t->left) == 2)
            // height of the right subtree increased
            if (t->right->element < x)
                // X was inserted to right-right subtree
                rotateWithRightChild(t);
            else // X was inserted to right-left subtree
                doubleWithRightChild(t);
    }
    else
        ;  // Duplicate; do nothing

    // update the height the node
    t->height = max(height(t->left), height(t->right)) + 1;
}

template <class Comparable>
int AvlTree<Comparable>::max(int lhs, int rhs) const
{
    if (lhs > rhs)
        return lhs;

    return rhs;
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithLeftChild(AvlNode<Comparable>*& k2) const
{
    AvlNode<Comparable>* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    k2 = k1;
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithLeftChild(AvlNode<Comparable>*& k3) const
{
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithRightChild(AvlNode<Comparable>*& k1) const
{
    AvlNode<Comparable>* k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(height(k2->right), k1->height) + 1;
    k1 = k2;
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithRightChild(AvlNode<Comparable>*& k1) const
{
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);
}

template <class Comparable>
const Comparable& AvlTree<Comparable>::elementAt(AvlNode<Comparable>* t) const
{
    return t == NULL ? ITEM_NOT_FOUND : t->element;
}

template <class Comparable>
AvlNode<Comparable>* AvlTree<Comparable>::find(const Comparable& x, AvlNode<Comparable>* t) const
{
    if (t == NULL)
        return NULL;
    else if (x < t->element)
        return find(x, t->left);
    else if (t->element < x)
        return find(x, t->right);
    else
        return t;    // Match
}


template <class Comparable>
const Comparable& AvlTree<Comparable>::find(const Comparable& x) const
{
    return elementAt(find(x, root));
}

template <class Comparable>
int AvlTree<Comparable>::height(AvlNode<Comparable>* t)const
{
    if (t == NULL)
        return -1;

    return t->height;
}

template <class Comparable>
void AvlTree<Comparable>::remove(const Comparable& x)
{
    remove(x, root);

}

template <class Comparable>
void AvlTree<Comparable>::remove(const Comparable& x, AvlNode<Comparable>*& t) const
{
    if (t == NULL)
        return;  // Item not found, do nothing

    if (x < t->element) {
        // The item to be removed is in the left subtree
        remove(x, t->left);
    }
    else if (t->element < x) {
        // The item to be removed is in the right subtree
        remove(x, t->right);
    }
    else if (t->left != NULL && t->right != NULL) {
        // Node with two children
        t->element = findMin(t->right)->element;
        remove(t->element, t->right);
    }
    else {
        // Node with one or zero child
        AvlNode<Comparable>* oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        delete oldNode;
    }

    // Update height and balance factor
    if (t != NULL) {
        t->height = max(height(t->left), height(t->right)) + 1;

        // Check balance factor and perform rotations if necessary
        if (height(t->left) - height(t->right) == 2) {
            if (height(t->left->left) >= height(t->left->right))
                rotateWithLeftChild(t);
            else
                doubleWithLeftChild(t);
        }
        else if (height(t->right) - height(t->left) == 2) {
            if (height(t->right->right) >= height(t->right->left))
                rotateWithRightChild(t);
            else
                doubleWithRightChild(t);
        }
    }
}

template <class Comparable>
const Comparable& AvlTree<Comparable>::findMin() const
{
    return elementAt(findMin(root));
}

template <class Comparable>
AvlNode<Comparable>* AvlTree<Comparable>::findMin(AvlNode<Comparable>* t) const
{
    if (t == NULL)
        return NULL;
    if (t->left == NULL)
        return t;
    return findMin(t->left);
}

template <class Comparable>
Comparable* AvlTree<Comparable>::elementUpdate(const Comparable& x) const
{
    AvlNode<Comparable>* foundNode = find(x, root);
    return &(foundNode->element);

}