/* File : example.i */
%module ccxx

%{
#include "cxapplication.h"
%}

/* Let's just grab the original header file here */
%include "cxapplication.h"
