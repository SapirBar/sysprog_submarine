//Author: Adi Mashiah, ID:305205676\
//Belongs to project: sysprog_submarine
//This module represents the submarine entity. It recieves the radar image and decides about the output command.
#include <stdlib.h>
#include "submarine.h"

Submarine *InitializeSubmarine(int initial_depth, int initial_direction, int initial_ammo) {
	//returns the pointer for Submarine struct created.
	Submarine *submarine = NULL;
	
	submarine = (Submarine *)malloc(sizeof(*submarine));
	if (submarine == NULL) {
		LOG_ERROR("Failed to allocate memory");
		return NULL;
	}

	submarine->depth = initial_depth;
	submarine->direction = initial_direction;
	submarine->ammo = initial_ammo;
	
	return submarine;
}

BOOL HandleRadarPicture(Submarine *submarine, Radar *radar) {
	if (IsSubmarineThreatened && (submarine->depth == SUBMARINE_LOW_DEPTH)) {
		submarine->depth = SUBMARINE_DEEP_DEPTH;

	}
}
