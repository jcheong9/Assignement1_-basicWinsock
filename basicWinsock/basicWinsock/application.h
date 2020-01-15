#pragma once
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: WinMain.h - contains the declaration functions
--							and libraries for this project
--
--
--
-- PROGRAM: DumbTerminalEmulation
--
-- DATE: September 21, 2019
--
-- REVISIONS:
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- NOTES:
-- This is a header file contains declaration functions and libraries.
----------------------------------------------------------------------------------------------------------------------*/
#pragma once
#pragma warning (disable: 4096)
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

#define IDM_HELP						100
#define ID_LOOKUP_HOSTNAME				101
#define ID_LOOKUP_SERVICENAME			102
#define ID_LOOKUP_PORTNUMBER			103
#define IDM_EXIT						105


//Declaration Functions application
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void PrintToScreen(WPARAM wParam);


typedef struct
{
	HWND hwnd;

	int selection = 0;
} PORTPARMA;
