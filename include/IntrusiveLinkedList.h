#pragma once

template <typename T>
class CircularIntrusiveLinkedListNode
{
public:
    CircularIntrusiveLinkedListNode();
    ~CircularIntrusiveLinkedListNode();

    void addToFront(CircularIntrusiveLinkedListNode* node);
    void addToEnd(CircularIntrusiveLinkedListNode* node);

    void insertBefore(CircularIntrusiveLinkedListNode* node);
    void insertAfter(CircularIntrusiveLinkedListNode* node);

    CircularIntrusiveLinkedListNode* getNextNode();
    CircularIntrusiveLinkedListNode* getPrevNode();
    
    void remove();

    void setOwner(T* owner);
    T* getOwner();
    
    bool isEmpty();

    T* nextOwner();

private:
    CircularIntrusiveLinkedListNode* nextNode;
    CircularIntrusiveLinkedListNode* prevNode;
    CircularIntrusiveLinkedListNode* head;
    T* owner;
};

//template <typename T>
//struct ListNode
//{
//    ListNode() : m_next(this), m_prev(this), owner(nullptr) {}
//
//    ListNode* m_next;
//    ListNode* m_prev;
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
CircularIntrusiveLinkedListNode<T>::CircularIntrusiveLinkedListNode() :
    nextNode(this), prevNode(this),
    head(this), owner(nullptr)
{
}

template <typename T>
CircularIntrusiveLinkedListNode<T>::~CircularIntrusiveLinkedListNode()
{
}

template<typename T>
void CircularIntrusiveLinkedListNode<T>::addToFront(CircularIntrusiveLinkedListNode* node)
{
    insertAfter(node->head);
}

template<typename T>
void CircularIntrusiveLinkedListNode<T>::addToEnd(CircularIntrusiveLinkedListNode* node)
{
    insertBefore(node->head);
}

template<typename T>
void CircularIntrusiveLinkedListNode<T>::insertBefore(CircularIntrusiveLinkedListNode<T>* node)
{
    remove();

    prevNode = node->prevNode;
    nextNode = node;

    node->prevNode->nextNode = this;
    node->prevNode = this;
    head = node->head;
}

template<typename T>
void CircularIntrusiveLinkedListNode<T>::insertAfter(CircularIntrusiveLinkedListNode<T>* node)
{
    remove();

    prevNode = node;
    nextNode = node->nextNode;
    node->nextNode->prevNode = this;
    node->nextNode = this;
    head = node->head;
}

template<typename T>
inline CircularIntrusiveLinkedListNode<T>* CircularIntrusiveLinkedListNode<T>::getNextNode()
{
    if (nextNode == head)
    {
        return nullptr;
    }
    
    return nextNode;
}

template<typename T>
inline CircularIntrusiveLinkedListNode<T>* CircularIntrusiveLinkedListNode<T>::getPrevNode()
{
    if (prevNode == head)
    {
        return nullptr;
    }

    return prevNode;
}

template<typename T>
void CircularIntrusiveLinkedListNode<T>::remove()
{
    prevNode->nextNode = nextNode;
    nextNode->prevNode = prevNode;

    nextNode = this;
    prevNode = this;
    head = this;
}

template<typename T>
void CircularIntrusiveLinkedListNode<T>::setOwner(T* owner)
{
    this->owner = owner;
}

template<typename T>
inline T * CircularIntrusiveLinkedListNode<T>::getOwner()
{
    return owner;
}

template<typename T>
inline bool CircularIntrusiveLinkedListNode<T>::isEmpty()
{
    return head->nextNode == head;
}

template<typename T>
inline T* CircularIntrusiveLinkedListNode<T>::nextOwner()
{
    if (isEmpty())
    {
        return nullptr;
    }

    return nextNode->getOwner();
}
