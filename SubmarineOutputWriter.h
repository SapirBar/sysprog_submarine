#ifndef SUBMARINE_OUTPUT_WRITE_H
#define SUBMARINE_OUTPUT_WRITE_H

#include <stdio.h>
#include "common.h"
#include "Submarine.h"

typedef struct {
	FILE *output_file;
} SubmarineOutputWriter;

SubmarineOutputWriter * InitializeSubmarineOutputWriter(
	const char *output_file_name
);

BOOL AddNewRound(
	SubmarineOutputWriter *output_writer
);

BOOL WriteNewCommand(
	SubmarineOutputWriter *output_writer, 
	SubmarineCommand *command
);

BOOL WriteWarningMessage(
	SubmarineOutputWriter *output_writer, 
	char *threatened_friend
);

BOOL CloseSubmarineOutputWriter(
	SubmarineOutputWriter *output_writer
);

#endif