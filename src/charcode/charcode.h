#ifndef __charcode_h__
#define __charcode_h__

#include "../include/general.h" 

// 関数プロトタイプ
// コード
enum {

	C_CC_TOP=0,
	C_CC_DUMMY,
	C_BOOT01_TOP = (C_CC_DUMMY),
		#include"boot/boot01.h"

	C_CHAR_TABLE_END,
};


typedef struct _codedata
{
	INT32		m_ResID;
	UINT32		m_PosX;
	UINT32		m_PosY;
	UINT32		m_SizeX;
	UINT32		m_SizeY;
	UINT32		m_CCode;		// チェック用キャラコード 
	INT32		m_Bunkatu;		// チェック用キャラコード 
	INT32		m_OfsX;		// 分割用オフセット
	INT32		m_OfsY;		// 分割用オフセット 
	INT32		m_CutOfsX;		// カット用オフセット
	INT32		m_CutOfsY;		// カット用オフセット 
} CODEDATA,*LPCODEDATA;


extern CODEDATA g_CharCodeData[];


#endif // __charcode_h__

