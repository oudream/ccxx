#ifndef CXUUID_H
#define CXUUID_H

#include "cxglobal.h"


#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct GUID {
  unsigned int Data1;
  unsigned short Data2;
  unsigned short Data3;
  unsigned char Data4[8 ];
} GUID;
#endif


class CxUuid
{
public:
    static std::string createGuidString();

    static GUID createGuid();

    static std::string guidToString(const GUID & guid);

    static std::string createRandString();
};





#define UUUID_OK	0
#define UUUID_ERR	-1

struct uuuid_t;

#ifdef __cplusplus
extern "C" {
#endif

void uuuid_create(struct uuuid_t**, int*);
void uuuid_create_nil(struct uuuid_t**, int*);
int uuuid_is_nil(struct uuuid_t*, int*);
int uuuid_compare(struct uuuid_t*, struct uuuid_t*, int*);
void uuuid_to_string(struct uuuid_t*, char**, int*);
void uuuid_from_string(const char*, struct uuuid_t**, int*);
void uuuid_free(struct uuuid_t*);

#ifdef __cplusplus
}
#endif



#endif // CXUUID_H
