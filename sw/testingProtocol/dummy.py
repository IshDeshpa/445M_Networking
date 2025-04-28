from scapy.all import Ether, IP, UDP, BOOTP, DHCP, raw, wrpcap, rdpcap, mac2str
import random
import os

def random_mac():
    return ":".join(f"{random.randint(0, 255):02x}" for _ in range(6))

def generate_random_frame():
    src_mac = random_mac()
    dst_mac = "D8:47:8F:87:18:3F"
    ether_type = 0x0800  # IPv4

    payload_size = 1000
    payload = bytes(random.getrandbits(8) for _ in range(payload_size))

    src_ip = f"192.168.{random.randint(0, 255)}.{random.randint(0, 255)}"
    dst_ip = f"192.168.0.100"
    src_port = random.randint(1024, 65535)
    dst_port = random.randint(1024, 65535)

    udp_layer = UDP(sport=src_port, dport=dst_port)
    ip_layer = IP(src=src_ip, dst=dst_ip, flags="DF", frag=0)

    packet = Ether(src=src_mac, dst=dst_mac, type=ether_type) / ip_layer / udp_layer / Raw(payload)
    os.makedirs("temp", exist_ok=True)
    wrpcap("temp/inbytes.pcap", [packet])

    print("Wrote random Ethernet frame to 'temp/inbytes.pcap'.")

def dhcp_disc_offer():
    # Read DHCP DISCOVER packet
    disc_pkt = rdpcap("temp/dhcp_disc.pcap")[0]  # Read packet from pcap

    # Extract needed fields
    client_mac = disc_pkt[Ether].src
    transaction_id = disc_pkt[BOOTP].xid

    offer_ip = "192.168.1.100"
    server_ip = "192.168.1.1"

    # Build DHCP OFFER
    ether = Ether(dst=client_mac, src=disc_pkt[Ether].dst)
    ip = IP(src=server_ip, dst="255.255.255.255")
    udp = UDP(sport=67, dport=68)
    bootp = BOOTP(
        op=2,
        yiaddr=offer_ip,
        siaddr=server_ip,
        chaddr=mac2str(client_mac),
        xid=transaction_id,
        flags=0x8000  # broadcast flag
    )
    dhcp = DHCP(options=[
        ("message-type", "offer"),
        ("server_id", server_ip),
        ("lease_time", 3600),
        ("subnet_mask", "255.255.255.0"),
        ("router", server_ip),
        ("name_server", "8.8.8.8"),
        "end"
    ])

    offer_pkt = ether / ip / udp / bootp / dhcp

    # Save
    os.makedirs("temp", exist_ok=True)
    wrpcap("temp/dhcp_offer.pcap", [offer_pkt])
    with open("temp/dhcp_offer_raw.txt", "wb") as f:
        f.write(raw(offer_pkt))

    print("Wrote DHCP OFFER to 'temp/dhcp_offer.pcap' and 'temp/dhcp_offer_raw.txt'.")

def dhcp_req_ack():
    # Read DHCP DISCOVER packet
    req_pkt = rdpcap("temp/dhcp_req.pcap")[0]  # Read packet from pcap

    # Extract needed fields
    client_mac = req_pkt[Ether].src
    transaction_id = req_pkt[BOOTP].xid

    offer_ip = "192.168.1.100"
    server_ip = "192.168.1.1"

    # Build DHCP ACK
    ether = Ether(dst=client_mac, src=req_pkt[Ether].dst)
    ip = IP(src=server_ip, dst="255.255.255.255")
    udp = UDP(sport=67, dport=68)
    bootp = BOOTP(
        op=2,
        yiaddr=offer_ip,
        siaddr=server_ip,
        chaddr=mac2str(client_mac),
        xid=transaction_id,
        flags=0x8000  # broadcast flag
    )
    dhcp = DHCP(options=[
        ("message-type", "offer"),
        ("server_id", server_ip),
        ("lease_time", 3600),
        ("subnet_mask", "255.255.255.0"),
        ("router", server_ip),
        ("name_server", "8.8.8.8"),
        "end"
    ])

    ack_pkt = ether / ip / udp / bootp / dhcp

    # Save
    os.makedirs("temp", exist_ok=True)
    wrpcap("temp/dhcp_ack.pcap", [offer_pkt])
    with open("temp/dhcp_ack_raw.txt", "wb") as f:
        f.write(raw(offer_pkt))

    print("Wrote DHCP ACK to 'temp/dhcp_ack.pcap' and 'temp/dhcp_ack_raw.txt'.")


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
