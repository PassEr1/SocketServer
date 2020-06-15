#pragma once
#include <Windows.h>
namespace Allocations {
	LPVOID AllocBuffer(SIZE_T cbBytes);
	void FreeBuffer(LPVOID lpBuffer);

}
typedef struct Answer
{
	PCHAR aBuff;
	DWORD cbSize;
}Answer;