#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

LinkedList *InitializeLinkedList()
{
	LinkedList *list = NULL;

	list = (LinkedList *)malloc(sizeof(*list));
	if (list == NULL)
	{
		LOG_ERROR("Failed to allocate memory");
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
		LOG_ERROR("Wrong parameters");
		return FALSE;
	}

	new_entry = (ListNode *)malloc(sizeof(*new_entry));
	if (new_entry == NULL)
	{
		LOG_ERROR("Failed to allocate memory");
		return FALSE;
	}
	memset(new_entry, '\0', sizeof(*new_entry));
	new_entry->entry = entry;

	if (list->head == NULL)
	{
		list->head = new_entry;
		list->tail = new_entry;
	} else
	{//$//CHANGED list->head->next to list->head
		new_entry->next = list->head;
	}
	new_entry->prev = NULL;
	list->head = new_entry;

	return TRUE;
}

BOOL DeleteLinkedListEntry(LinkedList *list, void* entry)
{
	ListNode *node = NULL;
	BOOL was_deleted = FALSE;

	if ((list == NULL) || (entry == NULL))
	{
		LOG_ERROR("Wrong parameters");
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
		LOG_ERROR("Wrong parameters");
		return FALSE;
	}

	for (entry=list->head; entry != NULL; )
	{
		next = entry->next;
		free(entry);
		entry = next;
	}

	free(list);

	return TRUE;

}
