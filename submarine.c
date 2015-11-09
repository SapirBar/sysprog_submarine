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
	
	/* CR: the type should be RadarObject (take it from the node entry) -V*/
	RadarObject *most_threatened_friend = NULL;
	/* CR: struct RadarObject_s -> RadarObject -V */
	RadarObject *foe_to_shoot = NULL;
	double min_threat_distance = INVALID_DISTANCE;
	/* CR: initialize variable to default value -V */
	/* CR: rename it to a non-temp variable name -V*/
	unsigned int new_direction=0;
	int new_ammo = submarine->ammo;
	RadarListNode *node = NULL;

	RadarObjectLinkedList *friends = NULL;
	BOOL are_there_foes = FALSE;
	BOOL succeeded_eliminating_foe = FALSE;

	/* CR: you don't need here a pointer, allocate the struct in the stack */
	SubmarineCommand *submarine_command = NULL;
	/* CR: don't needed. See CR comment at the bottom of the function */
	SubmarineOutputWriter *submarine_output_writer = NULL;
	
	// Check if submarine is in danger
	/* CR: IsSubmarineThreatened is a function. You should pas it parameters :) -V */
	if (IsSubmarineThreatened(radar) && (submarine->depth == SUBMARINE_LOW_DEPTH)) {
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
		/* CR: check the return value -V */
		if((CalculateThreats(radar, submarine->direction)) && (GetSubmarineFriends(radar, &friends))) {
		/* CR: check the return value -V */
		
		/* CR: move the following to another function */
		//initializing variables
		/* CR: if the friends list is empty then you cannot access to the entry fields
		 * (you will crash the program trying to access a NULL memory) --- Adi's response: that would not happen, since the above "if" prevents it.
		 * Check if the list is empty
		 */
		min_threat_distance = friends->head->entry->threat_distance;
		foe_to_shoot = friends->head->entry->most_threatening_foe;
		new_direction = friends->head->entry->most_threatening_foe->direction;
		most_threatened_friend = friends->head->entry;
		
		//calculating most threatening foe of all foes
		for (node = friends->head; node != NULL; node = node->next)
		{
			//checking whether current node's threat distance is less than seen so far, and with the case of two friends that their threat_distance equals
			/* CR: split this if to a multi-line if (it is not readable) -V */
			/* CR: it is recommended to write that you are relying of the fact the the INVALID_DISTANCE (and the initial min_threat_distance) 
			 * is the minimal not threatening one -V 
			 */
			//Reliying on the fact that INVALID_DISTANCE is the minimal not threatening foe (distance from submarine=1001)
			if ((node->entry->threat_distance < min_threat_distance)
				||((node->entry->threat_distance == min_threat_distance) 
					&& (node->entry->most_threatening_foe->direction < new_direction)
					&&(min_threat_distance != INVALID_DISTANCE)))
			{
					/* CR: Why is are_there_foes needed? ---Adi's response: because of AlreadySeenFriends*/
					are_there_foes = TRUE;
					new_direction = node->entry->most_threatening_foe->direction;
					foe_to_shoot = node->entry->most_threatening_foe;
					min_threat_distance = node->entry->threat_distance;
					/* CR: most_threatened_friend is a RadarObjectListNode, it is preferable to store 
					 * it's entry (of type RadarObject). RadarObjectListNode should be used only for list
					 * manipulation operations. -V
					 */
					most_threatened_friend = node->entry;
			}
		}
		submarine_command->new_direction = new_direction;
	}
	}

	// Eliminate target from Radar if necessary
	if (min_threat_distance != INVALID_DISTANCE)
	{
		//Then found a foe that has to be shut
		submarine_command->fire_command = FIRE;
		/* CR: use new_ammo-- -V*/
		new_ammo--;
		submarine_command->new_ammo = new_ammo;
		/* CR: Check the return value */
		succeeded_eliminating_foe = EliminateFoe(radar, foe_to_shoot);
		if(!succeeded_eliminating_foe) 
		{
			LOG_ERROR("Failed to eliminate foe");
			return FALSE;
		}
	}

	//update submarine object fields
	submarine->ammo = submarine_command->new_ammo;
	submarine->depth = submarine_command->new_depth;
	submarine->direction = submarine_command->new_direction;
	/* CR: the submarine_output_write that you have is NULL, you should
	 * not update this field. You should use the writer that is in
	 * the submarine from the InitializeSubmarine function
	 */
	submarine->submarine_output_writer = submarine_output_writer;

	//calculate threats again
	/* CR: check return value */
	CalculateThreats(radar,submarine->direction);

	// Warn threatened friends
	for (node = friends->head; node != NULL; node = node->next)
	{
		if (node->entry->threat_distance != INVALID_DISTANCE)
		{
			/* CR: why are you using the most_threatened_friend here?
			 * I think that you are confused with node->entry->most_threatening_foe -V
			 */
			WriteWarningMessage(submarine_output_writer, node->entry->name);
		}
	}

//	return TRUE;

	// If there are no enemies
	if (are_there_foes == FALSE && )
	{
		// Calculate NEW_DIRECTION (change in submarine_command)
		// Use already_see_friends
		// Set DEPTH to 100 (change in submarine_command)
//	}
	

		// update already seen friends

		return TRUE;

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

