; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CVerifieeDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "verifiee.h"

ClassCount=3
Class1=CVerifieeApp
Class2=CVerifieeDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_VERIFIEE_DIALOG
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_VERIFIEE_DIALOG (English (U.S.))

[CLS:CVerifieeApp]
Type=0
HeaderFile=verifiee.h
ImplementationFile=verifiee.cpp
Filter=N

[CLS:CVerifieeDlg]
Type=0
HeaderFile=verifieeDlg.h
ImplementationFile=verifieeDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST_STOPS

[CLS:CAboutDlg]
Type=0
HeaderFile=verifieeDlg.h
ImplementationFile=verifieeDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_VERIFIEE_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CVerifieeDlg

[DLG:IDD_VERIFIEE_DIALOG (English (U.S.))]
Type=1
Class=CVerifieeDlg
ControlCount=4
Control1=IDCANCEL,button,1342242816
Control2=IDC_BANG,button,1342242816
Control3=IDC_LIST_STOPS,listbox,1352728833
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

