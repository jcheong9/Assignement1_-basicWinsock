#include "application.h"
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: name_address.cpp -	A simple program get the IP address and host name
--									using the name of the web address or the IP.
--
--
-- PROGRAM: Basic Window Socket Application
--
-- FUNCTIONS:
--				int nameAddr(TCHAR* ip, HWND textHwnd)
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
-- This simple  program  will  get  the  canonical  name,
-- aliases,  and  '.'  separated  Internet IP addresses for a
-- given destination host using the window socket. 
-- 
----------------------------------------------------------------------------------------------------------------------*/


int nameAddr(TCHAR* ip, HWND textHwnd) {
	int		a;
	struct	hostent* hp;
	struct	in_addr my_addr, * addr_p;
	char** p;
	char	ip_address[256];      // String for IP address
	char	host_name[256];      // String for IP address
	char buff[256];
	TCHAR str[256];

	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	LPCWSTR strMessage;

	// Open up a Winsock v2.2 session
	WSAStartup(wVersionRequested, &wsaData);


	addr_p = (struct in_addr*)malloc(sizeof(struct in_addr));
	addr_p = &my_addr;

	//convert ip TCHAR in to ip_address
	wcstombs(ip_address, ip, 256);

	if (isdigit(*ip_address))	// Dotted IP?
	{
		if ((a = inet_addr(ip_address)) == 0)
		{

			SetWindowText(textHwnd, L"IP Address must be of the form x.x.x.x\n");
			return 1;
		}

		addr_p->s_addr = inet_addr(ip_address);


		hp = gethostbyaddr((char*)addr_p, PF_INET, sizeof(my_addr));

		if (hp == NULL)
		{
			sprintf(buff, "Host information is not found\n");
			mbstowcs(str, buff, 256);
			SetWindowText(textHwnd, str);
			return 1;
		}
	}
	else
	{
		if ((hp = gethostbyname(ip_address)) == NULL) 	// Host name?
		{
			switch (h_errno)
			{
			case HOST_NOT_FOUND:
				
				sprintf(buff, "No such host %s\n", ip_address);
				break;
			case TRY_AGAIN:
				sprintf(buff,  "host %s try again later\n", ip_address);
				break;
			case NO_RECOVERY:
				sprintf(buff, "host %s DNS Error\n", ip_address);
				break;

			case NO_ADDRESS:
				sprintf(buff, "No IP Address for %s\n", ip_address);
				break;

			default:
				sprintf(buff, "Unknown Error:  %d\n", h_errno);
				break;
			}
			mbstowcs(str, buff, 256);
			SetWindowText(textHwnd, str);
			return 1;
		}
	}

	for (p = hp->h_addr_list; *p != 0; p++)
	{
		struct in_addr in;
		char** q;
		TCHAR* temp;

		memcpy(&in.s_addr, *p, sizeof(in.s_addr));
		sprintf(buff, "IP Address: %s\t Host Name: %s\n", inet_ntoa(in), hp->h_name);
		mbstowcs(str, buff, 256);
		SetWindowText(textHwnd, str);
		for (q = hp->h_aliases; *q != 0; q++) {
			sprintf(buff, "\r--->   Aliases: %s\n", *q);
			mbstowcs(str, buff, 256);
			int left, right;
			int len = GetWindowTextLength(textHwnd);
			SendMessage(textHwnd, EM_GETSEL, (WPARAM)&left, (LPARAM)&right);
			SendMessage(textHwnd, EM_SETSEL, len, len);
			SendMessage(textHwnd, EM_REPLACESEL, TRUE, (LPARAM)str);
			SendMessage(textHwnd, EM_SETSEL, left, right);

		}
		
	}

	WSACleanup();
	return 0;
}