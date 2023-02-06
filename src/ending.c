#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "define.h"
#include "function.h"
#include "util_snd.h"
#include "extern.h"
#include "ending.h" 


// 関数プロトタイプ
void ending_main( void );
void ending_init( void );
void ending_relese( void );
void ending_keys( void );
void ending_drow( void );
void ending_init_save_data( void );


// 変数宣言
static int end_demo[10];
static int end_seq;
static int yurehaba[360];

//----------------------------------------------------------------------
// メイン関数
void ending_main( void )
{
	ending_keys( );		// キー処理
}

void ending_init( void )
{
	memset( end_demo, 0, sizeof( end_demo ) );

	soundPlayBgm( EN_BGM_GAME01 );
//	gameflag[100] = 0;
//	soundStopBgm(EN_BGM_GAME01);

	end_seq = 0;
	end_demo[0] = 160;      //白フェードカウンタ
	end_demo[1] = 0;        //シーン待ち時間
	end_demo[2] = (int)C_ENDING_P000_00_K;        //BGのキャラコード
	end_demo[3] = 0;        //テキストイン時間
	end_demo[4] = (int)C_ENDING_STORY01;        //テキストのキャラコード
	end_demo[5] = 0;        //シーン６の演出用カウンタ

	//            soundStopBgm(EN_BGM_GAME01);
	int i;
	for (i = 0; i < 360; i++)
	{
	    yurehaba[i] = i + 30;
	}
}

void ending_relese( void )
{
	soundStopBgm(EN_BGM_GAME01);

}


void ending_keys( void )
{
    switch (end_seq)
    {
        case 0:
            //白フェードアウト
            end_demo[0]--;
            if (end_demo[0] == 0)
            {
                end_seq++;
            }
            break;
        case 1:
            //シーン１テキスト挿入
            {
                int i;
                for (i = 1; i < 360; i++)
                {
                    if ((GetIntCnt() % 2) == 0)
                    {
                        yurehaba[i] -= 1;
                        if (yurehaba[i] < 0)
                        {
                            yurehaba[i] = 0;
                        }
                    }
                }
                if (yurehaba[359] == 0)
                {
                    end_seq++;
                }
            }
            break;
        case 2:
            //シーン１待ち
            end_demo[1]++;
            if (end_demo[1] > (60 * 3))
            {
                end_demo[1] = 0;
                end_seq++;
            }
            break;
        case 3:
            //白フェードイン
            end_demo[0]++;
            if (end_demo[0] > (140))
            {
                end_seq++;

                //シーン２の画像設定
                end_demo[1] = 0;        //シーン待ち時間
                end_demo[2] = (int)C_ENDING_P000_00_K + 1;        //BGのキャラコード
                end_demo[3] = 0;        //テキストイン時間
                end_demo[4] = (int)C_ENDING_STORY02;        //テキストのキャラコード

                int i;
                for (i = 0; i < 360; i++)
                {
                    yurehaba[i] = i + 30;
                }
            }
            break;
        case 4:
            //白フェードアウト
            end_demo[0]--;
            if (end_demo[0] == (0))
            {
                end_demo[1] = 0;
                end_seq++;
            }
            break;
        case 5:
            //シーン２テキスト挿入
            {
                int i;
                for (i = 1; i < 360; i++)
                {
                    if ((GetIntCnt() % 2) == 0)
                    {
                        yurehaba[i] -= 1;
                        if (yurehaba[i] < 0)
                        {
                            yurehaba[i] = 0;
                        }
                    }
                }
                if (yurehaba[359] == 0)
                {
                    end_seq++;
                }
            }
            break;
        case 6:
            //シーン２待ち
            end_demo[1]++;
            if (end_demo[1] > (60 * 3))
            {
                end_demo[1] = 0;
                end_seq++;
            }
            break;
        case 7:
            //白フェードイン
            end_demo[0]++;
            if (end_demo[0] > (140))
            {
                end_seq++;

                //シーン3の画像設定
                end_demo[1] = 0;        //シーン待ち時間
                end_demo[2] = (int)C_ENDING_P000_00_K + 2;        //BGのキャラコード
                end_demo[3] = 0;        //テキストイン時間
                end_demo[4] = (int)C_ENDING_STORY03;        //テキストのキャラコード

                int i;
                for (i = 0; i < 360; i++)
                {
                    yurehaba[i] = i + 30;
                }
            }
            break;
        case 8:
            //白フェードアウト
            end_demo[0]--;
            if (end_demo[0] == (0))
            {
                end_seq++;
            }
            break;
        case 9:
            //シーン3テキスト挿入
            {
                int i;
                for (i = 1; i < 360; i++)
                {
                    if ((GetIntCnt() % 2) == 0)
                    {
                        yurehaba[i] -= 1;
                        if (yurehaba[i] < 0)
                        {
                            yurehaba[i] = 0;
                        }
                    }
                }
                if (yurehaba[359] == 0)
                {
                    end_seq++;
                }
            }
            break;
        case 10:
            //シーン3待ち
            end_demo[1]++;
            if (end_demo[1] > (60 * 3))
            {
                end_demo[1] = 0;
                end_seq++;
            }
            break;
        case 11:
            //白フェードイン
            end_demo[0]++;
            if (end_demo[0] > (140))
            {
                end_seq++;

                //シーン4の画像設定
                end_demo[1] = 0;        //シーン待ち時間
                end_demo[2] = (int)C_ENDING_P000_00_K + 3;        //BGのキャラコード
                end_demo[3] = 0;        //テキストイン時間
                end_demo[4] = (int)C_ENDING_STORY04;        //テキストのキャラコード

                int i;
                for (i = 0; i < 360; i++)
                {
                    yurehaba[i] = i + 30;
                }
            }
            break;
        case 12:
            //白フェードアウト
            end_demo[0]--;
            if (end_demo[0] == (0))
            {
                end_seq++;
            }
            break;
        case 13:
            //シーン4テキスト挿入
            {
                int i;
                for (i = 1; i < 360; i++)
                {
                    if ((GetIntCnt() % 2) == 0)
                    {
                        yurehaba[i] -= 1;
                        if (yurehaba[i] < 0)
                        {
                            yurehaba[i] = 0;
                        }
                    }
                }
                if (yurehaba[359] == 0)
                {
                    end_seq++;
                }
            }
            break;
        case 14:
            //シーン4待ち
            end_demo[1]++;
            if (end_demo[1] > (60 * 3))
            {
                end_demo[1] = 0;
                end_seq++;
            }
            break;
        case 15:
            //白フェードイン
            end_demo[0]++;
            if (end_demo[0] > (140))
            {
                end_seq++;

                //シーン5の画像設定
                end_demo[1] = 0;        //シーン待ち時間
                end_demo[2] = (int)C_ENDING_P000_00_K + 4;        //BGのキャラコード
                end_demo[3] = 0;        //テキストイン時間
                end_demo[4] = (int)C_ENDING_STORY05;        //テキストのキャラコード

                int i;
                for (i = 0; i < 360; i++)
                {
                    yurehaba[i] = i + 30;
                }
            }
            break;
        case 16:
            //白フェードアウト
            end_demo[0]--;
            if (end_demo[0] == (0))
            {
                end_seq++;
            }
            break;
        case 17:
            //シーン5テキスト挿入
            {
                int i;
                for (i = 1; i < 360; i++)
                {
                    if ((GetIntCnt() % 2) == 0)
                    {
                        yurehaba[i] -= 1;
                        if (yurehaba[i] < 0)
                        {
                            yurehaba[i] = 0;
                        }
                    }
                }
                if (yurehaba[359] == 0)
                {
                    end_seq++;
                }
            }
            break;
        case 18:
            //シーン5待ち
            end_demo[1]++;
            if (end_demo[1] > (60 * 3))
            {
                end_demo[1] = 0;
                end_seq++;
            }
            break;
        case 19:
            //シーン6挿入
            end_demo[5]++;
            if (end_demo[5] >= (32))
            {
                end_seq++;
                //シーン5の画像設定
                end_demo[1] = 0;        //シーン待ち時間
                end_demo[2] = (int)C_ENDING_P000_00_K + 5;        //BGのキャラコード
                end_demo[3] = 0;        //テキストイン時間
                end_demo[4] = (int)C_ENDING_STORY06;        //テキストのキャラコード

                int i;
                for (i = 0; i < 360; i++)
                {
                    yurehaba[i] = i + 30;
                }
            }
            break;
        case 20:
            //シーン5テキスト挿入
            {
                int i;
                for (i = 1; i < 360; i++)
                {
                    if ((GetIntCnt() % 2) == 0)
                    {
                        yurehaba[i] -= 1;
                        if (yurehaba[i] < 0)
                        {
                            yurehaba[i] = 0;
                        }
                    }
                }
                if (yurehaba[359] == 0)
                {
                    end_seq++;
                }
            }
            break;
        case 21:
            //シーン6待ち
            end_demo[1]++;
            if (end_demo[1] > (60 * 6))
            {
                end_demo[1] = 0;
                end_seq++;
            }
            break;
        case 22:
            //白フェードイン
            end_demo[0]++;
            if (end_demo[0] > (140))
            {
                end_seq++;
            }
            break;
        default:
            SetScene((int)EN_SN_LOGO);
            break;
    }
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
	if ( 
	     ( ( IsPushKey(EN_PLAYERNUM_01, EN_INPUT_1) ) || ( IsPushKey(EN_PLAYERNUM_01, EN_INPUT_2) ) )
//	  && ( gameflag[100] == 1 )
	   )
	{
		SetScene( EN_SN_LOGO );
		return;
	}
	if ( 
	     ( ( IsPushKey(EN_PLAYERNUM_01, EN_INPUT_1) ) || ( IsPushKey(EN_PLAYERNUM_01, EN_INPUT_2) ) )
//	  && ( gameflag[100] == 0 )
	   )
	{
//		demo[0] = 0;
//		demo[1]++;
	}
	

}

void ending_drow( void )
{
	INT32 cx = DISPLY_WIDTH / 2;
	INT32 cy = DISPLY_HEIGHT / 2;

    //BG
    ObjSetEx(end_demo[2], 255, cx, cy, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, 0, 0, -1, -1);

    //テキスト
    int i;
    if (end_demo[4] > 0)
    {
        for (i = 0; i < 360; i++)
        {
            CODEDATA rCodet = g_CharCodeData[end_demo[4]];
            int y_ofs = 640;
            if (yurehaba[i] == 0)
            {
                y_ofs = 0;
            }
            BltFunction(rCodet.m_ResID, 0 + y_ofs, 0 + i, rCodet.m_PosX, rCodet.m_PosY + i, rCodet.m_SizeX, 1, 1.0f, 1.0f, (int)EN_MINI_BLEND, 255, 255, 255, 255, 0, 0, GM_SPR_NONE);
        }
    }

    if (end_seq == 19)
    {
        //シーン6
        int x = 0;
        for (x = 0; x < 640; x += 32 )
        {
            CODEDATA rCodet = g_CharCodeData[C_ENDING_P000_00_K + 5];
            BltFunction(rCodet.m_ResID, 0 + x, 0, rCodet.m_PosX + x, rCodet.m_PosY, end_demo[5], rCodet.m_SizeY, 1.0f, 1.0f, (int)EN_MINI_BLEND, 255, 255, 255, 255, 0, 0, GM_SPR_NONE);
        }
    }
    if (end_seq >= 20)
    {
        //ロゴ
        ObjSetEx((int)C_LOGO_P000_00_K, 255, 340, 260, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, 0, 0, -1, -1);
    }

	{
		//白フェード
		if ( end_demo[0] > 0 ) 
		{
			int ani = end_demo[0] / 30;
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
	}
/*
	{
		char str[256];
		memset( str, '\0', sizeof( str ) );
		sprintf( str, "%d", seq );
		DarwFont( str, 240, 10 + ( 11 * 16 ), 0 );
	}
*/
}


