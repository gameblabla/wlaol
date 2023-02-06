#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "define.h"
#include "function.h"
#include "util_snd.h"
#include "extern.h"
#include "logo.h" 


// 関数プロトタイプ
void credits_main( void );
void credits_init( void );
void credits_relese( void );
void credits_keys( void );
void credits_draw( void );

// 変数宣言


//----------------------------------------------------------------------
// メイン関数
void credits_main( void )
{
	credits_keys( );		// キー処理
}

void credits_init( void )
{
	
//	soundStopBgm(EN_BGM_GAME01);
}

void credits_relese( void )
{
}


void credits_keys( void )
{
	//	上下キーに応じて移動
	if (IsPushKey(EN_PLAYERNUM_01, EN_INPUT_UP))
	{
	}
	if (IsPushKey(EN_PLAYERNUM_01, EN_INPUT_DOWN))
	{
	}
	if (IsPushKey(EN_PLAYERNUM_01, EN_INPUT_LEFT))
	{
	}
	if (IsPushKey(EN_PLAYERNUM_01, EN_INPUT_RIGHT))
	{
	}

	//	スペースキーが押される
	if ( ( IsPushKey(EN_PLAYERNUM_01, EN_INPUT_1) ) || ( IsPushKey(EN_PLAYERNUM_01, EN_INPUT_2) ) )
	{
		{
			SetScene( EN_SN_TITLE );
			return;
		}
	}
}

void credits_draw( void )
{
	//背景クリア
	ClearSecondary();

	INT32 cx = DISPLY_WIDTH / 2;
	INT32 cy = DISPLY_HEIGHT / 2;
	
	int i;
	for ( i = 0; i < 640; i++ )
	{
		CODEDATA rCodet = g_CharCodeData[ C_TITILE_P000_00_K ];
		int y_ofs = 0;
//			if ( ( GetIntCnt( ) % 20 ) == 0 )
//			if ( funcRand( 10 ) == 0 )
		{
			int bure = ( funcSin( GetIntCnt( ) * 8 ) * 10 ) >> 16;
			y_ofs = funcRand( bure ) - abs( bure / 2 );
		}
		if ( i < 300 )
		{
			y_ofs = y_ofs / 2;
		}
		BltFunction( rCodet.m_ResID, 0 + i, 0 + y_ofs, rCodet.m_PosX + i, rCodet.m_PosY, 1, rCodet.m_SizeY, 1.0f, 1.0f, EN_MINI_BLEND, 255, 255, 255, 255, 0.0f, 0, GM_SPR_NONE );
	}
	//
    DarwFont("WISP LISP ARRAY OF LIST", cx, 40 + (16 * -1), 1);
    DarwFont("-STAFF-", cx, 40 + (16 * 0), 1);
    DarwFont("PROGRAMMER", cx, 40 + (16 * 3), 1);
    DarwFont("D.K",       cx, 40 + (16 * 4), 1);

    DarwFont("DESIGNERS", cx, 40 + (16 * 6), 1);
    DarwFont("DONG", cx, 40 + (16 * 7), 1);
    DarwFont("OMEGA", cx, 40 + (16 * 8), 1);
    DarwFont("D.K", cx, 40 + (16 * 9), 1);
    DarwFont("TERU", cx, 40 + (16 * 10), 1);

    DarwFont("MUSIC COMPOSER", cx, 40 + (16 * 12), 1);
    DarwFont("DONG", cx, 40 + (16 * 13), 1);

    DarwFont("ALPHA SECRET BASE / DONG 2011", cx, 40 + (16 * 16), 1);

    DarwFont("A:MAIN MENU", cx, 360 - 32, 1);

}


