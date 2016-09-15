#include "IntrusiveLinkedList.h"


//template <typename T>
//IntrusiveLinkedList<T>::IntrusiveLinkedList()
//{
//}
//
//template <typename T>
//IntrusiveLinkedList<T>::~IntrusiveLinkedList()
//{
//}
//template <typename T>
//void IntrusiveLinkedList<T>::insertBefore(ListNode<T>* node, ListNode<T>* rightNode)
//{
//    node->m_next = rightNode;
//    node->m_prev = rightNode->m_prev;
//    node->m_prev->m_next = node;
//    rightNode->m_prev = node;
//}
//
//template <typename T>
//void IntrusiveLinkedList<T>::insertAfter(ListNode<T>* node, ListNode<T>* leftNode)
//{
//    node->m_prev = leftNode;
//    node->m_next = leftNode->m_next;
//    node->m_next->m_prev = node;
//    leftNode->m_next = node;
//}
//
//template <typename T>
//void IntrusiveLinkedList<T>::addToFront(ListNode<T>* node)
//{
//    insertAfter(node, &head);
//}
//
//template <typename T>
//void IntrusiveLinkedList<T>::addToEnd(ListNode<T>* node)
//{
//    insertBefore(node, &head);
//}

//template <typename T>
//CircularIntrusiveLinkedListNode<T>::CircularIntrusiveLinkedListNode() : 
//    m_next(this), m_prev(this), 
//    /*head(this),*/ owner(nullptr)
//{
//}
//
//template <typename T>
//CircularIntrusiveLinkedListNode<T>::~CircularIntrusiveLinkedListNode()
//{
//}
//
////template<typename T>
////void IntrusiveLinkedList<T>::addToFront(IntrusiveLinkedList* node)
////{
////    insertAfter(node->head);
////}
////
////template<typename T>
////void IntrusiveLinkedList<T>::addToEnd(IntrusiveLinkedList* node)
////{
////    insertBefore(node->head);
////}
//
//template<typename T>
//void CircularIntrusiveLinkedListNode<T>::insertBefore(CircularIntrusiveLinkedListNode<T>* node)
//{
//    remove();
//
//    m_prev = node->m_prev;
//    m_next = node;
//
//    node->m_prev->m_next = this;
//    node->m_prev = this;
//    //head = node->head;
//}
//
//template<typename T>
//void CircularIntrusiveLinkedListNode<T>::insertAfter(CircularIntrusiveLinkedListNode<T>* node)
//{
//    m_prev = node;
//    m_next = node->m_next;
//    node->m_next->m_prev = this;
//    node->m_next = this;
//    //head = node->head;
//}
//
//template<typename T>
//void CircularIntrusiveLinkedListNode<T>::remove()
//{
//    m_prev->m_next = m_next;
//    m_next->m_prev = m_prev;
//
//    m_next = this;
//    m_prev = this;
//    //head = this;
//}
//
//template<typename T>
//void CircularIntrusiveLinkedListNode<T>::setOwner(T* owner)
//{
//    this->owner = owner;
//}
