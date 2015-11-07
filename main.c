#include <stdlib.h>
#include <stdio.h>
#include "Radar.h"
#include "Submarine.h"
#include "SubmarineOutputWriter.h"
#include "common.h"
#include "Hw1TextFileReader.h"

/* Reads the parametes and sets their values in the corresponding parameters */
BOOL HandleParameters(
	int argc,
	char *argv[],
	char **input_file,
	char **output_file,
	int *initial_direction,
	int *initial_depth,
	int *initial_ammo
);

BOOL IsEmptyLine(TextFileReader reader, int line_number)
{
	return (reader.WordsInLine[line_number] == 0);
}

BOOL RunSimulation(Submarine *submarine, TextFileReader reader)
{
	
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

	return TRUE;

}

int main(int argc, char *argv[])
{
	char *input_file = NULL;
	char *output_file = NULL;
	int initial_direction = 0;
	int initial_depth = 0;
	int initial_ammo = 0;
	Submarine *submarine = NULL;
	TextFileReader reader;
	SubmarineOutputWriter *output_writer;
	BOOL simulation_result = FALSE;

	HandleParameters(
		argc,
		argv,
		&input_file,
		&output_file,
		&initial_direction,
		&initial_depth,
		&initial_ammo
	);

	LOG_INFO("Started Submarine Run with the following parameters: \r\n \
			 Input File = %s \r\n \
			 Initial Direction = %d \r\n \
			 Initial Depth = %d \r\n \
			 Initial Ammo = %d \r\n \
			 Output File = %s"
	);

	// Initialize the Submarine
	submarine = InitializeSubmarine(initial_depth, initial_direction, initial_ammo);
	if (submarine == NULL) 
	{
		LOG_ERROR("Failed to initialize the submarine");
		exit(SUBMARINE_INITIALIZATION_FAILED);
	}

	// Create the input file reader
	reader = CreateTextFileReader(input_file);
	if (reader.IsInitialized == FALSE)
	{
		LOG_ERROR("Failed to initialize the Text Reader");
		FreeSubmarine(submarine);
		exit(TEXT_READER_INITIALIZATION_FAILED);
	}

	// Create the output file writer
	output_writer = InitializeSubmarineOutputWriter(output_file);
	if (output_writer == NULL)
	{
		LOG_ERROR("Failed to initialize the Submarine Output Writer");
		DeleteTextFileReader(reader);
		FreeSubmarine(submarine);
		exit(SUBMARINE_OUTPUT_WRITER_FAILED);
	}

	simulation_result = RunSimulation(submarine, reader);
	

	// Free Submarine
		// Free AlreadySeenFriends
	// Delete parsing object (maybe could be done at the start of the program??)

	// Cleanup
	DeleteTextFileReader(reader);
	FreeSubmarine(submarine);

	return SUCCESS;
}
