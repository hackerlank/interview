


#include "stdafx.h"
#include "MMsystem.h"
#include "midi.h"

int InitMidi(void)
{
	int result;

	result = midiConnect(hMidi, hMidiOut, Reserved);

	return result;
}



int PlayMidi(char file_name[])
{
	int result;

	result = midiConnect(hMidi, hMidiOut, Reserved);

	return result;
}

