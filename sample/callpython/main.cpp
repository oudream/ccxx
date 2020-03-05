#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "module1.h"

int
main(int argc, char *argv[])
{
    fprintf(stdout,"Usage: call pythonfile funcname [args]\n");

    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    //run as: ./callpython multiply multiply 3 2
    char * sArgvsDefault[] = {"callpython", "multiply", "multiply", "3", "2"};
    char ** sArgvs = argc<3 ? sArgvsDefault :argv;

    PyImport_AppendInittab("Module1",Module1::createModule);

    Py_Initialize();

    pName = PyUnicode_DecodeFSDefault(sArgvs[1]);
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, sArgvs[2]);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(argc - 3);
            for (i = 0; i < argc - 3; ++i) {
                pValue = PyLong_FromLong(atoi(sArgvs[i + 3]));
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", sArgvs[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", sArgvs[1]);
        return 1;
    }

    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}