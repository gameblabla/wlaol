#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "define.h"
#include "function.h"
#include "extern.h"
#include "scene.h"

#include "title.h"
#include "act.h"
#include "option.h"
#include "ending.h"
#include "logo.h"
#include "stageselect.h"
#include "demo.h"
#include "credits.h"

void scenemanager( void );

void scenemanager( void )
{
	int exit;
	
	exit = true;
	while( exit )
	{
		//キー入力検査
		KeyInput();
		

		if (
		     ( g_DebugKeyStat == EN_DEBUGFUNC_NONE )
		  || ( ( g_DebugKeyStat == EN_DEBUGFUNC_SLOW ) && ( ( GetIntCnt( ) % 2 ) == 0 ) )
		  || ( g_DebugKeyStat == EN_DEBUGFUNC_FAST1 )
		   )
		{
			int loop = 0;
			int loop_max = 1;
			if ( g_DebugKeyStat == EN_DEBUGFUNC_FAST1 )
			{
				loop_max = 8;
			}

			for ( loop = 0; loop < loop_max; loop++ )
			{
				if ( g_scene_req != g_scene )
				{
					//終了処理
					switch( g_scene )
					{
					case EN_SN_TITLE:
						title_relese( );
						break;
					case EN_SN_ACT:
						act_relese( );
						break;
					case EN_SN_OPTION:
						option_relese( );
						break;
					case EN_SN_ENDING:
						ending_relese( );
						break;
					case EN_SN_LOGO:
						logo_relese( );
						break;
					case EN_SN_STAGESELECT:
						stageselect_relese( );
						break;
					case EN_SN_DEMO:
						demo_relese( );
						break;
					case EN_SN_CREDITS:
						credits_relese( );
						break;
					default:
						break;
					}

					g_scene = g_scene_req;
					//初期化
					switch( g_scene )
					{
					case EN_SN_TITLE:
						title_init( );
						break;
					case EN_SN_ACT:
						act_init( );
						break;
					case EN_SN_OPTION:
						option_init( );
						break;
					case EN_SN_ENDING:
						ending_init( );
						break;
					case EN_SN_LOGO:
						logo_init( );
						break;
					case EN_SN_STAGESELECT:
						stageselect_init( );
						break;
					case EN_SN_DEMO:
						demo_init( );
						break;
					case EN_SN_CREDITS:
						credits_init( );
						break;
					default:
						break;
					}
				}
				
				//動作
				switch( g_scene )
				{
				case EN_SN_TITLE:
					title_main( );
					break;
				case EN_SN_ACT:
					act_main( );
					break;
				case EN_SN_OPTION:
					option_main( );
					break;
				case EN_SN_ENDING:
					ending_main( );
					break;
				case EN_SN_LOGO:
					logo_main( );
					break;
				case EN_SN_STAGESELECT:
					stageselect_main( );
					break;
				case EN_SN_DEMO:
					demo_main( );
					break;
				case EN_SN_CREDITS:
					credits_main( );
					break;
				default:
					exit = false;
					break;
				}
			}
		}

		//表示
		switch( g_scene )
		{
		case EN_SN_TITLE:
			title_drow( );
			break;
		case EN_SN_ACT:
			act_draw( );
			break;
		case EN_SN_OPTION:
			option_draw( );
			break;
		case EN_SN_ENDING:
			ending_drow( );
			break;
		case EN_SN_LOGO:
			logo_drow( );
			break;
		case EN_SN_STAGESELECT:
			stageselect_draw( );
			break;
		case EN_SN_DEMO:
			demo_draw( );
			break;
		case EN_SN_CREDITS:
			credits_draw( );
			break;
		default:
			break;
		}

		system_keys( );
		FPSWait( );		// 待ち
		halt( );	// 描画
	}

}


