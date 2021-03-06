// pirate.cpp : Defines the entry point for the console application.
// MIEM{700_m4ny_3xc3p710n5_70_h4ndl3}

#include "stdafx.h"
#include <windows.h>
#include <iostream>

const int size = 38;

void Prepare(char *xored,char *str, const char *m) {
	for (unsigned int i = 0; i < size; ++i)
		xored[i] = str[i] ^ m[str[i] - 0x20];
}

int main()
{
	const char templ[size]{ 118, 16, 93, 118, 42, 104, 3, 3, 112, 19, 4, 54, 36, 112, 2, 106, 81, 2, 116, 104, 65, 3, 54, 91, 112, 104, 3, 112, 94, 4, 54, 113, 45, 2, 103, 65, 19, 0 };
	const char matrix[]{ 78, 62, 8, 43, 15, 8, 109, 14, 124, 10, 100, 100, 13, 125, 77, 42, 32, 6, 25, 61, 98, 82, 110, 10, 56, 71, 60, 111, 115, 29, 6, 4, 74, 90, 1, 36, 102, 24, 13, 4, 81, 89, 69, 58, 88, 59, 70, 121, 66, 16, 68, 30, 17, 96, 41, 57, 69, 122, 93, 22, 128, 90, 93, 47, 128, 24, 31, 50, 21, 115, 102, 28, 54, 66, 73, 56, 65, 126, 88, 65, 4, 99, 93, 19, 67, 86, 127, 4, 18, 93, 71, 81, 53, 26, 109, 29, 13, 61, 101, 71, 24, 56, 79, 48, 85, 39, 107, 36, 90, 92, 36, 115, 39, 0, 69, 25, 125, 118, 86, 53, 91, 52, 65, 14, 105, 14, 68, 87 };
	char str[] = "";
	char xored[size]{};
	BOOL check1 = false;
	BOOL check2 = false;
	DWORD timing = 0;							//get execution time

	std::cout << "Enter the flag please: ";

	std::cin.getline(str, size, '\n');
	if (strlen(str) > size)
		exit(1);

	CheckRemoteDebuggerPresent(GetCurrentProcess(), &check1);

	__try {						
		__asm {					//debugger handle exception,
			push edx
			push 8
			mul[esp]
			add esp, 4
			pop edx
			mov ax, ss
			int 3				//and except block wont be executed
			mov ss, ax
			pushfd				//obfuscation
			pop eax				
			and eax, 0x100
			jz pass
			shr eax, 8
			add eax, check2
			not al
			mov check2, eax
			pass :
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		Prepare(xored, str, matrix);
	}

	if (check1 || check2 || GetTickCount() - timing >= 200000000) {
		exit(1);
	}
	else {
		for (int i = 0; i < size; ++i)
			if (templ[i] != xored[i]) {
				std::cout << "NO!";
				exit(1);
			}
		std::cout << "YES!";
	}
	
    return 0;
}

