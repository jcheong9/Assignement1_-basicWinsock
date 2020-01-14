/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: winmain.cpp -	An application that creates a window and process window messages
--								that define the behavior of dumb terminal.
--
--
-- PROGRAM: DumbTerminalEmulation
--
-- FUNCTIONS:
--				WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
--						LPSTR lspszCmdParam, int nCmdShow)
--				LRESULT CALLBACK WndProc(HWND hwnd, UINT Message,
--						WPARAM wParam, LPARAM lParam)
--				void PrintToScreen(WPARAM wParam)
--				DWORD WINAPI MonitorInputThread(LPVOID hwnd)
--
-- DATE: September 21, 2019
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- NOTES:
-- This is a minimal terminator emulator that performs basic functions of
-- connection to a serial port and transmitting or receiving characters from the port.
-- Also, it provides user interface for user to set port settings and see displayed transmitted char.
----------------------------------------------------------------------------------------------------------------------*/
#pragma once

#include <stdio.h>
#include <string>
#include <Windows.h>
#include <windows.h>
#include "application.h"
void prepWindow(HINSTANCE hInst);
//Textbox handlers for send and receive
HWND textHwnd;
HWND textHwndRx;

//Handlers for the tables for send and receive
HWND hWndListView;
HWND hWndListViewRx;
HWND hInput1;

BOOL comPortSet = FALSE;
PORTPARMA portparma;
HDC hdc;

static unsigned k = 0;
static TCHAR Name[] = TEXT("Dumb Terminal");
char str[255];	//output buffer
int X = 0, Y = 0; // Current coordinates


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WinMain
--
-- DATE: September 21, 2019
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: int WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
--					LPSTR lspszCmdParam, int nCmdShow)
--
-- RETURNS: int
--
-- NOTES:
-- This function creates window and the user interface.
--
----------------------------------------------------------------------------------------------------------------------*/
int WINAPI wWinMain(_In_ HINSTANCE hInst,_In_opt_ HINSTANCE hprevInstance,
	_In_ LPWSTR lspszCmdParam, _In_ int nCmdShow)
{
	MSG Msg;
	WNDCLASSEX Wcl;
	HBRUSH startBackGroundColor = CreateSolidBrush(RGB(255, 255, 255));
	// Define a Window class
	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = 0; // default style
	Wcl.hIcon = LoadIcon(NULL, (LPCSTR)IDI_APPLICATION); // large icon 
	Wcl.hIconSm = NULL; // use small version of large icon
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);  // cursor style

	Wcl.lpfnWndProc = WndProc; // window function
	Wcl.hInstance = hInst; // handle to this instance
	Wcl.hbrBackground = startBackGroundColor;
	Wcl.lpszClassName = Name; // window class name

	Wcl.lpszMenuName = TEXT("MYMENU");
	Wcl.cbClsExtra = 0;      // no extra memory needed
	Wcl.cbWndExtra = 0;

	//intialize readThread = 0;
	portparma.readThread = 0;

	// Register the class
	if (!RegisterClassEx(&Wcl))
		return 0;

	portparma.hwnd = CreateWindow(Name, Name, WS_OVERLAPPEDWINDOW, 10, 10,
		600, 400, NULL, NULL, hInst, NULL);

	prepWindow(hInst);

	ShowWindow(portparma.hwnd, nCmdShow);
	UpdateWindow(portparma.hwnd);
	void WINAPI ThreadFuc(HWND hwnd, LPVOID n); //second thread
	// Create the message loop
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg); // translate keybpard messages
		DispatchMessage(&Msg); // dispatch message and return control to windows
	}

	return Msg.wParam;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WndProc
--
-- DATE: September 21, 2019
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: LRESULT CALLBACK WndProc(HWND hwnd, UINT Message,
--						WPARAM wParam, LPARAM lParam)
--
-- RETURNS: LRESULT
--
-- NOTES:
-- This function process windows messages and define the behavior of the dumb terminal.
--
----------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message,
	WPARAM wParam, LPARAM lParam)
{

	char charByte[2];
	hdc = GetDC(hwnd);
	TCHAR spCOM1[] = TEXT("COM1");
	TCHAR spCOM2[] = TEXT("COM2");
	TCHAR spCOM3[] = TEXT("COM3");


	switch (Message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		}

		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:

			break;
		}
		break;
	case WM_CHAR:	// Process keystroke

		break;
	case WM_DESTROY:		// message to terminate the program
		PostQuitMessage(0);
		break;
	default: // Let Win32 process all other messages
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: PrintToScreen
--
-- DATE: Spetember 24, 2019
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: void printToScreen(char readBuffer[])
--
-- RETURNS: void.
--
-- NOTES:
-- This function prints characters on the screen.
----------------------------------------------------------------------------------------------------------------------*/

void PrintToScreen(WPARAM wParam) {

	//SIZE lastCharWidth;
	////hdc = GetDC(portparma.hwnd);
	//RECT windowDimension;
	//char strKey[2];

	////finds width of the window
	//GetWindowRect(portparma.hwnd, &windowDimension);
	//int windowWidth = windowDimension.right - windowDimension.left;

	////get the width of the last character read
	//sprintf_s(strKey, "%c", (char)wParam);
	//GetTextExtentPoint32(hdc, strKey, 1, &lastCharWidth);

	////print the characters
	//SetBkMode(hdc, TRANSPARENT);
	//TextOut(hdc, X, Y, strKey, 1);

	////adds the screen paint coordinates by the amount of the last char width
	//X += lastCharWidth.cx;

	////if the displayed characters exceed window width, jump to the next line
	//if (X > windowWidth - 20) {
	//	X = 0;
	//	Y += 15;
	//}
	//ReleaseDC(portparma.hwnd, hdc); // Release device context
	
	/*
	// Set the text in the edit control
	SetWindowText(textHwnd, newBuffer);
	*/
}

void prepWindow(HINSTANCE hInst) {
	/*
	Send section
	*/

	HWND textHwndLabel = CreateWindow("STATIC", "Send",
		WS_VISIBLE | WS_CHILD | SS_LEFT | ES_READONLY,
		0, 0, 550, 20, portparma.hwnd, NULL, hInst, NULL);

	textHwnd = CreateWindow("EDIT", "",
		WS_VISIBLE | WS_CHILD | SS_LEFT | ES_MULTILINE | WS_VSCROLL | ES_READONLY,
		30, 150, 525, 175, portparma.hwnd, NULL, hInst, NULL);

	hInput1 = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
		125 , 25 , 350, 25, portparma.hwnd, NULL, NULL, NULL);

}
