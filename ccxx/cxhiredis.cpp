#include "cxhiredis.h"

#include <ccxx/cxstring.h>
#include <ccxx/cxlog.h>
#include <ccxx/cxtime.h>

#include <hiredis.h>

static char ICS_REDIS_SUCCESS_FLAG_STRING[] = "OK";

static std::string f_sIpAddress = std::string();
static int f_iPort = 0;
static msepoch_t f_dtLoadTime = 0;

static redisContext * f_c = NULL;

static std::string f_sLastCommand = std::string();
static msepoch_t f_sLastTime = 0;
static std::string f_sLastError = std::string();

#define COMMAND_CHECK_RETURN_0(CMD, COND) if (f_c == NULL || !(COND)) { return 0;} else {f_sLastCommand = CMD; f_sLastTime = CxTime::currentSystemTime(); }
#define COMMAND_CHECK_RETURN_E(CMD, COND) if (f_c == NULL || !(COND)) { return std::string();} else {f_sLastCommand = CMD; f_sLastTime = CxTime::currentSystemTime(); }

#define REDISREPLY_ERROR_CHECK(reply) \
    if (reply->type == REDIS_REPLY_ERROR) { f_sLastError = CxString::format("ERROR: CMD[%s] SEND_TIME[%lld] RECV_TIME[%lld] REDIS_REPLY_ERROR[%s]", \
    f_sLastCommand.c_str(), f_sLastTime, CxTime::currentSystemTime(), reply->str); freeReplyObject(reply); cxLogWarn() << f_sLastError;

#define REDISREPLY_ERROR_CHECK_I(reply) REDISREPLY_ERROR_CHECK(reply) return reply->integer; }

#define REDISREPLY_ERROR_CHECK_S(reply) REDISREPLY_ERROR_CHECK(reply) return f_sLastError; }

#define REDISREPLY_RETURN_I(reply) \
    int r = -1; \
    if (reply->type == REDIS_REPLY_INTEGER) \
        r = reply->integer; \
    else if (reply->type == REDIS_REPLY_STATUS || reply->type == REDIS_REPLY_STRING) \
        r = CxString::toInt32(std::string(reply->str, reply->len)); \
    freeReplyObject(reply); \
    return r;

#define REDISREPLY_RETURN_S(reply) \
    std::string r = std::string(); \
    if (reply->type == REDIS_REPLY_INTEGER) \
        r = CxString::toString(reply->integer); \
    else if (reply->type == REDIS_REPLY_STATUS || reply->type == REDIS_REPLY_STRING) \
        r = std::string(reply->str, reply->len); \
    freeReplyObject(reply); \
    return r;

void fn_toChars(const std::string & s, std::vector<char> & vs)
{
    vs.resize(s.size());
    memcpy(&vs.front(), s.c_str(), s.size());
}

void fn_debugArgc(int iIndex, int argc, const char **argv, const size_t *argvlen)
{
    std::string sHex;
    for (int i = 0; i < argc; ++i)
    {
        const char * argv1 = argv[i];
        const size_t argvlen1 = argvlen[i]+1;
        sHex += CxString::toHexstring(argv1, argvlen1) + " ";
    }
    cxLog() << CxString::format("index - %d: ", iIndex) << sHex;
}

int fn_printArgc(int iIndex, int argc, const char **argv, const size_t *argvlen)
{
    fn_debugArgc(iIndex, argc, argv, argvlen);
    redisReply *reply = (redisReply *)redisCommandArgv(f_c, argc, argv, argvlen);
    REDISREPLY_ERROR_CHECK_I(reply)
    REDISREPLY_RETURN_I(reply)
}

bool CxHiredis::checkIsOk(const std::string& sReturn)
{
    return sReturn == std::string(ICS_REDIS_SUCCESS_FLAG_STRING);
}

bool CxHiredis::loadClient(const std::string& sIpAddress, int iPort)
{
    if (! CxString::isValidIp(sIpAddress))
        return false;
    if (! CxString::isValidPort(iPort))
        return false;

    const char *hostname = sIpAddress.c_str();
    int port = iPort;

    redisContext *c;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    c = redisConnectWithTimeout(hostname, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            cxLogWarn() << CxString::format("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            cxLogWarn() << CxString::format("Connection error: can't allocate redis context\n");
        }
        return false;
    }

    f_c = c;
    f_sIpAddress = sIpAddress;
    f_iPort = iPort;
    f_dtLoadTime = CxTime::currentSystemTime();

    return true;
}

void CxHiredis::unloadClient()
{
    if (f_c == NULL)
        return;

    redisFree(f_c);
    f_c = NULL;
    f_sIpAddress = std::string();
    f_iPort = -1;
    f_dtLoadTime = 0;
}

bool CxHiredis::isLoadedClient()
{
    return f_c != NULL;
}

std::string CxHiredis::select(int iIndex)
{
    COMMAND_CHECK_RETURN_E("select", (iIndex>=0 && iIndex<16))
    redisReply *reply = (redisReply *)redisCommand(f_c, "SELECT %d", iIndex);
    REDISREPLY_ERROR_CHECK_S(reply)
    REDISREPLY_RETURN_S(reply)
//    switch(r->type) {
//        case REDIS_REPLY_INTEGER:
//            break; /* Nothing to free */
//        case REDIS_REPLY_ARRAY:
//            if (r->element != NULL) {
//                for (j = 0; j < r->elements; j++)
//                    if (r->element[j] != NULL)
//                        freeReplyObject(r->element[j]);
//                free(r->element);
//            }
//            break;
//        case REDIS_REPLY_ERROR:
//
//        case REDIS_REPLY_STATUS:
//        case REDIS_REPLY_STRING:
//            return std::string(reply->str, reply->len);
//    }
//
//    return r;
}

std::string CxHiredis::ping()
{
    COMMAND_CHECK_RETURN_E("ping", true)
    redisReply *reply = (redisReply *)redisCommand(f_c, "PING");
    REDISREPLY_ERROR_CHECK_S(reply)
    REDISREPLY_RETURN_S(reply)
}

std::string CxHiredis::serverIpAddress()
{
    return f_sIpAddress;
}

int CxHiredis::serverPort()
{
    return f_iPort;
}

msepoch_t CxHiredis::loadedClientTime()
{
    return f_dtLoadTime;
}

std::string CxHiredis::lastCommandError()
{
    return std::string();
}

std::string CxHiredis::lastCommand()
{
    return std::string();
}

msepoch_t CxHiredis::lastSendCommandTime()
{
    return 0;
}

std::string CxHiredis::getKeyType(const std::string & sKey)
{
    if (f_c == NULL || sKey.empty())
        return "none";

    redisReply *reply;

    reply = (redisReply *)redisCommand(f_c, "TYPE %s", sKey.c_str());
    std::string r(reply->str);
    freeReplyObject(reply);

    return r;
}

int CxHiredis::deleteKey(const std::string & sKey)
{
    if (f_c == NULL || sKey.empty())
        return 0;

    redisReply *reply;

    reply = (redisReply *)redisCommand(f_c, "DEL %s", sKey.c_str());
    int r = reply->integer;
    freeReplyObject(reply);

    return r;
}

std::string CxHiredis::stringGet(const std::string & sKey)
{
    if (f_c == NULL || sKey.empty())
        return std::string();

    redisReply *reply;

    reply = (redisReply *)redisCommand(f_c, "GET %s", sKey.c_str());
    std::string r = std::string(reply->str, reply->len);
    freeReplyObject(reply);

    return r;
}

std::string CxHiredis::stringSet(const std::string & sKey, const std::string & sValue)
{
    if (f_c == NULL || sKey.empty())
        return std::string();

    redisReply *reply;

    reply = (redisReply *)redisCommand(f_c, "SET %s %b", sKey.c_str(), sValue.c_str(), sValue.size());
    std::string r = std::string(reply->str, reply->len);
    freeReplyObject(reply);

    return "";
}

int CxHiredis::listPush(const std::string & sKey, const std::string & sValue)
{
    if (f_c == NULL || sKey.empty())
        return 0;

    redisReply *reply;

    reply = (redisReply *)redisCommand(f_c, "LPUSH %s %b", sKey.c_str(), sValue.c_str(), sValue.size());
    int r = reply->integer;
    freeReplyObject(reply);

    return r;
}

int CxHiredis::listPushMultiple(const std::string & sKey, const std::vector<std::string> & sValues)
{
    if (f_c == NULL || sKey.empty() || sValues.size() <= 0)
        return 0;

    std::string sCommand = "LPUSH";
    int iSize = sCommand.size() + 1 + sKey.size() + 1 + CxString::sizeOf(sValues) + sValues.size();
    std::vector<char> buffer;
    std::vector<char *> argv;
    std::vector<size_t> argvlen;
    buffer.resize(iSize);
    char * data = & (buffer.front());
    int iOffset = 0;
    memcpy((void *)(data+iOffset), sCommand.c_str(), sCommand.size()); argv.push_back(data+iOffset); iOffset += sCommand.size(); data[iOffset++] = 0; argvlen.push_back(sCommand.size());
    memcpy((void *)(data+iOffset), sKey.c_str(), sKey.size()); argv.push_back(data+iOffset); iOffset += sKey.size(); data[iOffset++] = 0; argvlen.push_back(sKey.size());
    for (int i = 0; i < sValues.size(); ++i)
    {
        const std::string & sValue = sValues[i];
        memcpy((void *)(data+iOffset), sValue.c_str(), sValue.size()); argv.push_back(data+iOffset); iOffset += sValue.size(); data[iOffset++] = 0; argvlen.push_back(sValue.size());
    }

    redisReply *reply;

    int r = fn_printArgc(0, sValues.size()+1+1, (const char **)(&(argv.front())), (const size_t *)(&(argvlen.front())));

    /*
    int argc = 5;
    const char *argv2[] = { "LPUSH", "list0424v7", "i2a 1", "i2a 2", "i2a 3" };
    const size_t argvlen2[] = { strlen(argv[0]), strlen(argv[1]), strlen(argv[2]), strlen(argv[3]), strlen(argv[4]) };

    r = fn_printArgc(1, argc, argv2, argvlen2);
    /**/
    return r;
}

int CxHiredis::listGetLength(const std::string & sKey)
{
    if (f_c == NULL || sKey.empty())
        return 0;

    redisReply *reply;

    reply = (redisReply *)redisCommand(f_c, "LLEN %s", sKey.c_str());
    int r = reply->integer;
    freeReplyObject(reply);

    return r;
}

std::vector<std::string> CxHiredis::listGet(const std::string & sKey, int iStart, int iStop)
{
    std::vector<std::string> r;

    if (f_c == NULL || sKey.empty())
        return r;

    redisReply *reply;

    reply = (redisReply *)redisCommand(f_c, "LRANGE %s %d %d", sKey.c_str(), iStart, iStop);
    if (reply->type == REDIS_REPLY_ARRAY)
    {
        for (int i = 0; i < reply->elements; i++)
        {
            r.push_back(std::string(reply->element[i]->str, reply->element[i]->len));
        }
    }
    freeReplyObject(reply);
    return r;
}

int CxHiredis::listRemove(const std::string & sKey, int iCount, const std::string & sValue)
{
    if (f_c == NULL || sKey.empty())
        return 0;

    redisReply *reply;

    reply = (redisReply *)redisCommand(f_c, "LREM %s %d %b", sKey.c_str(), iCount, sValue.c_str(), sValue.size());
    int r = reply->integer;
    freeReplyObject(reply);

    return r;
}

std::string CxHiredis::listTrim(const std::string & sKey, int iStart, int iStop)
{
    if (f_c == NULL || sKey.empty())
        return std::string();

    redisReply *reply;

    reply = (redisReply *)redisCommand(f_c, "LTRIM %s %d %d", sKey.c_str(), iStart, iStop);
    std::string r = std::string(reply->str, reply->len);
    freeReplyObject(reply);

    return r;
}

std::string CxHiredis::listSet(const std::string & sKey, int iIndex, const std::string & sValue)
{
    if (f_c == NULL || sKey.empty())
        return std::string();

    redisReply *reply;

    reply = (redisReply *)redisCommand(f_c, "LSET %s %d %b", sKey.c_str(), iIndex, sValue.c_str(), sValue.size());
    std::string r = std::string(reply->str, reply->len);
    freeReplyObject(reply);

    return r;
}

int CxHiredis::hashGetLength(const std::string & sKey)
{
    if (f_c == NULL || sKey.empty())
        return 0;

    redisReply *reply;

    reply = (redisReply *)redisCommand(f_c, "HLEN %s", sKey.c_str());
    int r = reply->integer;
    freeReplyObject(reply);

    return r;
}

int CxHiredis::hashExists(const std::string & sKey, const std::string & sFieldName)
{
    if (f_c == NULL || sKey.empty())
        return 0;

    redisReply *reply;

    reply = (redisReply *)redisCommand(f_c, "HEXISTS %s %s", sKey.c_str(), sFieldName.c_str());
    int r = reply->integer;
    freeReplyObject(reply);

    return r;
}

std::string CxHiredis::hashGet(const std::string & sKey, const std::string & sFieldName)
{
    if (f_c == NULL || sKey.empty())
        return std::string();

    redisReply *reply;

    reply = (redisReply *)redisCommand(f_c, "HGET %s %s", sKey.c_str(), sFieldName.c_str());
    std::string r = std::string(reply->str, reply->len);
    freeReplyObject(reply);

    return r;
}

int CxHiredis::hashSet(const std::string & sKey, const std::string & sFieldName, const std::string & sFieldValue)
{
    return 0;
}

int CxHiredis::hashDelete(const std::string & sKey, std::string & sFieldName)
{
    return 0;
}

int CxHiredis::hashDelete(const std::string & sKey, const std::vector<std::string> & sFieldNames)
{
    return 0;
}

std::map<std::string, std::string> CxHiredis::hashGetAll(const std::string & sKey)
{
    return std::map<std::string, std::string>();
};

std::vector<std::string> CxHiredis::hashGetKeys(const std::string & sKey)
{
    return std::vector<std::string>();
}

std::map<std::string, std::string> CxHiredis::hashGetMultiple(const std::string & sKey, const std::vector<std::string> & sFieldNames)
{
    return std::map<std::string, std::string>();
}

std::string CxHiredis::hashSetMultiple(const std::string & sKey, const std::map<std::string, std::string> & sFieldNameValues)
{
    return "";
}
