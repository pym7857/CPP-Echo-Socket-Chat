#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[���� �߻�]: " << message << '\n';
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData; // Winsock API ����ü
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876; // ��Ʈ ��ȣ
	char received[256]; //���ڿ� ���� ũ��

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock�� �ʱ�ȭ �մϴ�. (2.2����), startup�� ����� winsockAPI �����Ϳ��� Ȯ���� �� ����(wsaData), ���� = 0 ��ȯ
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP ������ ����. socket(IP v4, TCP, 0)

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); // serverAddress�� ���� 0���� �ʱ�ȭ
	serverAddress.sin_family = AF_INET; // sin_family: � ü�踦 ����� ������. PF_INTE = AF_INET = IP v4
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // ���� ������ �ּҷμ� �ڽ��� �ּҸ� Ȱ���ؼ� ������ ���ڴ�.
	// serverAddress�� ���������� ��Ʈ��ũ ����Ʈ ������ ����. �׸� ���ؼ� htonl ���. (l�� long�� �ǹ�) -> 4����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������..
	serverAddress.sin_port = htons(serverPort); // htons : s�� short�� �ǹ� -> 2����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������..
	// �ش� serverSocket���� ���ε� ó���� �ؼ� ������ �������α׷��� �����ϰ� �ִ� ��ǻ���� ��Ʈ��ũ �������̽��� ���ε��� �� �� �ֵ��� ó��.
	// �� ���ε� ������ ���ؼ� serverAddress�� �� ����.
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[�������] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) // ���ÿ� ������ ��û�ϴ� Ŭ���̾�Ʈ�� ���� �ִ� 5����� �� �� �ֵ��� ����
		ShowErrorMessage("listen()");
	cout << "[�������] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress); // clientAddress�� client�� ���� ������ ��� 
	cout << "[�������] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");
	while (1) { // Ŭ���̾�Ʈ�� �޼����� ������ ��� �޾Ƽ� �״�� �ٽ� �����մϴ�.
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL; // ����ġ���� ���� �������� ����ؼ� NULL (?)
		cout << "[Ŭ���̾�Ʈ �޽���]: " << received << '\n';
		cout << "[�޽��� ����]: " << received << '\n'; 
		if (strcmp(received, "[exit]") == 0) { // ����ڰ� ���� ���ڰ� exit�̶��, ��������
			send(clientSocket, received, sizeof(received) - 1, 0); // client�� �޼��� ������ ���̵����� enter ���鹮�ڸ� -1�� ó��, ���� �޼����� �״�� client���� �ٽ� ����
			cout << "[��������]\n"; 
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0); // ���� �޼����� �״�� client���� �ٽ� ���� (=echo)
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;

}