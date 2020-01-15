#include "application.h"
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: port_service.cpp -	A simple program to resolve a port number to 
--									its service name. 
--
--
-- PROGRAM: Basic Window Socket Application
--
-- FUNCTIONS:
--				int port_service(TCHAR* port, TCHAR* protocol, HWND textHwnd)
--
-- DATE: January 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- NOTES:
-- This simple  program  will  take a user specified port number nd protocol
-- and resolve it into its service name using window socket. 
----------------------------------------------------------------------------------------------------------------------*/

int port_service(TCHAR* port, TCHAR* protocol, HWND textHwnd, int lengthInput2) {
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

	if (lengthInput2 == 0) {
		SetWindowText(textHwnd, L"Invalid input");
		return 1;
	}
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