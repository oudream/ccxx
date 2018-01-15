BOOL DebugActiveProcessStop(
  DWORD dwProcessId)
{
	NTSTATUS nStatus;
	HANDLE hProcess = ProcessIdToHandle(dwProcessId);
	if(hProcess==NULL)
		return FALSE;

	CloseAllProcessHandles(dwProcessId);
	nStatus = DbgUiStopDebugging(hProcess);
	NtClose(hProcess);

	if(NT_SUCCESS(nStatus))
		return TRUE;

	SetLastError(5);
	return FALSE;
}
