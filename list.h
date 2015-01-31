/*
Linked list class

Written by Bernhard Firner
Modified by Zihong Zheng
*/

#include "node.h"
#include <iostream>

#ifndef __LIST_H__
#define __LIST_H__

template<typename T>
class LList {
private:
	Node<T>* head;
	Node<T>* tail;

public:

	//Null constructor is for an empty list
	LList() {
		head = nullptr;
		tail = nullptr;
	}

	~LList() {
		//Keeps deleting the node after head until there aren't any
		if (nullptr != head) {
			while (nullptr != head->getNext()) {
				delete head->getNext();
			}
			delete head;
			/* used for debuging whether we delete all things when we exit the program */
			//cout << "finish delete linked list" << endl;
		}

		/*
		//Or we could loop through the entire list deleting as we go.
		Node<T>* target = head;
		while (nullptr != target) {
			Node<T>* tmp = target->getNext();
			delete target;
			target = tmp;
		}
		*/
	}

	void push_back(T val) {
		Node<T>* n = new Node<T>(val);
		//No head means we don't have a list yet
		if (nullptr == head) {
			head = n;
			tail = n;
		}
		else {
			tail->insert(n);
			tail = n;
		}
	}

	void push_front(T val) {
		Node<T>* n = new Node<T>(val);
		if (nullptr == head) {
			head = n;
			tail = n;
		}
		else {
			head->insertBefore(n);
			head = n;
		}
	}

	//Helper class for iterating through the linked list
	class iterator {
		//We can allow access to private variables to "friend" classes
		friend class LList;
	private:
		Node<T>* location;
		// LList* container;
	public:
		//Make sure that other templates can tell what type this iterator is holding
		typedef T value_type;

		iterator(){}

		// iterator(Node<T>* start, LList* container) {
		iterator(Node<T>* start) {
			location = start;
			// iterator::container = container;
		}

		//Prefix ++ operator
		iterator& operator++() {
			//Go to the next location if it exists
			if (nullptr != location) {
				location = location->getNext();
			}
			//Return the new value
			return *this;
		}

		//prefix operator--
		iterator& operator--() {
			//If we are at the end then go to the tail (the last element)
			if (nullptr == location) {
				location = container->tail;
			}
			else {
				if (nullptr != location->getPrev()) {
					location = location->getPrev();
				}
				//otherwise we should really throw an exception because we can't go before the beginning
			}
			return *this;
		}

		//Postfix ++ operator
		iterator operator++(int unused) {
			iterator copy = *this;
			operator++();
			return copy;
		}

		//* operator to access the element at this iterator's position
		T& operator*() {
			return location->getValue();
		}

		//The != operator compares this iterator's location to another iterator's location
		bool operator!=(iterator& other) {
			return location != other.location;
		}

		//We could define other things, such as operator+ or operator-
	};

	iterator begin() {
		// return iterator(head, this);
		return iterator(head);
	}

	iterator end() {
		//Return an iterator to nullptr rather than tail because end is one past the last element
		// return iterator(nullptr, this);
		return iterator(nullptr);
	}

	//Erases the element at a position and returns the new valid location for the iterator
	//Retruns the iterator following the last removed element. If the iterator pos refers to the last element,
	//the end() iterator is returned.
	iterator erase(iterator position) {
		iterator next = position;
		++next;
		//Make sure that the user isn't trying to delete end()
		if (nullptr != position.location) {

			delete position.location;
			// free(position.location);
		}
		return next;
	}
	/* define the erase method for the first element in the list because we need to modified the head of the linked list */
	void erase_first(iterator position){
		/* change the head */
		head = head -> getNext();
		/* delete the original head */
		delete position.location;
	}
	/* define the set_value method to set the specific node's value */
	void set_value(iterator position, T value){
		/* call the setValue method of the node class */
		position.location -> setValue(value);
	}

	//inserts value before the location pointed to by pos
	//Returns an iterator pointing to the inserted value
	iterator insert(iterator pos, const T& value) {
		Node<T>* n = new Node<T>(value);
		if (nullptr != pos.location) {
			pos.location->insertBefore(n);
			return iterator(pos.location->getPrev());
		}
		else {
			//What if we are trying to insert something before the end?
			push_back(value);
			return iterator(tail);
		}
	}

};


#endif
