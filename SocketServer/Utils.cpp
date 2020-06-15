#include "Utils.h"

LPVOID Allocations::AllocBuffer(SIZE_T cbBytes)
{
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cbBytes);
}

void Allocations::FreeBuffer(LPVOID lpBuffer)
{
	HeapFree(GetProcessHeap(), 0, lpBuffer);
}
