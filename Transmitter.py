import socket

udp_ip = "186.4.53.160"  # Use 0.0.0.0 to bind to all available interfaces
udp_port = 12345

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.connect((udp_ip, udp_port))

print("Listening on {}:{}".format(udp_ip, udp_port))

while True:
    data, addr = sock.recvfrom(1024)  # Buffer size is 1024 bytes
    print("Received message from ESP32: '{}' from {}".format(data.decode("utf-8"), addr))

    # Process the received data as needed

    # Send data to ESP32
    message = input("Enter message to send to ESP32: ")
    sock.sendto(message.encode("utf-8"), addr)
