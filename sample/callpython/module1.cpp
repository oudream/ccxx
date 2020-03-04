#include "module1.h"

static int fn_gcd(int x, int y)
{
    return (x + y) * (x * y);
}

static int fn_in_mandel(double x0, double y0, int n)
{
    return (x0 * y0) - (x0 + y0) * n;
}

static int fn_divide(int x, int y, int * r)
{
    * r = (x * y) * ( x - y );
    return x + y;
}


/* int gcd(int, int) */
static PyObject *py_gcd(PyObject *self, PyObject *args) {
    int x, y, result;

    if (!PyArg_ParseTuple(args,"ii", &x, &y)) {
        return NULL;
    }
    result = fn_gcd(x,y);
    return Py_BuildValue("i", result);
}

/* int in_mandel(double, double, int) */
static PyObject *py_in_mandel(PyObject *self, PyObject *args) {
    double x0, y0;
    int n;
    int result;

    if (!PyArg_ParseTuple(args, "ddi", &x0, &y0, &n)) {
        return NULL;
    }
    result = fn_in_mandel(x0,y0,n);
    return Py_BuildValue("i", result);
}

/* int divide(int, int, int *) */
static PyObject *py_divide(PyObject *self, PyObject *args) {
    int a, b, quotient, remainder;
    if (!PyArg_ParseTuple(args, "ii", &a, &b)) {
        return NULL;
    }
    quotient = fn_divide(a,b, &remainder);
    return Py_BuildValue("(ii)", quotient, remainder);
}

/* Module method table */
static PyMethodDef Module1Methods[] = {
    {"gcd",  py_gcd, METH_VARARGS, "Greatest common divisor"},
    {"in_mandel", py_in_mandel, METH_VARARGS, "Mandelbrot test"},
    {"divide", py_divide, METH_VARARGS, "Integer division"},
    { NULL, NULL, 0, NULL}
};

/* Module structure */
static struct PyModuleDef Module1Def = {
    PyModuleDef_HEAD_INIT,

    "Module1",           /* name of module */
    "A Module1 module",  /* Doc string (may be NULL) */
    -1,                 /* Size of per-interpreter state or -1 */
    Module1Methods       /* Method table */
};


PyObject* Module1::createModule()
{
    return PyModule_Create(&Module1Def);
}
