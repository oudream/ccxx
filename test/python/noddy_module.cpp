#include <Python.h>

#include "noddy_config.h"

static PyModuleDef noddyModuleDef = {
    PyModuleDef_HEAD_INIT,
    "noddy",
    "Example module that creates an extension type.",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_noddy(void)
{
    PyObject* m = PyModule_Create(&noddyModuleDef);
    if (m == NULL)
        return NULL;

    for (int i = 0; gInitTableNoddy[i].name != NULL; i++)
    {
        if (!gInitTableNoddy[i].getTypeProc || !gInitTableNoddy[i].getTypeProc())
        {
            return NULL;
        }
    }
    for (int i = 0; gInitTableNoddy[i].name != NULL; i++)
    {
        PyTypeObject * oPyTypeObject = gInitTableNoddy[i].getTypeProc();

        if (PyType_Ready(oPyTypeObject) < 0)
            return NULL;

        Py_INCREF(oPyTypeObject);
        PyModule_AddObject(m, gInitTableNoddy[i].name, (PyObject *)oPyTypeObject);
//        auto type = reinterpret_cast<PyObject*>(gInitTableNoddy[i].GetTypeProc());
//        Py_INCREF(type);
//        PyModule_AddObject(module, gInitTableNoddy[i].name, type);
    }
    return m;
}