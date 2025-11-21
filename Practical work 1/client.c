#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Chỉ thị liên kết thư viện
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // 1. Khởi tạo Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    // 2. Tạo socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Chuyển đổi IP (Localhost 127.0.0.1)
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Dia chi khong hop le\n");
        return 1;
    }

    // 3. Kết nối (Connect)
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Ket noi that bai. Hay chac chan Server da chay.\n");
        return 1;
    }

    printf("Da ket noi den Server!\n");

    // Mở file để đọc
    FILE *fp = fopen("send_me.txt", "rb");
    if (fp == NULL) {
        printf("Khong tim thay file 'send_me.txt' de gui.\n");
        return 1;
    }

    // 4. Gửi dữ liệu
    int bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        send(sock, buffer, bytes_read, 0);
        memset(buffer, 0, BUFFER_SIZE);
    }

    printf("Da gui file xong.\n");

    // 5. Dọn dẹp
    fclose(fp);
    closesocket(sock);
    WSACleanup();

    return 0;
}