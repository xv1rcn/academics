#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <vector>

int main(int argc, char **argv) {
    const std::string hexIpData =
        "4500002d1234000040011943645577e8df02097b08005f"
        "ec0000000049204c6f7665204e4e5521";

    std::vector<uint8_t> ipBytes;
    for (size_t i = 0; i < hexIpData.size(); i += 2) {
        std::string byteStr = hexIpData.substr(i, 2);
        ipBytes.push_back(
            static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16)));
    }

    const uint8_t version = (ipBytes[0] >> 4) & 0x0F;
    const uint8_t ihl = ipBytes[0] & 0x0F;
    const uint16_t headerLength = ihl * 4;
    const uint8_t ttl = ipBytes[8];
    const uint8_t protocol = ipBytes[9];

    struct in_addr srcIp;
    srcIp.s_addr = *reinterpret_cast<const uint32_t *>(&ipBytes[12]);
    const std::string srcIpStr = inet_ntoa(srcIp);

    struct in_addr dstIp;
    dstIp.s_addr = *reinterpret_cast<const uint32_t *>(&ipBytes[16]);
    const std::string dstIpStr = inet_ntoa(dstIp);

    const size_t icmpStart = headerLength;
    const uint8_t icmpType = ipBytes[icmpStart];
    const uint8_t icmpCode = ipBytes[icmpStart + 1];

    const size_t payloadStart = icmpStart + sizeof(struct icmphdr);
    std::string payloadContent;
    for (size_t i = payloadStart; i < ipBytes.size(); ++i) {
        payloadContent += static_cast<char>(ipBytes[i]);
    }

    std::ofstream fout("result.txt");
    if (!fout.is_open()) {
        return 1;
    }

    fout << "=== IPv4 Header ===" << std::endl;
    fout << "Version: " << static_cast<int>(version) << std::endl;
    fout << "Header Length: " << headerLength << std::endl;
    fout << "TTL: " << static_cast<int>(ttl) << std::endl;
    fout << "Protocol: " << static_cast<int>(protocol) << std::endl;
    fout << "Source IP: " << srcIpStr << std::endl;
    fout << "Dest IP: " << dstIpStr << std::endl;

    fout << "=== ICMP Header ===" << std::endl;
    fout << "Type: " << static_cast<int>(icmpType) << std::endl;
    fout << "Code: " << static_cast<int>(icmpCode) << std::endl;

    fout << "=== Payload ===" << std::endl;
    fout << "Content: " << payloadContent << std::endl;

    fout.close();
    return 0;
}
