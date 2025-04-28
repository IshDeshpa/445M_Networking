from scapy.all import Ether, IP, UDP, Raw, wrpcap, DHCP
import random

def random_mac():
    return ":".join(f"{random.randint(0, 255):02x}" for _ in range(6))

def generate_random_frame():
    src_mac = random_mac()
    dst_mac = "D8:47:8F:87:18:3F"
    ether_type = 0x0800  # IPv4

    # Create 46–1500 bytes of random payload
    payload_size = 1000
    payload = bytes(random.getrandbits(8) for _ in range(payload_size))

    # Create UDP and IP layers
    src_ip = f"192.168.{random.randint(0, 255)}.{random.randint(0, 255)}"
    #dst_ip = f"192.168.{random.randint(0, 255)}.{random.randint(0, 255)}"
    dst_ip = f"192.168.0.100"
    src_port = random.randint(1024, 65535)
    dst_port = random.randint(1024, 65535)

    udp_layer = UDP(sport=src_port, dport=dst_port)
    ip_layer = IP(src=src_ip, dst=dst_ip, flags="DF", frag=0)  

    # Create IP/UDP packet with payload
    packet = Ether(src=src_mac, dst=dst_mac, type=ether_type) / ip_layer / udp_layer / Raw(payload)
    return packet

def random_frame():
    # Generate one frame and write to PCAP
    frame = generate_random_frame()
    wrpcap("temp/inbytes.pcap", [frame])

    # Write frame details to a text file
    with open("temp/inbytes_raw.txt", "wb") as txt_file:
        txt_file.write(bytes(frame))

    print("Wrote random Ethernet frame to 'temp/inbytes.pcap' and 'temp/inbytes.txt'")

    # Write the frame in raw hex format to a third file
    hex_data = bytes(frame).hex()
    with open("temp/inbytes.txt", "w") as readable_file:
        for i in range(0, len(hex_data), 32):
            line = hex_data[i:i+32]
            readable_file.write(f"{int(i//2):04x} ")
            readable_file.write(" ".join(line[j:j+2] for j in range(0, len(line), 2)) + "\n") 

def dhcp_disc_offer():
    # Parse discover
    with open("temp/dhcp_disc.txt", "r") as f:
        hex_data = f.read().strip()

    # Convert the hex string into bytes
    packet_data = bytes.fromhex(hex_data)
    
    disc_pkt = Ether(packet_data)
    disc_pkt.show()
    
    # Send offer
    offer_ip = "192.168.1.100"
    server_ip = "192.168.1.1"

    ether = Ether(dst=disc_pkt[Ether].src, src=disc_pkt[Ether].dst)
    
    ip = IP(src=server_ip, dst='255.255.255.255')
    udp = UDP(sport=67, dport=68)
    # nothing in bootp
    empty_bootp = bytes(240)

    dhcp = DHCP(options=[
        ('message-type', 'offer'),
        ('server_id', server_ip),
        'end'
    ])    

    offer_pkt = ether / ip / udp / empty_bootp
    offer_pkt.show()

    with open("temp/dhcp_offer_raw.txt", "wb") as f:
        f.write(bytes(offer_pkt))

    wrpcap("temp/dhcp_offer.pcap", [offer_pkt])


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("type")
    args = parser.parse_args()

    if args.type == "random_frame":
        random_frame()
    elif args.type == "dhcp":
        dhcp_disc_offer()
    else:
        print("Invalid test")

