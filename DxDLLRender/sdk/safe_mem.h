#define safe_read(Addr, Type) (((((ULONG64)Addr) > 0x400000) && (((ULONG64)Addr + sizeof(Type)) < 0x00007FFFFFFF0000)) ? *(Type*)((ULONG64)Addr) : Type{})
#define safe_write(Addr, Data, Type) if ((((ULONG64)Addr) > 0x400000) && (((ULONG64)Addr + sizeof(Type)) < 0x00007FFFFFFF0000)) { *(Type*)((ULONG64)Addr) = (Data); }
#define safe_memcpy(Dst, Src, Size) safe_memcpy_wrapper(((ULONG64)Dst), ((ULONG64)Src), Size)
void safe_memcpy_wrapper(ULONG64 Dst, ULONG64 Src, ULONG Sz)
{
	if ((((ULONG64)Dst) > 0x400000) && (((ULONG64)Dst + Sz) < 0x00007FFFFFFF0000))
	{
		while (true)
		{
			//copy 8 byte
			if (Sz >= 8)
			{
				*(ULONG64*)Dst = *(ULONG64*)Src;
				Dst += 8; Src += 8; Sz -= 8;
			}

			//copy 4 byte
			else if (Sz >= 4)
			{
				*(ULONG*)Dst = *(ULONG*)Src;
				Dst += 4; Src += 4; Sz -= 4;
			}

			//copy 2 byte
			else if (Sz >= 2)
			{
				*(WORD*)Dst = *(WORD*)Src;
				Dst += 2; Src += 2; Sz -= 2;
			}

			//copy last byte
			else if (Sz)
			{
				*(BYTE*)Dst = *(BYTE*)Src;
				break;
			}

			//if(Sz == 0)
			else
				break;
		}
	}
}