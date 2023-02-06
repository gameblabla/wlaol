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
void logo_main( void );
void logo_init( void );
void logo_relese( void );
void logo_keys( void );
void logo_drow( void );
void logo_init_save_data( void );

// 変数宣言
static int demo;
static int demo2;
static int yurehaba[360];


//----------------------------------------------------------------------
// メイン関数
void logo_main( void )
{
	logo_keys( );		// キー処理
}

void logo_init( void )
{
	demo = 0;
	demo2 = 0;
	
//	LoadBitmap("image/a_logo.bmp",2,true);		//プレーンナンバー１にタイトル画面背景を読み込む
//	LoadBitmap("image/al_256.bmp",3,false);		//プレーンナンバー１にタイトル画面背景を読み込む

	soundStopBgm(EN_BGM_GAME01);
	
	int i;
	for ( i = 0; i < 360; i++ )
	{
		yurehaba[i] = i + 30;
	}
}

void logo_relese( void )
{
}


void logo_keys( void )
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
	if ( ( IsPushKey(EN_PLAYERNUM_01, EN_INPUT_1) ) || ( IsPushKey(EN_PLAYERNUM_01, EN_INPUT_2) ) || ( demo2 >= 2 ) )
	{
//		if ( demo2 >= 2 )
		{
			SetScene( EN_SN_TITLE );
			return;
		}
//		else 
//		{
//			demo2++;
//		}
	}

	switch( demo2 )
	{
	case 0:
		if ( demo >= ( 30 * 4 ) )
		{
			demo = 0;
			demo2++;
		}
		break;
	case 1:
		if ( demo2 >= 1 )
		{
			int i;
			for ( i = 1; i < 360; i++ )
			{
				if ( ( GetIntCnt( ) % 2 ) == 0 )
				{
					yurehaba[i] -= 1;
					if ( yurehaba[i] < 0 )
					{
						yurehaba[i] = 0;
					}
				}
			}
			if ( demo >= ( 60 * 20 ) )
			{
				demo = 0;
				demo2 = 2;
			}
		}
		break;
	}


}

void logo_drow( void )
{
	//背景クリア
	ClearSecondary();

	demo++;
	
	INT32 cx = DISPLY_WIDTH / 2;
	INT32 cy = DISPLY_HEIGHT / 2;
	
	switch( demo2 )
	{
	case 0:
//		ObjSetEx( C_A_LOGO_P000_00_K, 255, cx, cy - 32, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, 0, 0, -1, -1 );
		ObjSetEx( C_OP1_P000_00_K, 255, cx, cy, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, 0, 0, -1, -1 );
		if ( demo > 0 )
		{
			int ani = ( ( ( 30 * 4 ) - demo ) / 30 );
			if ( ani > 3 )
			{
				ani = 3;
			}
			int i;
			for ( i = 0; i < ( 21 ) * ( 20 ); i++ )
			{
				ObjSetEx( C_FEAD_P000_00_K + ani, 255, ( ( i % 21 ) * 32 ), ( ( i / 21 ) * 32 ), 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
			}
		}

		break;
	case 1:
		ObjSetEx( C_OP1_P000_00_K, 255, cx, cy, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, 0, 0, -1, -1 );

		int i;
		for ( i = 0; i < 360; i++ )
		{
			CODEDATA rCodet = g_CharCodeData[ C_STORY ];
			int y_ofs = 640;
			if ( yurehaba[i] == 0 )
			{
				y_ofs = 0;
			}
			BltFunction( rCodet.m_ResID, 0 + y_ofs, 0 + i, rCodet.m_PosX, rCodet.m_PosY + i, rCodet.m_SizeX, 1, 1.0f, 1.0f, EN_MINI_BLEND, 255, 255, 255, 255, 0.0f, 0, GM_SPR_NONE );
		}
//		ObjSetEx( C_STORY_P000_00_K, 255, cx, cy, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, 0, 0, -1, -1 );
		if ( demo >= ( 60 * 18 ) )
		{
			int ani = ( demo - ( 60 * 18 ) ) / 30;
			if ( ani > 3 )
			{
				ani = 3;
			}
			int i;
			for ( i = 0; i < ( 21 ) * ( 20 ); i++ )
			{
				ObjSetEx( C_FEAD_P000_00_K + ani, 255, ( ( i % 21 ) * 32 ), ( ( i / 21 ) * 32 ), 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
			}
		}


		break;
	default:
		break;
	}

}


