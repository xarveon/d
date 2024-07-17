from scapy.all import sniff
import requests
from urllib.parse import urlparse

def packet_callback(packet):
    if packet.haslayer('TCP') and packet.haslayer('Raw'):
        # Extract HTTP data from the packet
        http_data = packet['Raw'].load.decode('utf-8', errors='ignore')
        if 'HTTP' in http_data:
            # Parse HTTP request to extract the URL
            lines = http_data.split('\r\n')
            for line in lines:
                if line.startswith('GET') or line.startswith('POST'):
                    try:
                        url = line.split(' ')[1]
                        # Validate and normalize the URL
                        parsed_url = urlparse(url)
                        if not parsed_url.scheme:
                            url = 'http://' + url
                        # Make an HTTP request and check for status code 400
                        response = requests.get(url)
                        if response.status_code == 400:
                            print(f"Status code 400 found at URL: {url}")
                    except Exception as e:
                        print(f"Error processing URL {url}: {e}")

def start_sniffing():
    # Start sniffing packets
    print("Starting packet sniffing...")
    sniff(prn=packet_callback, filter="tcp port 80 or tcp port 443", store=0)

if __name__ == "__main__":
    start_sniffing()
