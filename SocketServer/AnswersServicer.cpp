
#include "AnswersServicer.h"
#include <strsafe.h>


Answer AnswerServicer::corespondingAnswer(PCHAR recvbuf, DWORD recvbuflen)
{
	Answer ans;
	if (recvbuflen)
	{
		ans.cbSize = recvbuflen;
		ans.aBuff = (PCHAR)Allocations::AllocBuffer(recvbuflen);
		if (NULL == ans.aBuff)
		{
			return Answer{ NULL,0 };
		}

		HRESULT status = StringCchCopyA((&ans)->aBuff, recvbuflen, recvbuf);
		
		if (S_OK == status)
		{
			return 	ans;
		}
		else
		{
			return Answer{ NULL, 0 };
		}
	}
}
