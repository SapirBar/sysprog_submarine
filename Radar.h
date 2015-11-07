#ifndef RADAR_H
#define RADAR_H

#include "LinkedList.h"
#include "common.h"

typedef enum {
	ALLIE=0,
	ENEMY
} RadarObjectType;

typedef struct {
	char *name;
	RadarObjectType type;
	unsigned int direction;
	unsigned int distance;
} RadarObject;

typedef struct RadarListNode_s {
	RadarObject *entry;
	struct RadarListNode_s *next;
	struct RadarListNode_s *prev;
} RadarListNode;

typedef struct Radar_s {
	ListNode allies;
	ListNode enemies;
} Radar;

Radar *InitializeRadar();

BOOL AddRadarObject(Radar *radar, RadarObjectType object);

BOOL GetRadarAllies(Radar *radar, RadarListNode **allies);

BOOL GetRadarEnemies(Radar *radar, RadarListNode **enemies);

BOOL free(Radar *radar);

#endif //RADAR_H