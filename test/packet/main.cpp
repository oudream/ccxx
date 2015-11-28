#include <cxmain.h>

#include "testPacket1.cpp"
#include "testPacket2.cpp"

using namespace std;


const uint c_ics_head = 0x5aa55aa5;


//ushort  version;        //4 5

//ushort  frameLength;    //6 7

//// 10 + 2 + 24
//struct psm_packet_body_t {
//    ushort  frameType;
//    ushort  sourceType;
//    ushort  sourceAddress;
//    ushort  targetType;
//    ushort  targetAddress;

//    ushort  controlWord;

//    uint    common;
//    uint    reason;
//    uint    res;
//    uint    container;
//    uint    paramType;
//    uint    paramCount;
//};

//ushort  crc;


//    uint    head;
//    ushort  version;
//    ushort  frameLength;
struct psm_packet_body_t
{
    int  frameType;
    short  sourceType;
    int  sourceAddress;
    short  targetType;
    int  targetAddress;
    //4(head) + 2 + 2 + 10 + 2
    short  controlWord;
    //  + 24 + 2(crc)
    int    command;
    int    reason;
    int    res;
    int    container;
    int    paramType;
    int    paramCount;
};
//    ushort  crc;



void testPacket01()
{
//    uchar buffer[] = {
//        0x15, 0x5a, 0x25, 0xa5, 0x5a, 0xa5, 0x5a,
//        0x01, 0x00, 0x06, 0x00, 0x30, 0x30,
//        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
//        0x02, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
//        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
//        0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
////        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x40,
//        0x07, 0x00
//    };

    typedef CxPacket5<uint, ushort, ushort, psm_packet_body_t, ushort>::PacketData PacketData;
    typedef CxPacket5<uint, ushort, ushort, psm_packet_body_t, ushort>::ReceivePacket ReceivePacket;
    typedef CxPacket5<uint, ushort, ushort, psm_packet_body_t, ushort>::SendPacket SendPacket;

    uint head = 0x5aa55aa5;

    ReceivePacket packet(head);

    string sBuffer1 = "00 a5 5a a5 5a 01 00 08 00 00 00 00 00 00 00 e9 02 00 00 00 00 00 00 4f 00 00 00 00 00 00 40 00 00 01 01 05 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 61 61 61 00 00 00 00 00 1b 64 00";
    string sBuffer2 = "11 a5 5a a5 5a 01 00 00 00 00 00 00 00 00 00 16 02 00 00 00 00 00 00 56 00 00 00 00 00 00 c0 cf 01 02 01 06 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f c7";
//    string sBuffer1 = "00 a5 5a a5 5a 01 00 08 00 00 00 00 00 00 00 e9 02 00 00 00 00 00 00 4f 00 00 00 00 00 00 40 00 00 01 01 05 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 61 61 61 00 00 00 00 00 1b 64 a5 5a a5 5a 01 00 00 00 00 00 00 00 00 00 16 ";
//    string sBuffer2 = "02 00 00 00 00 00 00 56 00 00 00 00 00 00 c0 cf 01 02 01 06 03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0f c7";
    vector<uchar> buffer;
    buffer = CxString::fromHexstring(sBuffer1);

    packet.push((char *)buffer.data(), buffer.size());

    const std::vector<PacketData> & packets1 = packet.packets();
    for (size_t i = 0; i < packets1.size(); ++i)
    {
        const PacketData & packet = packets1[i];
        const psm_packet_body_t & frameFixData = packet.body;
        cout << CxString::format("head[%x] - front[%x] - length[%d] - back.sourceType[%x] , back.command[%x], back.paramType[%x] - end[%x]", packet.head, packet.front, packet.length, frameFixData.sourceType, frameFixData.command, frameFixData.paramType, packet.end )
                << endl;
    }
    packet.clear();

    buffer = CxString::fromHexstring(sBuffer2);

    packet.push((char *)buffer.data(), buffer.size());

    const std::vector<PacketData> & packets2 = packet.packets();
    for (size_t i = 0; i < packets2.size(); ++i)
    {
        const PacketData & packet = packets2[i];
        const psm_packet_body_t & frameFixData = packet.body;
        cout << CxString::format("head[%x] - front[%x] - length[%d] - back.sourceType[%x] , back.command[%x], back.paramType[%x] - end[%x]", packet.head, packet.front, packet.length, frameFixData.sourceType, frameFixData.command, frameFixData.paramType, packet.end )
             << endl;
    }

    cout << "" << endl;
}

void testPacket02()
{
    uchar buffer[] = {
        0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39
    };

    typedef CxPacket5<uint, ushort, ushort, psm_packet_body_t, ushort>::PacketData PacketData;
    typedef CxPacket5<uint, ushort, ushort, psm_packet_body_t, ushort>::ReceivePacket ReceivePacket;
    typedef CxPacket5<uint, ushort, ushort, psm_packet_body_t, ushort>::SendPacket SendPacket;

    uint head = 0x5aa55aa5;

    SendPacket packet(head, 1, CxCrc::calcCRC16);

    psm_packet_body_t body;
    body.command = 11;
    body.paramType = 12;
    body.sourceType = 13;
    packet.setpack(body, (char *)buffer, sizeof(buffer));

    const std::vector<string> & packets = packet.packets();
    for (size_t i = 0; i < packets.size(); ++i)
    {
        const string & packet = packets[i];
        string sHex = CxString::toHexstring(packet.data(), packet.size());
        CxFile::save("c:/0.txt", sHex);
        cout << sHex << endl;
    }

//a5 5a a5 5a 01 00 09 00 e0 b0 0d 00 00 00 00 00 a0 fe 22 00 0b 00 00 00 c0 04 6a 77 ac fe 22 00 45 f3 60 77 0c 00 00 00 a9 fa 22 00 31 32 33 34 35 36 37 38 39 12 58
    cout << "" << endl;
}


int main(int argc, char *argv[])
{
    cout << "begin" << endl;

    CxApplication::init(argc, argv);

    testPacket01();

    return 0;
}
