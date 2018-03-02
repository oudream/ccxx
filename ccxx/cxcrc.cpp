#include "cxcrc.h"

#include "cxstring.h"
#include "cxfile.h"

using namespace std;

const uchar f_crc8Table[256] = {
        0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
        0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
        0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
        0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
        0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
        0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
        0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
        0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
        0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
        0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
        0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
        0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
        0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
        0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
        0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
        0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
        0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
        0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
        0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
        0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
        0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
        0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
        0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,
        0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
        0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
        0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
        0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
        0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
        0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB,
        0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
        0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB,
        0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};


const ushort f_crc16Table[256]= 	 //crc16 table
{
        0x0000,0x1021,0x2042,0x3063,0x4084,0x50A5,0x60C6,0x70E7,
        0x8108,0x9129,0xA14A,0xB16B,0xC18C,0xD1AD,0xE1CE,0xF1EF,
        0x1231,0x0210,0x3273,0x2252,0x52B5,0x4294,0x72F7,0x62D6,
        0x9339,0x8318,0xB37B,0xA35A,0xD3BD,0xC39C,0xF3FF,0xE3DE,
        0x2462,0x3443,0x0420,0x1401,0x64E6,0x74C7,0x44A4,0x5485,
        0xA56A,0xB54B,0x8528,0x9509,0xE5EE,0xF5CF,0xC5AC,0xD58D,
        0x3653,0x2672,0x1611,0x0630,0x76D7,0x66F6,0x5695,0x46B4,
        0xB75B,0xA77A,0x9719,0x8738,0xF7DF,0xE7FE,0xD79D,0xC7BC,
        0x48C4,0x58E5,0x6886,0x78A7,0x0840,0x1861,0x2802,0x3823,
        0xC9CC,0xD9ED,0xE98E,0xF9AF,0x8948,0x9969,0xA90A,0xB92B,
        0x5AF5,0x4AD4,0x7AB7,0x6A96,0x1A71,0x0A50,0x3A33,0x2A12,
        0xDBFD,0xCBDC,0xFBBF,0xEB9E,0x9B79,0x8B58,0xBB3B,0xAB1A,
        0x6CA6,0x7C87,0x4CE4,0x5CC5,0x2C22,0x3C03,0x0C60,0x1C41,
        0xEDAE,0xFD8F,0xCDEC,0xDDCD,0xAD2A,0xBD0B,0x8D68,0x9D49,
        0x7E97,0x6EB6,0x5ED5,0x4EF4,0x3E13,0x2E32,0x1E51,0x0E70,
        0xFF9F,0xEFBE,0xDFDD,0xCFFC,0xBF1B,0xAF3A,0x9F59,0x8F78,
        0x9188,0x81A9,0xB1CA,0xA1EB,0xD10C,0xC12D,0xF14E,0xE16F,
        0x1080,0x00A1,0x30C2,0x20E3,0x5004,0x4025,0x7046,0x6067,
        0x83B9,0x9398,0xA3FB,0xB3DA,0xC33D,0xD31C,0xE37F,0xF35E,
        0x02B1,0x1290,0x22F3,0x32D2,0x4235,0x5214,0x6277,0x7256,
        0xB5EA,0xA5CB,0x95A8,0x8589,0xF56E,0xE54F,0xD52C,0xC50D,
        0x34E2,0x24C3,0x14A0,0x0481,0x7466,0x6447,0x5424,0x4405,
        0xA7DB,0xB7FA,0x8799,0x97B8,0xE75F,0xF77E,0xC71D,0xD73C,
        0x26D3,0x36F2,0x0691,0x16B0,0x6657,0x7676,0x4615,0x5634,
        0xD94C,0xC96D,0xF90E,0xE92F,0x99C8,0x89E9,0xB98A,0xA9AB,
        0x5844,0x4865,0x7806,0x6827,0x18C0,0x08E1,0x3882,0x28A3,
        0xCB7D,0xDB5C,0xEB3F,0xFB1E,0x8BF9,0x9BD8,0xABBB,0xBB9A,
        0x4A75,0x5A54,0x6A37,0x7A16,0x0AF1,0x1AD0,0x2AB3,0x3A92,
        0xFD2E,0xED0F,0xDD6C,0xCD4D,0xBDAA,0xAD8B,0x9DE8,0x8DC9,
        0x7C26,0x6C07,0x5C64,0x4C45,0x3CA2,0x2C83,0x1CE0,0x0CC1,
        0xEF1F,0xFF3E,0xCF5D,0xDF7C,0xAF9B,0xBFBA,0x8FD9,0x9FF8,
        0x6E17,0x7E36,0x4E55,0x5E74,0x2E93,0x3EB2,0x0ED1,0x1EF0
};


const uchar f_modbusTableHi[256] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00,
    0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80,
    0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00,
    0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
    0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
    0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

const uchar f_modbusTableLo[256] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05,
    0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA,
    0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA,
    0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15,
    0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10, 0xF0,
    0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35,
    0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B,
    0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA,
    0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27,
    0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64,
    0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE,
    0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7,
    0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
    0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99,
    0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E,
    0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46,
    0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

uchar CxCrc::calcsum8(const uchar* pData, int iLength)
{
    uchar aSum = 0;
    for (int i = 0; i < iLength; ++i) {
        aSum =  aSum + pData[ i ];
    }
    return aSum;
}


ushort CxCrc::calcsum16(const uchar* pData, int iLength)
{
    ushort aSum = 0;
    for (int i = 0; i < iLength; ++i) {
        aSum =  aSum + pData[ i ];
    }
    return aSum;
}


uchar CxCrc::calcCRC8(const uchar* pData, int iLength){
    uchar result = 0;
    for (int i = 0; i < iLength; ++i) {
        result = f_crc8Table [ result ^ pData[ i ] ];
    }
    return ~ result;
}


ushort CxCrc::calcCRC16(const uchar *pData, int iLength){
    ushort result = 0;
    for (int i = 0; i < iLength; ++i) {
        uchar t = pData[i];
        uchar byh = result >> 8;
        uchar byl = result;
        result = byh ^ f_crc16Table[ t ^ byl ];
    };
    return result;
}

short CxCrc::calcCRC16(const char *pData, int iLength)
{
    const uchar * pData1 = (const uchar *)pData;
    ushort result = 0;
    for (int i = 0; i < iLength; ++i) {
        uchar t = pData1[i];
        uchar byh = result >> 8;
        uchar byl = result;
        result = byh ^ f_crc16Table[ t ^ byl ];
    };
    return result;
}


ushort CxCrc::calcModbus(const uchar *pData, int iLength)
{
    uchar hi = 0xFF;
    uchar lo = 0xFF;
    uchar index;
    for (int i = 0; i < iLength; ++i) {
        index = hi ^ pData[i];
        hi = lo ^ f_modbusTableHi[index];
        lo = f_modbusTableLo[index];
    }
    ushort wResult = (hi << 8) | lo;
    return wResult;
}


//------------ md4 --------------------------------------------------------------------------------------------


#define	MD4_RESULTLEN (128/8)

struct md4_context {
    uint32 lo, hi;
    uint32 a, b, c, d;
    unsigned char buffer[64];
    uint32 block[MD4_RESULTLEN];
};

/*
 * The basic MD4 functions.
 */
#define F(x, y, z)	((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)	(((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define H(x, y, z)	((x) ^ (y) ^ (z))

/*
 * The MD4 transformation for all four rounds.
 */
#define STEP(f, a, b, c, d, x, s) \
    (a) += f((b), (c), (d)) + (x);	 \
    (a) = ((a) << (s)) | ((a) >> (32 - (s)))


/*
 * SET reads 4 input bytes in little-endian byte order and stores them
 * in a properly aligned word in host byte order.
 *
 * The check for little-endian architectures which tolerate unaligned
 * memory accesses is just an optimization.  Nothing will break if it
 * doesn't work.
 */
#if defined(__i386__) || defined(__x86_64__)
#define SET(n) \
    (*(const uint32 *)&ptr[(n) * 4])
#define GET(n) \
    SET(n)
#else
#define SET(n) \
    (ctx->block[(n)] = \
    (uint32)ptr[(n) * 4] | \
    ((uint32)ptr[(n) * 4 + 1] << 8) | \
    ((uint32)ptr[(n) * 4 + 2] << 16) | \
    ((uint32)ptr[(n) * 4 + 3] << 24))
#define GET(n) \
    (ctx->block[(n)])
#endif

/*
 * This processes one or more 64-byte data blocks, but does NOT update
 * the bit counters.  There're no alignment requirements.
 */
static const unsigned char *body(struct md4_context *ctx, const unsigned char *data, size_t size)
{
    const unsigned char *ptr;
    uint32 a, b, c, d;
    uint32 saved_a, saved_b, saved_c, saved_d;

    ptr = data;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;

    do {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;

/* Round 1 */
        STEP(F, a, b, c, d, SET( 0),  3);
        STEP(F, d, a, b, c, SET( 1),  7);
        STEP(F, c, d, a, b, SET( 2), 11);
        STEP(F, b, c, d, a, SET( 3), 19);

        STEP(F, a, b, c, d, SET( 4),  3);
        STEP(F, d, a, b, c, SET( 5),  7);
        STEP(F, c, d, a, b, SET( 6), 11);
        STEP(F, b, c, d, a, SET( 7), 19);

        STEP(F, a, b, c, d, SET( 8),  3);
        STEP(F, d, a, b, c, SET( 9),  7);
        STEP(F, c, d, a, b, SET(10), 11);
        STEP(F, b, c, d, a, SET(11), 19);

        STEP(F, a, b, c, d, SET(12),  3);
        STEP(F, d, a, b, c, SET(13),  7);
        STEP(F, c, d, a, b, SET(14), 11);
        STEP(F, b, c, d, a, SET(15), 19);
/* Round 2 */
        STEP(G, a, b, c, d, GET( 0) + 0x5A827999,  3);
        STEP(G, d, a, b, c, GET( 4) + 0x5A827999,  5);
        STEP(G, c, d, a, b, GET( 8) + 0x5A827999,  9);
        STEP(G, b, c, d, a, GET(12) + 0x5A827999, 13);

        STEP(G, a, b, c, d, GET( 1) + 0x5A827999,  3);
        STEP(G, d, a, b, c, GET( 5) + 0x5A827999,  5);
        STEP(G, c, d, a, b, GET( 9) + 0x5A827999,  9);
        STEP(G, b, c, d, a, GET(13) + 0x5A827999, 13);

        STEP(G, a, b, c, d, GET( 2) + 0x5A827999,  3);
        STEP(G, d, a, b, c, GET( 6) + 0x5A827999,  5);
        STEP(G, c, d, a, b, GET(10) + 0x5A827999,  9);
        STEP(G, b, c, d, a, GET(14) + 0x5A827999, 13);

        STEP(G, a, b, c, d, GET( 3) + 0x5A827999,  3);
        STEP(G, d, a, b, c, GET( 7) + 0x5A827999,  5);
        STEP(G, c, d, a, b, GET(11) + 0x5A827999,  9);
        STEP(G, b, c, d, a, GET(15) + 0x5A827999, 13);
/* Round 3 */
        STEP(H, a, b, c, d, GET( 0) + 0x6ED9EBA1,  3);
        STEP(H, d, a, b, c, GET( 8) + 0x6ED9EBA1,  9);
        STEP(H, c, d, a, b, GET( 4) + 0x6ED9EBA1, 11);
        STEP(H, b, c, d, a, GET(12) + 0x6ED9EBA1, 15);

        STEP(H, a, b, c, d, GET( 2) + 0x6ED9EBA1,  3);
        STEP(H, d, a, b, c, GET(10) + 0x6ED9EBA1,  9);
        STEP(H, c, d, a, b, GET( 6) + 0x6ED9EBA1, 11);
        STEP(H, b, c, d, a, GET(14) + 0x6ED9EBA1, 15);

        STEP(H, a, b, c, d, GET( 1) + 0x6ED9EBA1,  3);
        STEP(H, d, a, b, c, GET( 9) + 0x6ED9EBA1,  9);
        STEP(H, c, d, a, b, GET( 5) + 0x6ED9EBA1, 11);
        STEP(H, b, c, d, a, GET(13) + 0x6ED9EBA1, 15);

        STEP(H, a, b, c, d, GET( 3) + 0x6ED9EBA1,  3);
        STEP(H, d, a, b, c, GET(11) + 0x6ED9EBA1,  9);
        STEP(H, c, d, a, b, GET( 7) + 0x6ED9EBA1, 11);
        STEP(H, b, c, d, a, GET(15) + 0x6ED9EBA1, 15);

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;

        ptr += 64;
    } while (size -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;

    return ptr;
}

static void md4_init(struct md4_context *ctx)
{
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;

    ctx->lo = 0;
    ctx->hi = 0;
}

static void md4_update(struct md4_context *ctx, const unsigned char *data, size_t size)
{
    /* @UNSAFE */
    uint32 saved_lo;
    unsigned long used, free;

    saved_lo = ctx->lo;
    if ((ctx->lo = (saved_lo + size) & 0x1fffffff) < saved_lo)
        ctx->hi++;
    ctx->hi += size >> 29;

    used = saved_lo & 0x3f;

    if (used) {
        free = 64 - used;

        if (size < free) {
            memcpy(&ctx->buffer[used], data, size);
            return;
        }

        memcpy(&ctx->buffer[used], data, free);
        data = (const unsigned char *) data + free;
        size -= free;
        body(ctx, ctx->buffer, 64);
    }

    if (size >= 64) {
        data = body(ctx, data, size & ~(unsigned long)0x3f);
        size &= 0x3f;
    }

    memcpy(ctx->buffer, data, size);
}

static void md4_final(struct md4_context *ctx, unsigned char result[MD4_RESULTLEN])
{
    /* @UNSAFE */
    unsigned long used, free;

    used = ctx->lo & 0x3f;

    ctx->buffer[used++] = 0x80;

    free = 64 - used;

    if (free < 8) {
        memset(&ctx->buffer[used], 0, free);
        body(ctx, ctx->buffer, 64);
        used = 0;
        free = 64;
    }

    memset(&ctx->buffer[used], 0, free - 8);

    ctx->lo <<= 3;
    ctx->buffer[56] = ctx->lo;
    ctx->buffer[57] = ctx->lo >> 8;
    ctx->buffer[58] = ctx->lo >> 16;
    ctx->buffer[59] = ctx->lo >> 24;
    ctx->buffer[60] = ctx->hi;
    ctx->buffer[61] = ctx->hi >> 8;
    ctx->buffer[62] = ctx->hi >> 16;
    ctx->buffer[63] = ctx->hi >> 24;

    body(ctx, ctx->buffer, 64);

    result[0] = ctx->a;
    result[1] = ctx->a >> 8;
    result[2] = ctx->a >> 16;
    result[3] = ctx->a >> 24;
    result[4] = ctx->b;
    result[5] = ctx->b >> 8;
    result[6] = ctx->b >> 16;
    result[7] = ctx->b >> 24;
    result[8] = ctx->c;
    result[9] = ctx->c >> 8;
    result[10] = ctx->c >> 16;
    result[11] = ctx->c >> 24;
    result[12] = ctx->d;
    result[13] = ctx->d >> 8;
    result[14] = ctx->d >> 16;
    result[15] = ctx->d >> 24;

    memset(ctx, 0, sizeof(*ctx));
}

#undef F
#undef G
#undef H



//------------ md5 --------------------------------------------------------------------------------------------

typedef unsigned char md5byte;
typedef uint32 UWORD32;

struct MD5Context {
    UWORD32 buf[4];
    UWORD32 bytes[2];
    UWORD32 in[16];
};

static void MD5Init(struct MD5Context *context);
static void MD5Update(struct MD5Context *context, md5byte const *buf, unsigned len);
static void MD5Final(struct MD5Context *context, unsigned char digest[16]);
static void MD5Transform(UWORD32 buf[4], UWORD32 const in[16]);

#ifdef _WIN32_WCE
#include <windef.h>
#include <types.h>
#endif

static void
byteSwap(UWORD32 *buf, unsigned words)
{
        const uint32 byteOrderTest = 0x1;
        if (((char *)&byteOrderTest)[0] == 0) {
            md5byte *p = (md5byte *)buf;

            do {
                *buf++ = (UWORD32)((unsigned)p[3] << 8 | p[2]) << 16 |
                    ((unsigned)p[1] << 8 | p[0]);
                p += 4;
            } while (--words);
        }
}

/*
 * start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
static void
MD5Init(struct MD5Context *ctx)
{
    ctx->buf[0] = 0x67452301;
    ctx->buf[1] = 0xefcdab89;
    ctx->buf[2] = 0x98badcfe;
    ctx->buf[3] = 0x10325476;

    ctx->bytes[0] = 0;
    ctx->bytes[1] = 0;
}

/*
 * Update context to reflect the concatenation of another buffer full
 * of bytes.
 */
static void
MD5Update(struct MD5Context *ctx, md5byte const *buf, unsigned len)
{
    UWORD32 t;

    /* Update byte count */

    t = ctx->bytes[0];
    if ((ctx->bytes[0] = t + len) < t)
        ctx->bytes[1]++;	/* Carry from low to high */

    t = 64 - (t & 0x3f);	/* Space available in ctx->in (at least 1) */
    if (t > len) {
        memcpy((md5byte *)ctx->in + 64 - t, buf, len);
        return;
    }
    /* First chunk is an odd size */
    memcpy((md5byte *)ctx->in + 64 - t, buf, t);
    byteSwap(ctx->in, 16);
    MD5Transform(ctx->buf, ctx->in);
    buf += t;
    len -= t;

    /* Process data in 64-byte chunks */
    while (len >= 64) {
        memcpy(ctx->in, buf, 64);
        byteSwap(ctx->in, 16);
        MD5Transform(ctx->buf, ctx->in);
        buf += 64;
        len -= 64;
    }

    /* Handle any remaining bytes of data. */
    memcpy(ctx->in, buf, len);
}

/*
 * Final wrapup - pad to 64-byte boundary with the bit pattern
 * 1 0* (64-bit count of bits processed, MSB-first)
 */
static void
MD5Final(struct MD5Context *ctx, md5byte digest[16])
{
    int count = ctx->bytes[0] & 0x3f;	/* Number of bytes in ctx->in */
    md5byte *p = (md5byte *)ctx->in + count;

    /* Set the first char of padding to 0x80.  There is always room. */
    *p++ = 0x80;

    /* Bytes of padding needed to make 56 bytes (-8..55) */
    count = 56 - 1 - count;

    if (count < 0) {	/* Padding forces an extra block */
        memset(p, 0, count + 8);
        byteSwap(ctx->in, 16);
        MD5Transform(ctx->buf, ctx->in);
        p = (md5byte *)ctx->in;
        count = 56;
    }
    memset(p, 0, count);
    byteSwap(ctx->in, 14);

    /* Append length in bits and transform */
    ctx->in[14] = ctx->bytes[0] << 3;
    ctx->in[15] = ctx->bytes[1] << 3 | ctx->bytes[0] >> 29;
    MD5Transform(ctx->buf, ctx->in);

    byteSwap(ctx->buf, 4);
    memcpy(digest, ctx->buf, 16);
    memset(ctx, 0, sizeof(ctx));	/* In case it's sensitive */
}

#ifndef ASM_MD5

/* The four core functions - F1 is optimized somewhat */

/* #define F1(x, y, z) (x & y | ~x & z) */
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define MD5STEP(f,w,x,y,z,in,s) \
     (w += f(x,y,z) + in, w = (w<<s | w>>(32-s)) + x)

/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data.  MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
static void
MD5Transform(UWORD32 buf[4], UWORD32 const in[16])
{
    register UWORD32 a, b, c, d;

    a = buf[0];
    b = buf[1];
    c = buf[2];
    d = buf[3];

    MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
    MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
    MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
    MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
    MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
    MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
    MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
    MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
    MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
    MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
    MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
    MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
    MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
    MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
    MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
    MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

    MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
    MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
    MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
    MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
    MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
    MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
    MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
    MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
    MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
    MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
    MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
    MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
    MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
    MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
    MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
    MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

    MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
    MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
    MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
    MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
    MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
    MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
    MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
    MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
    MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
    MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
    MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
    MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
    MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
    MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
    MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
    MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

    MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
    MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
    MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
    MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
    MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
    MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
    MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
    MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
    MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
    MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
    MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
    MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
    MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
    MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
    MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
    MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

    buf[0] += a;
    buf[1] += b;
    buf[2] += c;
    buf[3] += d;
}

#endif


//------------ sha1 --------------------------------------------------------------------------------------------

#define SHA1_WIPE_VARIABLES


struct Sha1State
{
    uint32 h0;
    uint32 h1;
    uint32 h2;
    uint32 h3;
    uint32 h4;

    uint64 messageSize;
    unsigned char buffer[64];
};


typedef union
{
    uint8  bytes[64];
    uint32 words[16];
} Sha1Chunk;

static inline uint32 rol32(uint32 value, unsigned int shift)
{
#ifdef GM_CC_MSVC
    return _rotl(value, shift);
#else
    return ((value << shift) | (value >> (32 - shift)));
#endif
}

static inline uint32 sha1Word(Sha1Chunk *chunk, const uint32 position)
{
    return (chunk->words[position & 0xf] = rol32(  chunk->words[(position+13) & 0xf]
                                                 ^ chunk->words[(position+ 8) & 0xf]
                                                 ^ chunk->words[(position+ 2) & 0xf]
                                                 ^ chunk->words[(position)    & 0xf], 1));
}

static inline void sha1Round0(Sha1Chunk *chunk, const uint32 position,
                              uint32 &v, uint32 &w, uint32 &x, uint32 &y, uint32 &z)
{
    z += ((( w & (x ^ y)) ^ y) + chunk->words[position] + 0x5A827999 + rol32(v, 5));
    w = rol32(w, 30);
}

static inline void sha1Round1(Sha1Chunk *chunk, const uint32 position,
                              uint32 &v, uint32 &w, uint32 &x, uint32 &y, uint32 &z)
{
    z += ((( w & (x ^ y)) ^ y) + sha1Word(chunk,position) + 0x5A827999 + rol32(v, 5));
    w = rol32(w, 30);
}

static inline void sha1Round2(Sha1Chunk *chunk, const uint32 position,
                              uint32 &v, uint32 &w, uint32 &x, uint32 &y, uint32 &z)
{
    z += (( w ^ x ^ y) + sha1Word(chunk, position) + 0x6ED9EBA1 + rol32(v, 5));
    w = rol32(w, 30);
}

static inline void sha1Round3(Sha1Chunk *chunk, const uint32 position,
                              uint32 &v, uint32 &w, uint32 &x, uint32 &y, uint32 &z)
{
    z += (((( w | x) & y) | (w & x)) + sha1Word(chunk, position) + 0x8F1BBCDC + rol32(v, 5));
    w = rol32(w, 30);
}

static inline void sha1Round4(Sha1Chunk *chunk, const uint32 position,
                              uint32 &v, uint32 &w, uint32 &x, uint32 &y, uint32 &z)
{
    z += ((w ^ x ^ y) + sha1Word(chunk, position) + 0xCA62C1D6 + rol32(v, 5));
    w = rol32(w, 30);
}

static inline void sha1ProcessChunk(Sha1State *state, const unsigned char *buffer)
{
    // Copy state[] to working vars
    uint32 a = state->h0;
    uint32 b = state->h1;
    uint32 c = state->h2;
    uint32 d = state->h3;
    uint32 e = state->h4;

    uint8 chunkBuffer[64];
    memcpy(chunkBuffer, buffer, 64);

    Sha1Chunk *chunk = reinterpret_cast<Sha1Chunk*>(&chunkBuffer);

    for (int i = 0; i < 16; ++i)
        chunk->words[i] = gfn_fromBigEndian(chunk->words[i]);

    sha1Round0(chunk,  0, a,b,c,d,e); sha1Round0(chunk,  1, e,a,b,c,d); sha1Round0(chunk,  2, d,e,a,b,c); sha1Round0(chunk,  3, c,d,e,a,b);
    sha1Round0(chunk,  4, b,c,d,e,a); sha1Round0(chunk,  5, a,b,c,d,e); sha1Round0(chunk,  6, e,a,b,c,d); sha1Round0(chunk,  7, d,e,a,b,c);
    sha1Round0(chunk,  8, c,d,e,a,b); sha1Round0(chunk,  9, b,c,d,e,a); sha1Round0(chunk, 10, a,b,c,d,e); sha1Round0(chunk, 11, e,a,b,c,d);
    sha1Round0(chunk, 12, d,e,a,b,c); sha1Round0(chunk, 13, c,d,e,a,b); sha1Round0(chunk, 14, b,c,d,e,a); sha1Round0(chunk, 15, a,b,c,d,e);
    sha1Round1(chunk, 16, e,a,b,c,d); sha1Round1(chunk, 17, d,e,a,b,c); sha1Round1(chunk, 18, c,d,e,a,b); sha1Round1(chunk, 19, b,c,d,e,a);
    sha1Round2(chunk, 20, a,b,c,d,e); sha1Round2(chunk, 21, e,a,b,c,d); sha1Round2(chunk, 22, d,e,a,b,c); sha1Round2(chunk, 23, c,d,e,a,b);
    sha1Round2(chunk, 24, b,c,d,e,a); sha1Round2(chunk, 25, a,b,c,d,e); sha1Round2(chunk, 26, e,a,b,c,d); sha1Round2(chunk, 27, d,e,a,b,c);
    sha1Round2(chunk, 28, c,d,e,a,b); sha1Round2(chunk, 29, b,c,d,e,a); sha1Round2(chunk, 30, a,b,c,d,e); sha1Round2(chunk, 31, e,a,b,c,d);
    sha1Round2(chunk, 32, d,e,a,b,c); sha1Round2(chunk, 33, c,d,e,a,b); sha1Round2(chunk, 34, b,c,d,e,a); sha1Round2(chunk, 35, a,b,c,d,e);
    sha1Round2(chunk, 36, e,a,b,c,d); sha1Round2(chunk, 37, d,e,a,b,c); sha1Round2(chunk, 38, c,d,e,a,b); sha1Round2(chunk, 39, b,c,d,e,a);
    sha1Round3(chunk, 40, a,b,c,d,e); sha1Round3(chunk, 41, e,a,b,c,d); sha1Round3(chunk, 42, d,e,a,b,c); sha1Round3(chunk, 43, c,d,e,a,b);
    sha1Round3(chunk, 44, b,c,d,e,a); sha1Round3(chunk, 45, a,b,c,d,e); sha1Round3(chunk, 46, e,a,b,c,d); sha1Round3(chunk, 47, d,e,a,b,c);
    sha1Round3(chunk, 48, c,d,e,a,b); sha1Round3(chunk, 49, b,c,d,e,a); sha1Round3(chunk, 50, a,b,c,d,e); sha1Round3(chunk, 51, e,a,b,c,d);
    sha1Round3(chunk, 52, d,e,a,b,c); sha1Round3(chunk, 53, c,d,e,a,b); sha1Round3(chunk, 54, b,c,d,e,a); sha1Round3(chunk, 55, a,b,c,d,e);
    sha1Round3(chunk, 56, e,a,b,c,d); sha1Round3(chunk, 57, d,e,a,b,c); sha1Round3(chunk, 58, c,d,e,a,b); sha1Round3(chunk, 59, b,c,d,e,a);
    sha1Round4(chunk, 60, a,b,c,d,e); sha1Round4(chunk, 61, e,a,b,c,d); sha1Round4(chunk, 62, d,e,a,b,c); sha1Round4(chunk, 63, c,d,e,a,b);
    sha1Round4(chunk, 64, b,c,d,e,a); sha1Round4(chunk, 65, a,b,c,d,e); sha1Round4(chunk, 66, e,a,b,c,d); sha1Round4(chunk, 67, d,e,a,b,c);
    sha1Round4(chunk, 68, c,d,e,a,b); sha1Round4(chunk, 69, b,c,d,e,a); sha1Round4(chunk, 70, a,b,c,d,e); sha1Round4(chunk, 71, e,a,b,c,d);
    sha1Round4(chunk, 72, d,e,a,b,c); sha1Round4(chunk, 73, c,d,e,a,b); sha1Round4(chunk, 74, b,c,d,e,a); sha1Round4(chunk, 75, a,b,c,d,e);
    sha1Round4(chunk, 76, e,a,b,c,d); sha1Round4(chunk, 77, d,e,a,b,c); sha1Round4(chunk, 78, c,d,e,a,b); sha1Round4(chunk, 79, b,c,d,e,a);

    // Add the working vars back into state
    state->h0 += a;
    state->h1 += b;
    state->h2 += c;
    state->h3 += d;
    state->h4 += e;

    // Wipe variables
#ifdef SHA1_WIPE_VARIABLES
    a = b = c = d = e = 0;
    memset(chunkBuffer, 0, 64);
#endif
}

static inline void sha1InitState(Sha1State *state)
{
    state->h0 = 0x67452301;
    state->h1 = 0xEFCDAB89;
    state->h2 = 0x98BADCFE;
    state->h3 = 0x10325476;
    state->h4 = 0xC3D2E1F0;

    state->messageSize = 0;
}

static inline void sha1Update(Sha1State *state, const unsigned char *data, int64 len)
{
    uint32 rest = static_cast<uint32>(state->messageSize & GM_UINT64_C(63));

    uint64 availableData = static_cast<uint64>(len) + static_cast<uint64>(rest);
    state->messageSize += len;

    if (availableData < GM_UINT64_C(64)) {
        memcpy(&state->buffer[rest], &data[0], len);

    } else {
        int64 i = static_cast<int64>(64 - rest);
        memcpy(&state->buffer[rest], &data[0], static_cast<int32>(i));
        sha1ProcessChunk(state, state->buffer);

        int64 lastI = len - ((len + rest) & GM_INT64_C(63));
        for( ; i < lastI; i += 64)
            sha1ProcessChunk(state, &data[i]);

        memcpy(&state->buffer[0], &data[i], len - i);
    }
}

static inline void sha1FinalizeState(Sha1State *state)
{
    uint64 messageSize = state->messageSize;
    unsigned char sizeInBits[8];
    gfn_toBigEndian(messageSize << 3, sizeInBits);

    sha1Update(state, (const unsigned char *)"\200", 1);

    unsigned char zero[64];
    memset(zero, 0, 64);
    if (static_cast<int>(messageSize & 63) > 56 - 1) {
        sha1Update(state, zero, 64 - 1 - static_cast<int>(messageSize & 63));
        sha1Update(state, zero, 64 - 8);
    } else {
        sha1Update(state, zero, 64 - 1 - 8 - static_cast<int>(messageSize & 63));
    }

    sha1Update(state, sizeInBits, 8);
#ifdef SHA1_WIPE_VARIABLES
    memset(state->buffer, 0, 64);
    memset(zero, 0, 64);
    state->messageSize = 0;
#endif
}

static inline void sha1ToHash(Sha1State *state, unsigned char* buffer)
{
    gfn_toBigEndian(state->h0, buffer);
    gfn_toBigEndian(state->h1, buffer + 4);
    gfn_toBigEndian(state->h2, buffer + 8);
    gfn_toBigEndian(state->h3, buffer + 12);
    gfn_toBigEndian(state->h4, buffer + 16);
}



//------------ CryptographicHash --------------------------------------------------------------------------------------------


#define GM_DISABLE_COPY(Class) \
    Class(const Class &); \
    Class &operator=(const Class &);

class CryptographicHashPrivate;

class CryptographicHash
{
public:
    enum Algorithm {
        Md4,
        Md5,
        Sha1
    };

    CryptographicHash(Algorithm method);
    ~CryptographicHash();

    void reset();

    void addData(const char *data, int length);
    void addData(const vector<char> &data);

    vector<char> result() const;

    static vector<char> hash(const vector<char> &data, Algorithm method);
private:
    GM_DISABLE_COPY(CryptographicHash)
    CryptographicHashPrivate *d;
};

class CryptographicHashPrivate
{
public:
    CryptographicHash::Algorithm method;
    union {
        MD5Context md5Context;
        md4_context md4Context;
        Sha1State sha1Context;
    };
    vector<char> result;
};

/*!
  \class CryptographicHash

  \brief The CryptographicHash class provides a way to generate cryptographic hashes.

  \since 4.3

  \ingroup tools
  \reentrant

  CryptographicHash can be used to generate cryptographic hashes of binary or text data.

  Currently MD4, MD5, and SHA-1 are supported.
*/

/*!
  \enum CryptographicHash::Algorithm

  \value Md4 Generate an MD4 hash sum
  \value Md5 Generate an MD5 hash sum
  \value Sha1 Generate an SHA1 hash sum
*/

/*!
  Constructs an object that can be used to create a cryptographic hash from data using \a method.
*/
CryptographicHash::CryptographicHash(Algorithm method)
    : d(new CryptographicHashPrivate)
{
    d->method = method;
    reset();
}

/*!
  Destroys the object.
*/
CryptographicHash::~CryptographicHash()
{
    delete d;
}

/*!
  Resets the object.
*/
void CryptographicHash::reset()
{
    switch (d->method) {
    case Md4:
        md4_init(&d->md4Context);
        break;
    case Md5:
        MD5Init(&d->md5Context);
        break;
    case Sha1:
        sha1InitState(&d->sha1Context);
        break;
    }
    d->result.clear();
}

/*!
    Adds the first \a length chars of \a data to the cryptographic
    hash.
*/
void CryptographicHash::addData(const char *data, int length)
{
    switch (d->method) {
    case Md4:
        md4_update(&d->md4Context, (const unsigned char *)data, length);
        break;
    case Md5:
        MD5Update(&d->md5Context, (const unsigned char *)data, length);
        break;
    case Sha1:
        sha1Update(&d->sha1Context, (const unsigned char *)data, length);
        break;
    }
    d->result.clear();
}

/*!
  \overload addData()
*/
void CryptographicHash::addData(const vector<char> &data)
{
    if (data.size() > 0)
    {
        addData((& data.front()), data.size());
    }
}

/*!
  Returns the final hash value.

  \sa vector<char>::toHex()
*/
vector<char> CryptographicHash::result() const
{
    if (!d->result.empty())
        return d->result;

    switch (d->method) {
    case Md4: {
        md4_context copy = d->md4Context;
        d->result.resize(MD4_RESULTLEN);
        md4_final(&copy, (unsigned char *)(& d->result.front()));
        break;
    }
    case Md5: {
        MD5Context copy = d->md5Context;
        d->result.resize(16);
        MD5Final(&copy, (unsigned char *)(& d->result.front()));
        break;
    }
    case Sha1: {
        Sha1State copy = d->sha1Context;
        d->result.resize(20);
        sha1FinalizeState(&copy);
        sha1ToHash(&copy, (unsigned char *)(& d->result.front()));
    }
    }
    return d->result;
}

/*!
  Returns the hash of \a data using \a method.
*/
vector<char> CryptographicHash::hash(const vector<char> &data, Algorithm method)
{
    CryptographicHash hash(method);
    hash.addData(data);
    return hash.result();
}

std::string CxCrc::md5HexCode(const std::string &sData)
{
    if (sData.size() > 0)
    {
        std::vector<char> rVec = md5(sData.data(), sData.size());
        return CxString::toHexstring(rVec, false);
    }
    else
    {
        return std::string();
    }
}

string CxCrc::md5HexCode(const std::vector<string> &sLines)
{
    if (sLines.size() > 0)
    {
        std::vector<char> rVec = md5(sLines);
        return CxString::toHexstring(rVec, false);
    }
    else
    {
        return std::string();
    }
}

std::vector<char> CxCrc::md5(const std::vector<string> &sLines)
{
    vector<char> rData;
    if (sLines.size()>0)
    {
        CryptographicHash md(CryptographicHash::Md5);
        for (size_t i = 0; i < sLines.size(); ++i)
        {
            const string & sLine = sLines.at(i);
            if (sLine.size()>0)
            {
                md.addData(sLine.data(), sLine.size());
            }
        }
        rData = md.result();
    }
    return rData;
}


std::vector<char> CxCrc::md5(const char *pData, int iLength)
{
    CryptographicHash md(CryptographicHash::Md5);
    md.addData(pData, iLength);
    vector<char> rData = md.result();
    return rData;
}

string CxCrc::file2md5(const string &sFilePath, int iCodeType)
{
    string r;
    vector<string> sFileContent;
    if (!CxFile::load(sFilePath, sFileContent, 1024 * 2))
    {
        return r;
    }

    if (iCodeType == 0)
    {
        std::vector<char> crcData = CxCrc::md5(sFileContent);
        if (crcData.size() > 0)
            r = string((char *) (&crcData.front()), crcData.size());
    }
    else if (iCodeType == 1)
    {
        r = CxCrc::md5HexCode(sFileContent);
    }

    return r;
}

bool CxCrc::isSameMd5FileData(const string &sFilePath1, const string &sFilePath2)
{
    CxFileSystem::PathInfo pathInfo1 = CxFileSystem::getPathInfo(sFilePath1);
    CxFileSystem::PathInfo pathInfo2 = CxFileSystem::getPathInfo(sFilePath2);
    if (pathInfo1.pathType == CxFileSystem::PathTypeFile && pathInfo1.pathType == pathInfo2.pathType)
    {
        if (pathInfo1.fileSize == pathInfo2.fileSize)
        {
            if (pathInfo1.fileSize > 0)
            {
                return file2md5(sFilePath1) == file2md5(sFilePath2);
            }
            else
            {
                return true;
            }
        }
    }
    return false;
}

uchar CxCrc::calcXOR(const uchar* pData, int iLength)
{
    uchar Res = pData[0];
    for (int i = 1; i < iLength; ++i) {
        Res ^= pData[ i ];
    }
    return Res;
}

uchar CxCrc::crc8(const uchar * ptr, int len)
{
    uchar crc = 0;
    int i=0;

    for ( ; len; len--)
    {
        for (i = 0x80; i != 0; i /= 2)
        {
            if ((crc & 0x80) != 0)
            {
                crc *= 2;
                crc ^= 0x07;
            }
            else
            {
                crc *= 2;
            }
            if ((*ptr & i) != 0)
            {
                crc ^= 0x07;
            }
        }
        ptr++;
    }
    return (~crc);
}

unsigned int CxCrc::calculate_crc16_2(unsigned char *ptr, unsigned char len)
{
    unsigned  char  i;
    unsigned  int  crc_value = 0;
    while(len--)
    {
        for(i=0x80; i!=0; i>>=1 )
        {
            if (crc_value&0x8000)
                crc_value = (crc_value << 1) ^0x8005 ;
            else
                crc_value = crc_value << 1 ;
            if(*ptr&i)
                crc_value^=0x8005 ;
        }
        ptr++;
    }
    return(crc_value);
}

string CxCrc::encodeXorFix(const char *pData, int iDataLength, const char *pKey, int iKeyLength)
{
    if (pData && iDataLength>0 && pKey && iKeyLength>0)
    {
        string r(pData, iDataLength);
        char * rData = (char*)(r.data());
        for(int i=0; i<iDataLength; i++)
            *(rData+i)=(*(pData+i)) ^ (pKey[i%iKeyLength]);
        return r;
    }
    else
    {
        return string();
    }
}

string CxCrc::decodeXorFix(const char *pData, int iDataLength, const char *pKey, int iKeyLength)
{
    if (pData && iDataLength>0 && pKey && iKeyLength>0)
    {
        string r(pData, iDataLength);
        char * rData = (char*)(r.data());
        for(int i=0; i<iDataLength; i++)
            *(rData+i)=(*(pData+i)) ^ (pKey[i%iKeyLength]);
        return r;
    }
    else
    {
        return string();
    }
}

string CxCrc::encodeXorFix(const char *pData, int iLength)
{
    return encodeXorFix(pData, iLength, (const char *)f_modbusTableLo, sizeof(f_modbusTableLo));
}

string CxCrc::decodeXorFix(const char *pData, int iLength)
{
    return decodeXorFix(pData, iLength, (const char *)f_modbusTableLo, sizeof(f_modbusTableLo));
}
