#pragma once
#include "Utils.h"

class AnswerServicer
{
	public:
		AnswerServicer() = default;
		Answer corespondingAnswer(PCHAR recvbuf, DWORD recvbuflen);

};
