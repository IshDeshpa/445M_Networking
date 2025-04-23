from scapy.all import Ether, IP, UDP, Raw, wrpcap
import random

def random_mac():
    return ":".join(f"{random.randint(0, 255):02x}" for _ in range(6))

def generate_random_frame():
    # src_mac = random_mac()
    # dst_mac = random_mac()
    # ether_type = 0x0800  # IPv4

    # Create 46–1500 bytes of random payload
    payload_size = random.randint(46, 1500)
    payload = bytes(random.getrandbits(8) for _ in range(payload_size))

    # Create UDP and IP layers
    src_ip = f"192.168.{random.randint(0, 255)}.{random.randint(0, 255)}"
    dst_ip = f"192.168.{random.randint(0, 255)}.{random.randint(0, 255)}"
    src_port = random.randint(1024, 65535)
    dst_port = random.randint(1024, 65535)

    udp_layer = UDP(sport=src_port, dport=dst_port)
    ip_layer = IP(src=src_ip, dst=dst_ip)

    # Create IP/UDP packet with payload
    packet = ip_layer / udp_layer / Raw(payload)
    return packet

# Generate one frame and write to PCAP
frame = generate_random_frame()
wrpcap("outbytes.pcap", [frame])

# Write frame details to a text file
with open("outbytes_raw.txt", "wb") as txt_file:
    txt_file.write(bytes(frame))

print("Wrote random Ethernet frame to 'outbytes.pcap' and 'outbytes.txt'")

# Write the frame in raw hex format to a third file
hex_data = bytes(frame).hex()
with open("outbytes.txt", "w") as readable_file:
    for i in range(0, len(hex_data), 32):
        line = hex_data[i:i+32]
        readable_file.write(f"{int(i//2):04x} ")
        readable_file.write(" ".join(line[j:j+2] for j in range(0, len(line), 2)) + "\n")