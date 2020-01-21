#define _WINSOCK_DEPRECATED_NO_WARNINGS // inet_addr ����ϱ� ����
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[���� �߻�]: " << message << '\n';
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET clientSocket; // ������ ����� �ڱ� �ڽ��� ����
	SOCKADDR_IN serverAddress; // ������ �ּҸ� ������� SOCKADDR ����ü

	int serverPort = 9876; // ������ ��Ʈ��ȣ �����ؾߵ�
	char received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock�� �ʱ�ȭ
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP ������ ����

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // inet_addr�� ���������� ���ڿ��� ����. ���ڿ� IP�� ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_port = htons(serverPort); // 2����Ʈ ���� ��Ʈ��ũ ����Ʈ ��������
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");
	cout << "[���� ����] connected()\n";

	while (1) { // �ݺ������� ������ �޼����� �����ϰ� ���� �޼����� ���� �޽��ϴ�.
		cout << "[�޽��� ����]: ";
		getline(cin, sent);
		if (sent == "") continue; // ��� �޼����� �Է����� �ʰ� ���͸� ġ�� ��� 
		send(clientSocket, sent.c_str(), sent.length(), 0); // string������ sent�� c����� ���ڿ� �������·� �ٲ��� (c_str)
		int length = recv(clientSocket, received, sizeof(received), 0); 
		received[length] = '\0'; // ���ڿ��� �������� ��(NULL)���� �־��� 
		if (strcmp(received, "[exit]") == 0) {
			cout << "[���� ����]\n";
			break;
		}
		cout << "[���� �޽���]: " << received << '\n';
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}