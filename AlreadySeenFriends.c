
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
	for (friend_node_radar = friends_list->head; friend_node_radar != NULL; friend_node_radar = friend_node_radar ->next)
	{
		//walking through the already seen friends check if the name is already exist
		seen_friend_curr=already_seen_friends->seen_friends->head;
		 while (seen_friend_curr != NULL)
		 {
			 if (strcmp(friend_node_radar->entry->name,(char *)seen_friend_curr->entry) == 0)
				 {
					 break;
			     }
				 seen_friend_curr=seen_friend_curr->next; 
		 }
		//if the name is not exist,the pointer is pointing to the the end of the list
		 
		 if (seen_friend_curr = NULL)
		 {    //add the friend name to the list
			 if (AddSeenFriend(already_seen_friends->seen_friends,friend_node_radar->entry->name) == FALSE)
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
	SeenFriendNode * new_seen_friend=NULL;
	int lenght_name = 0;
	lenght_name = strlen (name);
	//allocate memory for the object
	new_seen_friend= (SeenFriendNode *) malloc (sizeof (SeenFriendNode *));
	if (new_seen_friend == NULL)
	{
		LOG_ERROR("failed to allocate memory");
		return FALSE; 
	}
	new_seen_friend->entry = (SeenFriend *) malloc (sizeof(char)*(lenght_name+1));
	if (new_seen_friend->entry)
	{
		LOG_ERROR ("failed to allocate memory");
		free (new_seen_friend);
		return FALSE;
	}
	memcpy(new_seen_friend->entry, name, lenght_name+1);
	if (AddLinkedListEntry((LinkedList *)already_seen_list,new_seen_friend) == FALSE)
		{
			LOG_ERROR("failed to add a foe to the list");
			free (new_seen_friend->entry); //free the memory allocated in this function and return
			free (new_seen_friend);     
			return FALSE;
		}
	return TRUE;
}

BOOL GetAlreadySeenFriends(
	AlreadySeenFriends *already_seen_friends,
	SeenFriendsLinkedList **seen_friends_list
)
{
	if (already_seen_friends == NULL)
	{
		LOG_ERROR("already seen friend not initilized");
		return FALSE;
	}
	*seen_friends_list = already_seen_friends->seen_friends;
	return TRUE;
}

BOOL FreeAlreadySeenFriends (AlreadySeenFriends *already_seen_friends)
{
	SeenFriendNode * seen_friend_curr=NULL;
if (already_seen_friends == NULL)
	{
		LOG_ERROR("failed to free memory, received NULL pointer");
		return FALSE;
	}
seen_friend_curr=already_seen_friends->seen_friends->head;
while (seen_friend_curr != NULL)
{
	free (seen_friend_curr->entry);
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