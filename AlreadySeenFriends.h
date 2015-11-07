#ifndef ALREADY_SEEN_FRIENDS_H
#define ALREADY_SEEN_FRIENDS_H

#include "LinkedList.h"

typedef (char *) SeenFriend;

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

AlreadySeenFriends *InitializeAlreadySeenFriends();

BOOL UpdateFriends(
	AlreadySeenFriends *already_seen_friends,
	RadarObjectLinkedList *friends_list
);

BOOL GetAlreadySeenFriends(
	AlreadySeenFriends *already_seen_friends,
	SeenFriendsLinkedList **seen_friends_list
);

#endif