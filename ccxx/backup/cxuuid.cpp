#include "cxuuid.h"

#include "cxstring.h"
#include "cxtime.h"
#include "cxmath.h"

using namespace std;



#if defined(GM_OS_WIN)
#include <objbase.h>
#if defined(_MSC_VER)

string CxUuid::createGuidString()
{
    return guidToString(createGuid());
}

GUID CxUuid::createGuid()
{
    GUID guid;
    CoCreateGuid(&guid);
    return guid;
}

string CxUuid::guidToString(const GUID &guid)
{
    char buf[64] = {0};
        _snprintf_s(
            buf,
            sizeof(buf),
             "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
            guid.Data1, guid.Data2, guid.Data3,
            guid.Data4[0], guid.Data4[1],
            guid.Data4[2], guid.Data4[3],
            guid.Data4[4], guid.Data4[5],
            guid.Data4[6], guid.Data4[7]);
        return std::string(buf);
}

string CxUuid::createRandString()
{
    string r = CxString::toString(CxTime::currentMsepoch());

    int i1 = (CxMath::random(255) << 24) & 0xFF000000;
    int i2 = (CxMath::random(255) << 24) & 0x00FF0000;
    int i3 = (CxMath::random(255) << 24) & 0x0000FF00;
    int i4 = (CxMath::random(255) << 24) & 0x000000FF;
    uint32 iRand = i1 + i2 + i3 + i4;


    char str[10+1];
    sprintf(str, "%010u", iRand);

    return r + string(str);
}

#else

string CxUuid::createGuidString()
{
    return guidToString(createGuid());
}

GUID CxUuid::createGuid()
{
    GUID guid;
    CoCreateGuid(&guid);
    return guid;
}

string CxUuid::guidToString(const GUID &guid)
{
    char buf[64] = {0};
        snprintf(
            buf,
            sizeof(buf),
             "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
            guid.Data1, guid.Data2, guid.Data3,
            guid.Data4[0], guid.Data4[1],
            guid.Data4[2], guid.Data4[3],
            guid.Data4[4], guid.Data4[5],
            guid.Data4[6], guid.Data4[7]);
        return std::string(buf);
}

string CxUuid::createRandString()
{
    string r = CxString::toString(CxTime::currentMsepoch());

    int i1 = (CxMath::random(255) << 24) & 0xFF000000;
    int i2 = (CxMath::random(255) << 24) & 0x00FF0000;
    int i3 = (CxMath::random(255) << 24) & 0x0000FF00;
    int i4 = (CxMath::random(255) << 24) & 0x000000FF;
    uint32 iRand = i1 + i2 + i3 + i4;


    char str[10+1];
    sprintf(str, "%010u", iRand);

    return r + string(str);

/*
    srand((int)time(0));   //设置时间种子
    int num = rand()%100000;   //生成100000内的随机数
//    printf("rand num:%6d\n", num);

    struct tm *p;
    time_t second;
    time(&second);

    p = localtime(&second);//获取当前时间

    char buf[64] = {0};

    sprintf(buf, "%d-%d-%d %d:%d:%d_%06d", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday,
        p->tm_hour, p->tm_min, p->tm_sec, num);  //格式化时间和随机数字符串，存放在buf内

    return std::string(buf);

//    printf("guid:%s\n", buf);  //输出guid到控制台

*/
}

/*
//Rpcrt4.dll
//Rpcrt4.lib
#include <rpcdce.h>

struct uuuid_t {
    UUID uuid;
};

static struct uuuid_t* uuuid_new(void)
{
    return (struct uuuid_t*) malloc(sizeof(struct uuuid_t));
}

static void _uuuid_create(struct uuuid_t** uuuid, int* status, int nil)
{
    struct uuuid_t* u;
    RPC_STATUS st;

    u = uuuid_new();

    if (!u) {
        *status = UUUID_ERR;
        return;
    }

    if (nil)
        st = UuidCreateNil(&u->uuid);
    else
        st = UuidCreate(&u->uuid);

    if (st != RPC_S_OK) {
        uuuid_free(u);
        *status = UUUID_ERR;
        return;
    }

    *uuuid = u;
    *status = UUUID_OK;
}

void uuuid_create(struct uuuid_t** uuuid, int* status)
{
    _uuuid_create(uuuid, status, 0);
}

void uuuid_create_nil(struct uuuid_t** uuuid, int* status)
{
    _uuuid_create(uuuid, status, 1);
}

int uuuid_is_nil(struct uuuid_t* uuuid, int* status)
{
    RPC_STATUS st;
    int ret;

    ret = UuidIsNil(&uuuid->uuid, &st);

    if (st != RPC_S_OK) {
        *status = UUUID_ERR;
        return -1;
    }

    *status = UUUID_OK;

    return ret;
}

int uuuid_compare(struct uuuid_t* u1, struct uuuid_t* u2, int* status)
{
    RPC_STATUS st;
    int ret;

    ret = UuidCompare(&u1->uuid, &u2->uuid, &st);

    if (st != RPC_S_OK) {
        *status = UUUID_ERR;
        return -1;
    }

    *status = UUUID_OK;

    return ret;
}

void uuuid_to_string(struct uuuid_t* uuuid, char** out, int* status)
{
    RPC_STATUS st;

    st = UuidToString(&uuuid->uuid, (unsigned char**) out);

    if (st == RPC_S_OK)
        *status = UUUID_OK;
    else
        *status = UUUID_ERR;
}

void uuuid_from_string(const char* in, struct uuuid_t** uuuid, int* status)
{
    RPC_STATUS st;
    struct uuuid_t* u;

    u = uuuid_new();

    st = UuidFromString((unsigned char*) in, &u->uuid);

    if (st != RPC_S_OK) {
        uuuid_free(u);
        *status = UUUID_ERR;
        return;
    }

    *uuuid = u;
    *status = UUUID_OK;
}

void uuuid_free(struct uuuid_t* uuuid)
{
    free(uuuid);
}
*/

#endif
//#else
//#if defined(__FreeBSD__)

#include </usr/include/uuid.h>

struct uuuid_t {
    uuid_t uuid;
};

static struct uuuid_t* uuuid_new(void)
{
    return (struct uuuid_t*) malloc(sizeof(struct uuuid_t));
}

static void _uuuid_create(struct uuuid_t** uuuid, int* status, int nil)
{
    struct uuuid_t* u;
    uint32_t st;

    u = uuuid_new();

    if (!u) {
        *status = UUUID_ERR;
        return;
    }

    if (nil)
        uuid_create_nil(&u->uuid, &st);
    else
        uuid_create(&u->uuid, &st);

    if (st != uuid_s_ok) {
        uuuid_free(u);
        *status = UUUID_ERR;
        return;
    }

    *uuuid = u;
    *status = UUUID_OK;
}

void uuuid_create(struct uuuid_t** uuuid, int* status)
{
    _uuuid_create(uuuid, status, 0);
}

void uuuid_create_nil(struct uuuid_t** uuuid, int* status)
{
    _uuuid_create(uuuid, status, 1);
}

int uuuid_is_nil(struct uuuid_t* uuuid, int* status)
{
    uint32_t st;
    int ret;

    ret = uuid_is_nil(&uuuid->uuid, &st);

    if (st != uuid_s_ok) {
        *status = UUUID_ERR;
        return -1;
    }

    *status = UUUID_OK;

    return ret;
}

int uuuid_compare(struct uuuid_t* u1, struct uuuid_t* u2, int* status)
{
    uint32_t st;
    int ret;

    ret = uuid_compare(&u1->uuid, &u2->uuid, &st);

    if (st != uuid_s_ok) {
        *status = UUUID_ERR;
        return -1;
    }

    *status = UUUID_OK;

    return ret;
}

void uuuid_to_string(struct uuuid_t* uuuid, char** out, int* status)
{
    uint32_t st;

    uuid_to_string(&uuuid->uuid, out, &st);

    if (st == uuid_s_ok)
        *status = UUUID_OK;
    else
        *status = UUUID_ERR;
}

void uuuid_from_string(const char* in, struct uuuid_t** uuuid, int* status)
{
    uint32_t st;
    struct uuuid_t* u;

    u = uuuid_new();

    uuid_from_string(in, &u->uuid, &st);

    if (st != uuid_s_ok) {
        uuuid_free(u);
        *status = UUUID_ERR;
        return;
    }

    *uuuid = u;
    *status = UUUID_OK;
}

void uuuid_free(struct uuuid_t* uuuid)
{
    free(uuuid);
}

#else


#include <uuid.h>

struct uuuid_t {
    uuid_t uuid;
};

static struct uuuid_t* uuuid_new(void)
{
    return (struct uuuid_t*) malloc(sizeof(struct uuuid_t));
}

static void _uuuid_create(struct uuuid_t** uuuid, int* status, int nil)
{
    struct uuuid_t* u;

    u = uuuid_new();

    if (!u) {
        *status = UUUID_ERR;
        return;
    }

    if (nil)
        uuid_clear(u->uuid);
    else
        uuid_generate(u->uuid);

    *uuuid = u;
    *status = UUUID_OK;
}

void uuuid_create(struct uuuid_t** uuuid, int* status)
{
    _uuuid_create(uuuid, status, 0);
}

void uuuid_create_nil(struct uuuid_t** uuuid, int* status)
{
    _uuuid_create(uuuid, status, 1);
}

int uuuid_is_nil(struct uuuid_t* uuuid, int* status)
{
    *status = UUUID_OK;
    return uuid_is_null(uuuid->uuid);
}

int uuuid_compare(struct uuuid_t* u1, struct uuuid_t* u2, int* status)
{
    *status = UUUID_OK;
    return uuid_compare(u1->uuid, u2->uuid);
}

void uuuid_to_string(struct uuuid_t* uuuid, char** out, int* status)
{
    *out = malloc(sizeof(char) * 37);
    uuid_unparse(uuuid->uuid, *out);
    *status = UUUID_OK;
}

void uuuid_from_string(const char* in, struct uuuid_t** uuuid, int* status)
{
    int st;
    struct uuuid_t* u;

    u = uuuid_new();

    st = uuid_parse(in, u->uuid);

    if (st != 0) {
        uuuid_free(u);
        *status = UUUID_ERR;
        return;
    }

    *uuuid = u;
    *status = UUUID_OK;
}

void uuuid_free(struct uuuid_t* uuuid)
{
    free(uuuid);
}

#endif
#endif













































/* INTERNAL: import UUID object from string representation */
static uuid_rc_t uuid_import_str(uuid_t *uuid, const void *data_ptr, size_t data_len)
{
    uuid_uint16_t tmp16;
    const char *cp;
    char hexbuf[3];
    const char *str;
    unsigned int i;

    /* sanity check argument(s) */
    if (uuid == NULL || data_ptr == NULL || data_len < UUID_LEN_STR)
        return UUID_RC_ARG;

    /* check for correct UUID string representation syntax */
    str = (const char *)data_ptr;
    if (!uuid_isstr(str, 0))
        return UUID_RC_ARG;

    /* parse hex values of "time" parts */
    uuid->obj.time_low            = (uuid_uint32_t)strtoul(str,    NULL, 16);
    uuid->obj.time_mid            = (uuid_uint16_t)strtoul(str+9,  NULL, 16);
    uuid->obj.time_hi_and_version = (uuid_uint16_t)strtoul(str+14, NULL, 16);

    /* parse hex values of "clock" parts */
    tmp16 = (uuid_uint16_t)strtoul(str+19, NULL, 16);
    uuid->obj.clock_seq_low             = (uuid_uint8_t)(tmp16 & 0xff); tmp16 >>= 8;
    uuid->obj.clock_seq_hi_and_reserved = (uuid_uint8_t)(tmp16 & 0xff);

    /* parse hex values of "node" part */
    cp = str+24;
    hexbuf[2] = '\0';
    for (i = 0; i < (unsigned int)sizeof(uuid->obj.node); i++) {
        hexbuf[0] = *cp++;
        hexbuf[1] = *cp++;
        uuid->obj.node[i] = (uuid_uint8_t)strtoul(hexbuf, NULL, 16);
    }

    return UUID_RC_OK;
}











/* INTERNAL: export UUID object to string representation */
static uuid_rc_t uuid_export_str(const uuid_t *uuid, void *_data_ptr, size_t *data_len)
{
    char **data_ptr;
    char *data_buf;

    /* cast generic data pointer to particular pointer to pointer type */
    data_ptr = (char **)_data_ptr;

    /* sanity check argument(s) */
    if (uuid == NULL || data_ptr == NULL)
        return UUID_RC_ARG;

    /* determine output buffer */
    if (*data_ptr == NULL) {
        if ((data_buf = (char *)malloc(UUID_LEN_STR+1)) == NULL)
            return UUID_RC_MEM;
        if (data_len != NULL)
            *data_len = UUID_LEN_STR+1;
    }
    else {
        data_buf = (char *)(*data_ptr);
        if (data_len == NULL)
            return UUID_RC_ARG;
        if (*data_len < UUID_LEN_STR+1)
            return UUID_RC_MEM;
        *data_len = UUID_LEN_STR+1;
    }

    /* format UUID into string representation */
    if (str_snprintf(data_buf, UUID_LEN_STR+1,
        "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        (unsigned long)uuid->obj.time_low,
        (unsigned int)uuid->obj.time_mid,
        (unsigned int)uuid->obj.time_hi_and_version,
        (unsigned int)uuid->obj.clock_seq_hi_and_reserved,
        (unsigned int)uuid->obj.clock_seq_low,
        (unsigned int)uuid->obj.node[0],
        (unsigned int)uuid->obj.node[1],
        (unsigned int)uuid->obj.node[2],
        (unsigned int)uuid->obj.node[3],
        (unsigned int)uuid->obj.node[4],
        (unsigned int)uuid->obj.node[5]) != UUID_LEN_STR) {
        if (*data_ptr == NULL)
            free(data_buf);
        return UUID_RC_INT;
    }

    /* pass back new buffer if locally allocated */
    if (*data_ptr == NULL)
        *data_ptr = data_buf;

    return UUID_RC_OK;
}
