#ifndef SUBMARINE_COMMAND_H
#define SUBMARINE_COMMAND_H

typedef enum {
	FIRE=0,
	CEASE
} SubmarineFireCommand;

typedef struct {
	int new_direction;
	int new_depth;
	SubmarineFireCommand fire_command;
	int new_ammo;
} SubmarineCommand;

#endif
