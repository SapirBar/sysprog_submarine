
#include <stdlib.h>
#include "Radar.h"
#include "LinkedList.h"
#include <string.h>
#include "common.h"
BOOL FreeRadarList (LinkedList * list);

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
	if (direction < 0)
	{
		LOG_ERROR("illegal value of direction for sheep %s ", &name);
		return FALSE; 
	}
		if (distance < 0)
	{
		LOG_ERROR("illegal value of distance for sheep %s ", &name);
		return FALSE; 
	}
	new_radar_object->type = type;
	new_radar_object->name = name;
	new_radar_object->direction = direction;
	new_radar_object->distance = distance;
	new_radar_object->most_threatening_foe = NULL;
	new_radar_object->threat_distance = INVALID_DISTANCE;
	if (type == FOE) 
	{
		if (AddLinkedListEntry(radar->foes,new_radar_object) == FALSE)
		{
			LOG_ERROR("failed to add a foe to the list");
			free (new_radar_object);     //free the memory allocated in this function and return
			return FALSE;
		}
	}
	else //type is FRIEND 
	{
		if (AddLinkedListEntry(radar->friends,new_radar_object) == FALSE)
		{
			LOG_ERROR("failed to add a friend to the list");
			free (new_radar_object);     //free the memory allocated in this function and return
			return FALSE;
		}
	}
return TRUE;
}



BOOL CalculateThreats(
	Radar *radar,
	unsigned int submarine_direction //if there are two treating sheeps in the same priority, we need to choose the sheep which her relative direction from the submrine is minimal.
)
{
	RadarListNode *current_friend = NULL, *current_foe = NULL;
	if (radar == NULL || radar->friends == NULL || radar->foes == NULL)
	{
		LOG_ERROR ("radar picture is empty");
		return FALSE;
	}
	if (radar->friends
	radar->is_friends_list_updated = TRUE;
	return TRUE;
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