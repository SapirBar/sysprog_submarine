#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "Radar.h"
#include "SubmarineOutputWriter.h"

#define SUBMARINE_LOW_DEPTH 100
#define SUBMARINE_DEEP_DEPTH 200

/*
	basic field of struct Submarine
*/
typedef struct {
	int depth;
	int direction;
	int ammo;	//the amount of torpedos
	SubmarineOutputWriter *submarine_output_writer
} Submarine;

typedef enum {
	FIRE=0,
	CEASE
} SubmarineFireCommand;

typedef struct {
	int new_direction;
	int new_depth;
	SubmarineFireCommand fire_command;
	int new_ammo;
} SubmarineCommand;


Submarine *InitializeSubmarine(
	int initial_depth,
	int initial_direction,
	int initial_ammo
);

BOOL HandleRadarPicture(
	Submarine *submarine, 
	Radar *radar
);

BOOL FreeSubmarine(
	Submarine *submarine
);

#endif