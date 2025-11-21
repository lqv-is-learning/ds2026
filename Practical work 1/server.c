#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h> // Thư viện bổ trợ cho địa chỉ IP

// Chỉ thị cho trình biên dịch liên kết với thư viện ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // 1. Khởi tạo Winsock (Bắt buộc trên Windows)
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Khoi tao Winsock that bai.\n");
        return 1;
    }

    // 2. Tạo socket
    // Lưu ý: Trên Windows dùng kiểu dữ liệu SOCKET, không phải int
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Khong the tao socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Cấu hình địa chỉ
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 3. Bind (Gắn địa chỉ)
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Bind that bai: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // 4. Listen (Lắng nghe)
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        printf("Listen that bai\n");
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Server dang lang nghe tren cong %d... (Windows)\n", PORT);

    // 5. Accept (Chấp nhận kết nối)
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET) {
        printf("Accept that bai: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Client da ket noi!\n");

    // Mở file để ghi
    FILE *fp = fopen("received_file.txt", "wb");
    if (fp == NULL) {
        perror("Loi mo file");
        return 1;
    }

    // 6. Nhận dữ liệu
    int valread;
    while ((valread = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, valread, fp);
        memset(buffer, 0, BUFFER_SIZE);
    }

    printf("Da nhan file xong.\n");

    // 7. Dọn dẹp
    fclose(fp);
    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup(); // Giải phóng Winsock

    return 0;
}