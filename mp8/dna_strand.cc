#include "dna_strand.hpp"

#include <stdexcept>

void DNAstrand::SpliceIn(const char* pattern, DNAstrand& to_splice_in){
    if(&to_splice_in == this) return; 
    if(pattern == nullptr || to_splice_in.head_ == nullptr) return; //empty pattern or empty to_splice_in
    
    Node * head_pattern = new Node(*pattern);
    Node * slice = head_pattern;
    pattern ++;
    while(*pattern != '\0'){ //construct pattern DNAstrand
        slice->next = new Node(*pattern);
        slice = slice->next;
        pattern ++;
    }
    Node * cur = head_;
    Node * cur_pattern = head_pattern;
    Node * point = cur; //notation of the start node of the last match of pattern
    //find the matched pattern
    while(cur){
        if(cur->data != head_pattern->data) {
            cur = cur->next;
        } // if cur->next doesn't match the char in pattern, keep iterating
        else{
            cur_pattern = head_pattern; 
            Node * cur_sec = cur; //assign a new node to iterate
            while(cur_pattern && cur_sec){
                if(cur_sec->data != cur_pattern->data) break;
                else{
                    cur_pattern = cur_pattern->next;
                    cur_sec = cur_sec->next;
                }
            }
            if(cur_pattern == nullptr) {
                point = cur;
            }  //if cur_pattern is iterated to the end, update the point with the start point cur
        cur = cur->next;
        }
    }

    if(point == head_){
        cur_pattern = head_pattern;
        while(cur_pattern && point){
            if(cur_pattern->data == point->data){
                Node * point_next = point->next; 
                delete point; point = point_next;  
                cur_pattern = cur_pattern->next;
            }else break;
        }
        //To check whether pathern is completely deleted from DNAstrand
        if(cur_pattern != nullptr){throw std::runtime_error("Error pattern");}
        if(point == nullptr){tail_ = to_splice_in.tail_;}
        to_splice_in.tail_->next = point;
        head_ = to_splice_in.head_;
    }else{
        Node* joint = head_;
        while(joint->next != point){joint = joint->next;}
        //delete from 
        cur_pattern = head_pattern;
        while(cur_pattern && point){
            if(cur_pattern->data == point->data){
                Node * point_next = point->next; 
                delete point; point = point_next;  
                cur_pattern = cur_pattern->next;
            }else break;
        }
        //To check whether pathern is completely deleted from DNAstrand
        if(cur_pattern != nullptr){throw std::runtime_error("Error pattern");}
        //link two SLL
        if(point == nullptr){tail_ = to_splice_in.tail_;}  //update tail_ if point is null ()  
        to_splice_in.tail_->next = point;
        //delete joint->next;
        joint->next = to_splice_in.head_; 
        int i = 0;
        i++;
    }
    to_splice_in.head_ = to_splice_in.tail_ = nullptr;
}

// void DNAstrand::Push(char value){
//     if(head_ == nullptr){
//     head_ = tail_ = new Node(value);
//     }else{
//     Node *tmp = new Node(value);
//     tail_->next = tmp;
//     tail_ = tmp;
//   }
// }

DNAstrand::~DNAstrand(){
    while(head_){
    Node *next = head_->next;
    delete head_;
    head_ = next;
  }
}