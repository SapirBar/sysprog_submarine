//Author: Adi Mashiah, ID:305205676\Reut Lev, ID:305259186
//Belongs to project: sysprog_submarine
//Radar model receive parameters about ships in a specific point in time
//and divide them into two diffrent list: foes and friends.
//see RadarObject structure to understand which data saved for each,
// the radar can caculate the friends threats,
//send submarine a list of friends, eliminate foe from it's list
//tell is there are foes or if the submarine is threated.
// using the files: "LinkedList.h","common.h"
#ifndef RADAR_H
#define RADAR_H

#include "LinkedList.h"
#include "common.h"
#define INVALID_DISTANCE (1000)
typedef enum {
	FRIEND=0,
	FOE
} RadarObjectType;

typedef struct RadarObject_s{
	char *name;
	RadarObjectType type;
	unsigned int direction;
	unsigned int distance;
	struct RadarObject_s *most_threatening_foe;
	double threat_distance;
} RadarObject;

typedef struct RadarListNode_s {
	RadarObject *entry;
	struct RadarListNode_s *next;
	struct RadarListNode_s *prev;
} RadarListNode;

typedef struct {
	RadarListNode *head;
	RadarListNode *tail;
} RadarObjectLinkedList;

typedef struct Radar_s {
	LinkedList *friends;
	LinkedList *foes;
	BOOL is_friends_list_updated;
} Radar;

// This function crating a new radar image, containing two list: Friends and foes list, each one is emppty. 
//The return value is a pointer for Radar structure.
Radar *InitializeRadar();

//This function is creating a new object in the radar image, 
//Poping it to the foe list or frinds list according to the type of the object
//Return FALSE in case the function faild to insert the new object
BOOL AddRadarObject(
	Radar *radar,             //Pointer to Radar structure (created by InitializeRadar function)
	char *name,               //The name of the ship
	RadarObjectType type,     //gets: FOE or FRIEND
	unsigned int direction,   //The direction of the ship
	unsigned int distance     //The distance of the ship
);


//Update the radar friends list,
//finding the most threating foe for each friend in the friends list, 
//if there are two treating ships in the same priority,
//we need to choose the ship which her relative direction from the submrine is minimal.
// if there are no foes threating the friend ship, the friend ship field most_threatening_foe will remain NULL 
//and the threat_distance will remain INVALID_DISTANCE
BOOL CalculateThreats(
	Radar *radar,
	unsigned int submarine_direction //  
);

/* Return FALSE in case that the friends list threats is not updated.
 */
BOOL GetSubmarineFriends(
	Radar *radar, 
	RadarObjectLinkedList **friends
);

//Receive a radar picture (list of foe and frineds) and a pointer to the foe object
//remove the foe from the list and free the memory
//if succeeds return true.
BOOL EliminateFoe(
	Radar *radar, //A pointer to radar picture containing the foes and friends lists.
	RadarObject *foe //A pointer to a foe need to eliminate.
);

// The Submarine is threated if there is a ship in distance of less than 500 
//return TRUE-if the submarine is threated by a ship; FALSE- if the submarine is not treated by a ship

BOOL IsSubmarineThreatened(
	Radar *radar //Radar object pointer (contain pointer to the foe list)
);

//return True if there are foes and False if there are no foes.
//returnfalse also when received null pointer (and print a log error)
BOOL AreThereFoes (
	Radar *radar,
	BOOL *are_there_foes
);

//free all memory allocated for the radar picture;
BOOL FreeRadar(
	Radar *radar
);

#endif //RADAR_H
