#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[오류 발생]: " << message << '\n';
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData; // Winsock API 구조체
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876; // 포트 번호
	char received[256]; //문자열 버퍼 크기

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock을 초기화 합니다. (2.2버전), startup된 결과를 winsockAPI 데이터에서 확인할 수 있음(wsaData), 성공 = 0 반환
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓을 생성. socket(IP v4, TCP, 0)

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); // serverAddress를 전부 0으로 초기화
	serverAddress.sin_family = AF_INET; // sin_family: 어떤 체계를 사용할 것인지. PF_INTE = AF_INET = IP v4
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 서버 본연의 주소로서 자신의 주소를 활용해서 서버를 열겠다.
	// serverAddress는 내부적으로 네트워크 바이트 형식을 포함. 그를 위해서 htonl 사용. (l은 long을 의미) -> 4바이트 정수를 네트워크 바이트 형식으로..
	serverAddress.sin_port = htons(serverPort); // htons : s는 short를 의미 -> 2바이트 정수를 네트워크 바이트 형식으로..
	// 해당 serverSocket으로 바인딩 처리를 해서 실제로 서버프로그램을 실행하고 있는 컴퓨터의 네트워크 인터페이스에 바인딩이 될 수 있도록 처리.
	// 그 바인딩 과정을 위해서 serverAddress가 들어가 있음.
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[현재상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) // 동시에 연결을 요청하는 클라이언트의 수가 최대 5명까지 될 수 있도록 설정
		ShowErrorMessage("listen()");
	cout << "[현재상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress); // clientAddress에 client에 대한 정보가 담김 
	cout << "[현재상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");
	while (1) { // 클라이언트의 메세지를 무한히 계속 받아서 그대로 다시 전달합니다.
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL; // 예기치못한 문자 들어왔을때 대비해서 NULL (?)
		cout << "[클라이언트 메시지]: " << received << '\n';
		cout << "[메시지 전송]: " << received << '\n'; 
		if (strcmp(received, "[exit]") == 0) { // 사용자가 보낸 문자가 exit이라면, 서버종료
			send(clientSocket, received, sizeof(received) - 1, 0); // client가 메세지 보낼때 같이딸려온 enter 공백문자를 -1로 처리, 받은 메세지를 그대로 client에게 다시 보냄
			cout << "[서버종료]\n"; 
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0); // 받은 메세지를 그대로 client에게 다시 보냄 (=echo)
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;

}