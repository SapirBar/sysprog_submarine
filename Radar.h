#ifndef RADAR_H
#define RADAR_H

#include "LinkedList.h"
#include "common.h"

typedef enum {
	FRIEND=0,
	FOE
} RadarObjectType;

typedef struct RadarObject_s{
	char *name;
	RadarObjectType type;
	unsigned int direction;
	unsigned int distance;
	struct RadarObject_s *most_threatening_foe;
	float threat_distance;
} RadarObject;

typedef struct RadarListNode_s {
	RadarObject *entry;
	struct RadarListNode_s *next;
	struct RadarListNode_s *prev;
} RadarListNode;

typedef struct {
	RadarListNode *head;
	RadarListNode *tail;
} RadarObjectLinkedList;

typedef struct Radar_s {
	RadarObjectLinkedList friends;
	RadarObjectLinkedList foes;
	BOOL is_friends_list_updated;
} Radar;

Radar *InitializeRadar();

BOOL AddRadarObject(
	Radar *radar, 
	const char *name, 
	RadarObjectType type, 
	unsigned int direction,
	unsigned int distance
);

BOOL CalculateThreats(
	Radar *radar
);

/* Return FALSE in case that the friends list threats is not updated.
 */
BOOL GetSubmarineFriends(
	Radar *radar, 
	RadarObjectLinkedList **friends
);

BOOL EliminateFoe(
	Radar *radar,
	RadarObject *foe
);

BOOL IsSubmarineThreatened(
	Radar *radar
);

BOOL FreeRadar(
	Radar *radar
);

#endif //RADAR_H
