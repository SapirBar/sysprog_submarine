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
	struct RadarObject_s *foe_to_shoot = NULL;
	double min_threat_distance = INVALID_DISTANCE;
	unsigned int temp_new_direction;
	RadarListNode *node = NULL;

	RadarObjectLinkedList *friends = NULL;
	BOOL are_there_foes = FALSE;

	SubmarineCommand *submarine_command = NULL;
	SubmarineOutputWriter *submarine_output_writer = NULL;
	
	// Check if submarine is in danger
	if (IsSubmarineThreatened && (submarine->depth == SUBMARINE_LOW_DEPTH)) {
		submarine_command->new_depth = SUBMARINE_DEEP_DEPTH;
	}
	// Check if submarine is out of ammo
	if (submarine->ammo == 0)
	{
		submarine_command->fire_command = CEASE;
	}
	//if submarine has ammo, check if there is a friend in danger
	else {
		//getting information from radar
		CalculateThreats(radar, submarine->direction);
		GetSubmarineFriends(radar, &friends);
		
		//initializing variables
		min_threat_distance = friends->head->entry->threat_distance;
		foe_to_shoot = friends->head->entry->most_threatening_foe;
		temp_new_direction = friends->head->entry->most_threatening_foe->direction;
		most_threatened_friend = friends->head;
		
		//calculating most threatening foe of all foes
		for (node = friends->head; node != NULL; node = node->next)
		{
			//checking wether current node's threat distance is less than seen so far, and with the case of two friends that their threat_distance equals
			if ((node->entry->threat_distance < min_threat_distance)||((node->entry->threat_distance == min_threat_distance) && (node->entry->most_threatening_foe->direction < temp_new_direction)&&(min_threat_distance != INVALID_DISTANCE)))
			{
					are_there_foes = TRUE;
					temp_new_direction = node->entry->most_threatening_foe->direction;
					foe_to_shoot = node->entry->most_threatening_foe;
					min_threat_distance = node->entry->threat_distance;
					most_threatened_friend = node;
			}
		}
		submarine_command->new_direction = temp_new_direction;
	}

	// Eliminate target from Radar if necessary
	if (min_threat_distance != INVALID_DISTANCE)
	{
		submarine_command->fire_command = FIRE;
		submarine_command->new_ammo = (submarine->ammo) - 1;
		EliminateFoe(radar, foe_to_shoot);
	}

	//update submarine object fields
	submarine->ammo = submarine_command->new_ammo;
	submarine->depth = submarine_command->new_depth;
	submarine->direction = submarine_command->new_direction;
	submarine->submarine_output_writer = submarine_output_writer;

	//calculate threats again		
	CalculateThreats(radar,submarine->direction);

	// Warn threatened friends
	for (node = friends->head; node != NULL; node = node->next)
	{
		if (node->entry->threat_distance != INVALID_DISTANCE)
		{
			WriteWarningMessage(submarine_output_writer, most_threatened_friend->entry->name);
		}
	}

	return TRUE;

	// If there are no enemies
//	if (are_there_foes == FALSE && )
//	{
		// Calculate NEW_DIRECTION (change in submarine_command)
		// Use already_see_friends
		// Set DEPTH to 100 (change in submarine_command)
//	}
	

		// update already seen friends

}

BOOL FreeSubmarine(Submarine *submarine)
{
	if (submarine == NULL)
	{
		LOG_ERROR("Wrong parametes");
		return FALSE;
	}
	free(submarine);
	return TRUE;
}

