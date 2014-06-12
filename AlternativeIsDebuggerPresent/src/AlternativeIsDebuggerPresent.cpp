#include <iostream>
#include <windows.h>


//
// My version of IsDebuggerPresent is working like the normal one but my version is able to detect emulation/debugging software with one little trick.
// At first I'm checking for the IsDebuggerPresent flag in the PEB block, then I compare it with the API call. If nothing is found I set the 
// IsDebuggerPresent flag to TRUE. Then I compare again the flag in the PEB and the API result and compare them both, if there are differences then it's a (bad) emulation software.
//
__declspec(naked) bool CheckIsDebuggerPresent()
{
	__asm 
	{
		//
		// get debugger info from PEB
		//
		mov eax, dword ptr fs:[0x18]
		mov eax, dword ptr [eax + 0x30]
		movzx ebx, byte ptr [eax + 2]
		cmp ebx, TRUE
		
		je _exit_found

		//
		// save old PEB address
		//
		push eax

		//
		// get debugger info from windows API
		//
		call dword ptr [IsDebuggerPresent]
		cmp eax, TRUE
		je _exit_found

		//
		// nothing found - ok now update our PEB.IsDebuggerPresent flag with TRUE
		// IsDebuggerPresent should return TRUE - if its not returning TRUE then you are emulated
		//
		pop eax
		mov [eax + 2], TRUE
		push eax
		call dword ptr [IsDebuggerPresent]
		cmp eax, TRUE

		//
		// Debugger is still not enabled? Looks like emulation software
		//
		jne _exit_found

		//
		// patch PEB.IsDebuggerPresent flag back to normal (FALSE)
		//
		pop eax
		mov [eax + 2], FALSE
		mov eax, 0
		ret

_exit_found:
		//
		// debugger found
		//
		mov eax, 1
		ret
	}
}

int main(int argc, char* argv[])
{
	if ( CheckIsDebuggerPresent() )
	{
		printf( "Debugger detected\n" );
	}
	else
	{
		printf( "No debugger!\n" );
	}

	return 0;
}