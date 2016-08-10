#include "IntrusiveLinkedList.h"


template <typename T>
IntrusiveLinkedList<T>::IntrusiveLinkedList()
{
}

template <typename T>
IntrusiveLinkedList<T>::~IntrusiveLinkedList()
{
}
template <typename T>
void IntrusiveLinkedList<T>::insertBefore(ListNode<T>* node, ListNode<T>* rightNode)
{
    node->next = rightNode;
    node->prev = rightNode->prev;
    node->prev->next = node;
    rightNode->prev = node;
}

template <typename T>
void IntrusiveLinkedList<T>::insertAfter(ListNode<T>* node, ListNode<T>* leftNode)
{
    node->prev = leftNode;
    node->next = leftNode->next;
    node->next->prev = node;
    leftNode->next = node;
}

template <typename T>
void IntrusiveLinkedList<T>::addToFront(ListNode<T>* node)
{
    insertAfter(node, &head);
}

template <typename T>
void IntrusiveLinkedList<T>::addToEnd(ListNode<T>* node)
{
    insertBefore(node, &head);
}
