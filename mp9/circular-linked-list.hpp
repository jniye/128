#ifndef CIRCULAR_LINKED_LIST_HPP
#define CIRCULAR_LINKED_LIST_HPP

#include <iostream>
#include <cassert>

#include "node.hpp"

enum class Order { kASC, kDESC };

template <typename T>
class CircularLinkedList {
public:
  CircularLinkedList() = default;
  CircularLinkedList(const CircularLinkedList<T>& source);
  CircularLinkedList<T>& operator=(const CircularLinkedList<T>& source);
  ~CircularLinkedList();


  void InsertInOrder(const T& data);
  void Reverse();
  void Clear();

  template <typename U>
  friend std::ostream& operator<<(std::ostream& os,
                                  const CircularLinkedList<U>& cll);

private:
  Node<T>* head_ = nullptr;
  Node<T>* tail_ = nullptr;
  Order node_order_ = Order::kASC;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const CircularLinkedList<T>& cll) {
  Node<T>* iter = cll.head_;
  // empty list condition
  if (iter == nullptr) {
    os << "Empty list";
    return os;
  }
  // non-empty list condition
  do {
    os << iter->data << '\t';
    iter = iter->next;
  } while (iter != cll.head_);

  return os;
}

template <typename T>
CircularLinkedList<T>::CircularLinkedList(const CircularLinkedList<T>& source){
  *this = source;
}

template <typename T>
CircularLinkedList<T>& CircularLinkedList<T>::operator=(const CircularLinkedList<T>& source){
  if(this == &source) return *this;
  Node<T>* iter = source.head_;

  Clear();

  if(iter == nullptr){
    head_ = tail_ = nullptr;
    return *this;
  }
  node_order_ = source.node_order_;
  head_ = new Node<T>(source.head_->data);
  Node<T>* it = head_;
  while(iter->next != source.head_){
    it->next = new Node<T>(iter->next->data);
    iter = iter->next;
    it = it->next;
  }
  tail_ = it;
  it->next = head_;
  return *this;
}

template <typename T>
void CircularLinkedList<T>::Clear(){
  if(tail_==nullptr) return;
  tail_->next = nullptr;
  while(head_ != tail_){
    Node<T>* next = head_->next;
    delete head_;
    head_ = next;
  }
  delete tail_;
  head_ = nullptr;
  tail_ = nullptr;
}

template <typename T>
CircularLinkedList<T>::~CircularLinkedList(){
  Clear();
}

template <typename T>
void CircularLinkedList<T>::InsertInOrder(const T& data){
  if(head_ == nullptr) { 
    head_ = tail_ = new Node<T>(data);  
    tail_->next = head_; 
    return;}
  Node<T> * it = head_;
  if(node_order_==Order::kASC){
    while(it->next != head_){
      if(it->next->data > data) break;
      it = it->next;}
    if(it == head_ && it->data > data) {
      auto * add = new Node<T>(data);
      add->next = head_;
      head_ = add;
      tail_->next = add;
      return;}}
  else{
    while(it->next != head_){
      if(it->next->data < data) break;
      it = it->next;}
    if(it == head_ && it->data < data) {
      auto * add = new Node<T>(data);
      add->next = head_;
      head_ = add;
      tail_->next = add;
      return;}}
  if(it == tail_){
    auto * add = new Node<T>(data);
    add->next = head_;
    it->next = add;
    tail_ = add;
  }else{
    auto * add = new Node<T>(data);
    add->next = it->next;
    it->next = add;}
}

template <typename T>
void CircularLinkedList<T>::Reverse(){

  if(node_order_ ==  Order::kASC) {node_order_ =  Order::kDESC;}
  else{node_order_ = Order::kASC;}

  if(head_ == nullptr) 
  { return;
  }else{

  Node<T> * pre = head_;
  Node<T> * temp = head_;
  Node<T> * cur = head_->next;
  pre->next = pre;
  while (cur != head_){
    temp = cur->next;
    cur->next = pre;
    pre = cur;
    cur = temp;
  }
  head_->next = pre;
  tail_ = head_;
  head_ = pre;  }
}

#endif