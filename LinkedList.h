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

BOOL AddLinkedListEntry(LinkedList *list, void* entry);

BOOL DeleteLinkedListEntry(LinkedList *list, void* entry);

//ListNode *GetFirstLinkedListEntry(ListNode *list);

//ListNode *GetFirstLinkedNextEntry(ListNode *list);

BOOL FreeLinkedList(LinkedList *list);

#endif //LINKED_LIST_H