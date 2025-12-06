from mpi4py import MPI
import os

# Khởi tạo môi trường MPI
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

FILENAME = "send_me.txt"
OUTPUT_NAME = "received_via_mpi.txt"
TAG_DATA = 11

def sender():
    print(f"[Rank {rank}] I am the Sender.")
    
    if not os.path.exists(FILENAME):
        print(f"[Rank {rank}] Error: File '{FILENAME}' not found.")
        return

    # Đọc file dưới dạng binary
    with open(FILENAME, "rb") as f:
        file_data = f.read()
    
    # Gửi dữ liệu sang Rank 1
    print(f"[Rank {rank}] Sending {len(file_data)} bytes to Rank 1...")
    comm.send(file_data, dest=1, tag=TAG_DATA)
    print(f"[Rank {rank}] Send complete.")

def receiver():
    print(f"[Rank {rank}] I am the Receiver. Waiting for data...")
    
    # Nhận dữ liệu từ Rank 0
    file_data = comm.recv(source=0, tag=TAG_DATA)
    
    # Ghi file
    with open(OUTPUT_NAME, "wb") as f:
        f.write(file_data)
        
    print(f"[Rank {rank}] Received {len(file_data)} bytes.")
    print(f"[Rank {rank}] Saved as '{OUTPUT_NAME}'. Success!")

if __name__ == "__main__":
    # Yêu cầu ít nhất 2 tiến trình
    if size < 2:
        print("Error: This program requires at least 2 processes.")
        print("Run with: mpirun -n 2 python3 mpi_transfer.py")
    else:
        if rank == 0:
            sender()
        elif rank == 1:
            receiver()