#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "define.h"
#include "function.h"
#include "util_snd.h"
#include "extern.h"
#include "title.h" 


// ä÷êîÉvÉçÉgÉ^ÉCÉv
void title_main( void );
void title_init( void );
void title_relese( void );
void title_keys( void );
void title_drow( void );
int replay_file_find( void );

// ïœêîêÈåæ
static int mode;		//åªç›ëIëÇ≥ÇÍÇƒÇ¢ÇÈçÄñ⁄	ÇOÅFÇfÇ`ÇlÇdÇrÇsÇ`ÇqÇsÅ@ÇPÅFÇnÇoÇsÇhÇnÇmÅ@ÇQÅFÇdÇwÇhÇs
static int select_level;		//
static int playstat_disp;		//
static char string[1024];
//----------------------------------------------------------------------
// ÉÅÉCÉìä÷êî
void title_main( void )
{
	title_keys( );		// ÉLÅ[èàóù
}

void title_init( void )
{
	mode = 0;		//åªç›ëIëÇ≥ÇÍÇƒÇ¢ÇÈçÄñ⁄	ÇOÅFÇfÇ`ÇlÇdÇrÇsÇ`ÇqÇsÅ@ÇPÅFÇnÇoÇsÇhÇnÇmÅ@ÇQÅFÇdÇwÇhÇs
	select_level = GetSaveDate( EN_SAVEDATA_SELECTLEVEL );
	
	soundPlayBgm( EN_BGM_GAME01 );

}

void title_relese( void )
{
	SetSaveDate( EN_SAVEDATA_SELECTLEVEL, select_level );
}


void title_keys( void )
{
	if ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_UP ) )
	{
		soundPlaySe( EN_SE_SELECT );
		mode--;
		if ( mode == -1 )
		{
			mode = 3;
		}
	}
	else if ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_DOWN ) )
	{
		soundPlaySe( EN_SE_SELECT );
		mode++;
		if ( mode == 4 )
		{
			mode = 0;
		}
	}
	if ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_LEFT ) )
	{
		soundPlaySe( EN_SE_SELECT );
		select_level--;
		if ( select_level < 0 )
		{
			select_level = 3;
		}
	}
	else if ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_RIGHT ) )
	{
		soundPlaySe( EN_SE_SELECT );
		select_level++;
		if ( select_level > 3 )
		{
			select_level = 0;
		}
	}
	if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_LEFT ) )
	{
	}
	else if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_RIGHT ) )
	{
	}
	
	if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_2 ) )
	{
		playstat_disp = 1;
	}
	else
	{
		playstat_disp = 0;
	}

	//	åàíËÉLÅ[ÇâüÇµÇΩ
	if ( IsPushOKKey( ) )
	{
		switch( mode )	// Exit
		{
		case 3:
			SetScene( EN_SN_EXIT );
			break;
		case 2:
			SetScene( EN_SN_CREDITS );
			break;
		case 1:
			SetScene( EN_SN_OPTION );
			break;
		case 0:
			SetScene( EN_SN_ACT );
			break;
		}
	}
	//	ÉLÉÉÉìÉZÉãÉLÅ[ÇâüÇµÇΩ
	if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_3 ) )
	{
		//ÉfÉoÉbÉOã@î\
		if ( DebugDisp != 0 )
		{
//			SetScene( EN_SN_ENDING );
		}
	}
}

void title_drow( void )
{
	//-------------------------------------------------------------
	//ïœêîêÈåæ
	INT32 cx = DISPLY_WIDTH / 2;
	INT32 cy = DISPLY_HEIGHT / 2;
	//-------------------------------------------------------------

	//îwåiÉNÉäÉA
	ClearSecondary();


	//É^ÉCÉgÉãâÊñ ÇÃì]ëó
	//-----------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------
	
	{
	//	ObjSetEx( C_TITILE_P000_00_K, 200, cx, cy, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, 0, 0, -1, -1 );
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
	}
	
	{
		//ÉçÉS
		int bure = ( funcSin( GetIntCnt( ) * 8 ) * 4 ) >> 16;
		int r = 255;
		int g = 255;
		int b = 255;
		if ( funcRand( 60 ) == 0 )
		{
			r = funcRand( 255 );
			g = funcRand( 255 );
			b = funcRand( 255 );
		}
		ObjSetEx( C_LOGO_P000_00_K, 200, cx, cy + bure, 1.0f, 1.0f, 0, r, g, b, 255, 0.0f, 0, 0, -1, -1 );
	}
	
	int font_y = 250;
	int menu_x = cx - 100;
	DarwFont( "GAME START", menu_x, font_y + ( 16 *  0 ), 0 );
	switch( select_level )
	{
	case LEVEL_EASY:
		DarwFont( "EASY", menu_x + 300, font_y + ( 16 *  0 ), 1 );
		break;
	case LEVEL_NOMAL:
		DarwFont( "NORMAL", menu_x + 300, font_y + ( 16 *  0 ), 1 );
		break;
	case LEVEL_HARD:
		DarwFont( "HARD", menu_x + 300, font_y + ( 16 *  0 ), 1 );
		break;
	case LEVEL_ENDRESS:
		DarwFont( "ENDRESS", menu_x + 300, font_y + ( 16 *  0 ), 1 );
		break;
	}
	
	
	DarwFont( "HOW TO PLAY", menu_x, font_y + ( 16 *  1 ), 0 );
	DarwFont( "CREDITS", menu_x, font_y + ( 16 *  2 ), 0 );
	DarwFont( "EXIT", menu_x, font_y + ( 16 *  3 ), 0 );

	DarwFont( "-", menu_x - 20, font_y + ( 16 *  mode ), 0 );

	if ( playstat_disp != 0 )
	{
		int suuzi_x = 320;
		DarwFont( "HI SCORE EASY", 10, 10 + ( 0 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_SCORE_EASY ), 9, suuzi_x, 10 + ( 0 * 16 ), 0, 0 );
		DarwFont( "HI SCORE NORMAL", 10, 10 + ( 1 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_SCORE_NOMAL ), 9, suuzi_x, 10 + ( 1 * 16 ), 0, 0 );
		DarwFont( "HI SCORE HARD", 10, 10 + ( 2 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_SCORE_HARD ), 9, suuzi_x, 10 + ( 2 * 16 ), 0, 0 );
		DarwFont( "HI SCORE ENDRESS", 10, 10 + ( 3 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_SCORE_ENDRESS ), 9, suuzi_x, 10 + ( 3 * 16 ), 0, 0 );
		DarwFont( "ENEMY BULLET", 10, 10 + ( 4 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_BULLET ), 9, suuzi_x, 10 + ( 4 * 16 ), 0, 0 );
		DarwFont( "OUT BULLET", 10, 10 + ( 5 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_OUT_BULLET ), 9, suuzi_x, 10 + ( 5 * 16 ), 0, 0 );
		DarwFont( "PLAYER SHOT", 10, 10 + ( 6 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_SHOT ), 9, suuzi_x, 10 + ( 6 * 16 ), 0, 0 );
		DarwFont( "OUT SHOT", 10, 10 + ( 7 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_OUT_SHOT ), 9, suuzi_x, 10 + ( 7 * 16 ), 0, 0 );
		DarwFont( "DEATH ENEMY", 10, 10 + ( 8 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_ENEMY ), 9, suuzi_x, 10 + ( 8 * 16 ), 0, 0 );
		DarwFont( "OUT ENEMY", 10, 10 + ( 9 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_OUT_ENEMY ), 9, suuzi_x, 10 + ( 9 * 16 ), 0, 0 );
		DarwFont( "MISS", 10, 10 + ( 10 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_MISS ), 9, suuzi_x, 10 + ( 10 * 16 ), 0, 0 );
		DarwFont( "CLEAR", 10, 10 + ( 11 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_CLEAR ), 9, suuzi_x, 10 + ( 11 * 16 ), 0, 0 );
		DarwFont( "GAMEOVER", 10, 10 + ( 12 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_GAMEOVER ), 9, suuzi_x, 10 + ( 12 * 16 ), 0, 0 );
		DarwFont( "MANDARA", 10, 10 + ( 13 * 16 ), 0 );
		DrawNumer( GetSaveDate( EN_SAVEDATA_MANDARA ), 9, suuzi_x, 10 + ( 13 * 16 ), 0, 0 );
		DarwFont( "PLAY TIME", 10, 10 + ( 14 * 16 ), 0 );
		char str[256];
		memset( str, '\0', sizeof( str ) );
		sprintf( str, "%d:%02d:%02d:%03d", GetSaveDate( EN_SAVEDATA_PLAYTIME_H )
		                              , GetSaveDate( EN_SAVEDATA_PLAYTIME_M )
		                              , GetSaveDate( EN_SAVEDATA_PLAYTIME_S )
		                              , ( int )( GetSaveDate( EN_SAVEDATA_PLAYTIME_F ) * 16.6f )
		                              );
		DarwFont( str, suuzi_x, 10 + ( 14 * 16 ), 0 );
	}
	else
	{
//		DarwFont( "WISPLISP ARRAY OF LIST", 10, 10, 0 );
	}


	DarwFont( "PRESS 2 BUTTON TO STATUS DISPLAY", cx, 360 - 32, 1 );
	DarwFont( "ALPHA SECRET BASE / DONG 2011", cx, 360 - 16, 1 );

//	if ( gameflag[61] == 0 )
//	{
//		Blt( 109 , -160, -120 );
//	}

}


/***************************************************************************/
// NAME      = replay_file_find
// FUNCTION  = ÉäÉvÉåÉCÉtÉ@ÉCÉãÇÃåüçı
// NOTES     = 
// DATE      = 
// AUTHER    = koizumi
// HISTORY   =
// PARAMETER = Ç»Çµ
// RETURN    = ÉtÉ@ÉCÉãÇÃóLñ≥
/***************************************************************************/
int replay_file_find( void )
{
	int32_t i;
	int file_j;
	int stage;
	
	stage = 1;
	if ( gameflag[126] == 1 )
	{
		stage = 2;	/* ó†ÉXÉeÅ[ÉW */
	}
	else 
	{
	}
	
	file_j = 1;
	
	sprintf( string, "replay/%d/replay_data_%d.dat", ( int )stage, ( int )gameflag[120] );
	if ( LoadFile( string, &i, 1 ) )
	{
		file_j = 0;
	}
	
	return( file_j );
} 

