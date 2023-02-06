#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "define.h"
#include "function.h"
#include "util_snd.h"
#include "extern.h"
#include "demo.h" 


// 関数プロトタイプ
void demo_main( void );
void demo_init( void );
void demo_relese( void );
void demo_keys( void );
void demo_draw( void );


// 変数宣言
static int scene_exit;
static int demo[10];


static char string[1024];
//----------------------------------------------------------------------
// メイン関数
void demo_main( void )
{
	demo_keys( );
}

void demo_init( void )
{
	scene_exit = 1;

	sprintf(string,"image/demo%d/demo1_1.bmp", ( int )gameflag[131] );
	LoadBitmap(string,1,true);				//プレーンナンバー２にシステム用ＢＭＰを読み込む
	sprintf(string,"image/demo%d/demo1_2.bmp", ( int )gameflag[131] );
	LoadBitmap(string,2,true);				//プレーンナンバー２にシステム用ＢＭＰを読み込む
//	sprintf(string,"image/demo%d/demo%d.bmp", ( int )gameflag[131] , ( int )3 );
//	LoadBitmap(string,3,true);				//プレーンナンバー２にシステム用ＢＭＰを読み込む
//	sprintf(string,"image/demo%d/demo%d.bmp", ( int )gameflag[131] , ( int )4 );
//	LoadBitmap(string,4,true);				//プレーンナンバー２にシステム用ＢＭＰを読み込む
//	sprintf(string,"image/demo%d/demo%d.bmp", ( int )gameflag[131] , ( int )5 );
//	LoadBitmap(string,5,true);				//プレーンナンバー２にシステム用ＢＭＰを読み込む

	LoadBitmap("image/bak/1_256.bmp",11,true);				//プレーンナンバー２にシステム用ＢＭＰを読み込む
//	LoadBitmap("image/bak/2_256.bmp",12,true);				//プレーンナンバー２にシステム用ＢＭＰを読み込む
//	LoadBitmap("image/bak/3_256.bmp",13,true);				//プレーンナンバー２にシステム用ＢＭＰを読み込む
//	LoadBitmap("image/bak/4_256.bmp",14,true);				//プレーンナンバー２にシステム用ＢＭＰを読み込む
//	LoadBitmap("image/bak/5_256.bmp",15,true);				//プレーンナンバー２にシステム用ＢＭＰを読み込む

	LoadBitmap("image/demo0/mesh.bmp",20,true);				//プレーンナンバー２にシステム用ＢＭＰを読み込む

	if ( gameflag[70] == 0 )
	{
		if ( ( gameflag[120] > 0 ) && ( gameflag[120] <= 10 ) )
		{
			soundPlayBgm( EN_BGM_GAME01 );
		}
		if ( ( gameflag[120] > 10 ) && ( gameflag[120] <= 20 ) )
		{
			soundPlayBgm( EN_BGM_GAME01 );
		}
		if ( ( gameflag[120] > 20 ) && ( gameflag[120] <= 30 ) )
		{
			soundPlayBgm( EN_BGM_GAME01 );
		}
		if ( ( gameflag[120] > 30 ) && ( gameflag[120] <= 40 ) )
		{
			soundPlayBgm( EN_BGM_GAME01 );
		}
		if ( ( gameflag[120] > 40 ) && ( gameflag[120] <= 50 ) )
		{
			soundPlayBgm( EN_BGM_GAME01 );
		}
	}
//	soundPlayBgm( EN_BGM_GAME01 );

	demo[0] = 0;

}

void demo_relese( void )
{
	int i;
	
	for ( i = 0; i < BMPBUFF_MAX; i++ )
	{
		ReleaseBitmap( i );
	}
	soundStopBgm(EN_BGM_GAME01);

}


void demo_keys( void )
{
	if ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_UP ) )
	{
//		soundPlaySe( EN_SE_SELECT );
	}
	else if ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_DOWN ) )
	{
//		soundPlaySe( EN_SE_SELECT );
	}
	if ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_LEFT ) )
	{
	}

	//	決定キーを押した
	if ( IsPushOKKey( ) )
	{
		if ( demo[0] < ( 60 * 20 ) )
		{
			demo[0] = ( 60 * 20 );

		}
		else 
		{
			g_scene = EN_SN_ACT;
			scene_exit=0;
		}
	}
	if ( demo[0] >= ( ( 60 * 20 ) + ( 60 * 20 ) ) )
	{
		g_scene = EN_SN_ACT;
		scene_exit=0;
	}
	
	//	キャンセルキーを押した
	if ( IsPushCancelKey( ) )
	{
		g_scene = EN_SN_ACT;
		scene_exit=0;
	}
	
	if ( ( gameflag[120] > 10 ) && ( gameflag[120] <= 20 ) )
	{
		g_scene = EN_SN_ACT;
		scene_exit=0;
	}
	if ( ( gameflag[120] > 20 ) && ( gameflag[120] <= 30 ) )
	{
		g_scene = EN_SN_ACT;
		scene_exit=0;
	}
	if ( ( gameflag[120] > 30 ) && ( gameflag[120] <= 40 ) )
	{
		g_scene = EN_SN_ACT;
		scene_exit=0;
	}
	if ( ( gameflag[120] > 40 ) && ( gameflag[120] <= 50 ) )
	{
		g_scene = EN_SN_ACT;
		scene_exit=0;
	}
	
}

void demo_draw( void )
{
	//-------------------------------------------------------------
	//変数宣言
	//-------------------------------------------------------------
	
	//背景クリア
	ClearSecondary();
	
	demo[0]++;
	
	if ( ( gameflag[120] > 0 ) && ( gameflag[120] <= 10 ) )
	{
		Blt( 11, 0, 0 );
		Blt( 20, 0, 0 );
		
		if ( demo[0] < ( 60 * 20 ) )
		{
			Blt( 1, 0, 0 );
		}
		else if ( demo[0] < ( ( 60 * 20 ) + ( 60 * 20 ) ) )
		{
			Blt( 2, 0, 0 );
		}
	}
/*
	if ( ( gameflag[120] > 10 ) && ( gameflag[120] <= 20 ) )
	{
		Blt( 12, 0, 0 );
		Blt( 20, 0, 0 );
		Blt( 2, 0, 0 );
	}
	if ( ( gameflag[120] > 20 ) && ( gameflag[120] <= 30 ) )
	{
		Blt( 13, 0, 0 );
		Blt( 20, 0, 0 );
		Blt( 3, 0, 0 );
	}
	if ( ( gameflag[120] > 30 ) && ( gameflag[120] <= 40 ) )
	{
		Blt( 14, 0, 0 );
		Blt( 20, 0, 0 );
		Blt( 4, 0, 0 );
	}
	if ( ( gameflag[120] > 40 ) && ( gameflag[120] <= 50 ) )
	{
		Blt( 15, 0, 0 );
		Blt( 20, 0, 0 );
		Blt( 5, 0, 0 );
	}
*/	

}

