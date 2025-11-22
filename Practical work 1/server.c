#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     
#include <arpa/inet.h>  

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // 1. Tạo socket
    // AF_INET: IPv4, SOCK_STREAM: TCP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Tùy chọn: Giúp tái sử dụng cổng ngay lập tức (tránh lỗi "Address already in use")
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Cấu hình địa chỉ server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Lắng nghe mọi IP
    address.sin_port = htons(PORT);

    // 2. Bind (Gắn địa chỉ)
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen (Lắng nghe)
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server (Linux) dang lang nghe tren cong %d...\n", PORT);

    // 4. Accept (Chấp nhận kết nối)
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Client da ket noi!\n");

    // Mở file để ghi
    FILE *fp = fopen("received_file.txt", "wb");
    if (fp == NULL) {
        perror("File open error");
        exit(1);
    }

    // 5. Nhận dữ liệu
    int valread;
    while ((valread = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
        fwrite(buffer, 1, valread, fp);
        memset(buffer, 0, BUFFER_SIZE);
    }

    printf("Da nhan file xong.\n");

    // 6. Đóng kết nối
    fclose(fp);
    close(new_socket);
    close(server_fd);
    return 0;
}
