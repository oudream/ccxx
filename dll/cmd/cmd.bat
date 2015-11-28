
@echo off
set DeployRoot=%~dp0%\
set DeployRoot=%DeployRoot:\\=\%
set DeployRoot=%DeployRoot%..\..\..\deploy\win32\gcl_fes\

rem gcl_vdi1.dll
if exist "%DeployRoot%bin\gcl_vdi1.dll" (
  echo %time% gcl_vdi1.dll
  copy %DeployRoot%bin\gcl_vdi1.dll %DeployRoot%bin_qt\gcl_vdi1.dll
)
if exist "%DeployRoot%bin_d\gcl_vdi1.dll" (
echo %time% gcl_vdi1.dll
copy %DeployRoot%bin_d\gcl_vdi1.dll %DeployRoot%bin_qt_d\gcl_vdi1.dll
)


