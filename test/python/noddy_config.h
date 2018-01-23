
#include <python.h>

struct InitTypeTable
{
    const char* name;
    PyTypeObject* (*getTypeProc)();
};

extern InitTypeTable gInitTableNoddy[];
