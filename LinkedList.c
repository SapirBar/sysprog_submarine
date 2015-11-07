#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

LinkedList *InitializeLinkedList()
{
	LinkedList *list = NULL;

	list = (LinkedList *)malloc(sizeof(*list));
	if (list == NULL)
	{
		return NULL;
	}
	memset(list, '\0', sizeof(*list));
	list->head = NULL;
	list->tail = NULL;

	return list;
}

BOOL AddLinkedListEntry(LinkedList *list, void* entry)
{
	ListNode *new_entry = NULL;

	if ((list == NULL) || (entry == NULL))
	{
		return FALSE;
	}

	new_entry = (ListNode *)malloc(sizeof(*new_entry));
	if (new_entry == NULL)
	{
		return FALSE;
	}
	memset(new_entry, '\0', sizeof(*new_entry));
	new_entry->entry = entry;
	new_entry->next = list->head->next;
	new_entry->prev = NULL;
	list->head = new_entry;

	if (list->tail == NULL)
	{
		list->tail = new_entry;
	}

	return TRUE;
}

BOOL DeleteLinkedListEntry(LinkedList *list, void* entry)
{
	ListNode *node = NULL;
	BOOL was_deleted = FALSE;

	if ((list == NULL) || (entry == NULL))
	{
		return FALSE;
	}

	node = list->head;

	while (node != NULL)
	{
		if (node->entry == entry)
		{
			if (node->prev == NULL)
			{
				list->head = node->next;
			} else {
				node->prev->next = node->next;
			}

			if (node->next == NULL)
			{
				list->tail = node->prev;
			} else {
				node->next->prev = node->prev;
			}
			
			free(node);
			was_deleted = TRUE;
			break;
		}
	}

	return was_deleted;

}

BOOL FreeLinkedList(LinkedList *list)
{
	ListNode *entry = NULL;
	ListNode *next = NULL;
	if (list == NULL)
	{
		return FALSE;
	}

	for (entry=list->head; entry->next != NULL; )
	{
		next = entry->next;
		free(entry);
		entry = next;
	}

	free(list);

	return TRUE;

}
