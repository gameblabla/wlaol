#include "charcode.h"
#include "../define.h"


CODEDATA g_CharCodeData[] = {
	{0,	  0,  0,0,0,0,0,0,0,0,0}, // dummy 

	#include "./boot/boot01.inc"		//  
};

UINT32 g_CCodeCount = sizeof(g_CharCodeData)/sizeof(CODEDATA);

