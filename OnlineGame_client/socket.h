#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <string>
#include <atomic>
#include <thread>

#pragma comment(lib, "ws2_32.lib")


#define PORT 3500
#define MAX_BUF_SIZE 256

using namespace std;

SOCKET clientSocket{};
SOCKADDR_IN serverAddress{};
std::atomic<bool> running(true);

DWORD WINAPI RecvThread(LPVOID param) {
    SOCKET sock = (SOCKET)param;

    SOCKADDR_IN fromAddr{};
    int fromLen = sizeof(fromAddr);

    char buf[MAX_BUF_SIZE + 20]{};

    while (running.load()) {
        ZeroMemory(buf, sizeof(buf));

        int recvLen = recvfrom(sock, buf, MAX_BUF_SIZE + 20, 0,
            (SOCKADDR*)&fromAddr, &fromLen);

        if (recvLen > 0) {
            buf[recvLen] = '\0';
            cout << "\n[서버 수신] " << buf << endl;
        }
    }

    return 0;
}

int InitSocketClient() {
    WSADATA wsdata;

    if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) {
        cout << "윈도우 소켓 초기화 실패" << endl;
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cout << "소켓 생성 실패" << endl;
        WSACleanup();
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("192.168.0.63");

    CreateThread(
        nullptr,
        0,
        RecvThread,
        (LPVOID)clientSocket,
        0,
        nullptr
    );

    // 서버에 접속 알림
    const char* hello = "UDP Client Hello";
    sendto(clientSocket, hello, strlen(hello), 0,
        (SOCKADDR*)&serverAddress, sizeof(serverAddress));

    return true;
}

void CleanupSocket() 
{
    running.store(false);
    closesocket(clientSocket);
    WSACleanup();
}
