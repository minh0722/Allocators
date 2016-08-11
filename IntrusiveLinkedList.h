#pragma once

template <typename T>
class IntrusiveLinkedListNode
{
public:
    IntrusiveLinkedListNode();
    ~IntrusiveLinkedListNode();

    void addToFront(IntrusiveLinkedListNode* node);
    void addToEnd(IntrusiveLinkedListNode* node);

    void insertBefore(IntrusiveLinkedListNode* node);
    void insertAfter(IntrusiveLinkedListNode* node);
    
    void remove();

    void setOwner(T* owner);
    T* getOwner();
    
private:
    IntrusiveLinkedListNode* next;
    IntrusiveLinkedListNode* prev;
    IntrusiveLinkedListNode* head;
    T* owner;
};

//template <typename T>
//struct ListNode
//{
//    ListNode() : next(this), prev(this), owner(nullptr) {}
//
//    ListNode* next;
//    ListNode* prev;
//    T* owner;
//};
//
//template <typename T>
//class IntrusiveLinkedList
//{
//public:
//    IntrusiveLinkedList();
//    ~IntrusiveLinkedList();
//    
//    void insertBefore(ListNode<T>* node, ListNode<T>* rightNode);
//    void insertAfter(ListNode<T>* node, ListNode<T>* leftNode);
//    void addToFront(ListNode<T>* node);
//    void addToEnd(ListNode<T>* node);
//
//private:
//    ListNode head;  // pivot element
//};


template <typename T>
IntrusiveLinkedListNode<T>::IntrusiveLinkedListNode() :
    next(this), prev(this),
    head(this), owner(nullptr)
{
}

template <typename T>
IntrusiveLinkedListNode<T>::~IntrusiveLinkedListNode()
{
}

template<typename T>
void IntrusiveLinkedListNode<T>::addToFront(IntrusiveLinkedListNode* node)
{
    insertAfter(node->head);
}

template<typename T>
void IntrusiveLinkedListNode<T>::addToEnd(IntrusiveLinkedListNode* node)
{
    insertBefore(node->head);
}

template<typename T>
void IntrusiveLinkedListNode<T>::insertBefore(IntrusiveLinkedListNode<T>* node)
{
    remove();

    prev = node->prev;
    next = node;

    node->prev->next = this;
    node->prev = this;
    head = node->head;
}

template<typename T>
void IntrusiveLinkedListNode<T>::insertAfter(IntrusiveLinkedListNode<T>* node)
{
    prev = node;
    next = node->next;
    node->next->prev = this;
    node->next = this;
    head = node->head;
}

template<typename T>
void IntrusiveLinkedListNode<T>::remove()
{
    prev->next = next;
    next->prev = prev;

    next = this;
    prev = this;
    head = this;
}

template<typename T>
void IntrusiveLinkedListNode<T>::setOwner(T* owner)
{
    this->owner = owner;
}

template<typename T>
inline T * IntrusiveLinkedListNode<T>::getOwner()
{
    return owner;
}
