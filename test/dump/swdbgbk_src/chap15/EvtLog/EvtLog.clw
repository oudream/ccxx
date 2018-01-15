; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CEvtLogDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "EvtLog.h"

ClassCount=3
Class1=CEvtLogApp
Class2=CEvtLogDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_EVTLOG_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_EVTLOG_DIALOG (English (U.S.))

[CLS:CEvtLogApp]
Type=0
HeaderFile=EvtLog.h
ImplementationFile=EvtLog.cpp
Filter=N

[CLS:CEvtLogDlg]
Type=0
HeaderFile=EvtLogDlg.h
ImplementationFile=EvtLogDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST_INFO

[CLS:CAboutDlg]
Type=0
HeaderFile=EvtLogDlg.h
ImplementationFile=EvtLogDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_EVTLOG_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CEvtLogDlg

[DLG:IDD_EVTLOG_DIALOG (English (U.S.))]
Type=1
Class=CEvtLogDlg
ControlCount=4
Control1=ID_LOG,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=ID_LOG_ORPHAN,button,1342242816
Control4=IDC_LIST_INFO,listbox,1352728835

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

