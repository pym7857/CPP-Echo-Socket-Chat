#define _WINSOCK_DEPRECATED_NO_WARNINGS // inet_addr 사용하기 위해
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[오류 발생]: " << message << '\n';
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET clientSocket; // 서버와 연결된 자기 자신의 소켓
	SOCKADDR_IN serverAddress; // 서버의 주소를 담기위한 SOCKADDR 구조체

	int serverPort = 9876; // 서버와 포트번호 동일해야됨
	char received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock을 초기화
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓을 생성

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // inet_addr는 내부적으로 문자열을 받음. 문자열 IP를 네트워크 바이트 형식으로
	serverAddress.sin_port = htons(serverPort); // 2바이트 정수 네트워크 바이트 형식으로
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");
	cout << "[현재 상태] connected()\n";

	while (1) { // 반복적으로 서버로 메세지를 전송하고 에코 메세지를 전달 받습니다.
		cout << "[메시지 전송]: ";
		getline(cin, sent);
		if (sent == "") continue; // 어떠한 메세지도 입력하지 않고 엔터만 치는 경우 
		send(clientSocket, sent.c_str(), sent.length(), 0); // string형태인 sent를 c언어의 문자열 버퍼형태로 바꿔줌 (c_str)
		int length = recv(clientSocket, received, sizeof(received), 0); 
		received[length] = '\0'; // 문자열의 마지막에 널(NULL)값을 넣어줌 
		if (strcmp(received, "[exit]") == 0) {
			cout << "[서버 종료]\n";
			break;
		}
		cout << "[서버 메시지]: " << received << '\n';
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}