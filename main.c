#include "Radar.h"
#include "Submarine.h"
#include "common.h"

int main(int argc, char *argv[])
{
	// Parse the input file
	// Initialize Submarine
		// Initialize AlreadySeenFriends

	// For each batch:

		// Initialize new Radar
		// Insert Radar Objects to Radar
		// Run Submarine
			// Ask Radar For Friends List
			// Check if there is any submarine being threatened (me or any friend) and set the command
			// Update Radar Picture (eliminate most threatening enemy)
			// Get from Radar the updated Friends List
			// If there are no enemies then change direction according to the previous friends picture
		// Get Submarine Command (Fire + Direction) & Friend's warnings
		// Free Radar

	// Free Submarine
		// Free AlreadySeenFriends
	// Delete parsing object (maybe could be done at the start of the program??)

	return 0;
}
