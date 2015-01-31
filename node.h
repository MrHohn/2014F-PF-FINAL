/*
The Node class for a linked list

Written by Bernhard Firner
Modified by Zihong Zheng
*/

#include <iostream>
#ifndef __NODE_H__
#define __NODE_H__
int count_delete = 0;


template <typename T>
class Node {
private:
	//The data being stored
	T value;
	//Links to the previous and next nodes
	Node* next;
	Node* prev;

public:
	//Constructor that sets the initial value and sets the pointers to null
	Node(T value) {
		Node::value = value;
		//Indicate that next and previous are invalid
		next = nullptr;
		prev = nullptr;
	}

	//Get the value of this node
	T& getValue() {
		return value;
	}
	/* define the setValue method to set the value of the node */
	void setValue(T value_set){
		/* delete the original content pointed by the pointer */
		delete value;
		value = value_set;
	}

	void insert(Node* n) {
		//Fix the link from the new node to our current next
		if (nullptr != next) {
			next->prev = n;
		}
		n->next = next;

		//Fix the link between this and the new node
		n->prev = this;
		next = n;
	}

	void insertBefore(Node* n) {
		//Fix the link between the new node and the previous node
		if (nullptr != prev) {
			prev->next = n;
		}
		n->prev = prev;

		//Fix the link between this node and the new node
		n->next = this;
		prev = n;
	}

	//Ways to fetch previous and next to navigate through the list
	Node* getPrev() {
		return prev;
	}

	Node* getNext() {
		return next;
	}

	~Node() {
		//cout << "finish delete node" << endl;
		// count_delete++;
		// cout << count_delete << endl;
		//Connect the nodes on either side since this one is going away
		if (nullptr != prev) {
			// cout<< "setting up the bridge between form node to next node" << endl;
			prev->next = next;
		}
		if (nullptr != next) {
			// cout<< "setting up the bridge between next node to form node" << endl;
			next->prev = prev;
		}
	}
};

#endif
