//Author: Adi Mashiah, ID:305205676\
//Belongs to project: sysprog_submarine
//This module represents the submarine entity. It recieves the radar image and decides about the output command.
#include <stdlib.h>
#include "submarine.h"

Submarine *InitializeSubmarine(
	int initial_depth, 
	int initial_direction, 
	int initial_ammo,
	SubmarineOutputWriter *output_writer
) {
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
	submarine->submarine_output_writer = output_writer;
	
	return submarine;
}

BOOL HandleRadarPicture(Submarine *submarine, Radar *radar) {
	
	RadarListNode *most_threatened_friend = NULL;
	char foe_to_shoot;
	float min_threat_distance;
	unsigned int new_direction;
	RadarObjectLinkedList *friends = NULL;
	SubmarineCommand *submarine_command = NULL;
	RadarListNode *node = NULL;
	submarine_command = (SubmarineCommand *)malloc(sizeof(submarine_command));

	// Check if we need to fire
	
	if (IsSubmarineThreatened && (submarine->depth == SUBMARINE_LOW_DEPTH)) {
		submarine_command->new_depth = SUBMARINE_DEEP_DEPTH;
	}
	// CEASE - if AMMO == 0
	if (submarine->ammo == 0) {
		submarine_command->fire_command = CEASE;
	}	
	
	// FIRE - if there is a threatened ship
	//getting information from radar
	CalculateThreats(radar);
	GetSubmarineFriends(radar, &friends);
	
	//calculating most threatening foe of all foes 
	min_threat_distance = friends->head->entry->threat_distance;
	foe_to_shoot = friends->head->entry->most_threatening_foe;
	new_direction = friends->head->entry->most_threatening_foe->direction;
	most_threatened_friend = friends->head;
	for (node = friends->head; node == friends->tail; node = node->next)
	{
		//dealing with the case of two friend ships that their threat_distance equals
		if (node->entry->threat_distance == min_threat_distance)
		{
			if(node->entry->most_threatening_foe->direction < new_direction)
		}
		if (node->entry->threat_distance < min_threat_distance)
		{
			min_threat_distance = node->entry->threat_distance;
			foe_to_shoot = node->entry->most_threatening_foe;
			new_direction = node->entry->most_threatening_foe->direction;
			most_threatened_friend = node;
		}
	}


			// set AMMO-=1, NEW_DIRECTION
			// don't change DEPTH
			// Eliminate target from Radar
			//calculate threats again		

	// Warn threatened friends

	// If there are no enemies
		// Calculate NEW_DIRECTION
			// Use already_see_friends
		// Set DEPTH to 100

		// update already seen friends
		//update submarine object fields


}
