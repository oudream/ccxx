DWORD WINAPI DbgUiRemoteBreakin( LPVOID lpParameter)
{
	__try
	{
		if(NtCurrentPeb()->BeingDebugged)
			DbgBreakPoint();
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return 1;
	}
	RtlExitUserThread(0); //never return	
}