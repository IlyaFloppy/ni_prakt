//
//  task2.hpp
//  ni_prakt
//
//  Created by Ilya Popovnin on 2019-09-20.
//  Copyright © 2019 Ilya Popovnin. All rights reserved.
//

#ifndef task2_hpp
#define task2_hpp

#include <iostream>
#include <vector>
#include <stdexcept>


template <class T> class LinkedListNode {
private:
    T* data = nullptr;
public:
    LinkedListNode* previous = nullptr;
    LinkedListNode* next = nullptr;
public:
    LinkedListNode(T* data) {
        this->data = data;
        this->next = next;
    }
    
    T* getData() {
        return data;
    }
    
    void setData(T* data) {
        this->data = data;
    }
};

template <class T> class LinkedList {
private:
    
    LinkedListNode<T>* first = nullptr;
    LinkedListNode<T>* last = nullptr;
    
    uint64_t size = 0;
    
    LinkedListNode<T>* findNodeAtIndex(uint64_t index) const {
        if(index < 0 || index >= this->size) {
            throw std::invalid_argument("index out of bounds");
        }
        LinkedListNode<T>* node = this->first;
        bool lookFromLast = index > this->size / 2;
        if(lookFromLast) {
            node = this->last;
            index = this->size - index - 1;
        }
        
        while(index > 0) {
            node = lookFromLast?node->previous:node->next;
            index -= 1;
        }
        
        return node;
    }
    
public:
    
    LinkedList() {
        
    }
    
    void addFirst(T* object) {
        LinkedListNode<T>* newNode = new LinkedListNode<T>(object);
        if(this->size == 0) {
            this->first = newNode;
            this->last = newNode;
            this->size = 1;
        } else {
            newNode->next = this->first;
            this->first->previous = newNode;
            
            this->first = newNode;
            this->size += 1;
        }
    }
    
    void addLast(T* object) {
        LinkedListNode<T>* newNode = new LinkedListNode<T>(object);
        if(this->size == 0) {
            this->first = newNode;
            this->last = newNode;
            this->size = 1;
        } else {
            this->last->next = newNode;
            newNode->previous = this->last;
            this->last = newNode;
            this->size += 1;
        }
    }
    
    void addAtIndex(T* object, uint64_t index) {
        if(index == 0) {
            addFirst(object);
            return;
        } else if(index == size) {
            addLast(object);
            return;
        }
        
        LinkedListNode<T>* previousNode = findNodeAtIndex(index - 1);
        LinkedListNode<T>* newNode = new LinkedListNode<T>(object);
        LinkedListNode<T>* nextNode = previousNode->next;
        
        previousNode->next = newNode;
        newNode->next = nextNode;
        newNode->previous = previousNode;
        nextNode->previous = newNode;
        
        this->size += 1;
    }
    
    T* remove(T* object) const {
        LinkedListNode<T>* node = this->first;
        for(int i = 0; i < this->size; i++) {
            if(node->getData() == object) {
                if(node->previous) {
                    node->previous->next = node->next;
                }
                if(node->next) {
                    node->next->previous = node->previous;
                }
                delete node;
                this->size -= 1;
                return object;
            } else {
                node = node->next;
            }
        }
        return nullptr;
    }
    
    T* peekFirst() {
        return this->first->getData();
    }
    
    T* peekLast() {
        return this->last->getData();
    }
    
    T* popFirst() {
        LinkedListNode<T>* node = this->first;
        this->first = this->first->next;
        T* object = node->getData();
        delete node;
        this->size -= 1;
        return object;
    }
    
    T* popLast() {
        LinkedListNode<T>* node = this->last;
        this->last = this->last->previous;
        T* object = node->getData();
        delete node;
        this->size -= 1;
        return object;
    }
    
    T* popAtIndex(uint64_t index) {
        LinkedListNode<T>* node = findNodeAtIndex(index);
        if(node->previous) {
            node->previous->next = node->next;
        }
        if(node->next) {
            node->next->previous = node->previous;
        }
        T* object = node->getData();
        delete node;
        this->size -= 1;
        return object;
    }
    
    uint64_t getSize() const {
        return this->size;
    }
    
    LinkedListNode<T>* merge(LinkedListNode<T>* head1, LinkedListNode<T>* head2) const {
        if (!head1)
            return head2;
        
        if (!head2)
            return head1;
        
        if (*head1->getData() < *head2->getData()) {
            head1->next = merge(head1->next, head2);
            head1->next->previous = head1;
            head1->previous = nullptr;
            return head1;
        }
        else {
            head2->next = merge(head1, head2->next);
            head2->next->previous = head2;
            head2->previous = nullptr;
            return head2;
        }
    }
    
    LinkedListNode<T>* mergeSort(LinkedListNode<T>* head, uint64_t size) const {
        // sublist has length of either 0 or 1 => sorted
        if(size <= 1)
            return head;
        
        // split
        LinkedListNode<T>* partitionNode = head;
        for(uint64_t i = 0; i < size / 2; i++) {
            partitionNode = partitionNode->next;
        }
        partitionNode->previous->next = nullptr;
        
        // sort parts
        head = mergeSort(head, size / 2);
        partitionNode = mergeSort(partitionNode, size - size / 2);
        
        // merge parts
        return merge(head, partitionNode);
    }
    
    void mergeSort() {
        if(this->size <= 1)
            return;
        
        this->first = mergeSort(this->first, this->size);
        this->last = this->first;
        while(this->last->next)
            this->last = this->last->next;
    }
    
    void swap(uint64_t index1, uint64_t index2) {
        LinkedListNode<T>* node1 = findNodeAtIndex(index1);
        LinkedListNode<T>* node2 = findNodeAtIndex(index2);
        
        T* temp = node1->getData();
        node1->setData(node2->getData());
        node2->setData(temp);
        
    }
    
    const T* operator[](uint64_t index) const {
        return findNodeAtIndex(index)->getData();
    }
    
    void print() {
        auto ref = this->first;
        while(ref) {
            std::cout << *ref->getData() << std::endl;
            ref = ref->next;
        }
    }
};



void task2();
#endif /* task2_hpp */
