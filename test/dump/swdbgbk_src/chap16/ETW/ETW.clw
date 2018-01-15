; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CETWDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ETW.h"

ClassCount=3
Class1=CETWApp
Class2=CETWDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_ETW_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_ETW_DIALOG (English (U.S.))

[CLS:CETWApp]
Type=0
HeaderFile=ETW.h
ImplementationFile=ETW.cpp
Filter=N

[CLS:CETWDlg]
Type=0
HeaderFile=ETWDlg.h
ImplementationFile=ETWDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST1

[CLS:CAboutDlg]
Type=0
HeaderFile=ETWDlg.h
ImplementationFile=ETWDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_ETW_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CETWDlg

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_ETW_DIALOG (English (U.S.))]
Type=1
Class=CETWDlg
ControlCount=4
Control1=ID_START_NKL,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,listbox,1352728833
Control4=ID_STOP_NKL,button,1342242817

