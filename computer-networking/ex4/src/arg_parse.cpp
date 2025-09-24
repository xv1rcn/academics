/**
 * 利用C++语言编写代码
 * 实现ARP数据包的解析
 * 实验使用的ARP数据包内容为：0001080006040001eeeeeeeeeeeeac15e50b0000000000006463ae47
 *
 */
#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sys/stat.h>

/**
 * @brief 将十六进制字符串转换为二进制数据
 *
 * 该函数将一个十六进制字符串转换为二进制数据，并将结果存储在指定的缓冲区中。
 * 每两个十六进制字符表示一个字节。
 *
 * @param hexString 输入的十六进制字符串，长度必须为偶数。
 * @param buffer 用于存储转换后的二进制数据的缓冲区。
 * @param bufferSize 缓冲区的大小，必须至少为 `hexString.length() / 2`。
 *
 */
void hexStringToBinary(const std::string &hexString, uint8_t *buffer,
                       size_t bufferSize) {
    for (size_t i = 0; i < hexString.length(); i += 2) {
        std::string byteString = hexString.substr(i, 2);
        uint8_t byte =
            static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
        buffer[i / 2] = byte;
    }
}

/**
 * @brief 检查文件是否存在
 *
 * 该函数检查指定路径的文件是否存在。
 *
 * @param filename 文件路径。
 * @return true 文件存在。
 * @return false 文件不存在。
 */
bool fileExists(const std::string &filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

/**
 * @brief 解析ARP数据包
 *
 * 该函数解析ARP数据包，并将解析结果输出到指定的文件中。
 *
 * @param arpData ARP数据包的二进制数据。
 * @param outputFile 输出文件的路径。
 *
 * 此函数待补充完整。
 */
void parseARPPacket(const uint8_t *arpData, const std::string &outputFile) {
    if (!fileExists(outputFile)) {
        std::ofstream createFile(outputFile);
        if (!createFile.is_open()) {
            std::cerr << "Failed to create output file: " << outputFile
                      << std::endl;
            return;
        }
        createFile.close();
    }
    std::ofstream outFile(outputFile, std::ios::out | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        return;
    }

    uint16_t hardwareType = ntohs(*(const uint16_t *)&arpData[0]);
    uint16_t protocolType = ntohs(*(const uint16_t *)&arpData[2]);
    uint8_t hardwareLen = arpData[4];
    uint8_t protocolLen = arpData[5];
    uint16_t operation = ntohs(*(const uint16_t *)&arpData[6]);

    char senderMacStr[18];
    sprintf(senderMacStr, "%02x:%02x:%02x:%02x:%02x:%02x", arpData[8],
            arpData[9], arpData[10], arpData[11], arpData[12], arpData[13]);

    char senderIpStr[16];
    sprintf(senderIpStr, "%d.%d.%d.%d", arpData[14], arpData[15], arpData[16],
            arpData[17]);

    char targetMacStr[18];
    sprintf(targetMacStr, "%02x:%02x:%02x:%02x:%02x:%02x", arpData[18],
            arpData[19], arpData[20], arpData[21], arpData[22], arpData[23]);

    char targetIpStr[16];
    sprintf(targetIpStr, "%d.%d.%d.%d", arpData[24], arpData[25], arpData[26],
            arpData[27]);

    outFile << "Hardware Type: " << hardwareType << std::endl;
    outFile << "Protocol Type: 0x" << std::hex << std::setw(4)
            << std::setfill('0') << protocolType << std::dec << std::endl;
    outFile << "Hardware Address Length: " << static_cast<int>(hardwareLen)
            << std::endl;
    outFile << "Protocol Address Length: " << static_cast<int>(protocolLen)
            << std::endl;
    outFile << "Operation: " << operation << std::endl;
    outFile << "Sender MAC: " << senderMacStr << std::endl;
    outFile << "Sender IP: " << senderIpStr << std::endl;
    outFile << "Target MAC: " << targetMacStr << std::endl;
    outFile << "Target IP: " << targetIpStr << std::endl;

    outFile.close();
}

/**
 * main 函数无需改动
 *
 */
int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ARP_hex_string> <output_file>"
                  << std::endl;
        return 1;
    }

    std::string ARP_string = argv[1];
    std::string outputFile = argv[2];

    uint8_t ARP_data[28];
    hexStringToBinary(ARP_string, ARP_data, sizeof(ARP_data));

    // 解析ARP数据包并将结果保存到指定文件
    parseARPPacket(ARP_data, outputFile);

    return 0;
}
