#include "application.h"
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: service_port.cpp -	A simple program to resolve a service name to 
--									its port number. 
--
--
-- PROGRAM: Basic Window Socket Application
--
-- FUNCTIONS:
--				int service_port(TCHAR* serivce, TCHAR* protocol, HWND textHwnd)
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
-- This simple  program  will  take a user specified service name and protocol
-- and resolve it into its port number using window socket. 
----------------------------------------------------------------------------------------------------------------------*/
int service_port(TCHAR* serivce, TCHAR* protocol, HWND textHwnd, int lengthInput2) {
	struct servent* sv;
	char serviceName[256];
	char protocalUsed[256];
	char buff[256];
	TCHAR str[256];

	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;

	if (lengthInput2 == 0) {
		SetWindowText(textHwnd, L"Invalid input");
		return 1;
	}

	// Open up a Winsock v2.2 session
	WSAStartup(wVersionRequested, &wsaData);


	//convert ip TCHAR in to ip_address
	wcstombs(serviceName, serivce, 256);
	wcstombs(protocalUsed, protocol, 256);

	sv = getservbyname(serviceName, protocalUsed);
	if (sv == NULL)
	{
		sprintf(buff, "Error in getservbyname\n");
		mbstowcs(str, buff, 256);
		SetWindowText(textHwnd, str);
		return 1;
	}
	sprintf(buff,"The port number for %s is: %d\n", serviceName, ntohs(sv->s_port));
	mbstowcs(str, buff, 256);
	SetWindowText(textHwnd, str);

	WSACleanup();
	return 0;
}