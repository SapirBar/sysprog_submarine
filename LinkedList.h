#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "common.h"

typedef struct Node {
	void *entry;
	struct Node *next;
	struct Node *prev;
} ListNode;

typedef struct {
	ListNode *head;
	ListNode *tail;
} LinkedList;

LinkedList *InitializeLinkedList();

//Create new entry ,receive a pointer to the entry and the list to insert to, return FALSE in case not succeeded to create new entry. 
BOOL AddLinkedListEntry(
	LinkedList *list, //pointer to the list need to contain the object in.
	void* entry		  //A pointer to entry to insert
);

BOOL DeleteLinkedListEntry(
	LinkedList *list, 
	void* entry
);

BOOL FreeLinkedList(
	LinkedList *list
);

#endif //LINKED_LIST_H
