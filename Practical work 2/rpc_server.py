import xmlrpc.server
import base64
import os

# Cấu hình Server
PORT = 8080
ADDRESS = ("localhost", PORT)

def save_file_rpc(filename, file_data_base64):
    """
    Hàm này sẽ được gọi từ xa bởi Client.
    Dữ liệu file được mã hóa base64 để truyền qua XML an toàn.
    """
    try:
        # Giải mã dữ liệu từ base64 về binary
        file_data = base64.b64decode(file_data_base64)
        
        # Lưu file với tên mới để phân biệt
        output_name = "received_" + filename
        with open(output_name, "wb") as handle:
            handle.write(file_data)
            
        print(f"Sent file: {filename} -> Saved: {output_name}")
        return "Upload Successfully!"
    except Exception as e:
        print(f"Loi: {str(e)}")
        return "Upload Failed!"

def start_server():
    print(f"RPC Server is running at port {PORT}...")
    # allow_none=True cho phép hàm trả về None nếu cần
    with xmlrpc.server.SimpleXMLRPCServer(ADDRESS, allow_none=True) as server:
        server.register_introspection_functions()
        
        # Đăng ký hàm để Client gọi
        server.register_function(save_file_rpc, "upload_file")
        
        # Chạy server mãi mãi
        server.serve_forever()

if __name__ == "__main__":
    start_server()