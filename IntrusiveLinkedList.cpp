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
//    node->next = rightNode;
//    node->prev = rightNode->prev;
//    node->prev->next = node;
//    rightNode->prev = node;
//}
//
//template <typename T>
//void IntrusiveLinkedList<T>::insertAfter(ListNode<T>* node, ListNode<T>* leftNode)
//{
//    node->prev = leftNode;
//    node->next = leftNode->next;
//    node->next->prev = node;
//    leftNode->next = node;
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
//IntrusiveLinkedListNode<T>::IntrusiveLinkedListNode() : 
//    next(this), prev(this), 
//    /*head(this),*/ owner(nullptr)
//{
//}
//
//template <typename T>
//IntrusiveLinkedListNode<T>::~IntrusiveLinkedListNode()
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
//void IntrusiveLinkedListNode<T>::insertBefore(IntrusiveLinkedListNode<T>* node)
//{
//    remove();
//
//    prev = node->prev;
//    next = node;
//
//    node->prev->next = this;
//    node->prev = this;
//    //head = node->head;
//}
//
//template<typename T>
//void IntrusiveLinkedListNode<T>::insertAfter(IntrusiveLinkedListNode<T>* node)
//{
//    prev = node;
//    next = node->next;
//    node->next->prev = this;
//    node->next = this;
//    //head = node->head;
//}
//
//template<typename T>
//void IntrusiveLinkedListNode<T>::remove()
//{
//    prev->next = next;
//    next->prev = prev;
//
//    next = this;
//    prev = this;
//    //head = this;
//}
//
//template<typename T>
//void IntrusiveLinkedListNode<T>::setOwner(T* owner)
//{
//    this->owner = owner;
//}
