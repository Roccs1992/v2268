#define in_range(x, a, b) (x >= a && x <= b)
#define get_bits(x) (in_range((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (in_range(x, '0', '9') ? x - '0': 0))
#define get_byte(x) (get_bits(x[0]) << 4 | get_bits(x[1]))

//DBG
//#define fp(a) printf("FACE_FLOAT: %f\n", a)
//#define wsp(a) printf("FACE_WSTR: %ws\n", a)
//#define hp(a) printf("FACE_HEX: 0x%p\n", a)
//#define dp(a) printf("FACE_DEC: %d\n", a)
//#define sp(a) printf("FACE_STR: %s\n", a)
//#define vp(a) printf("FACE_VEC: %f %f %f\n", a.x, a.y, a.z)
//#define vp2(a) printf("FACE_VEC: %f %f\n", a.x, a.y)
//#define vp4(a) printf("FACE_VEC: %f %f %f %f\n", a.w, a.x, a.y, a.z)



//DWORD64 FindPattern(const wchar_t* module, const char* pattern)
//{
//	MODULEINFO mod;
//	HANDLE CurProc = FC(kernel32, GetCurrentProcess);
//	FC(kernel32, K32GetModuleInformation, CurProc, (HMODULE)GetModBase(module), &mod, sizeof(MODULEINFO));
//	uintptr_t start = (uintptr_t)mod.lpBaseOfDll;
//	uintptr_t end = (uintptr_t)mod.lpBaseOfDll + (uintptr_t)mod.SizeOfImage;
//	uintptr_t match = 0;
//	const char* current = pattern;
//
//	for (uintptr_t pCur = start; pCur < end; pCur++)
//	{
//		if (!*current)
//			return match;
//
//		if (*(PBYTE)current == ('\?') || *(BYTE*)pCur == get_byte(current))
//		{
//			if (!match)
//				match = pCur;
//
//			if (!current[2])
//				return match;
//
//			if (*(PWORD)current == ('\?\?') || *(PBYTE)current != ('\?'))
//				current += 3;
//			else
//				current += 2;
//		}
//		else
//		{
//			current = pattern;
//			match = 0;
//		}
//	}
//
//	return 0;
//}

void CreateThreadSafe(PVOID StartAddr)
{
	typedef HANDLE(__fastcall* CreateSimpleThreadFn)(void*, PVOID, PVOID, unsigned int);
	static CreateSimpleThreadFn CST = nullptr;
	CST = (CreateSimpleThreadFn)FindPattern((PBYTE)"\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x30", StrA("xxxx?xxxx?xxxx?xxxxx"), StrW(L"tier0_s64.dll"));
	CST(StartAddr, 0, 0, 0);
}