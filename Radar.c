//Creating new 
#include <stdlib.h>
#include "Radar.h"
#include "LinkedList.h"
#include <string.h>


Radar *InitializeRadar()
{
Radar * radar_image = NULL;
	radar_image= (Radar *) malloc (sizeof (Radar *));
	if (radar_image == NULL)
	{
		return NULL; 
	}
	memset(radar_image , '\0', sizeof(Radar));
	radar_image->foes = (RadarObjectLinkedList *)InitializeLinkedList();
	if (radar_image->foes == NULL)
	{
		LOG("failed to allocate memory");
		exit(1);
	}
	radar_image->friends = (RadarObjectLinkedList *)InitializeLinkedList();
	if (radar_image->friends == NULL)
	{
		LOG("failed to allocate memory");
		exit(1);
	}
	radar_image->is_friends_list_updated=FALSE;
}