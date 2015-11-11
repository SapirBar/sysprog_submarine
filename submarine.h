//Author: Adi Mashiah, ID:305205676\Reut Lev, ID:305259186
//Belongs to project: sysprog_submarine
//This module represents the submarine entity. It recieves the radar image and decides about the output command.
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

/*//$//
Submarine *InitializeSubmarine(
	int initial_depth, 
	int initial_direction, 
	int initial_ammo,
	SubmarineOutputWriter *output_writer,
	AlreadySeenFriends ** already_seen_friends_pointer
);
*/
Submarine *InitializeSubmarine(
	int initial_depth,
	int initial_direction,
	int initial_ammo,
	SubmarineOutputWriter *output_writer
);

BOOL HandleRadarPicture(
	Submarine *submarine, 
	Radar *radar,
	AlreadySeenFriends *already_seen_friends
);

BOOL FreeSubmarine(
	Submarine *submarine
);
/*//$//
BOOL FreeSubmarine(
	Submarine *submarine,
	AlreadySeenFriends **already_seen_friends_pointer
);
*/
#endif