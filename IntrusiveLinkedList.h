#pragma once

template <typename T>
struct ListNode
{
    ListNode() : next(this), prev(this) {}

    ListNode* next;
    ListNode* prev;
    T* owner;
};

template <typename T>
class IntrusiveLinkedList
{
public:
    IntrusiveLinkedList();
    ~IntrusiveLinkedList();
    
    void insertBefore(ListNode<T>* node, ListNode<T>* rightNode);
    void insertAfter(ListNode<T>* node, ListNode<T>* leftNode);
    void addToFront(ListNode<T>* node);
    void addToEnd(ListNode<T>* node);
    
    T* search(ListNode<T>* node);

private:
    ListNode head;  // pivot element
};