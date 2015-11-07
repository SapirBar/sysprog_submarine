#include <stdlib.h>
#include <stdio.h>
#include "Radar.h"
#include "Submarine.h"
#include "SubmarineOutputWriter.h"
#include "common.h"
#include "Hw1TextFileReader.h"

#define WORDS_IN_INPUT_LINE (4)
#define INPUT_PARAMETERS_NUM (5)

typedef enum {
	INPUT_FILE_SIGNATURE_OFFSET = 0,
	INPUT_FILE_SHIP_TYPE_OFFSET,
	INPUT_FILE_DIRECTION_OFFSET,
	INPUT_FILE_DISTANCE_OFFSET
} InputLineOffset;

typedef enum {
	CMD_PARAMETER_INPUT_FILE_OFFSET = 0,
	CMD_PARAMETER_INIT_DIRECTION_OFFSET,
	CMD_PARAMETER_INIT_DEPTH_OFFSET,
	CMD_PARAMETER_INIT_AMMO_OFFSET,
	CMD_PARAMETER_OUTPUT_FILE_OFFSET,
} CmdParameter;

/* Reads the parametes and sets their values in the corresponding parameters */
BOOL HandleParameters(
	int argc,
	char *argv[],
	char **input_file,
	char **output_file,
	int *initial_direction,
	int *initial_depth,
	int *initial_ammo
) {
	int atoi_result;

	if (argc != INPUT_PARAMETERS_NUM)
	{
		LOG_ERROR("Wrong parameters given in cmd");
		LOG_ERROR("Usage: Submarine.exe <input_file> <initial_direction> <initial_depth> <initial_ammo> <output_file>");
		return FALSE;
	}

	*input_file = argv[CMD_PARAMETER_INPUT_FILE_OFFSET];
	*output_file = argv[CMD_PARAMETER_OUTPUT_FILE_OFFSET];
	
	atoi_result = atoi(argv[CMD_PARAMETER_INIT_DIRECTION_OFFSET]);
	if ((errno == ERANGE) || (errno == EINVAL) || (atoi_result < 0))
	{
		LOG_ERROR("Wrong intial direction parameter");
		return FALSE;
	}
	*initial_direction = atoi_result;

	atoi_result = atoi(argv[CMD_PARAMETER_INIT_DEPTH_OFFSET]);
	if ((errno == ERANGE) || (errno == EINVAL) || (atoi_result < 0))
	{
		LOG_ERROR("Wrong intial depth parameter");
		return FALSE;
	}
	*initial_depth = atoi_result;
	
	atoi_result = atoi(argv[CMD_PARAMETER_INIT_AMMO_OFFSET]);
	if ((errno == ERANGE) || (errno == EINVAL) || (atoi_result < 0))
	{
		LOG_ERROR("Wrong intial ammo parameter");
		return FALSE;
	}
	*initial_ammo = atoi_result;

	return TRUE;
}

BOOL IsEmptyLine(TextFileReader reader, int line_number)
{
	return (reader.WordsInLine[line_number] == 0);
}

BOOL HandleInputFileLine(
	ConstWord *words_arr,
	int words_no_in_line,
	char **signature,
	RadarObjectType *type,
	unsigned int *direction,
	unsigned int *distance
)
{
	int word_index = 0;
	int atoi_result = 0;

	if (words_no_in_line != WORDS_IN_INPUT_LINE)
	{
		LOG_ERROR("Wrong number of words in line");
		return FALSE;
	}

	*signature = (char *)words_arr[INPUT_FILE_SIGNATURE_OFFSET];
	
	if (strcmp(words_arr[INPUT_FILE_SHIP_TYPE_OFFSET], "FRIEND") == 0)
	{
		*type = FRIEND;
	} else if (strcmp(words_arr[INPUT_FILE_SHIP_TYPE_OFFSET], "FOE") == 0)
	{
		*type = FOE;
	} else {
		LOG_ERROR("Wrong ship type (not FRIEND/FOE)");
		return FALSE;
	}

	atoi_result = atoi(words_arr[INPUT_FILE_DIRECTION_OFFSET]);
	if ((errno == ERANGE) || (errno == EINVAL) || (atoi_result < 0))
	{
		LOG_ERROR("Wrong direction value");
		return FALSE;
	}
	*direction = atoi_result;

	atoi_result = atoi(words_arr[INPUT_FILE_DISTANCE_OFFSET]);
	if ((errno == ERANGE) || (errno == EINVAL) || (atoi_result < 0))
	{
		LOG_ERROR("Wrong dstance value");
		return FALSE;
	}
	*distance = atoi_result;
	
	return TRUE;
}

BOOL RunSimulation(Submarine *submarine, TextFileReader reader)
{
	int line_index = 0;
	int word_index = 0;
	BOOL is_new_batch = TRUE;
	Radar *radar = NULL;
	char *signature = 0;
	RadarObjectType type;
	unsigned int direction = 0;
	unsigned int distance = 0;
	BOOL result = FALSE;

	for (line_index=0; line_index < reader.NumOfLines; line_index++)
	{
		if (IsEmptyLine(reader, line_index))
		{
			if (!is_new_batch)
			{
				if (!HandleRadarPicture(submarine, radar))
				{
					LOG_ERROR("Submarine failed to handle radar picture");
					goto cleanup;
				}
				if (radar != NULL)
				{
					if (!FreeRadar(radar))
					{
						LOG_ERROR("Failed to free radar");
						goto cleanup;
					}
					radar = NULL;
				}
				is_new_batch = TRUE;
			}
			continue;
		}

		// If this is not an empty line and is_new_batch == TRUE, then we 
		// should initialize a new Radar
		if (is_new_batch)
		{
			radar = InitializeRadar();
			if (radar == NULL)
			{
				LOG_ERROR("Failed to initialize radar");
				goto cleanup;
			}
		}
		
		if (!HandleInputFileLine(
				reader.WordsArr[line_index], 
				reader.WordsInLine[line_index],
				&signature,
				&type,
				&direction,
				&distance
			)
		) {
			LOG_ERROR("Failed to read line number %d", line_index);
			goto cleanup;
		}

		if (!AddRadarObject(radar, signature, type, direction, distance))
		{
			LOG_ERROR("Failed to add object to radar");
			goto cleanup;
		}

	}

	result = TRUE;

cleanup:
	if (radar != NULL)
	{
		FreeRadar(radar);
		radar = NULL;
	}

	return result;

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
	SubmarineOutputWriter *output_writer = NULL;
	BOOL simulation_result = FALSE;
	ErrorCode error_code = SUCCESS;

	if (!HandleParameters(
		argc,
		argv,
		&input_file,
		&output_file,
		&initial_direction,
		&initial_depth,
		&initial_ammo
	))
	{
		LOG_ERROR("Failed to parse the cmd parameters");
		error_code = WRONG_PARAMETERS;
		goto cleanup;
	};

	LOG_INFO("Started Submarine Run with the following parameters: \r\n \
			 Input File = %s \r\n \
			 Initial Direction = %d \r\n \
			 Initial Depth = %d \r\n \
			 Initial Ammo = %d \r\n \
			 Output File = %s",
			 input_file,
			 initial_direction,
			 initial_depth,
			 initial_ammo,
			 output_file
	);

	// Create the input file reader
	reader = CreateTextFileReader(input_file);
	if (reader.IsInitialized == FALSE)
	{
		LOG_ERROR("Failed to initialize the Text Reader");
		error_code = TEXT_READER_INITIALIZATION_FAILED;
		goto cleanup;
	}

	// Create the output file writer
	output_writer = InitializeSubmarineOutputWriter(output_file);
	if (output_writer == NULL)
	{
		LOG_ERROR("Failed to initialize the Submarine Output Writer");
		error_code = SUBMARINE_OUTPUT_WRITER_FAILED;
		goto cleanup;
	}

	// Initialize the Submarine
	submarine = InitializeSubmarine(
		initial_depth, 
		initial_direction, 
		initial_ammo,
		output_writer
	);
	if (submarine == NULL) 
	{
		LOG_ERROR("Failed to initialize the submarine");
		error_code = SUBMARINE_INITIALIZATION_FAILED;
		goto cleanup;
	}

	simulation_result = RunSimulation(submarine, reader);
	if (!simulation_result)
	{
		LOG_ERROR("Failed during the simulation");
		error_code = SIMULATION_FAILED;
	}

cleanup:
	// Cleanup
	if (submarine != NULL)
	{
		FreeSubmarine(submarine);
	}
	if (output_writer != NULL)
	{
		CloseSubmarineOutputWriter(output_writer);
	}
	if (reader.IsInitialized != FALSE)
	{
		DeleteTextFileReader(reader);
	}

	return SUCCESS;
}
