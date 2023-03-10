#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "define.h"
#include "function.h"
#include "util_snd.h"
#include "extern.h"
#include "option.h" 


// 関数プロトタイプ
void option_main( void );
void option_init( void );
void option_relese( void );
void option_keys( void );
void option_draw( void );


// 変数宣言
static int mode;		//現在選択されている項目	０：ＧＡＭＥＳＴＡＲＴ　１：ＯＰＴＩＯＮ　２：ＥＸＩＴ

//----------------------------------------------------------------------
// メイン関数
void option_main( void )
{
	option_keys( );
}

void option_init( void )
{
	mode = 0;
}

void option_relese( void )
{
}


void option_keys( void )
{
	if ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_UP ) )
	{
	}
	else if ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_DOWN ) )
	{
	}
	if ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_LEFT ) )
	{
	}
	else if ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_RIGHT ) )
	{
	}
	
	
	//	決定が押される
	if ( IsPushOKKey( ) )
	{
		soundPlaySe( EN_SE_SELECT );
		mode++;
		if ( mode == 2 )
		{
			SetScene( EN_SN_TITLE );
		}
	}
	
	//	キャンセルが押される
	if ( IsPushCancelKey(  ) )
	{
		soundPlaySe( EN_SE_SELECT );
		mode--;
		if ( mode == -1 )
		{
			SetScene( EN_SN_TITLE );
		}
	}

}

void option_draw( void )
{
	//-------------------------------------------------------------
	//変数宣言
	INT32 cx = DISPLY_WIDTH / 2;
	INT32 cy = DISPLY_HEIGHT / 2;
	//-------------------------------------------------------------

	//背景クリア
	ClearSecondary();

	ObjSetEx( C_HOWTOPLAY_P000_00_K + mode, 200, cx, cy, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, 0, 0, -1, -1 );

	DarwFont( "A:NEXT B:BACK", cx, 360 - 16, 1 );


}


