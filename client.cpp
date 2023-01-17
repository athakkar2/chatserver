#include <winsock2.h>
// dont move

#include <windows.h>
#include <ws2tcpip.h>

#include <iostream>

using namespace std;
bool sent = true;
bool acceptit = false;

void sendit(SOCKET clientSocket, char message[], int size) {
  int byteCount = send(clientSocket, message, size, 0);
  if (byteCount > 0) {
    cout << "you sent: " << message << endl;
  } else {
    WSACleanup();
  }
  acceptit = true;
  sent = false;
}
void recvit(SOCKET clientSocket, char message[], int size) {
  int byteCount = recv(clientSocket, message, size, 0);
  if (byteCount > 0) {
    cout << "you got: " << message << endl;
  } else {
    WSACleanup();
  }
  acceptit = false;
  sent = true;
}

int main() {
  SOCKET clientSocket;
  int port = 55555;
  WSADATA wsaData;
  int wsaerr;
  WORD wVR = MAKEWORD(2, 2);
  wsaerr = WSAStartup(wVR, &wsaData);
  if (wsaerr != 0) {
    cout << "ahh shit winsock dll not found" << endl;
    return 0;
  } else {
    cout << "HEY NICE JOB" << endl;
  }
  clientSocket = INVALID_SOCKET;
  clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (clientSocket == INVALID_SOCKET) {
    cout << "ya socket fucked up" << endl;
    return 0;
  } else {
    cout << "socket good" << endl;
  }
  sockaddr_in service;
  service.sin_family = AF_INET;
  LPCWSTR ip = L"127.0.0.1";
  InetPtonW(AF_INET, ip, &service.sin_addr.s_addr);
  service.sin_port = htons(port);
  if (connect(clientSocket, (SOCKADDR*)&service, sizeof(service)) ==
      SOCKET_ERROR) {
    cout << "connect failed" << WSAGetLastError() << endl;
    closesocket(clientSocket);
    WSACleanup();
    return 0;
  } else {
    cout << "connect is ok" << endl;
  }
  char buffer[200];
  while (sent || acceptit) {
    if (sent) {
      cin.getline(buffer, 200);
      sendit(clientSocket, buffer, 200);
    } else {
      recvit(clientSocket, buffer, 200);
    }
  }

  system("pause");
  WSACleanup();
}