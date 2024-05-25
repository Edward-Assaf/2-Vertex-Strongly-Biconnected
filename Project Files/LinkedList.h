/*
File 2: LinkedList.h
Written By:
1- Edward Assaf (Lines 1-170) (all of it)
*/

#pragma once
#include <iostream>
#include <cassert>
typedef long long int ll;

//represents a node in a linked list
//warning: always use 'nullptr' as the null value for ListNode pointers
template<typename T>
class ListNode { 
public:
	T val; //node's value
	ListNode<T>* next; //pointer to next node in linked list
	//constructor
	ListNode(const T& v = 0, ListNode<T>* n = nullptr) : val(v), next(n) {}
};

//provides a method of iterating over a linked list
template <typename T>
class ListIterator {
private:
	ListNode<T>* current; //pointer to node of current iteration
public:
	//constructor
	ListIterator(ListNode<T>* head = nullptr) : current(head) {}
	//returns the value in the current node
	//null pointers will lead to exceptions
	T value() { return current->val; }
	//returns the address of the current node
	ListNode<T>* reference() { return current; }
	//returns true if there exists a node after the current node
	//null pointers will return false
	bool hasNext() { return (current && current->next); }
	//moves the iterator to the next iteration
	//null pointers will lead to exceptions
	void next() { current = current->next; }
};

template<typename T>
class LinkedList {
private:
	ListNode<T>* head; //head of linked list
	ll len; //length of linked list
public:
	//constructor
	LinkedList(ListNode<T>* h = nullptr) : head(h), len(0) {
		if (head != nullptr) len++;
	}
	//returns length of linked list
	ll get_len() { return len; }
	//returns a clone of the head of linked list
	ListNode<T>* get_clone() { return head; }
	//outputs linked list
	void print() {
		ListNode<T>* clone = head;
		for (ll i = 0; i < len; i++) {
			std::cout << clone->val << " ";
			clone = clone->next;
		}
		std::cout << '\n';
	}
	//adds an element v to head of linked list
	void push_front(const T& v) {
		ListNode<T>* newNode = new ListNode<T>(v, head);
		head = newNode;
		len++;
	}
	//adds an element v to tail of linked list
	void push_back(const T& v) {
		if (head == nullptr) { //resolves case when linked list is empty
			push_front(v); return;
		}
		ListNode<T>* clone = head;
		for (ll i = 0; i < len - 1; i++) clone = clone->next;
		ListNode<T>* newNode = new ListNode<T>(v);
		clone->next = newNode;
		len++;
	}
	//drops and returns head of linked list 'involves c-assertion'
	T pop_front() {
		assert(len > 0);
		T droppedHead = head->val;
		head = head->next;
		len--;
		return droppedHead;
	}
	//drops and returns tail of linked list 'involves c-assertion'
	T pop_back() {
		assert(len > 0);
		T droppedHead;
		if (len == 1) return pop_front(); //resolves case when linked list is of length 1
		ListNode<T>* clone = head;
		for (ll i = 0; i < len - 2; i++) clone = clone->next;
		droppedHead = clone->next->val;
		clone->next = nullptr;
		len--;
		return droppedHead;
	}
	//returns element at (index)th position (starting from 0) 'involves c-assertion'
	T get_element(const ll& index) {
		assert(index >= 0 && index < len);
		ListNode<T>* clone = head;
		for (ll i = 0; i < index; i++) clone = clone->next;
		return clone->val;
	}
	//returns the index of the first encounter of a value v, in case of no encounter -1 is returned
	ll find(const T& v) {
		ListNode<T>* clone = head;
		for (ll i = 0; i < len; i++) {
			if (clone->val == v) return i;
			clone = clone->next;
		}
		return -1;
	}
	//returns a pair <iterator,bool>, where the iterator points to the node BEFORE the first encounter of a
	//value v, and the boolean value is true if the value is found and false otherwise
	std::pair<ListIterator<T>, bool> locate(const T& v) {
		ListIterator<T> itr(head);
		ListIterator<T> prev;
		for (ll i = 0; i < len; i++) {
			if (itr.value() == v) return { prev,true };
			prev = itr; itr.next();
		}
		return { nullptr, false };
	}
	//inserts an element v AFTER the node referenced by iterator itr
	//null pointer is assumed a 'push_front' attempt
	void insert(const T& v, ListIterator<T>& itr) {
		if (!itr.reference()) { 
			push_front(v); return;
		}
		ListNode<T>* newNode = new ListNode<T>(v, itr.reference()->next);
		itr.reference()->next = newNode;
		len++;
	}
	//drops and returns the element AFTER the node referenced by iterator itr 'involes c-assertion'
	//null pointer is assumed a 'pop_front' attempt
	T drop(ListIterator<T>& itr) {
		assert(len > 0);
		if (!itr.reference()) return pop_front();
		ListNode<T>* ref = itr.reference();
		assert(itr.hasNext());
		itr.next();
		T droppedValue = itr.value();
		ref->next = itr.reference()->next;
		itr = ListIterator<T>(ref);
		len--;
		return droppedValue;
	}
	//removes all occurences of a value v in the linked list
	void removeValue(const T& v) {
		if (len == 0) return;
		LinkedList<T> clone;
		ListIterator<T> itr(head);
		while (itr.hasNext()) { //elements will be in clone in reverse order
			if (itr.value() != v) clone.push_front(itr.value());
			itr.next();
		}
		if (itr.value() != v) clone.push_front(itr.value());
		head = nullptr; len = 0;
		while (clone.len) { //push elements back from clone to main to re-reverse order
			push_front(clone.pop_front());
		}
	}
};