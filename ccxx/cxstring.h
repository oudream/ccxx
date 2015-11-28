#ifndef CXSTRING_H
#define CXSTRING_H

#include "cxglobal.h"

class CxString
{
public:
    /**
     * @brief convert to string
     * @param b
     * @return
     */
    static std::string toString(const bool& b);

    static std::string toString(const int& i);

    static std::string toString(const uint32& n);

    static std::string toString(const long& n);

    static std::string toString(const ulong& n);

    static std::string toString(const int64& n);

    static std::string toString(const uint64& n);

    static std::string toString(const float& f);

    static std::string toString(const double& d);

    static std::string toString(const std::string& s);

    /**
     * @brief convert from string
     * @param b
     * @return
     */
    static bool toBoolean(const std::string& sSource);

    static int toInt32(const std::string& sSource);

    static uint toUint32(const std::string& sSource);

    static int64 toInt64(const std::string& sSource);

    static uint64 toUint64(const std::string& sSource);

    static double toDouble(const std::string& sSource);

    static float toFloat(const std::string& sSource);

    static bool toBoolean(const std::string& sSource, bool * bOk);

    static int toInt32(const std::string& sSource, bool * bOk);

    static uint toUint32(const std::string& sSource, bool * bOk);

    static int64 toInt64(const std::string& sSource, bool * bOk);

    static uint64 toUint64(const std::string& sSource, bool * bOk);

    static double toDouble(const std::string& sSource, bool * bOk);

    static float toFloat(const std::string& sSource, bool * bOk);

    static bool fromString(const std::string& sSource, const bool & b, bool * bOk = NULL);

    static int fromString(const std::string& sSource, const int & i, bool * bOk = NULL);

    static uint32 fromString(const std::string& sSource, const uint32 & n, bool * bOk = NULL);

    static int64 fromString(const std::string& sSource, const int64 & n, bool * bOk = NULL);

    static uint64 fromString(const std::string& sSource, const uint64 & n, bool * bOk = NULL);

    static double fromString(const std::string& sSource, const double & d, bool * bOk = NULL);

    static float fromString(const std::string& sSource, const float & f, bool * bOk = NULL);

    static std::string fromString(const std::string& sSource, const std::string & s, bool * bOk = NULL);

    static bool setFromString(const std::string& sSource, bool & b);

    static bool setFromString(const std::string& sSource, int & i);

    static bool setFromString(const std::string& sSource, uint32 & n);

    static bool setFromString(const std::string& sSource, int64 & n);

    static bool setFromString(const std::string& sSource, uint64 & n);

    static bool setFromString(const std::string& sSource, double & d);

    static bool setFromString(const std::string& sSource, float & f);

    static bool setFromString(const std::string& sSource, std::string & s);

    /**
     * @brief isvalidBoolean
     * @param sSource
     * @return
     */
    static bool isvalidBoolean(const std::string& sSource);

    static bool isvalidInteger(const std::string& sSource);

    static bool isvalidLong(const std::string& sSource);

    static bool isvalidFloat(const std::string& sSource);

    static bool isvalidDouble(const std::string& sSource);

    static inline bool isvalid(const std::string& sSource, bool) { return isvalidBoolean(sSource); }

    static bool isvalid(const std::string& sSource, int) { return isvalidInteger(sSource); }

    static bool isvalid(const std::string& sSource, uint32) { return isvalidInteger(sSource); }

    static bool isvalid(const std::string& sSource, const int64&) { return isvalidLong(sSource); }

    static bool isvalid(const std::string& sSource, const uint64&) { return isvalidLong(sSource); }

    static bool isvalid(const std::string& sSource, const float&) { return isvalidFloat(sSource); }

    static bool isvalid(const std::string& sSource, const double&) { return isvalidDouble(sSource); }

    static bool isvalid(const std::string&, std::string&) { return true; }

    /**
     * @brief toHexstring
     * @param pData
     * @param iLength
     * @param bHasEmptyChar
     * @return
     */
    static std::string toHexstring(const char* pData, int iLength, bool bHasEmptyChar = true);

    static std::string toHexstring(const uchar* pData, int iLength, bool bHasEmptyChar = true);

    static std::string toHexstring(const std::vector<char>& data, bool bHasEmptyChar = true);

    static std::string toHexstring(const std::vector<uchar>& data, bool bHasEmptyChar = true);

    static std::string toHexstring(const char& v);

    static std::string toHexstring(const uchar& v);

    static std::string toHexstring(const short& v);

    static std::string toHexstring(const ushort& v);

    static std::string toHexstring(const int& v);

    static std::string toHexstring(const uint32& v);

    static std::string toHexstring(const float& v);

    static std::string toHexstring(const double& v);

    static std::string toHexstring(const int& v, bool bHasHead);

    static std::string toHexstring(const uint32& v, bool bHasHead);

    static std::vector<uchar> fromHexstring(const std::string& s);

    static std::vector<char> hexToData(const std::string& s);

    static int hexToInt32(const std::string& s, bool * bOk = NULL);

    static uint hexToUint32(const std::string& s, bool * bOk = NULL);

    static int64 hexToInt64(const std::string& s, bool * bOk = NULL);

    static uint64 hexToUint64(const std::string& s, bool * bOk = NULL);

    /**
     * @brief remove
     * @param s
     * @param c
     */
    static void remove(std::string& s, char c);

    /**
     * @brief split
     * @param ss
     * @param cSplitCharacter
     * @param bHasEmptyString
     * @return
     */
    static std::vector<std::string> split(const std::string& ss, char cSplitCharacter = CxGlobal::lineCharacter, bool bHasEmptyString = false);

    static std::vector<std::string> split(const std::string& ss, const std::string& sSplitString, bool bHasEmptyString = false);

    static std::vector<std::string> split(const std::vector<std::string>& ss, const std::string& sSplitString, bool bHasEmptyString = false);

    static std::vector<std::string> splitCase(const std::string& ss, const std::string& sSplitString, bool bHasEmptyString = false);

    static void splitCase(int mode,char *pBuf,int num,std::string &sVal,std::string &split);

    static void splitCase(std::vector<std::vector<char> > &v,std::string &sVal,std::string &split);

    //must  : k1=v1,k2=v2,k3=v3
    static std::map<std::string, std::string> splitToMap(const std::string& ss, char cMid, char cSplitCharacter, bool bTrim = true);

    //maybe : k1=v1,k2,k3,k4=v4
    //trim(k) , do not trim v
    static std::map<std::string, std::string> splitToMap_mix(const std::string& ss, char cMid, char cSplitCharacter);

    //must : v1=k1,v2=k2,v3=k3
    static std::map<std::string, std::string> splitToMap_reverse(const std::string& ss, char cMid, char cSplitCharacter, bool bTrim = true);

    static std::vector<std::vector<std::string> > splitToLines(const std::string& ss, char cMid, char cSplitCharacter);

    static std::vector<std::map<std::string, std::string> > splitToLines(const std::vector<std::string>& ss, char cMid, char cSplitCharacter);

    static std::vector<std::map<std::string, std::string> > sortToLines(const std::map<std::string, std::map<std::string, std::string> >& ss);

    /**
     * @brief join
     * @param ss
     * @param cJoin
     * @return
     */
    static std::string join(const std::vector<std::string>& ss, char cJoin = CxGlobal::lineCharacter);

    static std::string join(const std::vector<std::string>& ss, const std::string & sJoin);

    static std::string join(const std::map<std::string, std::string> & pairs, const std::string& sMid, const std::string& sJoin);

    static std::vector<std::string> joinToStringList(const std::map<std::string, std::string> & pairs, const std::string& sMid);

    static std::string join(const std::vector<std::vector<std::string> > & lines, char cMid, char cSplitCharacter);

    /**
     * @brief replace
     * @param strBase
     * @param cSrc
     * @param cDes
     * @return
     */
    static std::string replace(const std::string& strBase, char cSrc, char cDes);
    
    static std::string replace(const std::string& strBase, const std::string& strSrc, const std::string& strDes);

    static std::string replaceCase(const std::string& strBase, const std::string& strSrc, const std::string& strDes);

    /**
     * @brief trim
     * @param s
     * @return
     */
    static std::string trim(const std::string& s);

    static std::string trim(const std::string& s, const char cDelete);

    static std::string trim(const std::string& s, const std::string& sDelete);

    /**
     * @brief toLowerSelf
     * @param r
     */
    static void toLowerSelf(std::string & r);

    static std::string toLower(const std::string &s);

    static void toUpperSelf(std::string & s);

    static std::string toUpper(const std::string &s);

    /**
     * @brief equalCase
     * @param s1
     * @param s2
     * @return
     */
    static bool equalCase(const std::string& s1, const std::string& s2);

    static bool equalIgnoreAll(const std::string& s1, const std::string& s2);

    /**
     * @brief findLeftCase
     * @param sMaster
     * @param sSub
     * @return
     */
    static size_t findLeftCase(const std::string& sMaster, const std::string& sSub);
    static size_t findLeftCase(const std::string& sMaster, const std::vector<std::string>& sSubs);

    static size_t findRightCase(const std::string& sMaster, const std::string& sSub);

    inline static bool contain(const std::string& sMaster, const std::string& sSub) { return exist(sMaster, sSub); }
    static bool exist(const std::string& sMaster, const std::string& sSub);

    inline static bool containCase(const std::string& sMaster, const std::string& sSub) { return existCase(sMaster, sSub); }
    static bool existCase(const std::string& sMaster, const std::string& sSub);
    static bool existCase(const std::string& sMaster, const std::vector<std::string>& sSubs);

    static bool beginWith(const std::string& sMaster, const std::string& sSub);

    static bool beginWithCase(const std::string& sMaster, const std::string& sSub);

    static bool endWith(const std::string& sMaster, const std::string& sSub);

    static bool endWithCase(const std::string& sMaster, const std::string& sSub);

    /**
     * @brief format
     * @param sFormat
     * @return
     */
    static std::string format(const char * sFormat, ...);

    /**
     * \brief token
     * \param s
     * \param cToken
     * \param ok
     * \return
     * \sample : string s1 = "aaaaa.12341234.bbbbbbbbb"; string r1 = CxString::tokenxx(s1, '.');
     * token result=aaaaa input string=12341234.bbbbbbbbb
     * tokenLeft result=12341234.bbbbbbbbb input string=aaaaa
     * tokenRight result=bbbbbbbbb input string=aaaaa.12341234
     */
    static std::string token(std::string & s, char cToken, bool * ok = NULL);

    static std::string token(std::string & s, const std::string & sToken, bool * ok = NULL);

    static std::string tokenLeft(std::string & s, char cToken, bool * ok = NULL);

    static std::string tokenLeft(std::string & s, const std::string & sToken, bool * ok = NULL);

    static std::string tokenRight(std::string & s, char cToken, bool * ok = NULL);

    static std::string tokenRight(std::string & s, const std::string & sToken, bool * ok = NULL);

    /**
     * find start string quote_l -> end string quote_r
     * fine pair sample : (), [], <>
     */
    static std::string unquote(const std::string & s, char quote_l, char quote_r);

    static std::string unquote(const std::string & s, char c);

    static std::string unquote(const std::string & s, const std::string & quote_l, const std::string & quote_r);

    static bool find(const std::string &src,const std::string &start,const std::string &end,int &pos,int &len,std::string &des);

    ///
    /// \brief lengthCString
    /// \param n
    /// \param ... : const char * , const char *, ...
    /// \return strlen() * n + n
    ///
    static int lengthCString(int n, ...);

    static int copyCString(char * dest, int n, ...);

    static uint32 toUint32(char *pBuff,int iLenth, bool bInverse = true);

    static uint32 toUint32(uchar *pBuff,int iLenth,bool bInverse = true);

//  CxIpAddress::ipV4ToString
//    static std::string ipToString(uint32 val);

//  CxIpAddress::ipV4ToInt32
//    static uint32 ipToUint32(std::string val);

    static bool isValidIp(const std::string &sIp);

    static bool isValidPort(int iPort);

    static bool isValidPath(const std::string & sPath);

    static bool isValidFullPath(const std::string & sPath);

    static bool isValidFileName(const std::string & sPath);

    static bool isValidPathName(const std::string & sPath);

    static bool isValidCodeName(const std::string & sName);

    static int copyTo(const std::vector<std::string> & sLines, char * dest, int iLength);

    static int sizeOf(const std::vector<std::string> & sLines);

    inline static std::string newString(const std::string & s) { return std::string(s.data(), s.size()); }

    inline static std::string newString(const char * pText, size_t iMaxSize) { size_t iSize = strlen(pText); if (iMaxSize>iSize) return std::string(pText, iSize); else return std::string(pText, iMaxSize); }

    static int countOfString(const std::string & src, const std::string & sub);

    static int countOfString(const std::string & src, char sub);

private:
    static std::map<std::string, std::string> doSplitToMap_trim(const std::string& s, char cMid, char cSplitCharacter);

    static std::map<std::string, std::string> doSplitToMap_notrim(const std::string& s, char cMid, char cSplitCharacter);

    static std::map<std::string, std::string> doSplitToMap_reverse_trim(const std::string& s, char cMid, char cSplitCharacter);

    static std::map<std::string, std::string> doSplitToMap_reverse_notrim(const std::string& s, char cMid, char cSplitCharacter);


};

class CxStringC
{
public:
    //***
    //be care begin : malloc
    //***
    /**
     * Duplicate null terminated text into the heap.
     * @param text to duplicate.
     * @return duplicate copy of text allocated from heap.
     */
    static char * create(const char *text);

    /**
     * Duplicate null terminated text of specific size to heap.
     * @param text to duplicate.
     * @param size of text, maximum space allocated.
     * @return duplicate copy of text allocated on heap.
     */
    static char * createLeft(const char *text, size_t size);

    inline static char * createRight(const char *text, size_t size)
        {return create(pos(text, -(signed)size));}

    inline static char * create(const char *text, size_t offset, size_t len)
        {return createLeft(pos(text, offset), len);}
    //***
    //be care end : malloc
    //***

    /**
     * Safely set a null terminated string buffer in memory.  If the text
     * is too large to fit into the buffer, it is truncated to the size.
     * @param buffer to set.
     * @param size of buffer ( = string.size() , include \0 at the end).  Includes null byte at end of string.
     * @param text to set in buffer.
     * @return pointer to text buffer.
     */
    static char * set(char *buffer, size_t size, const char *text);

    /**
     * Safely set a null terminated string buffer in memory.  If the text
     * is too large to fit into the buffer, it is truncated to the size.
     * @param buffer to set.
     * @param size of buffer.  Includes null byte at end of string.
     * @param text to set in buffer.
     * @param max size of text to set.
     * @return pointer to text buffer.
     */
    static char * set(char *buffer, size_t size, const char *text, size_t max);

    /**
     * A thread-safe token parsing routine for null terminated strings.  This
     * is related to strtok, but with safety checks for NULL values and a
     * number of enhancements including support for quoted text that may
     * contain token separators within quotes.  The text string is modified
     * as it is parsed.
     * @param text string to examine for tokens.
     * @param last token position or set to NULL for start of string.
     * @param list of characters to use as token separators.
     * @param quote pairs of characters for quoted text or NULL if not used.
     * @param end of line marker characters or NULL if not used.
     * @return token extracted from string or NULL if no more tokens found.
     * e.g. :
            char buff[33];
            char *tokens = NULL;
            unsigned count = 0;
            const char *tp;
            const char *array[5];

            memset(buff, ' ', 32);
            CxStringC::set(buff, sizeof(buff), "this is \"a test\"");
            while(NULL != (tp = CxString::token(buff, &tokens, " ", "\"\"")) && count < 4)
                array[count++] = tp;
     * array[0] == "this";
     * array[1] == "is";
     * array[2] == "a test";
     */
    static char * token(char *text, char **last, const char *list, const char *quote = NULL, const char *end = NULL);

    /**
     * Unquote a quoted null terminated string.  Returns updated string
     * position and replaces trailing quote with null byte if quoted.
     * @param text to examine.
     * @param quote pairs of character for open and close quote.
     * @return new text pointer if quoted, NULL if unchanged.
     * e.g. : unquote("[abc]", "[]()") == "abc"
     */
    static char *unquote(char *text, const char *quote);

    /**
     * Depreciated case insensitive string comparison function.
     * @param text1 to compare.
     * @param text2 to compare.
     * @return 0 if equal, >0 if text1 > text2, <0 if text1 < text2.
     */
    static int compareCase(const char *text1, const char *text2);

    /**
     * Return start of string after characters to trim from beginning.
     * This function does not modify memory.
     * @param text buffer to examine.
     * @param list of characters to skip from start.
     * @return position in text past lead trim.
     */
    static char * trimLeft(char *text, const char *list);

    /**
     * Strip trailing characters from the text string.  This function will
     * modify memory.
     * @param text buffer to examine.
     * @param list of characters to chop from trailing end of string.
     * @return pointer to text buffer.
     */
    static char * trimRight(char *text, const char *list);

    /**
     * Skip lead and remove trailing characters from a text string.  This
     * function will modify memory.
     * @param text buffer to examine.
     * @param list of characters to trim and chop.
     * @return position in text past lead trim.
     */
    static char * trim(char *text, const char *list);

    /**
     * Offset until next occurrence of character in a text or length.
     * @param text buffer to examine.
     * @param list of characters to search for.
     * @return offset to next occurrence or length of string.
     */
    static size_t strcspnRight(char *text, const char *list);

    /**
     * inside after lead characters in a null terminated string.
     * @param text buffer to examine.
     * @param list of characters to search for.
     * @return pointer to first part of string past insided characters.
     */
    static char * inside(char *text, const char *list);

    /**
     * inside after lead characters in a null terminated string.
     * @param text buffer to examine.
     * @param list of characters to search for.
     * @return pointer to first part of string past insided characters.
     */
    static char * insideRight(char *text, const char *list);

    /**
     * Skip after lead characters in a null terminated string.
     * @param text pointer to start at.
     * @param list of characters to skip when found.
     * @return pointer to first part of string past skipped characters.
     */
    static char * skip(char *text, const char *list);

    /**
     * Skip before trailing characters in a null terminated string.
     * @param text pointer to start at.
     * @param list of characters to skip when found.
     * @return pointer to last part of string past skipped characters.
     */
    static char * skipRight(char *text, const char *list);

    /**
     * Dump hex data to a string buffer.
     * @param binary memory to dump.
     * @param string to save into.
     * @param format string to convert with.
     * @return number of bytes processed.
     */
    static unsigned hexdump(const unsigned char *binary, char *string, const char *format);

    /**
     * Pack hex data from a string buffer.
     * @param binary memory to pack.
     * @param string to save into.
     * @param format string to convert with.
     * @return number of bytes processed.
     */
    static unsigned hexpack(unsigned char *binary, const char *string, const char *format);

    static unsigned hexsize(const char *format);

    /**
     * Standard radix 64 encoding.
     * @param string of encoded text save into.
     * @param binary data to encode.
     * @param size of binary data to encode.
     * @param width of string buffer for data if partial supported.
     * @return number of bytes encoded.
     */
    static size_t b64encode(char *string, const uint8 *binary, size_t size, size_t width = 0);

    /**
     * Standard radix 64 decoding.
     * @param binary data to save.
     * @param string of encoded text.
     * @param size of destination buffer.
     * @return number of bytes actually decoded.
     */
    static size_t b64decode(uint8 *binary, const char *string, size_t size);

private:
    /**
     * Compute position in string.
     * @param text of string.
     * @param offset from start, negative values from end.
     * @return pointer to string position.
     */
    static const char * pos(const char *text, ssize_t offset);

};

/*** toString
atof        Convert string to double (function )
atoi        Convert string to integer (function )
atol        Convert string to long integer (function )
strtod      Convert string to double (function )
strtol      Convert string to long integer (function )
strtoul     Convert string to unsigned long integer (function )
 */

/*** search
//Returns a pointer to the first occurrence
//If the value is not found, the function returns a null pointer.
void * memchr (void * ptr, int value, size_t num );
char * strchr (char * str, int character );
char * strrchr (char * str, int character ); //Returns a pointer to the last occurrence
char * strstr (       char * str1, const char * str2 );

//返回字符串中第一个在指定字符组中出现的字符下标+1
size_t strcspn ( const char * str, const char * list );
char * strpbrk ( char * str1, const char * list );
static char * inside(char *text, const char *list);
static char * insideRight(char *text, const char *list);
//返回字符串中第一个不在指定字符组中出现的字符下标+1
size_t strspn ( const char * str, const char * list );
static char * skip(char *text, const char *list);
static char * skipRight(char *text, const char *list);
 */

#endif // CXSTRING_H
