#include <winsock2.h>
// dont move

#include <windows.h>
#include <ws2tcpip.h>

#include <iostream>

using namespace std;
bool acceptit = true;
bool sent = false;

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
  SOCKET serverSocket, acceptSocket;
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
  serverSocket = INVALID_SOCKET;
  serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serverSocket == INVALID_SOCKET) {
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
  if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) ==
      SOCKET_ERROR) {
    cout << "bind failed" << WSAGetLastError() << endl;
    closesocket(serverSocket);
    WSACleanup();
    return 0;
  } else {
    cout << "bind is ok" << endl;
  }
  if (listen(serverSocket, 1) == SOCKET_ERROR) {
    cout << "you aint listenin like dat" << endl;
  } else {
    cout << "im listenin like dat" << endl;
  }
  acceptSocket = accept(serverSocket, NULL, NULL);
  if (acceptSocket == INVALID_SOCKET) {
    cout << "accept failed" << endl;
    WSACleanup();
    return -1;
  }
  cout << "accepted connection" << endl;

  char buffer[200];
  while (sent || acceptit) {
    if (sent) {
      cin.getline(buffer, 200);
      sendit(acceptSocket, buffer, 200);
    } else {
      recvit(acceptSocket, buffer, 200);
    }
  }

  system("pause");
  WSACleanup();
}