#include "application.h"
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

int port_service(TCHAR* port, TCHAR* protocol, HWND textHwnd) {
	struct servent* sv;
	int s_port;
	char portNum[256];
	char protocalUsed[256];
	char buff[256];
	TCHAR str[256];

	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;

	//convert ip TCHAR in to ip_address
	wcstombs(portNum, port, 256);
	wcstombs(protocalUsed, protocol, 256);

	// Open up a Winsock v2.2 session
	WSAStartup(wVersionRequested, &wsaData);

	s_port = atoi(portNum);

	sv = getservbyport(htons(s_port), protocalUsed);
	if (sv == NULL)
	{
		sprintf(buff,"Error in getservbyport\n");
		mbstowcs(str, buff, 256);
		SetWindowText(textHwnd, str);
		return 1;
	}
	sprintf(buff,"The service for %s port %d is: %s\n", protocalUsed, s_port, sv->s_name);
	mbstowcs(str, buff, 256);
	SetWindowText(textHwnd, str);
	WSACleanup();
	return 0;
}