#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "Radar.h"
#include "SubmarineCommand.h"
#include "SubmarineOutputWriter.h"
#include "AlreadySeenFriends.h"

#define SUBMARINE_LOW_DEPTH 100
#define SUBMARINE_DEEP_DEPTH 200

/*
	basic field of struct Submarine
*/
typedef struct {
	int depth;
	int direction;
	int ammo;	//the amount of torpedos
	SubmarineOutputWriter *submarine_output_writer;
} Submarine;


Submarine *InitializeSubmarine(
	int initial_depth,
	int initial_direction,
	int initial_ammo,
	SubmarineOutputWriter *output_writer,
	AlreadySeenFriends ** already_seen_friends_pointer
);

BOOL HandleRadarPicture(
	Submarine *submarine, 
	Radar *radar,
	AlreadySeenFriends *already_seen_friends
);

BOOL FreeSubmarine(
	Submarine *submarine,
	AlreadySeenFriends ** already_seen_friends_pointer
);

#endif