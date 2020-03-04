#include <Python.h>

#include "shoddy_config.hpp"

static PyModuleDef shoddyModuleDef = {
    PyModuleDef_HEAD_INIT,
    "shoddy",
    "Example module that creates an extension type.",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_shoddy(void)
{
    PyObject* m = PyModule_Create(&shoddyModuleDef);
    if (m == NULL)
        return NULL;

    for (int i = 0; gInitTableShoddy[i].name != NULL; i++)
    {
        if (!gInitTableShoddy[i].getTypeProc || !gInitTableShoddy[i].getTypeProc())
        {
            return NULL;
        }
    }
    for (int i = 0; gInitTableShoddy[i].name != NULL; i++)
    {
        PyTypeObject * oPyTypeObject = gInitTableShoddy[i].getTypeProc();

        if (PyType_Ready(oPyTypeObject) < 0)
            return NULL;

        Py_INCREF(oPyTypeObject);
        PyModule_AddObject(m, gInitTableShoddy[i].name, (PyObject *)oPyTypeObject);
//        auto type = reinterpret_cast<PyObject*>(gInitTableShoddy[i].GetTypeProc());
//        Py_INCREF(type);
//        PyModule_AddObject(module, gInitTableShoddy[i].name, type);
    }
    return m;
}