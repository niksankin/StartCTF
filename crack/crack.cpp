// crack.cpp : Defines the entry point for the console application.
// MIEM{0h_dud3_u0y_4r3_7h3_d4mn3d_r3v3r53_m45h1n3!}

#include "stdafx.h"
#include "windows.h"
#include <stdlib.h> 
#include <string.h> 
#include <tchar.h> 
#include <iostream>

int main();
int HiddenStructuredHandler();
int StructuredHandler();
LONG WINAPI VectoredHandler1(_EXCEPTION_POINTERS *);
LONG WINAPI VectoredHandler2(_EXCEPTION_POINTERS *);

unsigned char hash;
DWORD *CRC_Start;
DWORD *CRC_Fin;
DWORD *codes;	//address of crypted code start point
DWORD *end;	//of crypted end start point
DWORD size;	//size of crypted code
DWORD old_SEH;

int ThrashFunc1() {
	char nies[]{ "NIIIIIIIIICE!!!"};
	return 0;
}

int ThrashFunc2() {
	char nies[]{ "HOLY SHEEEEEEEET!1!" };
	return 0;
}

struct Hidden {
	PVOID h1, h2;	//handler
	DWORD old_flag;	//old memory assigment flag
	
	//1, gives rights for changing code section
	Hidden() {
		__asm {
			lea eax, codes
			lea ebx, main
			mov dword ptr[eax], ebx
			add dword ptr[eax], 0x10ddf		//codes=crypt_start
			lea eax, end
			mov dword ptr[eax], ebx
			add dword ptr[eax], 0x10e12		//end=crypt_fin
		}

		//make crypted code writable
		size = (DWORD)(end)-(DWORD)(codes);
		VirtualProtect(codes, size, PAGE_EXECUTE_READWRITE, &old_flag);
	}

	~Hidden() {
		RemoveVectoredExceptionHandler(h1);
		RemoveVectoredExceptionHandler(h2);
		VirtualProtect(codes, size, old_flag, &old_flag);
	}
	//2, calls HiddenSEH only when app is not under debugging
	int HiddenMain() {
		//registration of SEH-handler
		__asm {
			lea eax, HiddenStructuredHandler
			push eax
			push fs : [0]
			mov fs : [0], esp
		}

		__asm {
			mov eax, fs:0x18		//IsDebuggerPresent
			mov eax, [eax + 0x30]
			mov al, [eax + 2]
			test al, al
			jne passed
			int 1
			passed :
				   pop eax
				   mov fs : [0], eax
				   pop eax
		}
	}
	//3, called when exceptions are throwed within main
	int HiddenAddVEH() {
		LONG(_stdcall *VEH1) (_EXCEPTION_POINTERS *) = VectoredHandler1;
		LONG(_stdcall *VEH2) (_EXCEPTION_POINTERS *) = VectoredHandler2;

		h1 = AddVectoredExceptionHandler(1, VEH1);
		h2 = AddVectoredExceptionHandler(0, VEH2);
		return 0;
	}

	int HiddenAddSEH() {
		__asm {
			sub esp, 0x1000				//saves old SEH-handler in the top of stack
			lea eax, StructuredHandler
			push eax
			push fs : [0]
			mov fs : [0], esp
			add esp, 0x1008
		}
	}
};

Hidden kek = Hidden();
int lol = kek.HiddenMain();		//hidden encryption
int puk = kek.HiddenAddVEH();	//adding VEH
int lel = kek.HiddenAddSEH();	//adding SEH

//substract 7 from encrypted bytes
__declspec(naked) int HiddenStructuredHandler() {
	__asm {
		mov eax, [esp + 0xc]
		add[eax + 0xb8], 2
	}
	
	__asm {
		lea edi, ThrashFunc2
		add edi, 0x13a6c		//codes=crypt_start
		lea ebx, ThrashFunc2
		add ebx, 0x13a9f		//end=crypt_fin
		repeat :
			cmp edi, ebx
			jnb over
			xor al, al		//anti-IDA obfuscation
			jz cont
			_emit 0x90
			_emit 0xa0
		cont :
			mov al, [edi]
			sub al, 0x7 
			stosb
			jmp repeat
	}

	__asm {
	over:
		xor eax, eax
		ret
	}
}

//SEH naked-handler, decrypt code
__declspec(naked) int StructuredHandler() {
	//add 2 to EIP
	__asm {
		mov eax, [esp + 0xc]
		add [eax + 0xb8], 2
	}

	//decroptor
	__asm {
		pushfd
		pop eax
		and eax, 0x200000
		shr eax, 10
		add ah, hash			//add 8 to hash without any palevo
		lea edi, main
		add edi, 0x10ddf		//codes=crypt_start
		lea ebx, main
		add ebx, 0x10e12		//end=crypt_fin
		repeat :
			cmp edi, ebx
			jnb over
			xor al, al		//anti-IDA obfuscation
			jz cont
			_emit 0x90
			_emit 0xa0
		cont :
			mov al, [edi]
			xor al, ah
			stosb
			jmp repeat
	}
	
	__asm{
	over :
		xor eax, eax
		ret
	}
}

//saves info in flag ID about being debugged
__declspec(naked) LONG WINAPI VectoredHandler2(struct _EXCEPTION_POINTERS *e) {
	__asm {
		xor al, al				//anti-IDA obfuscation
		jz cont
		_emit 0x90
		_emit 0xa0
	cont:
		mov eax, fs : 0x18		//IsDebuggerPresent
		mov eax, [eax + 0x30]
		mov al, [eax + 2]
		xor ah, ah
		test al, al
		je passed
		add ah, 1
	passed:
		pushfd				//set ID flag, will be used in decryptor
		pop edx
		movzx eax, ah
		shl eax, 21
		or edx, eax
		push edx
		popfd
	}

	__asm {
		mov eax, EXCEPTION_CONTINUE_SEARCH
		ret 4
	}
}

//VEH naked-handler, calculates CRC
__declspec(naked) LONG WINAPI VectoredHandler1(struct _EXCEPTION_POINTERS *e) {
	//It's only him
	__asm {
		jmp Jojo
		_emit 'Y'
		_emit 'o'
		_emit 'u'
		_emit '`'
		_emit 'v'
		_emit 'e'
		_emit ' '
		_emit 'e'
		_emit 'x'
		_emit 'p'
		_emit 'e'
		_emit 'c'
		_emit 't'
		_emit 'e'
		_emit 'd'
		_emit ' '
		_emit 'a'
		_emit ' '
		_emit 'f'
		_emit 'l'
		_emit 'a'
		_emit 'g'
		_emit '?'
		_emit ' '
		_emit 'B'
		_emit 'u'
		_emit 't'
		_emit ' '
		_emit 'h'
		_emit 'e'
		_emit 'r'
		_emit 'e'
		_emit ' '
		_emit 'i'
		_emit 's'
		_emit ' '
		_emit 'o'
		_emit 'n'
		_emit 'l'
		_emit 'y'
		_emit ' '
		_emit 'm'
		_emit 'e'
		_emit ','
		_emit ' '
		_emit 'D'
		_emit 'i'
		_emit 'o'
		_emit '!'
	Jojo:
	}
	
	//hash generator
	__asm {
		push esi		//save ESI
		push edi		//save EDI
		lea esi, ThrashFunc1
		mov edi, esi
		add esi, 0x146ab		//start_CRC
		add edi, 0x1481d		//end_CRC
	repeat :
		lodsb
		add ah, al
		cmp esi, edi
		jb repeat
		mov hash, ah
		pop edi
		pop esi
	}

	__asm{
		mov eax, EXCEPTION_CONTINUE_SEARCH
		ret 4
	}
}

int main()
{
	/*
	Shifts:
	start_code: 0xf147
	end_code: 0xf295
	crypt_start: 0xf2a1
	crypt_fin: 0xf2d4
	*/

	__asm {		//push all variables to make code static
		lea ebx, end_flag		//mov
		lea esi, flag		//mov
	}
	
	//obfuscation
	__asm {
	start_code:
		add eax, 0
		cmp edx, edx
		rol edx, 2
		ror edx, 2
		test eax, eax
		add ecx, 1
		sub ecx, 1
		ror ecx, 6
		ror eax, 0
		rol ecx, 6
		add edx, 1
		add edx, 0
		add ecx, 0
		sub edx, 1
		add eax, 0
		rol eax, 0
		sub eax, 0
		cmp ecx, ecx
		add eax, 0
		cmp edx, edx
		rol edx, 2
		ror edx, 2
		test eax, eax
		add ecx, 1
		sub ecx, 1
		ror ecx, 6
		rol ecx, 6
		add edx, 1
		add edx, 0
		add ecx, 0
		sub edx, 1
		add eax, 1
		sub eax, 1
		ror eax, 3
		rol eax, 3
		cmp ecx, ecx
		add eax, 0
		cmp edx, edx
		rol edx, 2
		ror edx, 2
		test eax, eax
		add ecx, 1
		sub ecx, 1
		ror ecx, 6
		rol ecx, 6
		add edx, 1
		add edx, 0
		add ecx, 0
		sub edx, 1
		add eax, 1
		sub eax, 1
		ror eax, 3
		rol eax, 3
		cmp ecx, ecx
		add eax, 0
		cmp edx, edx
		rol edx, 2
		ror edx, 2
		test eax, eax
		add ecx, 1
		sub ecx, 1
		ror ecx, 6
		rol ecx, 6
		add edx, 1
		add edx, 0
		add ecx, 0
		sub edx, 1
		add eax, 1
		sub eax, 1
		ror eax, 3
		rol eax, 3
		cmp ecx, ecx
	}

	__asm {
		push ebx
		push esi
	}

	//obfuscation
	__asm {
		add eax, 0
		cmp edx, edx
		rol edx, 2
		ror edx, 2
		test eax, eax
		add ecx, 1
		sub ecx, 1
		ror ecx, 6
		rol ecx, 6
		add edx, 1
		add edx, 0
		add ecx, 0
		sub edx, 1
		add eax, 1
		sub eax, 1
		ror eax, 3
		rol eax, 3
		cmp ecx, ecx
	}

	__asm {
		mov eax, 0
		mov eax, [eax]
	}

	//obfuscation
	__asm {
		add eax, 0
		cmp edx, edx
		rol edx, 2
		ror edx, 2
		test eax, eax
		add ecx, 1
		sub ecx, 1
		ror ecx, 6
		rol ecx, 6
		add edx, 1
		add edx, 0
		add ecx, 0
		sub edx, 1
		add eax, 1
		sub eax, 1
		ror eax, 3
		rol eax, 3
		cmp ecx, ecx
	}

	//ciphered flag
	__asm {
		pop esi
		pop ebx
		jmp ebx
	}

	//obfuscation
	__asm {
		add eax, 0
		cmp edx, edx
		rol edx, 2
		ror edx, 2
		test eax, eax
		add ecx, 1
		sub ecx, 1
		ror ecx, 6
		rol ecx, 6
		add edx, 1
		add edx, 0
		add ecx, 0
		sub edx, 1
		add eax, 1
		sub eax, 1
		ror eax, 3
		rol eax, 3
		cmp ecx, ecx
	//end of CRC code
	end_code :
	}
	//crypted code
	__asm {
	crypt_start:
	}

	__asm {
	flag:
		_emit 0x27
			_emit 0x7f
			_emit 0x56
			_emit 0x7b
			_emit 0x6c
			_emit 0x7b
			_emit 0x2a
			_emit 0x56
			_emit 0x6c
			_emit 0x27
			_emit 0x70
			_emit 0x56
			_emit 0x2b
			_emit 0x69
			_emit 0x2a
			_emit 0x56
			_emit 0x2e
			_emit 0x7f
			_emit 0x2a
			_emit 0x56
			_emit 0x7b
			_emit 0x2b
			_emit 0x84
			_emit 0x85
			_emit 0x2a
			_emit 0x7b
			_emit 0x56
			_emit 0x69
			_emit 0x2a
			_emit 0x6d
			_emit 0x2a
			_emit 0x69
			_emit 0x2c
			_emit 0x2a
			_emit 0x56
			_emit 0x84
			_emit 0x2b
			_emit 0x2c
			_emit 0x7f
			_emit 0x28
			_emit 0x85
			_emit 0x2a
			_emit 0x38
		end_flag:
			_emit 0xfa
			_emit 0x8d
			_emit 0xdb
			_emit 0x53
			_emit 0x17
			_emit 0xd9
			_emit 0x4a
			_emit 0x92
	}
	__asm {
	crypt_fin:
	}
	__asm{
		jb end_flag
	}

	std::cout << "It seems that there is nothing interesting here...\n";

	//obfuscation
	__asm {
		add eax, 0
		cmp edx, edx
		rol edx, 2
		ror edx, 2
		test eax, eax
		add ecx, 1
		sub ecx, 1
		ror ecx, 6
		rol ecx, 6
		add edx, 1
		add edx, 0
		add ecx, 0
		sub edx, 1
		add eax, 1
		sub eax, 1
		ror eax, 3
		rol eax, 3
		cmp ecx, ecx
	}

	return 0;
}