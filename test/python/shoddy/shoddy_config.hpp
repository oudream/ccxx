
#include <map>
#include <string>

#include "shoddy1.h"

struct InitTypeTable
{
    const char* name;
    PyTypeObject* (*getTypeProc)();
};

InitTypeTable gInitTableShoddy[] =
    {
        { "Shoddy1", Shoddy1::getPyType},
        { NULL, NULL },
    };
