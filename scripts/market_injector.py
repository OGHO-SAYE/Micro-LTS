import csv
import socket
import struct
import time
from pathlib import Path

# Simple Binary Encoding (SBE) Layout Mock:
# Total 24 bytes: Char (1B action: 'B'/'S'), Padding (3B), 
#                 Char[8] (8B Ticker), Int32 (4B Qty), Double (8B Price)
SBE_FORMAT = "<c3s8sid" 
SERVER_ADDRESS = ("127.0.0.1", 8888)
CSV_FILE_PATH = Path(__file__).parent / "data" / "mock_market_data.csv"

def generate_mock_csv():
    """Helper to generate sample data if the CSV doesn't exist yet."""
    CSV_FILE_PATH.parent.mkdir(parents=True, exist_ok=True)
    with open(CSV_FILE_PATH, mode="w", newline="") as f:
        writer = csv.writer(f)
        writer.writerows([
            ["B", "AAPL", 100, 175.50],
            ["S", "MSFT", 50, 420.25],
            ["B", "GOOG", 200, 150.10],
            ["S", "TSLA", 150, 175.00]
        ])

def stream_market_data():
    if not CSV_FILE_PATH.exists():
        generate_mock_csv()

    print(f"Connecting to Micro-LTS Core Engine at {SERVER_ADDRESS}...")
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        try:
            client_socket.connect(SERVER_ADDRESS)
            print("Connected! Blasting binary market data stream...")
            
            with open(CSV_FILE_PATH, mode="r") as f:
                reader = csv.reader(f)
                for row in reader:
                    action, ticker, qty, price = row[0], row[1], int(row[2]), float(row[3])
                    
                    # Pack data into a deterministic, compact binary byte array
                    binary_payload = struct.pack(
                        SBE_FORMAT, 
                        action.encode('ascii'), 
                        b'\x00\x00\x00',          # 3-byte structure padding
                        ticker.encode('ascii').ljust(8, b'\x00'), # Fixed 8B string
                        qty, 
                        price
                    )
                    
                    client_socket.sendall(binary_payload)
                    print(f"Sent binary packet for {ticker.strip()} (Size: {len(binary_payload)} bytes)")
                    time.sleep(0.01) # 10ms pacing to simulate high-frequency pacing intervals
                    
        except ConnectionRefusedError:
            print("Error: Could not connect to C++ engine. Ensure your backend is running.")

if __name__ == "__main__":
    stream_market_data()