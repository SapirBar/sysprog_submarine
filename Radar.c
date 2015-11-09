
#include <stdlib.h>
#include "Radar.h"
#include "LinkedList.h"
#include <string.h>
#include "common.h"
#include <math.h>

BOOL FreeRadarList (LinkedList * list);
double OclideDistance ( unsigned int dist1, unsigned int dir1, unsigned int dist2, unsigned int dir2);
unsigned int RelativeAngle (unsigned int absolute_dir1, unsigned int absolute_dir2);
Radar *InitializeRadar()
{
	Radar *radar_picture = NULL;
	radar_picture= (Radar *) malloc (sizeof (Radar *));
	if (radar_picture == NULL)
	{
		LOG_ERROR("failed to allocate memory");
		return NULL; 
	}
	memset(radar_picture , '\0', sizeof(Radar));
	radar_picture->foes = InitializeLinkedList();
	if (radar_picture->foes == NULL)
	{
		LOG_ERROR("failed to initialize foes list");
		free(radar_picture);
		return NULL;
	}
	radar_picture->friends = InitializeLinkedList();
	if (radar_picture->friends == NULL)
	{
		LOG_ERROR("failed to initialize friends list");
		FreeLinkedList(radar_picture->foes);
		free(radar_picture);
		return NULL; 
	}
	//When initializing the list, the threats foes are not updated. 
	radar_picture->is_friends_list_updated=FALSE;
	return radar_picture;
}



BOOL AddRadarObject(
	Radar *radar,             //Pointer to Radar structure (created by InitializeRadar function)
	char *name,               //The name of the sheep
	RadarObjectType type,     //gets: FOE or FRIEND
	unsigned int direction,   //The direction of the sheep
	unsigned int distance     //The distance of the sheep
)
{
	RadarObject *new_radar_object=NULL;
	int lenght_name = 0;
	if (radar == NULL)
	{
		LOG_ERROR("cann't add object to Radar: radar object not intialized!") ;
		return FALSE;
	}
	new_radar_object= (RadarObject *) malloc (sizeof (RadarObject *));
	if (new_radar_object == NULL)
	{
		LOG_ERROR("failed to allocate memory");
		return FALSE; 
	}

	new_radar_object->type = type;
	lenght_name = strlen (name);
		//malloc and copy the name string
	new_radar_object->name = (char *) malloc (sizeof(char)*(lenght_name+1));
	if (new_radar_object->name == NULL)
	{
		LOG_ERROR ("failed to malloc memory");
		free (new_radar_object);
		return FALSE;
	}
	memcpy(new_radar_object->name, name, lenght_name+1);
	new_radar_object->direction = direction;
	new_radar_object->distance = distance;
	new_radar_object->most_threatening_foe = NULL;
	new_radar_object->threat_distance = INVALID_DISTANCE;
	if (type == FOE) 
	{
		if (AddLinkedListEntry(radar->foes,new_radar_object) == FALSE)
		{
			LOG_ERROR("failed to add a foe to the list");
			free (new_radar_object->name); //free the memory allocated in this function and return
			free (new_radar_object);     
			return FALSE;
		}
	}
	else //type is FRIEND 
	{
		if (AddLinkedListEntry(radar->friends,new_radar_object) == FALSE)
		{
			LOG_ERROR("failed to add a friend to the list");
			free (new_radar_object->name); //free the memory allocated in this function and return
			free (new_radar_object);
			return FALSE;
		}
	}
return TRUE;
}



BOOL CalculateThreats(
	Radar *radar,
	unsigned int submarine_direction //if there are two treating ships in the same priority, we need to choose the ship which her relative direction from the submrine is minimal.
)
{
	RadarListNode *current_friend = NULL, *current_foe = NULL;
	double min_distance = INVALID_DISTANCE, curr_distance = 0;
	if (radar == NULL || radar->friends == NULL || radar->foes == NULL)
	{
		LOG_ERROR ("radar picture is empty");
		return FALSE;
	}
	current_friend = (RadarListNode *) radar->friends->head;
	while (current_friend != NULL) //For each friend in the list of friends
		{
			//initialized the foe enemy (dealing the case we fire the enemy and calling the function for second time)
			current_friend->entry->most_threatening_foe=NULL;
			current_friend->entry->threat_distance = INVALID_DISTANCE;
			current_foe = (RadarListNode *) radar->foes->head;
			while (current_foe != NULL) //scanning each foe in the enemy foe
			{
				curr_distance = OclideDistance(current_foe->entry->distance,current_foe->entry->direction,current_friend->entry->distance,current_friend->entry->direction);
				if (curr_distance < min_distance) // found an enemy threats a friend ship closer to the friend ship (min_distance initialized to 1000) 
				{
					min_distance=curr_distance;
				    //update the fields in the friend entry in case there is an enemy threats
					current_friend->entry->threat_distance = min_distance;
					current_friend->entry->most_threatening_foe = current_foe->entry;
				}
				if (curr_distance ==  min_distance)
				{    //compare between the relative angle of the current foe and the previos foe
					 //with the same distance from the friend ship
					if(RelativeAngle(current_foe->entry->direction,submarine_direction) < RelativeAngle(current_friend->entry->most_threatening_foe->direction, submarine_direction))
						{
							//update the threated foe if his relative angle is smaller than the previous
							current_friend->entry->most_threatening_foe = current_foe->entry;
					    }
				}
				current_foe = current_foe->next;
			}
			current_friend = current_friend->next;
	    }
	radar->is_friends_list_updated = TRUE; //after scanning all the friends, the list is updated.
	return TRUE;
}

//Computing the oclide distance using the distance from the radar and the absolute direction from north
double OclideDistance ( unsigned int dist1, unsigned int dir1, unsigned int dist2, unsigned int dir2)
{
	double alpha = 0;
	double oclide_distance = 0;
	alpha = (double)RelativeAngle (dir1, dir2);
	//computing the oclide distance using the cosinos statement. 
	oclide_distance = 2*dist1*dist2*cos(alpha);
	oclide_distance = sqrt (dist1*dist1+dist2*dist2-oclide_distance);
	return (oclide_distance);
}
unsigned int RelativeAngle (unsigned int absolute_dir1, unsigned int absolute_dir2) 
{
	unsigned int rel_angle = 0;
	(absolute_dir1>absolute_dir2)?(rel_angle=(absolute_dir1-absolute_dir2)):(rel_angle=(absolute_dir2-absolute_dir1));
	if (rel_angle > 180)
	{
		rel_angle = 360 - rel_angle;
	}
	return rel_angle;
}

BOOL IsSubmarineThreatened(
	Radar *radar
)
{
	RadarListNode * radar_current_node;
	if (radar == NULL || radar->friends == NULL || radar->foes == NULL)
	{
      LOG_ERROR("failed to calculate threats: Radar picture is empty");
	  return FALSE;
	}
	radar_current_node=(RadarListNode *)radar->foes->head;
	// As long we didn't reach the end of the list
	//we walk through the foe list and search for threating sheep
	while (radar_current_node != NULL)
	{
		if (radar_current_node->entry->distance<500) //if there is a sheep in distance less than 500, we return TRUE
		{
			return TRUE;
		}
		radar_current_node=radar_current_node->next;
	}
	return FALSE;
}
BOOL IsThereFoes (
Radar *radar
)
{
	if (radar == NULL || radar->friends == NULL || radar->foes == NULL)
	{
      LOG_ERROR("failed to calculate threats: Radar picture is empty");
	  return FALSE;
	}
	if (radar->foes->head == NULL)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL EliminateFoe(
	Radar *radar, //A pointer to radar picture containing the foes and friends lists.
	RadarObject *foe //A pointer to a foe need to eliminate.
)
{
	RadarListNode *current_foe_node=NULL;
	if (radar == NULL || radar->foes == NULL)
	{
		LOG_ERROR("couldn't eliminate foe due to empty radar image");
		return FALSE;
	}
	if (foe == NULL)
	{
		LOG_WARN("couldn't eliminate foe, no foe received");
	    return FALSE;
	}
	current_foe_node = (RadarListNode *)radar->foes->head;
	while (current_foe_node != NULL)
	{
		if (current_foe_node->entry == foe)
		{
			DeleteLinkedListEntry(radar->foes,foe);//eliminate the node from the list memory and free the memory
			free (foe->name); //free the memory holding the name of the foe (string)
			free (foe);
			radar->is_friends_list_updated=FALSE;//After eliminating the foe, the list of the friends is not updated because the foe is not potential threat for the friends sheeps anymore.
			return TRUE;
		}
		current_foe_node=current_foe_node->next;	
	}
	LOG_WARN("couldn't find the foe");
	return FALSE;
}

BOOL GetSubmarineFriends(
	Radar *radar, 
	RadarObjectLinkedList **friends
)
{
	if(radar == NULL)
	{
		LOG_ERROR("couldn't get friends list, radar image not initialized");
		return FALSE;
	}
	if (radar->is_friends_list_updated=FALSE)
	{
		LOG_WARN ("sumbrine friend list not updated!");
		return FALSE;
	}
	*friends = (RadarObjectLinkedList *)radar->friends; //casting the list type needed;
	return TRUE;
}


BOOL FreeRadar(
	Radar *radar
)
{
	LinkedList *current_list = NULL;
	if (radar == NULL)
	{
		LOG_ERROR("radar pointer is not valid");
	}
	current_list = (LinkedList *)radar->foes;
		if (FreeRadarList(current_list) == FALSE)
	{
		LOG_ERROR("problem occur while trying to free foes list");
		return FALSE;
	}
	current_list = (LinkedList *)radar->friends;
	if (FreeRadarList(current_list) == FALSE)
	{
		LOG_ERROR("problem occur while trying to free friends list");
		return FALSE;
	}
	free (radar);
	return TRUE;
}

BOOL FreeRadarList (LinkedList * list)
{
	RadarListNode *current_radar_object = NULL;
	if (list != NULL)
	{
		current_radar_object= (RadarListNode *)list->head;
		while (current_radar_object != NULL)
		{                 
			free (current_radar_object->entry->name); //free the memory malloc for the string holsing the name
			free (current_radar_object->entry);   //free each radar object in the list
			current_radar_object=current_radar_object->next;
		}
		if (FreeLinkedList(list) == FALSE) //free the list itself
		{
			return FALSE;
		}
	}
	return TRUE;
}