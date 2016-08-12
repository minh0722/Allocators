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
    
    bool isEmpty();

    T* next();

private:
    IntrusiveLinkedListNode* nextNode;
    IntrusiveLinkedListNode* prevNode;
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
    nextNode(this), prevNode(this),
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

    prevNode = node->prevNode;
    nextNode = node;

    node->prevNode->nextNode = this;
    node->prevNode = this;
    head = node->head;
}

template<typename T>
void IntrusiveLinkedListNode<T>::insertAfter(IntrusiveLinkedListNode<T>* node)
{
    remove();

    prevNode = node;
    nextNode = node->nextNode;
    node->nextNode->prevNode = this;
    node->nextNode = this;
    head = node->head;
}

template<typename T>
void IntrusiveLinkedListNode<T>::remove()
{
    prevNode->nextNode = nextNode;
    nextNode->prevNode = prevNode;

    nextNode = this;
    prevNode = this;
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

template<typename T>
inline bool IntrusiveLinkedListNode<T>::isEmpty()
{
    return head->nextNode != head;
}

template<typename T>
inline T* IntrusiveLinkedListNode<T>::next()
{
    if (isEmpty())
    {
        return nullptr;
    }

    return nextNode->getOwner();
}
