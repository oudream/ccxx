#include <Python.h>

#include "cxpy_noddy2.h"

static PyModuleDef cxpyModuleDef = {
    PyModuleDef_HEAD_INIT,
    "cxpy",
    "Example module that creates an extension type.",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_cxpy(void)
{
    PyObject* m;

    PyTypeObject & pyTypeNoddy2 = CxpyNoddy2::getPyType();

    if (PyType_Ready(&pyTypeNoddy2) < 0)
        return NULL;

    m = PyModule_Create(&cxpyModuleDef);
    if (m == NULL)
        return NULL;

    Py_INCREF(&pyTypeNoddy2);
    PyModule_AddObject(m, "Noddy2", (PyObject *)&pyTypeNoddy2);
    return m;
}