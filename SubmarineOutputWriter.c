#include <stdio.h>
#include <stdlib.h>
#include "SubmarineOutputWriter.h"

static char* FIRE_COMMANDS_STRINGS[2] = {"FIRE", "CEASE"};

SubmarineOutputWriter *InitializeSubmarineOutputWriter(
	const char *output_file_name
) {
	SubmarineOutputWriter *output_writer = NULL;
	errno_t err = -1;

	if (output_file_name == NULL)
	{
		LOG_ERROR("Wrong parameters");
		return NULL;
	}
	
	output_writer = (SubmarineOutputWriter *)malloc(sizeof(*output_writer));
	if (output_writer == NULL)
	{
		LOG_ERROR("Failed to allocate memory");
		return NULL;
	}
	
	err = fopen_s(&output_writer->output_file, output_file_name, "w");
	if (err != 0)
	{
		LOG_ERROR("Failed to open the output file");
		free(output_writer);
		return NULL;
	}

	return output_writer;
}

BOOL AddNewRound(
	SubmarineOutputWriter *output_writer
) {
	if (output_writer == NULL) 
	{
		LOG_ERROR("Wrong parameters");
		return FALSE;
	}

	fprintf(output_writer->output_file, "\r\n");
	return TRUE;
}

BOOL WriteNewCommand(
	SubmarineOutputWriter *output_writer, 
	SubmarineCommand *command
) {
	if ((output_writer == NULL) || (command == NULL))
	{
		LOG_ERROR("Wrong parameters");
		return FALSE;
	}

	fprintf(
		output_writer->output_file,
		"%d %d %s %d\r\n",
		command->new_direction,
		command->new_depth,
		FIRE_COMMANDS_STRINGS[command->fire_command],
		command->new_ammo
	);
	return TRUE;
}

BOOL WriteWarningMessage(
	SubmarineOutputWriter *output_writer, 
	char *threatened_friend
) {
	if ((output_writer == NULL) || (threatened_friend == NULL))
	{
		LOG_ERROR("Wrong parameters");
		return FALSE;
	}

	fprintf(
		output_writer->output_file,
		"Ship %s is in danger\r\n",
		threatened_friend
	);
	return TRUE;
}

BOOL CloseSubmarineOutputWriter(
	SubmarineOutputWriter *output_writer
) {
	if (output_writer == NULL)
	{
		LOG_ERROR("Wrong parameters");
		return FALSE;
	}
	fclose(output_writer->output_file);
	free(output_writer);

	return TRUE;
}
