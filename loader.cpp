/*
 * Simple loader to demonstrate patching an exe after its loaded
 * into the memory.
 */

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int CDECL MessageBoxPrintf (TCHAR * szCaption, TCHAR * szFormat, ...)
{
	TCHAR   szBuffer [1024] ;
	va_list pArgList ;
	
	// The va_start macro (defined in STDARG.H) is usually equivalent to:
	// pArgList = (char *) &szFormat + sizeof (szFormat) ;
	va_start (pArgList, szFormat) ;

	// The last argument to wvsprintf points to the arguments
	_vsntprintf (szBuffer, sizeof (szBuffer) / sizeof (TCHAR), 
	szFormat, pArgList) ;

	// The va_end macro just zeroes out pArgList for no good reason
	va_end (pArgList) ;

	return MessageBox (NULL, szBuffer, szCaption, 0) ;
}

int main()
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	
	DWORD PatchVA = 0x004012C8;
	BYTE PatchByte[] = {0x74};
	BYTE ReadByte[1];
	
	SIZE_T bytesWritten = 0,bytesRead = 0;
	
	ZeroMemory(&si,sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi,sizeof(pi));

	if(!CreateProcess(TEXT("sim.exe"),NULL,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,NULL,&si,&pi))
	{
		MessageBoxPrintf("Loader","CreateProcess Failed. Maybe sim.exe not found.");
		return 0;
	}
	
	if(ReadProcessMemory(pi.hProcess,(LPVOID)PatchVA,&ReadByte,sizeof(BYTE),&bytesRead) == FALSE)
	{
		MessageBoxPrintf("Loader","ReadProcessMemory failed.");
		TerminateProcess(pi.hProcess,0);
		return 0;
	}
	// MessageBoxPrintf("Sim.exe Loader","ReadByte = %x",ReadByte[0]);
	
	if(WriteProcessMemory(pi.hProcess,(LPVOID)PatchVA,&PatchByte,1,&bytesWritten) == FALSE)
	{
		MessageBoxPrintf("Loader","WriteProcessMemory failed.");
		TerminateProcess(pi.hProcess,0);
		return 0;
	}
	
	if(bytesWritten != 1)
		MessageBoxPrintf("Loader","Error : Patch Failed.");
	
	ResumeThread(pi.hThread);
	return 0;
}
