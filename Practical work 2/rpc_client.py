import xmlrpc.client
import base64
import os

PORT = 8080
SERVER_URL = f"http://localhost:{PORT}/"
FILENAME = "send_me.txt"

def send_file_rpc():
    # 1. Kết nối đến Server (Stub)
    proxy = xmlrpc.client.ServerProxy(SERVER_URL)
    
    if not os.path.exists(FILENAME):
        print(f"Cannot found file '{FILENAME}'")
        return

    print(f"Connect to RPC Server at {SERVER_URL}...")

    # 2. Đọc file và mã hóa
    with open(FILENAME, "rb") as handle:
        binary_data = handle.read()
        # XML-RPC truyền text tốt nhất, nên ta mã hóa binary sang base64 string
        encoded_data = base64.b64encode(binary_data).decode("utf-8")

    # 3. Gọi hàm từ xa (Remote Procedure Call)
    print(f"Loading file '{FILENAME}'...")
    try:
        # Gọi hàm 'upload_file' đã đăng ký bên Server
        result = proxy.upload_file(FILENAME, encoded_data)
        print("Server received:", result)
    except ConnectionRefusedError:
        print("Cannot connect to RPC Server. Is it running?")

if __name__ == "__main__":
    send_file_rpc()