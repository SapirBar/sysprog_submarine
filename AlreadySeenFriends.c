#include <stdlib.h>
#include "AlreadySeenFriends.h"
#include "LinkedList.h"
BOOL AddSeenFriend (SeenFriendsLinkedList * already_seen_list, char * name);

AlreadySeenFriends *InitializeAlreadySeenFriends()
{
	AlreadySeenFriends * already_seen_obj = NULL;
	already_seen_obj = (AlreadySeenFriends *)malloc (sizeof (AlreadySeenFriends *));
	if (already_seen_obj == NULL)
	{
		LOG_ERROR("failed to allocate memory for initialize already seen friends");
		return NULL; 
	}
	
	already_seen_obj->seen_friends = (SeenFriendsLinkedList *)InitializeLinkedList();
	if (already_seen_obj->seen_friends == NULL)
	{
		LOG_ERROR("failed to initialize seen_friends list");
		free(already_seen_obj);//free memory allocated earlier in this function
		return NULL;
	}
	
	return already_seen_obj;
}


BOOL UpdateFriends(
	AlreadySeenFriends *already_seen_friends,
	RadarObjectLinkedList *friends_list
)
{
	RadarListNode * friend_node_radar = NULL;
	SeenFriendNode * seen_friend_curr = NULL;
	BOOL is_already_seen = FALSE;
	
	if(already_seen_friends == NULL || already_seen_friends->seen_friends ==NULL)
	{
		LOG_ERROR("already seen friends list is not initialized");
			return FALSE;
	}
	if (friends_list == NULL)
	{
		LOG_ERROR ("Couldn't add friends to already seen list: received uninitialized radar list");
	}
	
	//walking through the friends in the radar list (all the friends in this point of time)
	for (friend_node_radar=friends_list->head; friend_node_radar != NULL; friend_node_radar=friend_node_radar->next)
	{
		//if the name is not already in the list, we will add it
		LOG_INFO ("try to add friend %s to seen friends", friend_node_radar->entry->name);
		 if (!IsAlreadySeenFriend(already_seen_friends, friend_node_radar->entry->name, &is_already_seen))
		 {   
			 LOG_ERROR("failed to check if the friend already seen");
			 return FALSE;
		 }

		 if (!is_already_seen)
		 {
			 //add the friend name to the list
			 if (!AddSeenFriend(already_seen_friends->seen_friends, friend_node_radar->entry->name))
			 {     //creating new node failed
				 LOG_ERROR("failed to add a friend to the already seen list");
				 return FALSE;
			 }
		 }
	}
	
	return TRUE;
}


//Add a friend to the list
BOOL AddSeenFriend (SeenFriendsLinkedList * already_seen_list, char * name)
{
	SeenFriend new_seen_friend = NULL;
	SeenFriend *new_seen_friend_ptr = NULL;
	int length_name = 0;
	length_name = strlen (name);
	
	//allocate memory for the object
	new_seen_friend = (SeenFriend)malloc(sizeof(char)*(length_name+1));
	if (new_seen_friend == NULL)
	{
		LOG_ERROR ("failed to allocate memory");
		return FALSE;
	}
	memcpy(new_seen_friend, name, length_name+1);

	new_seen_friend_ptr = (SeenFriend *)malloc(sizeof(new_seen_friend_ptr));
	if (new_seen_friend_ptr == NULL)
	{
		LOG_ERROR ("failed to allocate memory");
		return FALSE;
	}
	*new_seen_friend_ptr = new_seen_friend;

	if (!AddLinkedListEntry((LinkedList *)already_seen_list, new_seen_friend_ptr))
	{
		LOG_ERROR("failed to add a friend to the list");
		//free the memory allocated in this function and return
		free (new_seen_friend);     
		return FALSE;
	}
	LOG_INFO("added %s to seen friend list",name);
	
	return TRUE;
}


BOOL IsAlreadySeenFriend(
	AlreadySeenFriends *already_seen_friends,
	char * name,
	BOOL *is_already_seen
)
{
	SeenFriendNode *seen_friend_curr = NULL;
	*is_already_seen = FALSE;

	if (already_seen_friends == NULL || already_seen_friends->seen_friends == NULL)
	{
		LOG_ERROR("already seen friend not initilized");
		return FALSE;
	}
	
	//walking through the already seen friends check if the name is already exist
	seen_friend_curr=already_seen_friends->seen_friends->head;
	while (seen_friend_curr != NULL)
	{
		if (strcmp(name,(char *)*(seen_friend_curr->entry)) == 0)
		{
			//the name are identical and we return TRUE
			LOG_INFO("Already seen %s",name);
			*is_already_seen = TRUE;
			break;
		}
		seen_friend_curr=seen_friend_curr->next; 
	}

	//if the name is not exist,we return FALSE
	return TRUE; 	
}


BOOL FreeAlreadySeenFriends (AlreadySeenFriends *already_seen_friends)
{
	SeenFriendNode * seen_friend_curr=NULL;
	if (already_seen_friends == NULL || already_seen_friends->seen_friends == NULL)
	{
		LOG_ERROR("failed to free memory, received NULL pointer");
		return FALSE;
	}

	seen_friend_curr=already_seen_friends->seen_friends->head;
	while (seen_friend_curr != NULL)
	{
		free(*(seen_friend_curr->entry));
		free(seen_friend_curr->entry);
		seen_friend_curr=seen_friend_curr->next;
	}

	if (FreeLinkedList((LinkedList *)already_seen_friends->seen_friends) == FALSE) //free the list itself
	{
		LOG_ERROR("failed to free seen friend list");
		return FALSE;
	}

	free (already_seen_friends);
	return TRUE;
}