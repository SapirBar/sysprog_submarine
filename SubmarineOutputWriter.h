#ifndef SUBMARINE_OUTPUT_WRITE_H
#define SUBMARINE_OUTPUT_WRITE_H

#include <stdio.h>
#include "common.h"
#include "Submarine.h"

typedef struct {
	FILE *output_file;
} SubmarineOutputWriter;

BOOL InitializeSubmarineOutputWriter(char *output_file_name, SubmarineOutputWriter **output_writer);

void AddNewRound(SubmarineOutputWriter *output_writer);

void WriteNewCommand(SubmarineOutputWriter *output_writer, SubmarineCommand *command);

void WriteWarningMessage(SubmarineOutputWriter *output_writer, char *threatened_friend);

#endif