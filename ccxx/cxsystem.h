#ifndef CXSYSTEM_H
#define CXSYSTEM_H

#include "cxglobal.h"

class CxSystem
{
public:
    static bool sendEmailInternal(const char *dest, const char *subject, const char *body);

    static std::vector<std::string> getTempDirectories();

    static std::string getTempDirectorie();

    static void setAutoStart(bool value);

private:
    static CxSystem * systemInit();

    static CxSystem * s_system;

};

/*
 * ENDIAN FUNCTIONS
*/
inline void gfn_bswap_helper(const uchar *src, uchar *dest, int size)
{
    for (int i = 0; i < size ; ++i) dest[i] = src[size - 1 - i];
}

/*
 * gfn_bswap(const T src, const uchar *dest);
 * Changes the byte order of \a src from big endian to little endian or vice versa
 * and stores the result in \a dest.
 * There is no alignment requirements for \a dest.
*/
template <typename T> inline void gfn_bswap(const T src, uchar *dest)
{
    gfn_bswap_helper(reinterpret_cast<const uchar *>(&src), dest, sizeof(T));
}

// Used to implement a type-safe and alignment-safe copy operation
// If you want to avoid the memcopy, you must write specializations for this function
template <typename T> inline void gfn_toUnaligned(const T src, uchar *dest)
{
    memcpy(dest, &src, sizeof(T));
}

/* T gfn_fromLittleEndian(const uchar *src)
 * This function will read a little-endian encoded value from \a src
 * and return the value in host-endian encoding.
 * There is no requirement that \a src must be aligned.
*/
#if defined GM_CC_SUN
inline uint64 gfn_fromLittleEndian_helper(const uchar *src, uint64 *dest)
{
    return 0
        | src[0]
        | src[1] * GM_UINT64_C(0x0000000000000100)
        | src[2] * GM_UINT64_C(0x0000000000010000)
        | src[3] * GM_UINT64_C(0x0000000001000000)
        | src[4] * GM_UINT64_C(0x0000000100000000)
        | src[5] * GM_UINT64_C(0x0000010000000000)
        | src[6] * GM_UINT64_C(0x0001000000000000)
        | src[7] * GM_UINT64_C(0x0100000000000000);
}

inline uint32 gfn_fromLittleEndian_helper(const uchar *src, uint32 *dest)
{
    return 0
        | src[0]
        | src[1] * uint32(0x00000100)
        | src[2] * uint32(0x00010000)
        | src[3] * uint32(0x01000000);
}

inline uint16 gfn_fromLittleEndian_helper(const uchar *src, uint16 *dest)
{
    return 0
        | src[0]
        | src[1] * 0x0100;
}

inline int64 gfn_fromLittleEndian_helper(const uchar *src, int64 * dest)
{ return static_cast<int64>(gfn_fromLittleEndian_helper(src, reinterpret_cast<uint64*>(0))); }
inline int32 gfn_fromLittleEndian_helper(const uchar *src, int32 * dest)
{ return static_cast<int32>(gfn_fromLittleEndian_helper(src, reinterpret_cast<uint32*>(0))); }
inline int16 gfn_fromLittleEndian_helper(const uchar *src, int16 * dest)
{ return static_cast<int16>(gfn_fromLittleEndian_helper(src, reinterpret_cast<uint16*>(0))); }

template <class T> inline T gfn_fromLittleEndian(const uchar *src)
{
    return gfn_fromLittleEndian_helper(src, reinterpret_cast<T*>(0));
}

#else
template <typename T> inline T gfn_fromLittleEndian(const uchar *src);
template <> inline uint64 gfn_fromLittleEndian<uint64>(const uchar *src)
{
    return 0
        | src[0]
        | src[1] * GM_UINT64_C(0x0000000000000100)
        | src[2] * GM_UINT64_C(0x0000000000010000)
        | src[3] * GM_UINT64_C(0x0000000001000000)
        | src[4] * GM_UINT64_C(0x0000000100000000)
        | src[5] * GM_UINT64_C(0x0000010000000000)
        | src[6] * GM_UINT64_C(0x0001000000000000)
        | src[7] * GM_UINT64_C(0x0100000000000000);
}

template <> inline uint32 gfn_fromLittleEndian<uint32>(const uchar *src)
{
    return 0
        | src[0]
        | src[1] * uint32(0x00000100)
        | src[2] * uint32(0x00010000)
        | src[3] * uint32(0x01000000);
}

template <> inline uint16 gfn_fromLittleEndian<uint16>(const uchar *src)
{
    return uint16(0
                   | src[0]
                   | src[1] * 0x0100);
}

// signed specializations
template <> inline int64 gfn_fromLittleEndian<int64>(const uchar *src)
{ return static_cast<int64>(gfn_fromLittleEndian<uint64>(src)); }

template <> inline int32 gfn_fromLittleEndian<int32>(const uchar *src)
{ return static_cast<int32>(gfn_fromLittleEndian<uint32>(src)); }

template <> inline int16 gfn_fromLittleEndian<int16>(const uchar *src)
{ return static_cast<int16>(gfn_fromLittleEndian<uint16>(src)); }
#endif

/* This function will read a big-endian (also known as network order) encoded value from \a src
 * and return the value in host-endian encoding.
 * There is no requirement that \a src must be aligned.
*/
#if defined GM_CC_SUN
inline uint64 gfn_fromBigEndian_helper(const uchar *src, uint64 *dest)
{
    return 0
        | src[7]
        | src[6] * GM_UINT64_C(0x0000000000000100)
        | src[5] * GM_UINT64_C(0x0000000000010000)
        | src[4] * GM_UINT64_C(0x0000000001000000)
        | src[3] * GM_UINT64_C(0x0000000100000000)
        | src[2] * GM_UINT64_C(0x0000010000000000)
        | src[1] * GM_UINT64_C(0x0001000000000000)
        | src[0] * GM_UINT64_C(0x0100000000000000);
}

inline uint32 gfn_fromBigEndian_helper(const uchar *src, uint32 * dest)
{
    return 0
        | src[3]
        | src[2] * uint32(0x00000100)
        | src[1] * uint32(0x00010000)
        | src[0] * uint32(0x01000000);
}

inline uint16 gfn_fromBigEndian_helper(const uchar *src, uint16 * des)
{
    return 0
        | src[1]
        | src[0] * 0x0100;
}


inline int64 gfn_fromBigEndian_helper(const uchar *src, int64 * dest)
{ return static_cast<int64>(gfn_fromBigEndian_helper(src, reinterpret_cast<uint64*>(0))); }
inline int32 gfn_fromBigEndian_helper(const uchar *src, int32 * dest)
{ return static_cast<int32>(gfn_fromBigEndian_helper(src, reinterpret_cast<uint32*>(0))); }
inline int16 gfn_fromBigEndian_helper(const uchar *src, int16 * dest)
{ return static_cast<int16>(gfn_fromBigEndian_helper(src, reinterpret_cast<uint16*>(0))); }

template <class T> inline T gfn_fromBigEndian(const uchar *src)
{
    return gfn_fromBigEndian_helper(src, reinterpret_cast<T*>(0));
}

#else
template <class T> inline T gfn_fromBigEndian(const uchar *src);
template<>
inline uint64 gfn_fromBigEndian<uint64>(const uchar *src)
{
    return 0
        | src[7]
        | src[6] * GM_UINT64_C(0x0000000000000100)
        | src[5] * GM_UINT64_C(0x0000000000010000)
        | src[4] * GM_UINT64_C(0x0000000001000000)
        | src[3] * GM_UINT64_C(0x0000000100000000)
        | src[2] * GM_UINT64_C(0x0000010000000000)
        | src[1] * GM_UINT64_C(0x0001000000000000)
        | src[0] * GM_UINT64_C(0x0100000000000000);
}

template<>
inline uint32 gfn_fromBigEndian<uint32>(const uchar *src)
{
    return 0
        | src[3]
        | src[2] * uint32(0x00000100)
        | src[1] * uint32(0x00010000)
        | src[0] * uint32(0x01000000);
}

template<>
inline uint16 gfn_fromBigEndian<uint16>(const uchar *src)
{
    return uint16( 0
                    | src[1]
                    | src[0] * uint16(0x0100));
}


// signed specializations
template <> inline int64 gfn_fromBigEndian<int64>(const uchar *src)
{ return static_cast<int64>(gfn_fromBigEndian<uint64>(src)); }

template <> inline int32 gfn_fromBigEndian<int32>(const uchar *src)
{ return static_cast<int32>(gfn_fromBigEndian<uint32>(src)); }

template <> inline int16 gfn_fromBigEndian<int16>(const uchar *src)
{ return static_cast<int16>(gfn_fromBigEndian<uint16>(src)); }
#endif
/*
 * T gfn_bswap(T source).
 * Changes the byte order of a value from big endian to little endian or vice versa.
 * This function can be used if you are not concerned about alignment issues,
 * and it is therefore a bit more convenient and in most cases more efficient.
*/
template <typename T> T gfn_bswap(T source);

#ifdef __GLIBC__
template <> inline uint64 gfn_bswap<uint64>(uint64 source)
{
//    return bswap_64(source);
}
template <> inline uint32 gfn_bswap<uint32>(uint32 source)
{
//    return bswap_32(source);
}
template <> inline uint16 gfn_bswap<uint16>(uint16 source)
{
//    return bswap_16(source);
}
#else
template <> inline uint64 gfn_bswap<uint64>(uint64 source)
{
    return 0
        | ((source & GM_UINT64_C(0x00000000000000ff)) << 56)
        | ((source & GM_UINT64_C(0x000000000000ff00)) << 40)
        | ((source & GM_UINT64_C(0x0000000000ff0000)) << 24)
        | ((source & GM_UINT64_C(0x00000000ff000000)) << 8)
        | ((source & GM_UINT64_C(0x000000ff00000000)) >> 8)
        | ((source & GM_UINT64_C(0x0000ff0000000000)) >> 24)
        | ((source & GM_UINT64_C(0x00ff000000000000)) >> 40)
        | ((source & GM_UINT64_C(0xff00000000000000)) >> 56);
}

template <> inline uint32 gfn_bswap<uint32>(uint32 source)
{
    return 0
        | ((source & 0x000000ff) << 24)
        | ((source & 0x0000ff00) << 8)
        | ((source & 0x00ff0000) >> 8)
        | ((source & 0xff000000) >> 24);
}

template <> inline uint16 gfn_bswap<uint16>(uint16 source)
{
    return uint16( 0
                    | ((source & 0x00ff) << 8)
                    | ((source & 0xff00) >> 8) );
}
#endif // __GLIBC__

// signed specializations
template <> inline int64 gfn_bswap<int64>(int64 source)
{
    return gfn_bswap<uint64>(uint64(source));
}

template <> inline int32 gfn_bswap<int32>(int32 source)
{
    return gfn_bswap<uint32>(uint32(source));
}

template <> inline int16 gfn_bswap<int16>(int16 source)
{
    return gfn_bswap<uint16>(uint16(source));
}

#if GM_BYTE_ORDER == GM_BIG_ENDIAN

template <typename T> inline T gfn_toBigEndian(T source)
{ return source; }
template <typename T> inline T gfn_fromBigEndian(T source)
{ return source; }
template <typename T> inline T gfn_toLittleEndian(T source)
{ return gfn_bswap<T>(source); }
template <typename T> inline T gfn_fromLittleEndian(T source)
{ return gfn_bswap<T>(source); }
template <typename T> inline void gfn_toBigEndian(T src, uchar *dest)
{ gfn_toUnaligned<T>(src, dest); }
template <typename T> inline void gfn_toLittleEndian(T src, uchar *dest)
{ gfn_bswap<T>(src, dest); }
#else // GM_LITTLE_ENDIAN

template <typename T> inline T gfn_toBigEndian(T source)
{ return gfn_bswap<T>(source); }
template <typename T> inline T gfn_fromBigEndian(T source)
{ return gfn_bswap<T>(source); }
template <typename T> inline T gfn_toLittleEndian(T source)
{ return source; }
template <typename T> inline T gfn_fromLittleEndian(T source)
{ return source; }
template <typename T> inline void gfn_toBigEndian(T src, uchar *dest)
{ gfn_bswap<T>(src, dest); }
template <typename T> inline void gfn_toLittleEndian(T src, uchar *dest)
{ gfn_toUnaligned<T>(src, dest); }

#endif // GM_BYTE_ORDER == GM_BIG_ENDIAN

template <> inline uint8 gfn_bswap<uint8>(uint8 source)
{
    return source;
}

#endif // CXSYSTEM_H
