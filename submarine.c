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
	
	LOG_INFO("InitializeSubmarine called");
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
	
	RadarObject *foe_to_shoot = NULL;
	RadarObject *current_object = NULL;
	RadarListNode *node = NULL;
	RadarObjectLinkedList *friends = NULL;
	SubmarineCommand submarine_command;
	double min_threat_distance = INVALID_DISTANCE;
	BOOL are_there_foes = FALSE;
	SubmarineFireCommand fire_command = CEASE;
	unsigned int new_direction = submarine->direction;
	int new_ammo = submarine->ammo;
	int new_depth = submarine->depth;
	
	LOG_INFO("HandleRadarPicture called");
	if ((submarine == NULL) || (radar == NULL))
	{
		LOG_ERROR("Wrong parameters");
		return FALSE;
	}
	memset(&submarine_command, '\0', sizeof(submarine_command));

	// Check if submarine is in danger
	if (IsSubmarineThreatened(radar) && (submarine->depth == SUBMARINE_LOW_DEPTH)) {
		new_depth = SUBMARINE_DEEP_DEPTH;
	}

	// Check if submarine is out of ammo
	if (submarine->ammo == 0)
	{
		fire_command = CEASE;
	}

	//if submarine has ammo, check if there is a friend in danger
	else {
		//getting information from radar
		if (!CalculateThreats(radar, submarine->direction)) {
			LOG_ERROR("Failed to calculate threats");
			return FALSE;
		}
		if (!GetSubmarineFriends(radar, &friends)) {
			LOG_ERROR("Failed to ger submarine friends");
			return FALSE;
		}

		// check for threatened friends
		if (friends->head == NULL)
		{
			LOG_INFO("Not checking for threatened friends since the friends list is empty");
		} else {
			/* TBD: move the following to another function */
			//initializing variables
			min_threat_distance = friends->head->entry->threat_distance;
			foe_to_shoot = friends->head->entry->most_threatening_foe;
			if (foe_to_shoot != NULL)
			{
				new_direction = friends->head->entry->most_threatening_foe->direction;
			}

			//calculating most threatening foe of all foes
			for (node = friends->head; node != NULL; node = node->next)
			{
				current_object = node->entry;
				if (current_object->most_threatening_foe == NULL)
				{
					LOG_INFO("Friend %s has no threatening foes", current_object->name);
					continue;
				}
				//checking whether current node's threat distance is less than seen so far, and with the case of two friends that their threat_distance equals
				//Reliying on the fact that INVALID_DISTANCE is the minimal not threatening foe (distance from submarine=1001)
				if ((current_object->threat_distance < min_threat_distance) ||
						((current_object->threat_distance == min_threat_distance) &&
						 (node->entry->most_threatening_foe->direction < new_direction) &&
						 (min_threat_distance != INVALID_DISTANCE)
					)
				)
				{
						are_there_foes = TRUE;
						new_direction = node->entry->most_threatening_foe->direction;
						foe_to_shoot = node->entry->most_threatening_foe;
						min_threat_distance = node->entry->threat_distance;
				}
			}
		}
	}

	// Eliminate target from Radar if necessary
	if (min_threat_distance != INVALID_DISTANCE)
	{
		//Then found a foe that has to be shut
		fire_command = FIRE;
		new_ammo--;
		if(!EliminateFoe(radar, foe_to_shoot)) 
		{
			LOG_ERROR("Failed to eliminate foe");
			return FALSE;
		}
	}

	// fill submarine_command fields
	submarine_command.new_ammo = new_ammo;
	submarine_command.new_direction = new_direction;
	submarine_command.fire_command = fire_command;
	submarine_command.new_depth = new_depth;

	//update submarine object fields
	submarine->ammo = submarine_command.new_ammo;
	submarine->depth = submarine_command.new_depth;
	submarine->direction = submarine_command.new_direction;

	//calculate threats again
	if (!CalculateThreats(radar,submarine->direction))
	{
		LOG_ERROR("Failed to recalculate the threats");
		return FALSE;
	}

	// Warn threatened friends
	for (node = friends->head; node != NULL; node = node->next)
	{
		if (node->entry->threat_distance != INVALID_DISTANCE)
		{
			WriteWarningMessage(submarine->submarine_output_writer, node->entry->name);
		}
	}

	WriteNewCommand(submarine->submarine_output_writer, &submarine_command);

//	return TRUE;

	// If there are no enemies
	//if (are_there_foes == FALSE && )
	//{
		// Calculate NEW_DIRECTION (change in submarine_command)
		// Use already_see_friends
		// Set DEPTH to 100 (change in submarine_command)
//	}
	

		// update already seen friends

	return TRUE;

}

BOOL FreeSubmarine(Submarine *submarine)
{
	LOG_INFO("FreeSubmarine called");
	if (submarine == NULL)
	{
		LOG_ERROR("Wrong parametes");
		return FALSE;
	}
	free(submarine);
	return TRUE;
}
