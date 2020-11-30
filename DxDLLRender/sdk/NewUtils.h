//FACE NoImporter v2
#define FC_NTDLL(Name, ...) [&](){static DWORD64 FAddr;if(!FAddr)FAddr=GetExport(GetModBase(((const wchar_t*)1)),StrA(#Name));return((decltype(Name(__VA_ARGS__))(*)(...))FAddr)(__VA_ARGS__);}()
#define FC(Mod, Name, ...) [&](){static DWORD64 FAddr;if(!FAddr)FAddr=GetExport(LoadLibUnc({sizeof(L#Mod)-2,sizeof(L#Mod),(PWSTR)(StrW(L#Mod))}),StrA(#Name));return((decltype(Name(__VA_ARGS__))(*)(...))FAddr)(__VA_ARGS__);}()

__declspec(noinline) DWORD64 GetExport(DWORD64 hDll, const char* Name)
{
	//process image data
	PIMAGE_NT_HEADERS NT_Head = (PIMAGE_NT_HEADERS)(hDll + ((PIMAGE_DOS_HEADER)hDll)->e_lfanew);
	PIMAGE_EXPORT_DIRECTORY ExportDir = (PIMAGE_EXPORT_DIRECTORY)(hDll + NT_Head->OptionalHeader.DataDirectory[0].VirtualAddress);

	//process list
	for (DWORD i = 0; i < ExportDir->NumberOfNames; i++)
	{
		//get ordinal & name
		USHORT Ordinal = ((USHORT*)(hDll + ExportDir->AddressOfNameOrdinals))[i];
		const char* ExpName = (const char*)hDll + ((DWORD*)(hDll + ExportDir->AddressOfNames))[i];

		//check name
		for (int i = 0; ExpName[i] == Name[i]; i++) if (!ExpName[i])
			return hDll + ((DWORD*)(hDll + ExportDir->AddressOfFunctions))[Ordinal];
	} return 0;
}

__declspec(noinline) DWORD64 GetModBase(const wchar_t* Name) {
	PPEB_LDR_DATA Ldr = ((PTEB)__readgsqword(FIELD_OFFSET(NT_TIB, Self)))->ProcessEnvironmentBlock->Ldr;
	for (PLIST_ENTRY CurEnt = Ldr->InMemoryOrderModuleList.Flink; CurEnt != &Ldr->InMemoryOrderModuleList; CurEnt = CurEnt->Flink) {
		PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(CurEnt, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
		if (((DWORD64)Name > 1 && !FC_NTDLL(_wcsicmp, Name, pEntry->BaseDllName.Buffer)) ||
			((DWORD64)Name == 1 && pEntry->BaseDllName.Length == 18 && *(DWORD64*)pEntry->BaseDllName.Buffer == 0x6C00640074006E) || !Name)
			return (DWORD64)pEntry->DllBase;
	} return 0;
}

__declspec(noinline) DWORD64 LoadLibUnc(UNICODE_STRING Mod) {
	typedef NTSTATUS(__stdcall* _LdrLoadDll)(PWCHAR, PULONG, PUNICODE_STRING, HMODULE*); static _LdrLoadDll LdrLoadDllFn = nullptr;
	if (!LdrLoadDllFn) LdrLoadDllFn = (_LdrLoadDll)GetExport(GetModBase((const wchar_t*)1), StrA("LdrLoadDll"));
	HMODULE hMod; LdrLoadDllFn(nullptr, nullptr, &Mod, &hMod); return (DWORD64)hMod;
}

//FACE Str Helper v2
#define FC_FORMAT_A(...) [&](){wchar_t Str[64];FC_NTDLL(_swprintf, Str, __VA_ARGS__);return Str;}()
#define FC_FORMAT_W(...) [&](){wchar_t Str[64];FC_NTDLL(_swprintf, Str, __VA_ARGS__);return Str;}()
#define FC_WCHAR(a) [&](){wchar_t Str[64];for(int i=0;;i++){if(!(Str[i]=a[i]))break;}return Str;}()

//FACE DBG Helper v2
//#define printf(...) { char* Str = FC_FORMAT_A(__VA_ARGS__); WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), Str, FC_NTDLL(strlen, Str), NULL, NULL); }
#define dp(a) { char Buff[32]; FC_NTDLL(sprintf, Buff, "Dec (%d): %d\n",  __LINE__, (a)); WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), Buff, FC_NTDLL(strlen, Buff), NULL, NULL); }
#define hp(a) { char Buff[32]; FC_NTDLL(sprintf, Buff, "Hex (%d): %p\n",  __LINE__, (a)); WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), Buff, FC_NTDLL(strlen, Buff), NULL, NULL); }
#define sp(a) { char Buff[32]; FC_NTDLL(sprintf, Buff, "Str (%d): %s\n",  __LINE__, (a)); WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), Buff, FC_NTDLL(strlen, Buff), NULL, NULL); }
#define v3p(a) { char Buff[32]; FC_NTDLL(sprintf, Buff, "Vec3 (%d): %d %d %d\n",  __LINE__, (int)a.x, (int)a.y, (int)a.z); WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), Buff, FC_NTDLL(strlen, Buff), NULL, NULL); }
#define v3p_crt(a) printf("Vec3 (%d): %f %f %f\n", __LINE__, a.x, a.y, a.z);

//FACE Find Pattern v1
DWORD64 FindPattern(const PBYTE Pattern, const char* Mask, const wchar_t* Module = nullptr)
{
	PBYTE Start = (PBYTE)GetModBase(Module);
	PIMAGE_NT_HEADERS NTHead = (PIMAGE_NT_HEADERS)(Start + ((PIMAGE_DOS_HEADER)Start)->e_lfanew);
	DWORD Len = NTHead->OptionalHeader.SizeOfImage;

	for (PBYTE region_it = Start; region_it < (Start + Len); ++region_it)
	{
		if (*region_it == *Pattern)
		{
			bool found = true;
			const unsigned char* pattern_it = Pattern, * mask_it = (const PBYTE)Mask, * memory_it = region_it;
			for (; *mask_it && (memory_it < (Start + Len)); ++mask_it, ++pattern_it, ++memory_it) {
				if (*mask_it != 'x') continue;
				if (*memory_it != *pattern_it) {
					found = false;
					break;
				}
			}

			if (found)
				return (DWORD64)region_it;
		}
	}

	return 0;
}

//FACE AddUtils
__forceinline DWORD64 RVA(DWORD64 Instr, DWORD InstructionSize) {
	if (!Instr) return 0; LONG RipOffset = *(PLONG)(Instr + (InstructionSize - 4));
	return (Instr + InstructionSize + RipOffset);
}

template<typename T> T __forceinline CallVFunc(void* VFTable, int Index) {
	return (*(T**)VFTable)[Index];
}

class VMTHook
{
private:
	PVOID* NewVMTable;
	DWORD CalcSize(PVOID* VMTable)
	{
		DWORD VFT_Size = 0;
		MEMORY_BASIC_INFORMATION Mem;
		for (int i = 0; i < 512; i++) {
			if (!FC(kernel32, VirtualQuery, (PVOID)VMTable[i], &Mem, sizeof(Mem))) break;
			if (!(Mem.State & MEM_COMMIT) || !(Mem.Protect & PAGE_EXECUTE_READ)) break;
			VFT_Size += 8;
		} return VFT_Size;
	}

public:
	VMTHook(PVOID VFTable)
	{
		//calc len & copy original to shadow
		DWORD Len = CalcSize((PVOID*)VFTable);
		NewVMTable = (PVOID*)FC(kernel32, VirtualAlloc, nullptr, Len, MEM_COMMIT, PAGE_READWRITE);
		FC_NTDLL(memcpy, NewVMTable, VFTable, Len);

		//scan readwrite memory
		MEMORY_BASIC_INFORMATION Mem;
		for (DWORD64 Addr = 0; FC(kernel32, VirtualQuery, (PVOID)Addr, &Mem, sizeof(Mem)); Addr = (DWORD64)Mem.BaseAddress + Mem.RegionSize) {
			if (!(Mem.State & MEM_COMMIT) || (Mem.Protect & (PAGE_NOACCESS | PAGE_GUARD)) || !(Mem.Protect & PAGE_READWRITE))
				continue;

			//scan pointers to set shadow
			DWORD64 StartAddr = (DWORD64)Mem.BaseAddress;
			for (DWORD64 CurAddr = StartAddr; CurAddr < (StartAddr + Mem.RegionSize); CurAddr += 8) {
				if (VFTable == *(PVOID*)CurAddr) *(PVOID*)CurAddr = NewVMTable;
			}
		}
	}

	void SetupHook(PVOID Hook, PVOID* OrgFn, int Index) {
		if (OrgFn) *OrgFn = NewVMTable[Index];
		NewVMTable[Index] = Hook;
	}
};