/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: winmain.cpp -	An application that creates a window and process window messages
--								that define the behavior of dumb terminal.
--
--
-- PROGRAM: Basic Window Socket Application
--
-- FUNCTIONS:
--				WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
--						LPSTR lspszCmdParam, int nCmdShow)
--				LRESULT CALLBACK WndProc(HWND hwnd, UINT Message,
--						WPARAM wParam, LPARAM lParam)
--				void PrintToScreen(WPARAM wParam)
--				DWORD WINAPI MonitorInputThread(LPVOID hwnd)
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
-- This is a minimal terminator emulator that performs basic functions of
-- connection to a serial port and transmitting or receiving characters from the port.
-- Also, it provides user interface for user to set port settings and see displayed transmitted char.
----------------------------------------------------------------------------------------------------------------------*/
#pragma once

#include "application.h"

//Textbox handlers for send and receive
HWND textHwnd;
HWND textHwndRx;

//Handlers for the tables for send and receive
HWND hWndListView;
HWND hWndListViewRx;
HWND hInput1;
HWND hInput2;
HWND hRadioBtn[3];
HWND hwndButton;
HWND textHwndLabel;
HWND textHwndLabel1;
HWND textHwndLabel2;

PORTPARMA portparma;
HDC hdc;

static unsigned k = 0;
static TCHAR Name[] = TEXT("Baic Window Socket");
static HBRUSH startBackGroundColor = CreateSolidBrush(RGB(255, 255, 255));

char str[255];	//output buffer
int X = 0, Y = 0; // Current coordinates


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WinMain
--
-- DATE: January 15, 2020
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
	
	// Define a Window class
	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = 0; // default style
	Wcl.hIcon = LoadIcon(NULL, (LPCWSTR)IDI_APPLICATION); // large icon 
	Wcl.hIconSm = NULL; // use small version of large icon
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);  // cursor style

	Wcl.lpfnWndProc = WndProc; // window function
	Wcl.hInstance = hInst; // handle to this instance
	Wcl.hbrBackground = startBackGroundColor;
	Wcl.lpszClassName = Name; // window class name

	Wcl.lpszMenuName = TEXT("MYMENU");
	Wcl.cbClsExtra = 0;      // no extra memory needed
	Wcl.cbWndExtra = 0;



	// Register the class
	if (!RegisterClassEx(&Wcl))
		return 0;

	portparma.hwnd = CreateWindow(Name, Name, WS_OVERLAPPEDWINDOW, 10, 10,
		600, 400, NULL, NULL, hInst, NULL);

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
-- DATE: January 15, 2020
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
	hdc = GetDC(hwnd);
	TCHAR str[256];
	TCHAR input2Text[256];
	switch (Message)
	{
	case WM_CREATE:
		hwndButton = CreateWindow(L"BUTTON", L"Send", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			455, 115, 100, 20, hwnd, (HMENU)ID_ENTER_BTN, NULL, NULL);

		textHwndLabel = CreateWindow(L"STATIC", L"Find Host and IP",
			WS_CHILD | WS_VISIBLE | SS_CENTER,
			30, 10, 525, 20, hwnd, NULL, NULL, NULL);

		textHwndLabel1 = CreateWindow(L"STATIC", L"Host name or IP",
			WS_VISIBLE | WS_CHILD | SS_LEFT | ES_READONLY,
			30, 45, 150, 20, hwnd, NULL, NULL, NULL);

		textHwnd = CreateWindow(L"EDIT", L"",
			WS_VISIBLE | WS_CHILD | SS_LEFT | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL | ES_READONLY | WS_BORDER,
			30, 150, 525, 175, hwnd, NULL, NULL, NULL);

		hInput1 = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			205, 45, 350, 20, hwnd, NULL, NULL, NULL);

		textHwndLabel2 = CreateWindow(L"STATIC", L"Protocol: ",
			WS_VISIBLE | WS_CHILD | SS_LEFT | ES_READONLY,
			30, 80, 150, 20, hwnd, NULL, NULL, NULL);

		hInput2 = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			205, 80, 350, 20, hwnd, NULL, NULL, NULL);
		ShowWindow(hInput2, SW_HIDE);
		ShowWindow(textHwndLabel2, SW_HIDE);

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_LOOKUP_HOSTNAME:
			portparma.selection = 0;
			ShowWindow(hInput2, SW_HIDE);
			ShowWindow(textHwndLabel2, SW_HIDE);
			SetWindowText(textHwndLabel1, L"Host name or IP");
			SetWindowText(textHwndLabel, L"host name -> IP OR IP -> host name");

			break;
		case ID_LOOKUP_SERVICENAME:
			portparma.selection = 1;
			ShowWindow(textHwndLabel2, SW_RESTORE);
			ShowWindow(hInput2, SW_RESTORE);
			SetWindowText(textHwndLabel1, L"Service: ");
			SetWindowText(textHwndLabel, L"service name/protocol -> port number");

			break;
		case ID_LOOKUP_PORTNUMBER:
			portparma.selection = 2;
			ShowWindow(textHwndLabel2, SW_RESTORE);
			ShowWindow(hInput2, SW_RESTORE);
			SetWindowText(textHwndLabel1, L"Port: ");
			SetWindowText(textHwndLabel, L"port number -> service name/protocol");

			break;
		case ID_EXIT:
			PostQuitMessage(0);
			break;

		case ID_ENTER_BTN:
			GetWindowText(hInput1, str, 256);
			if (GetWindowTextLengthA(hInput1) != 0) {
				switch (portparma.selection) {
					case 1:
						GetWindowText(hInput2, input2Text, 256);
						service_port(str, input2Text, textHwnd);
						break;
					case 2:
						GetWindowText(hInput2, input2Text, 256);
						port_service(str, input2Text, textHwnd);
						break;
					default:
						nameAddr(str, textHwnd);
						break;
				}
			}
			else {
				SetWindowText(textHwnd, L"Invalid input");
			}
			OutputDebugStringW(L"YOOYOYO");
			
			break;
		}

		break;
	case WM_CTLCOLORSTATIC:
		hdc = (HDC)wParam;
		SetBkColor(hdc, RGB(255, 255, 255));
		return (INT_PTR)startBackGroundColor;
	case WM_DESTROY:		// message to terminate the program
		PostQuitMessage(0);
		break;
	default: // Let Win32 process all other messages
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

