#ifndef ALREADY_SEEN_FRIENDS_H
#define ALREADY_SEEN_FRIENDS_H

#include "LinkedList.h"
#include "Radar.h" 

typedef char * SeenFriend;

typedef struct SeenFriendNode_s {
	SeenFriend *entry;
	struct SeenFriendNode_s *next;
	struct SeenFriendNode_s *prev;
} SeenFriendNode;

typedef struct {
	SeenFriendNode *head;
	SeenFriendNode *tail;
} SeenFriendsLinkedList;

typedef struct {
	SeenFriendsLinkedList *seen_friends;
} AlreadySeenFriends;

//Create new list of string holding the names of all the seen friendly ships.
AlreadySeenFriends *InitializeAlreadySeenFriends();

//Receive a pointer for a already seen friend type 
//(contating a pointer to the list of already seen friends) 
//and the list of the current friends in this point in time
//Update the already seen friend list to contain the new friends
//return TRUE if succeed  and FALSE if failed (problem with inputs for eample)
BOOL UpdateFriends(
	AlreadySeenFriends *already_seen_friends,
	RadarObjectLinkedList *friends_list
);

BOOL GetAlreadySeenFriends(
	AlreadySeenFriends *already_seen_friends,
	SeenFriendsLinkedList **seen_friends_list
);

//free memory??
#endif