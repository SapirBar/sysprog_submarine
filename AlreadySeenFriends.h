//Author: Adi Mashiah, ID:305205676\Reut Lev, ID:305259186
//Belongs to project: sysprog_submarine
//Already seen friends save the friends which already seen
//and can tell if any friend already seen or not. 
// using the files: "LinkedList.h","Radar.h"
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
//receive a pointer to SeenFriends structure and a name of a ship,
// the function return TRUE if the ship is already in the list 
// retyeb False if it is not in the list or there is a problem with the pointers.
// in case pf problem- also output log error
BOOL IsAlreadySeenFriend(
	AlreadySeenFriends *already_seen_friends,
	char *name,
	BOOL *is_already_seen
);

//receive a pointer to the list and free all the memory allocated in the list.
//return false in case nulll
BOOL FreeAlreadySeenFriends (AlreadySeenFriends *already_seen_friends);
#endif
