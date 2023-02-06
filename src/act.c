#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "SDL.h"
#include "define.h"
#include "function.h"
#include "util_snd.h"
#include "extern.h"
#include "act.h" 
#include "include/dconv.h" 
#include "include/general.h" 

char string[1024];
// 変数宣言
static int32_t scene = 0;									/* 場面用変数これを変えることによって描画関数内の処理を変える		*/

enum {
	PRI_BG = 0,
	PRI_SCOER_ITEM,
	PRI_ITEM,
	PRI_PLAYER,
	PRI_ENEMY,
	PRI_PLYSHOT_TUCH,
	PRI_EXP,
	PRI_PLYSHOT,
	PRI_ENESHOT,
	PRI_INFO,
};

enum {
	INIT = 0,
	START,
	MAIN,
	CLEAR,
	END,
	PAUSE,
	RELESE,
	EXIT,
} SCENE ;

enum {
	EN_PLAYER_01 = 0,
	EN_PLAYER_02,
} PLAER_NUM ;

enum {
	EN_ENEMY_NONE = 0,
	EN_ENEMY_01,
	EN_ENEMY_02,
	EN_ENEMY_03,
	EN_ENEMY_MBOSS_01,
	EN_ENEMY_MBOSS_02,
	EN_ENEMY_BOSS_01,
	EN_ENEMY_BOSS_02,
	EN_ENEMY_BOSS_03,
	EN_ENEMY_BOSS_04,
	EN_ENEMY_BOSS_05,
} ENEMY_NUM ;

enum {
	EN_ENESHOT_01 = 0,
	EN_ENESHOT_02,
} ENESHOT_NUM ;

enum {
	EN_EFC_PLYMZL_01 = 0,
	EN_EFC_ENEMZL_01,
	EN_EFC_PLYSHT_TOUCH_01,
	EN_EFC_PLYSHT_TOUCH_02,
	EN_EFC_ENEBAKU_01,
	EN_EFC_ENEBAKU_02,
	EN_EFC_ENEBAKU_03,
	EN_EFC_ENEBAKU_04,
	EN_EFC_BULCLEAR,
	EN_EFC_EFBALL,
	EN_EFC_BOSSDEAD,
	EN_EFC_BOSSDEAD_SERIHU,
	EN_EFC_EXTEND01,
	EN_EFC_EXTEND02,
	EN_EFC_MISSTIME,
	EN_EFC_SCOER_PLATE,
	EN_EFC_RED_ADD,
} EFFECT_NUM ;

enum {
	EN_ITEM_NONE = 0,
	EN_ITEM_SCOER_S,
	EN_ITEM_SCOER_M,
	EN_ITEM_SCOER_L,
	EN_ITEM_ENELGY_S,
	EN_ITEM_ENELGY_M,
	EN_ITEM_ENELGY_L,
} ITEM_NUM ;

enum {
	EN_SHOT_TYPE_NONE = 0,
	EN_SHOT_TYPE_SHOT,
	EN_SHOT_TYPE_LASER,
	EN_SHOT_TYPE_HYPER,
} SHOT_TYPE ;

typedef struct {
	int x;			//位置
	int y;			//位置
} POS;

typedef struct {
	int rensya_inicount;
	int rensya_iniinter;
	int rensya_inter;
	int rensya_count;
	int rensou_count;
	int loop_count;
	int start_inter;
} ATTACK;

typedef struct {
	int life;		//存在有無
	POS pos;		//位置
	int spd;		//速度
	int dir;		//方向
	int motion;		//状態
	int anime;		//アニメーション
	int anime_max;	//アニメーションパターン数
	int anime_cnt;	//アニメーションカウント
	int anime_int;	//アニメーションインターバル
	int type;		//種類
	int pri;		//表示優先度
	int timer;		//生存からのカウンタ
	int muteki;		//無敵フラグ
	int akirame;	//諦めカウンタ
	int damege;		//衝突時のダメージ
	int dispsize;	//表示時の大きさ
	POS collsize;	//あたり変定のサイズ
	int power;		//パワーアップ段階
	int mode;		//ショット状態
	int mode_count;	//ショット状態変更までのカウンタ
	int oya;		//生成親の情報
	int seq;		//シーケンス
	int wait;		//出現待ち
	POS targetpos;	//目的位置
	int score;		//スコア
	int movetype;	//移動タイプ
	int movetype_flg;	//移動タイプ動作フラグ
	int move[10];	//動作用ワーク
	ATTACK atk;
} CHARCTAR;


#define DOT ( 100 )
#define PLAYER_SPEED_SHOT ( 500 )
#define PLAYER_SPEED_LASER ( 350 )
#define PLAYER_SHOT_SPEED ( 20 )
#define PLAYER_SHOT_MODE_CHANGE  ( 8 )
#define PLAYER_SHOT_START_INTMAX  ( 10 )
#define RED_SCALE ( 20 )

enum {
	EN_PLYMODE_SHOT = 0,
	EN_PLYMODE_LASER,
} PLAYER_MODE ;

static int func_return_x;
static int func_return_y;
static int Bullet_ClaerTimer;
static int Bullet_ClaerItem;
static int g_Rank;
static int g_Rank_Red;
static int g_Shake;
static int g_HitStop;
static int g_ScoerPlateInter;

static int g_menu;
enum {
	EN_ACTMENU_RESUME = 0,
	EN_ACTMENU_RESTRAT,
	EN_ACTMENU_EXIT,
};

#define WORLD_WIDTH  ( 640 * DOT )
#define WORLD_HEIGHT ( 360 * DOT )

#define PLAYER_MAX ( 2 )
static CHARCTAR player[PLAYER_MAX];
#define ENEMY_MAX ( 128 )
static CHARCTAR enemy[ENEMY_MAX];
#define ENESHOT_MAX ( 1024 )
static CHARCTAR eneshot[ENESHOT_MAX];
#define PLYSHOT_TATAKI_MAX ( 8 )
#define PLYSHOT_NAGAOSI_MAX ( 160 )
#define PLYSHOT_MAX ( 160 )
static CHARCTAR plyshot[PLYSHOT_MAX];
#define EFFECT_MAX ( 1024 )
static CHARCTAR effect[EFFECT_MAX];
#define ITEM_MAX ( 1024 )
static CHARCTAR item[ITEM_MAX];

#define SAKIYOMI_MAX ( 30 )
static POS sakiyomi[SAKIYOMI_MAX];

enum {
	EN_COLTYPE_NONE = 0,
	EN_COLTYPE_PLAYER,
	EN_COLTYPE_PLAYER_SHOT,
	EN_COLTYPE_ENEMY,
	EN_COLTYPE_ENEMY_SHOT,
	EN_COLTYPE_ITEM,
} COLTYPE;

typedef struct {
	int life;		//存在の有無
	int coltype;	//コリジョンタイプ
	int index;		//関連してるキャラクターのインデックス
	POS pos;		//位置   キャラクターの中央からのオフセット
	POS size;		//サイズ
	int hitparam;	//判定の種類
	int damege;		//相手へのダメージ
} COLLISION;

#define COLLISION_MAX ( 2048 )
static COLLISION collision[COLLISION_MAX];

enum {
	EN_HITPARAM_KU = 0,
	EN_HITPARAM_TI,
} HITPARAM_NUM ;

#define OPTION_MAX ( 2 )
static int option[OPTION_MAX * PLAYER_MAX];

typedef struct {
	int stage;		//ステージ番号
	int disp;		//画面番号
	int round;		//周回数
	int BG_Count;	//背景
	int enemy;		//敵の数
	int scoeritem;	//アイテムの数
	int eneshot;	//敵ショットの数
	int plyshot;	//自機ショットの数
	int enxt_stage;	//次のステージへ進む状態となった
	int enxt_stage_wait;	//次のステージまでの待ち
	int boss;			//ボスが画面にいる
	int scene_wait;		//画面遷移待ち
	int info_yofs;		//ステータス表示位置オフセット
	int bg_change;		//背景変更
} STAGE_INFO;
static STAGE_INFO stage_info;

POS bg_change_scroll[640];

static int test[10];

typedef struct {
	int type;		//タイプ
	int dir;		//方向
	int spd;		//速度
	int x;			//位置
	int y;			//位置
	int wait;		//出現までの待ち
    int move_type;	//動きの種類
    int kurikaesi_num;	//繰り返し出現数
    int kurikaesi_wait;	//繰り返し出現間隔
} ENEMY_MAKETBL;

enum {
	EN_ENE_MOVETYPE_NONE = 0,
	EN_ENE_MOVETYPE_DEC_ADD_UE,		//減速後上に移動
	EN_ENE_MOVETYPE_DEC_ADD_SITA,	//減速後下に移動
	EN_ENE_MOVETYPE_HOMING,			//自機に向かって移動
	EN_ENE_MOVETYPE_DEC_ADD,		//減速＞加速
	EN_ENE_MOVETYPE_DEC_STOP,		//減速＞停止
	EN_ENE_MOVETYPE_DEC_STOP_YURE,	//減速＞停止＞適当にゆれる
	EN_ENE_MOVETYPE_DEC_SONOMAMA,	//減速＞低速で移動
	EN_ENE_MOVETYPE_SONOMAMA,		//そのままの速度で移動
} ENE_MOVETYPE;

#define STAGE_MAX ( 5 )
#define ENEATTACH_MAX ( 100 )
ENEMY_MAKETBL ene_attach[ENEATTACH_MAX];
int g_AttachIndex;








// 関数プロトタイプ
void set_attach_data(int type, int dir, int spd, int x, int y, int wait, int move_type, int kurikaesi_num, int kurikaesi_wait);

void act_main( void );
void act_draw( void );
void act_init( void );
void act_relese( void );
void act_keys( void );
void debugdisp( void );

int getScreenWidth( void );
int getScreenHeight( void );
int getMapWidth( void );
int getMapHeight( void );
void set_Shake( int Shake );
void set_HitStop( int time );
void move_start( void );
void draw_start( void );
void move_main( void );
void draw_main( void );
void move_clear( void );
void draw_clear( void );
void move_end( void );
void draw_end( void );
void move_pause( void );
void draw_pause( void );
void move_collision_run( void );
void draw_red( int val, int x, int y );
void Save_hiscore( void );

void create_player( int player_num );
void move_player( int player_num );
void darw_player( int player_num );
void SetplayerDamege( COLLISION my, COLLISION you );
void AddScoer( int player_num, int add );

void diffic( void );
void diffic_red_add( int add );
void diffic_red_dec( int dec );

void create_enemy( int type, int x, int y, int dir, int spd, int wait, int movetype );
void move_enemy( void );
void darw_enemy( void );
void GetEnemyPos( int i );
int GetEnemyLife( int i );
void SetEnemyDamege( COLLISION my, COLLISION you );

void create_eneshot( int type, int x, int y, int dir, int spd );
void move_eneshot( void );
void darw_eneshot( void );
void SeteneshotDamege( COLLISION my, COLLISION you );

void create_plyshot( int type, int x, int y, int dir, int spd );
void move_plyshot( void );
void darw_plyshot( void );
void SetplyshotDamege( COLLISION my, COLLISION you );

void create_item( int type, int x, int y, int dir, int spd );
void move_item( void );
void darw_item( void );
void SetItemDamege( COLLISION my, COLLISION you );

void create_effect( int type, int x, int y, int dir, int spd );
void move_effect( void );
void darw_effect( void );

void set_collision( int coltype, int index, int x, int y, int w, int h, int hitparam, int damege );
void move_collision( void );
void darw_collision( void );
void move_delete_collision( void );
CHARCTAR GetCollisionType_to_CharctarBuffer( int CollisionType, int index );

void move_BG( void );
void darw_BG( void );

void BltRectEx( int bno, int dx, int dy, int sx, int sy, int w, int h );

void Bullet( int type ,int x, int y, int dir, int spd );
void BulletWay( int type , int x, int y, int dir, int spd, int waydir, int wayinter );
void BulletWayWay( int type , int x, int y, int dir, int spd, int waydir, int wayinter, int waydir2, int wayinter2 );
void BulletTat( int type , int x, int y, int dir, int spd, int tatnum, int addspd );
void BulletWayTat( int type , int x, int y, int dir, int spd, int waydir, int wayinter, int tatnum, int addspd );
void BulletClaer( int time, int item );

int AddDir( int dir, int add );
int EndDirAddDir( int end_dir, int dir, int add_dir );
int PointDir( int sx, int sy, int ex, int ey );
int FigDir( int sx, int sy );
void DirMove( int dir, int spd, int x, int y );
int limit( int x, int y, int size );
int DirChange( int dir, int seido );
int DirToAnim( int dir, int max_anim );
int Kinjiti( int x1, int y1, int x2, int y2 );






//敵出現テーブルの生成
void create_enemy_attach( int stage, int area )
{
    g_AttachIndex = 0;
    memset( &ene_attach[0], 0, sizeof( ene_attach ) );

    switch (stage)
    {
    case 0:
        switch( area )
        {
        case 0:
            //1-1
            //				種類　　　　方向　速度　　　X　　　　　Y　　　　　　　　　　WAIT　移動パターン　　　　　　　　　繰り返し　繰り返し待ち
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;
        case 1:
            //1-2
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;
        case 2:
            //1-3
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_MBOSS_01, 768, 10 * DOT, 720 * DOT, (64 + 64 * 2) * DOT, 100, EN_ENE_MOVETYPE_DEC_STOP_YURE, 1, 30);
            break;
        case 3:
            //1-4
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_03, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_03, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 100,  EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;
        case 4:
            //1-5
            set_attach_data(EN_ENEMY_BOSS_01, 768, 10 * DOT, 720 * DOT, (64 + 64 * 2) * DOT,  0, EN_ENE_MOVETYPE_DEC_STOP_YURE, 1, 30);
            break;
        case 5:
            //1-6
            break;
        case 6:
            //1-7
            break;
        case 7:
            //1-8
            break;
        case 8:
            //1-9
            break;
        case 9:
            //1-10
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (area)
        {
        case 0:
            //2-1
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;                                                                      
        case 1:                                                                         
            //2-2                                                                       
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;                                                                      
        case 2:                                                                         
            //2-3                                                                       
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_MBOSS_02, 768, 10 * DOT, 720 * DOT, (64 + 64 * 2) * DOT, 100, EN_ENE_MOVETYPE_DEC_STOP_YURE, 1, 30);
            break;                                                                      
        case 3:                                                                         
            //2-4                                                                       
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_03, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_03, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 100,  EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;                                                                      
        case 4:                                                                         
            //2-5                                                                       
            set_attach_data(EN_ENEMY_BOSS_02, 768, 10 * DOT, 720 * DOT, (64 + 64 * 2) * DOT,  0, EN_ENE_MOVETYPE_DEC_STOP_YURE, 1, 30);
            break;                                                                      
        case 5:                                                                         
            //2-6                                                                       
            break;
        case 6:
            //2-7
            break;
        case 7:
            //2-8
            break;
        case 8:
            //2-9
            break;
        case 9:
            //2-10
            break;
        default:
            break;
        }
        break;
    case 2:
        switch (area)
        {
        case 0:
            //3-1
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;                                                                      
        case 1:                                                                         
            //3-2                                                                       
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;                                                                      
        case 2:                                                                         
            //3-3                                                                       
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_MBOSS_01, 768, 10 * DOT, 720 * DOT, (64 + 64 * 2) * DOT, 100, EN_ENE_MOVETYPE_DEC_STOP_YURE, 1, 30);
            break;                                                                      
        case 3:                                                                         
            //3-4                                                                       
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_03, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_03, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 100,  EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;                                                                      
        case 4:                                                                         
            //3-5                                                                       
            set_attach_data(EN_ENEMY_BOSS_03, 768, 10 * DOT, 720 * DOT, (64 + 64 * 2) * DOT,  0, EN_ENE_MOVETYPE_DEC_STOP_YURE, 1, 30);
            break;                                                                      
        case 5:                                                                         
            //3-6                                                                       
            break;
        case 6:
            //3-7
            break;
        case 7:
            //3-8
            break;
        case 8:
            //3-9
            break;
        case 9:
            //3-10
            break;
        default:
            break;
        }
        break;
    case 3:
        switch (area)
        {
        case 0:
            //4-1
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;                                                                      
        case 1:                                                                         
            //4-2                                                                       
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;                                                                      
        case 2:                                                                         
            //4-3                                                                       
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_MBOSS_02, 768, 10 * DOT, 720 * DOT, (64 + 64 * 2) * DOT, 100, EN_ENE_MOVETYPE_DEC_STOP_YURE, 1, 30);
            break;                                                                      
        case 3:                                                                         
            //4-4                                                                       
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_03, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_03, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 100,  EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;                                                                      
        case 4:                                                                         
            //4-5                                                                       
            set_attach_data(EN_ENEMY_BOSS_04, 768, 10 * DOT, 720 * DOT, (64 + 64 * 2) * DOT,  0, EN_ENE_MOVETYPE_DEC_STOP_YURE, 1, 30);
            break;                                                                      
        case 5:                                                                         
            //4-6                                                                       
            break;
        case 6:
            //4-7
            break;
        case 7:
            //4-8
            break;
        case 8:
            //4-9
            break;
        case 9:
            //4-10
            break;
        default:
            break;
        }
        break;
    case 4:
        switch (area)
        {
        case 0:
            //5-1
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;                                                                      
        case 1:                                                                         
            //5-2                                                                       
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;                                                                      
        case 2:                                                                         
            //5-3                                                                       
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 1) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 3) * DOT, 20,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 60,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_MBOSS_01, 768, 10 * DOT, 720 * DOT, (64 + 64 * 2) * DOT, 100, EN_ENE_MOVETYPE_DEC_STOP_YURE, 1, 30);
            break;                                                                      
        case 3:                                                                         
            //5-4                                                                       
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 0,    EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_01, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 40,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_03, 768, 10 * DOT, 670 * DOT, (64 + 64 * 0) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_03, 768, 10 * DOT, 670 * DOT, (64 + 64 * 4) * DOT, 80,   EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            set_attach_data(EN_ENEMY_02, 768, 10 * DOT, 670 * DOT, (64 + 64 * 2) * DOT, 100,  EN_ENE_MOVETYPE_DEC_SONOMAMA, 1, 30);
            break;                                                                      
        case 4:                                                                         
            //5-5                                                                       
            set_attach_data(EN_ENEMY_BOSS_05, 768, 10 * DOT, 720 * DOT, (64 + 64 * 2) * DOT,  0, EN_ENE_MOVETYPE_DEC_STOP_YURE, 1, 30);
            break;                                                                      
        case 5:                                                                         
            //5-6                                                                       
            break;
        case 6:
            //5-7
            break;
        case 7:
            //5-8
            break;
        case 8:
            //5-9
            break;
        case 9:
            //5-10
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    set_attach_data(EN_ENEMY_NONE, 0, 0, 0, 0, 0, 0, 0, 0);	//end of data
}
void set_attach_data(int type, int dir, int spd, int x, int y, int wait, int move_type, int kurikaesi_num, int kurikaesi_wait)
{
    ene_attach[g_AttachIndex].type = type;
    ene_attach[g_AttachIndex].dir = dir;
    ene_attach[g_AttachIndex].spd = spd;
    ene_attach[g_AttachIndex].x = x;
    ene_attach[g_AttachIndex].y = y;
    ene_attach[g_AttachIndex].wait = wait;
    ene_attach[g_AttachIndex].move_type = move_type;
    ene_attach[g_AttachIndex].kurikaesi_num = kurikaesi_num;
    ene_attach[g_AttachIndex].kurikaesi_wait = kurikaesi_wait;

    g_AttachIndex++;
}


//----------------------------------------------------------------------
// メイン関数
void act_main( void )
{
	act_keys( );		// キー処理
}


void act_init( void )
{
	scene = START;
	
	Bullet_ClaerTimer = 0;
	Bullet_ClaerItem = 0;
	func_return_x = 0;
	func_return_y = 0;
	g_Shake = 0;
	g_HitStop = 0;
	g_Rank_Red = 0;
	g_menu = EN_ACTMENU_RESUME;
	g_AttachIndex = 0;
	g_ScoerPlateInter = 0;
	
//	g_DebugDisp = 1;
	
	memset( &player[0], 0, sizeof( player ) );
	memset( &enemy[0], 0, sizeof( enemy ) );
	memset( &eneshot[0], 0, sizeof( eneshot ) );
	memset( &plyshot[0], 0, sizeof( plyshot ) );
	memset( &effect[0], 0, sizeof( effect ) );
	memset( &collision[0], 0, sizeof( collision ) );
	memset( &item[0], 0, sizeof( item ) );
	memset( &stage_info, 0, sizeof( stage_info ) );
	memset( &sakiyomi[0], 0, sizeof( sakiyomi ) );
	memset( &bg_change_scroll[0], 0, sizeof( bg_change_scroll ) );

	memset( &test[0], 0, sizeof( test ) );
	
	soundStopSeAll();					/* wavファイルの全開放 */
//	soundPlayBgm( EN_BGM_GAME02 );

	stage_info.stage = 0;
	stage_info.disp = 0;
	stage_info.round = 0;
	stage_info.enxt_stage = 1;
	create_enemy_attach(stage_info.stage, stage_info.disp);

	create_player( EN_PLAYER_01 );

}

void act_relese( void )
{
	BltOffset( 0, 0 );
}


void act_keys( void )
{
	g_Shake--;
	if ( g_Shake > 0 )
	{
		int rx = funcRand( g_Shake ) - ( g_Shake / 2 );
		int ry = funcRand( g_Shake ) - ( g_Shake / 2 );
		BltOffset( rx, ry );
	}
	else
	{
		BltOffset( 0, 0 );
	}
	g_HitStop--;
	
	if ( g_HitStop > 0 )
	{
	
	}
	else
	{
		move_delete_collision( );
		switch( scene )
		{
		case INIT:
			break;
		case START:
			move_start( );
			break;
		case MAIN:
			diffic( );
			
			move_BG( );
			move_player( EN_PLAYER_01 );
			move_plyshot( );
			move_enemy( );
			move_eneshot( );
			move_effect( );
			move_item( );
			move_collision( );

			move_main( );
			break;
		case CLEAR:
			diffic( );
			
			move_BG( );
			move_player( EN_PLAYER_01 );
			move_plyshot( );
			move_enemy( );
			move_eneshot( );
			move_effect( );
			move_item( );
			move_collision( );

//			move_main( );
			move_clear( );
			break;
		case END:
			diffic( );
			
			move_BG( );
	//		move_player( EN_PLAYER_01 );
			move_plyshot( );
			move_enemy( );
			move_eneshot( );
			move_effect( );
			move_item( );
			move_collision( );

//			move_main( );
			move_end( );
			break;
		case PAUSE:
			move_pause( );
			break;
		case RELESE:
			break;
		case EXIT:
			break;
		}
	}
	
}
void act_draw( void )
{
	//背景クリア
	ClearSecondary();

	switch( scene )
	{
	case INIT:
		break;
	case START:
		darw_BG( );
		darw_enemy( );
		darw_item( );
		darw_effect( );
		darw_player( EN_PLAYER_01 );
		darw_plyshot( );
		darw_eneshot( );
		darw_collision( );
		draw_main( );
		draw_start( );
		break;
	case MAIN:
		darw_BG( );
		darw_enemy( );
		darw_item( );
		darw_effect( );
		darw_player( EN_PLAYER_01 );
		darw_plyshot( );
		darw_eneshot( );
		darw_collision( );
		draw_main( );
		break;
	case CLEAR:
		darw_BG( );
		darw_enemy( );
		darw_item( );
		darw_effect( );
		darw_player( EN_PLAYER_01 );
		darw_plyshot( );
		darw_eneshot( );
		darw_collision( );
		draw_main( );
		draw_clear( );
		break;
	case END:
		darw_BG( );
		darw_enemy( );
		darw_item( );
		darw_effect( );
		darw_player( EN_PLAYER_01 );
		darw_plyshot( );
		darw_eneshot( );
		darw_collision( );
		draw_main( );
		draw_end( );
		break;
	case PAUSE:
		darw_BG( );
		darw_enemy( );
		darw_item( );
		darw_effect( );
		darw_player( EN_PLAYER_01 );
		darw_plyshot( );
		darw_eneshot( );
		darw_collision( );
		draw_main( );
		draw_pause( );
		break;
	case RELESE:
		break;
	case EXIT:
		break;
	}

	debugdisp( );
}

//画面の幅
int getScreenWidth( void )
{
	return DISPLY_WIDTH;
}
//画面の高さ
int getScreenHeight( void )
{
	return DISPLY_HEIGHT;
}
//マップの幅
int getMapWidth( void )
{
	return WORLD_WIDTH;
}
//マップの高さ
int getMapHeight( void )
{
	return WORLD_HEIGHT;
}

//画面シェイク要求
void set_Shake( int Shake )
{
	g_Shake = Shake;
}
//画面シェイク要求
void set_HitStop( int time )
{
	g_HitStop = time;
}


void move_start( void )
{
	scene = MAIN;

}
void draw_start( void )
{
}

void move_main( void )
{
	//敵の出現
	stage_info.enxt_stage_wait--;
	if ( stage_info.enxt_stage_wait < 0 )
	{
		stage_info.enxt_stage_wait = 0;
	}
	if ( ( stage_info.enemy == 0 ) && ( stage_info.enxt_stage_wait == 0 ) )
	{
		stage_info.enxt_stage = 1;
	}
	
	if ( stage_info.stage == 5 )
	{
		switch( GetSaveDate( EN_SAVEDATA_SELECTLEVEL ) )
		{
		case LEVEL_EASY:
		case LEVEL_NOMAL:
		case LEVEL_HARD:
			stage_info.enxt_stage = 0;
			if ( stage_info.boss == 0 )
			{
	//				SetScene( EN_SN_ENDING );
				stage_info.scene_wait = 0;
				scene = CLEAR;
				AddSaveDate( EN_SAVEDATA_CLEAR, 1 );
			}
			break;
		case LEVEL_ENDRESS:
			stage_info.enxt_stage = 1;
			stage_info.stage = 0;
			stage_info.disp = 0;
			stage_info.round++;
			break;
		}
	}
	
	if ( ( stage_info.enxt_stage == 1 ) && ( stage_info.boss == 0 ) )
	{

		if ( ( stage_info.stage == 1 ) && ( stage_info.disp == 0 ) )
		{
			if ( stage_info.bg_change == 0 )
			{
				switch( GetSaveDate( EN_SAVEDATA_SELECTLEVEL ) )
				{
				case LEVEL_EASY:
					break;
				case LEVEL_NOMAL:
				case LEVEL_HARD:
				case LEVEL_ENDRESS:
					SwapToSecondary( FID_WORK01 );
					BltOffset( 0, 0 );
					darw_BG( );
					SwapToSecondary( FID_WORK01 );
		//			SaveBmp( FID_WORK01, "test.bmp" );
					if ( funcRand( 2 ) == 0 )
					{
						soundPlayBgm( EN_BGM_GAME02 );
					}
					else
					{
						soundPlayBgm( EN_BGM_GAME03 );
					}
					stage_info.bg_change = 1;
					break;
				}
			}
		}

		stage_info.enxt_stage_wait = 3;
		stage_info.enxt_stage = 0;
		int ei = 0;
		for ( ei = 0; ei < ENEATTACH_MAX; ei++ )
		{
			int type = ene_attach[ei].type;
			int dir = ene_attach[ei].dir;
			int spd = ene_attach[ei].spd;
			int x = ene_attach[ei].x;
			int y = ene_attach[ei].y;
			int wait = ene_attach[ei].wait;
			int movetype = ene_attach[ei].move_type;
			int kurikaesi_num = ene_attach[ei].kurikaesi_num;
			int kurikaesi_wait = ene_attach[ei].kurikaesi_wait;
			
			if ( type != EN_ENEMY_02 )
			{
				kurikaesi_num += ( g_Rank_Red / RED_SCALE );
			}
			if ( type != EN_ENEMY_NONE )
			{
				int kuri = 0;
				for ( kuri = 0; kuri < kurikaesi_num; kuri++ )
				{
					create_enemy( type, x, y, dir, spd, wait + ( kuri * kurikaesi_wait ), movetype );
				}
			}
		}
		stage_info.disp++;
		create_enemy_attach(stage_info.stage, stage_info.disp);
		if ( stage_info.disp >= STAGE_MAX )
		{
			stage_info.disp = 0;
			stage_info.stage++;
			
			if ( stage_info.stage >= 5 )
			{
				stage_info.stage = 5;
			}
		}
	}

	if ( ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_4 ) ) )	//ポーズ
	{
		g_menu = EN_ACTMENU_RESUME;
		scene = PAUSE;
	}

}
void draw_main( void )
{

	if ( player[0].pos.y > ( 300 * DOT ) )
	{
		stage_info.info_yofs += 4;
		if ( stage_info.info_yofs > 40 )
		{
			stage_info.info_yofs = 40;
		}
	}
	else
	{
		stage_info.info_yofs -= 4;
		if ( stage_info.info_yofs < 0 )
		{
			stage_info.info_yofs = 0;
		}
	}
	
	int red_color = 0;
	if ( g_Rank_Red >= 50 )
	{
		red_color = 1;
	}
	
	//下段
	DarwFont( "TIME", 10, 360 - 20 + stage_info.info_yofs, 0 );
	DrawNumer( player[0].life, 9, 90, 360 - 20 + stage_info.info_yofs, 0, 0 );	

	DarwFont( "SCORE", 200, 360 - 20 + stage_info.info_yofs, 0 );
	DrawNumer( player[0].score, 9, 300, 360 - 20 + stage_info.info_yofs, 0, 0 );
		
	if ( red_color == 0 )
	{
		DarwFont( "RED", 500, 360 - 20 + stage_info.info_yofs, 0 );
		DrawNumer( g_Rank_Red, 9, 580, 360 - 20 + stage_info.info_yofs, 0, 0 );	
	}
	else
	{
		ObjSetEx( C_RED_P000_00_K, PRI_BG, 500, 360 - 16 + stage_info.info_yofs, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
		draw_red( g_Rank_Red, 544, 360 - 20 + stage_info.info_yofs );
	}

	//上段
	DarwFont( "TIME", 10, -34 + stage_info.info_yofs, 0 );
	DrawNumer( player[0].life, 9, 90, -34 + stage_info.info_yofs, 0, 0 );	

	DarwFont( "SCORE", 200, -34 + stage_info.info_yofs, 0 );
	DrawNumer( player[0].score, 9, 300, -34 + stage_info.info_yofs, 0, 0 );

	if ( red_color == 0 )
	{
		DarwFont( "RED", 500, -34 + stage_info.info_yofs, 0 );
		DrawNumer( g_Rank_Red, 9, 580, -34 + stage_info.info_yofs, 0, 0 );	
	}
	else
	{
		ObjSetEx( C_RED_P000_00_K, PRI_BG, 500, -20 + stage_info.info_yofs, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
		draw_red( g_Rank_Red, 544, -22 + stage_info.info_yofs );
	}

}

void draw_red( int val, int x, int y )
{
	int len = 1000;
	int w = 32;
	int zero = 0;
	
	do 
	{
		int c = 0;
		c = val / len;

		if ( len == 1 )
		{
			zero = 1;
		}
		if ( c != 0 )
		{
			zero = 1;
			val -= ( c * len );
		}
		if ( zero != 0 )
		{
			ObjSetEx( C_RED_FONT_P000_00_K + c, PRI_BG, x, y, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
			x += w;
		}
		len = len / 10;
		
	} while ( len );
}


void move_clear( void )
{
	stage_info.scene_wait++;
	if ( stage_info.scene_wait > 180 ) 
	{
		stage_info.scene_wait = 180;
		SetScene( EN_SN_ENDING );

		//ハイスコアの保存
		Save_hiscore( );
	}
}
void draw_clear( void )
{
	int ani = stage_info.scene_wait / 40;
	if ( ani > 3 )
	{
		ani = 3;
	}
	int i;
	for ( i = 0; i < ( 21 ) * ( 20 ); i++ )
	{
		ObjSetEx( C_FEAD_P000_00_K + ani, PRI_BG, ( ( i % 21 ) * 32 ), ( ( i / 21 ) * 32 ), 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
	}
}

void move_end( void )
{
	stage_info.scene_wait++;
	if ( stage_info.scene_wait > 180 ) 
	{
		soundStopBgm(EN_BGM_GAME01);
		stage_info.scene_wait = 180;
		SetScene( EN_SN_TITLE );

		//ハイスコアの保存
		Save_hiscore( );
	}
}
void draw_end( void )
{
	int ani = stage_info.scene_wait / 40;
	if ( ani > 3 )
	{
		ani = 3;
	}
	int i;
	for ( i = 0; i < ( 21 ) * ( 20 ); i++ )
	{
		ObjSetEx( C_FEAD_P000_00_K + ani, PRI_BG, ( ( i % 21 ) * 32 ), ( ( i / 21 ) * 32 ), 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
	}
}

void move_pause( void )
{
	if ( ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_UP ) ) )
	{
		soundPlaySe( EN_SE_SELECT );
		g_menu--;
		if ( g_menu < EN_ACTMENU_RESUME )
		{
			g_menu = EN_ACTMENU_EXIT;
		}
	}
	if ( ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_DOWN ) ) )
	{
		soundPlaySe( EN_SE_SELECT );
		g_menu++;
		if ( g_menu > EN_ACTMENU_EXIT )
		{
			g_menu = EN_ACTMENU_RESUME;
		}
	}
	if ( ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_1 ) ) )
	{
		switch( g_menu )
		{
		default:
		case EN_ACTMENU_RESUME:
			scene = MAIN;
			break;
		case EN_ACTMENU_RESTRAT:
			soundPlayBgm( EN_BGM_GAME01 );
			act_init( );
			break;
		case EN_ACTMENU_EXIT:
			SetScene( EN_SN_TITLE );
			break;
		}
	}
	if ( ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_3 ) ) )
	{
		scene = MAIN;
	}
	if ( ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_4 ) ) )
	{
		scene = MAIN;
	}
}
void draw_pause( void )
{
	INT32 cx = DISPLY_WIDTH / 2;

	DarwFont( "PAUSE\0", cx, 140 , 1 );
	DarwFont( "RESUME\0", cx-80, 160 +16, 0 );
	DarwFont( "RESTART\0", cx-80, 160 +32, 0 );
	DarwFont( "RETURN TO TITLE\0", cx-80, 160 +48, 0 );
	DarwFont( "-", cx-100, 160 +16 + ( g_menu * 16 ), 0 );
}

void Save_hiscore( void )
{
	//ハイスコアの保存
	int hi;
	switch( GetSaveDate( EN_SAVEDATA_SELECTLEVEL ) )
	{
	case LEVEL_EASY:
		hi = GetSaveDate( EN_SAVEDATA_SCORE_EASY );
		if ( hi < player[0].score )
		{
			SetSaveDate( EN_SAVEDATA_SCORE_EASY, player[0].score );
		}
		break;
	case LEVEL_NOMAL:
		hi = GetSaveDate( EN_SAVEDATA_SCORE_NOMAL );
		if ( hi < player[0].score )
		{
			SetSaveDate( EN_SAVEDATA_SCORE_NOMAL, player[0].score );
		}
		break;
	case LEVEL_HARD:
		hi = GetSaveDate( EN_SAVEDATA_SCORE_HARD );
		if ( hi < player[0].score )
		{
			SetSaveDate( EN_SAVEDATA_SCORE_HARD, player[0].score );
		}
		break;
	case LEVEL_ENDRESS:
		hi = GetSaveDate( EN_SAVEDATA_SCORE_ENDRESS );
		if ( hi < player[0].score )
		{
			SetSaveDate( EN_SAVEDATA_SCORE_ENDRESS, player[0].score );
		}
		break;
	}
}


//プレイヤー関連----------------------------------------------
void create_player( int player_num )
{
	memset( &player[player_num], 0, sizeof( CHARCTAR ) );
	
	player[player_num].pos.x = 100 * DOT;
	player[player_num].pos.y = 240 * DOT;
	player[player_num].spd = PLAYER_SPEED_SHOT;
	player[player_num].life = 60 * 2;
	player[player_num].collsize.x = 8 * DOT;
	player[player_num].collsize.y = 8 * DOT;
	player[player_num].damege = 1;

	player[player_num].atk.rensya_inicount = 4;
	player[player_num].atk.rensya_iniinter = 2;
	player[player_num].atk.rensya_inter = 0;
	player[player_num].atk.rensya_count = 0;
	player[player_num].atk.rensou_count = 3;
	player[player_num].atk.loop_count = 0;
}

void move_player( int player_num )
{
	if ( player[player_num].life > 0 )
	{
		
		int oldx = player[player_num].pos.x;
		int oldy = player[player_num].pos.y;
		
		int inkey = 0;
		int indir = 0;

		//移動	
		if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_LEFT ) )		// 左
		{
			inkey = 1;
			indir = 768;
		}
		else if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_RIGHT ) )	// 右
		{
			inkey = 1;
			indir = 256;
		}

		if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_UP ) )
		{
			inkey = 1;
			indir = 0;
			if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_LEFT ) )		// 左
			{
				indir = 896;
			}
			else if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_RIGHT ) )	// 右
			{
				indir = 128;
			}
		}
		else if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_DOWN ) )
		{
			inkey = 1;
			indir = 512;
			if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_LEFT ) )		// 左
			{
				indir = 640;
			}
			else if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_RIGHT ) )	// 右
			{
				indir = 384;
			}
		}
		//	1
		if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_3 ) )
		{
			//ショット
			player[player_num].atk.rensya_count = player[player_num].atk.rensya_inicount;	//連射数を設定
			player[player_num].mode_count++;
			if ( player[player_num].mode_count > PLAYER_SHOT_MODE_CHANGE )
			{
				player[player_num].mode_count = PLAYER_SHOT_MODE_CHANGE;
				player[player_num].mode = EN_PLYMODE_LASER;
			}
		}
		else 
		{
			player[player_num].mode_count = 0;
			player[player_num].mode = EN_PLYMODE_SHOT;
		}
		
		//	2
		if ( IsPushKey( EN_PLAYERNUM_01, EN_INPUT_2 ) )
		{
			if ( player[player_num].move[0] == 0 )
			{
				player[player_num].move[0] = 180;	//使用不可硬直
				player[player_num].move[1] = 30;	//敵弾吸収時間
				soundPlaySe( EN_SE_BOM3 );
				AddSaveDate( EN_SAVEDATA_MANDARA, 1 );
			}
		}
		player[player_num].move[0]--;
		if ( player[player_num].move[0] < 0 ) 
		{
			player[player_num].move[0] = 0;
		}
		if ( player[player_num].move[0] == 1 )
		{
			soundPlaySe( EN_SE_DEFENCE );
		}
		player[player_num].move[1]--;
		if ( player[player_num].move[1] < 0 ) 
		{
			player[player_num].move[1] = 0;
		}
		else
		{
			//敵弾吸収処理
			int i;
			for ( i = 0; i < ENESHOT_MAX; i++ )
			{
				if ( eneshot[i].life > 0 )
				{
					int kin = Kinjiti( player[player_num].pos.x / DOT, player[player_num].pos.y / DOT, eneshot[i].pos.x / DOT, eneshot[i].pos.y / DOT );
					if ( kin < ( 48 ) )
					{
						eneshot[i].life = 0;	//消す
						player[player_num].move[2] += 8;	//敵弾吸収時間
						if ( player[player_num].move[2] > 60 )
						{
							player[player_num].move[2] = 60;	//マンダラショット時間
						}
						else
						{
//							diffic_red_dec( 5 );
						}
					}
				}
			}
		}
		player[player_num].move[2]--;
		if ( player[player_num].move[2] < 0 ) 
		{
			player[player_num].move[2] = 0;
		}
		
		
		//	3
		if ( IsPressKey( EN_PLAYERNUM_01, EN_INPUT_1 ) )
		{
			player[player_num].atk.rensya_count = 1;	//連射数を設定
		}

		//ショットモード固有の処理
		if ( player[player_num].mode == EN_PLYMODE_SHOT )
		{
			player[player_num].atk.start_inter--;	//連射数を設定
			if ( player[player_num].atk.start_inter < 0 )
			{
				player[player_num].atk.start_inter = 0;
			}
			player[player_num].spd = PLAYER_SPEED_SHOT;		//移動力設定
		}
		if ( player[player_num].mode == EN_PLYMODE_LASER )
		{
			player[player_num].atk.start_inter++;	//連射数を設定
			if ( player[player_num].atk.start_inter > PLAYER_SHOT_START_INTMAX )
			{
				player[player_num].atk.start_inter = PLAYER_SHOT_START_INTMAX;
			}
			player[player_num].spd = PLAYER_SPEED_LASER;	//移動力設定
		}

		if ( inkey != 0 )
		{
			player[player_num].dir = EndDirAddDir( indir, player[player_num].dir, 1024 );
			DirMove( player[player_num].dir, player[player_num].spd, player[player_num].pos.x, player[player_num].pos.y );
			player[player_num].pos.x = func_return_x;
			player[player_num].pos.y = func_return_y;
		}
		if ( player[player_num].move[2] > 0 )
		{
			//マンダラショット発射中
			DirMove( 768, PLAYER_SPEED_LASER , player[player_num].pos.x, player[player_num].pos.y );
			player[player_num].pos.x = func_return_x;
			player[player_num].pos.y = func_return_y;
			player[player_num].atk.rensya_count = 1;
		}
		
		//ショットの発射
		if ( player[player_num].atk.rensya_count > 0 )			//発射を行う
		{
			player[player_num].atk.rensya_inter--;				//インターバルカウント
			if ( player[player_num].atk.rensya_inter <= 0 )		//発射
			{
				player[player_num].atk.rensya_inter = player[player_num].atk.rensya_iniinter;	//インターバル初期化
				player[player_num].atk.rensya_count--;
				
				if ( player[player_num].move[2] > 0 )
				{
					//マンダラショット
					//連装数以上の空きがある場合に発射
					if ( ( PLYSHOT_NAGAOSI_MAX - stage_info.plyshot ) >= player[player_num].atk.rensou_count )
					{
						int vx = 0;
						int vy = 0;
						int dir = ( player[player_num].timer * 30 ) % 1024;
						
						DirMove( dir , 20 * DOT, 0, 0 );
						vx = func_return_x / 2;
						vy = func_return_y;
						vx += 10 * DOT;
						vy += 0;
						create_plyshot( EN_SHOT_TYPE_HYPER, player[player_num].pos.x + vx, player[player_num].pos.y + vy, 256, PLAYER_SHOT_SPEED * DOT );

						DirMove( dir + 512 , 20 * DOT, 0, 0 );
						vx = func_return_x / 2;
						vy = func_return_y;
						vx += 10 * DOT;
						vy += 0;
						create_plyshot( EN_SHOT_TYPE_HYPER, player[player_num].pos.x + vx, player[player_num].pos.y + vy, 256, PLAYER_SHOT_SPEED * DOT );

						DirMove( 1024-dir , 10 * DOT, 0, 0 );
						vx = func_return_x / 2;
						vy = func_return_y;
						vx += 10 * DOT;
						vy += 0;
						create_plyshot( EN_SHOT_TYPE_HYPER, player[player_num].pos.x + vx, player[player_num].pos.y + vy, 256, PLAYER_SHOT_SPEED * DOT );

						DirMove( 1024-dir + 512 , 10 * DOT, 0, 0 );
						vx = func_return_x / 2;
						vy = func_return_y;
						vx += 10 * DOT;
						vy += 0;
						create_plyshot( EN_SHOT_TYPE_HYPER, player[player_num].pos.x + vx, player[player_num].pos.y + vy, 256, PLAYER_SHOT_SPEED * DOT );
					}
				}
				else
				{
					//連装数以上の空きがある場合に発射
					if ( ( PLYSHOT_TATAKI_MAX - stage_info.plyshot ) >= player[player_num].atk.rensou_count )
					{
						{
							int vx = 0;
							int vy = 0;
							vx += 10 * DOT;
							vy += 0;
							create_plyshot( EN_SHOT_TYPE_SHOT, player[player_num].pos.x + vx, player[player_num].pos.y + vy, 256, PLAYER_SHOT_SPEED * DOT );
							vy = -4 * DOT;
							create_plyshot( EN_SHOT_TYPE_SHOT, player[player_num].pos.x + vx, player[player_num].pos.y + vy, 256-42, PLAYER_SHOT_SPEED * DOT );
							vy = +4 * DOT;
							create_plyshot( EN_SHOT_TYPE_SHOT, player[player_num].pos.x + vx, player[player_num].pos.y + vy, 256+42, PLAYER_SHOT_SPEED * DOT );
						}
					}
				}
			}
		}
		
		player[player_num].muteki--;
		if ( player[player_num].muteki < 0 )
		{
			player[player_num].muteki = 0;
		}
		
		player[player_num].timer++;
		
		if ( player[player_num].pos.x < ( 10 * DOT ) )
		{
			player[player_num].pos.x = ( 10 * DOT );
		}
		if ( player[player_num].pos.x > getMapWidth( ) )
		{
			player[player_num].pos.x = getMapWidth( );
		}
		if ( player[player_num].pos.y < 0 )
		{
			player[player_num].pos.y = 0;
		}
		if ( player[player_num].pos.y > getMapHeight( ) )
		{
			player[player_num].pos.y = getMapHeight( );
		}
		set_collision( EN_COLTYPE_PLAYER, player_num, player[player_num].pos.x, player[player_num].pos.y, player[player_num].collsize.x * 2, player[player_num].collsize.y * 2, EN_HITPARAM_TI, player[player_num].damege );
		
		{
			int ei = 0;
			for ( ei = SAKIYOMI_MAX - 2; ei >= 0; ei-- )
			{
				sakiyomi[ei+1].x = sakiyomi[ei].x;
				sakiyomi[ei+1].y = sakiyomi[ei].y;
			}
			sakiyomi[0].x = player[player_num].pos.x - oldx;
			sakiyomi[0].y = player[player_num].pos.y - oldy;
			
			
		}
		
		if ( ( player[player_num].timer % 60 ) == 0 )
		{
				player[player_num].life--;
		}

	}
	else
	{
		//存在しないかゲームオーバー
		//ゲームオーバー
		if ( g_DebugMuteki == 0 )
		{
			if ( player[player_num].life <= 0 )
			{
				player[player_num].life = 0;
				stage_info.scene_wait = 0;
				scene = END;
				AddSaveDate( EN_SAVEDATA_GAMEOVER, 1 );
			}
		}
		else
		{
			player[player_num].life = 10;	//無敵
		}
	}
	if ( scene == END )
	{
		player[player_num].life = 0;	//強制死亡
	}
	
}

void darw_player( int player_num )
{
	if ( player[player_num].life > 0 )
	{
		if ( ( player[player_num].move[0] <= 20 ) && ( player[player_num].move[0] > 0 ) )
		{
			int ei = 0;
			for ( ei = 0; ei < 32; ei++ )
			{
				DirMove( ( ei * 32 ), ( player[player_num].move[0] * 4 ) * DOT, player[player_num].pos.x, player[player_num].pos.y );
				int x = func_return_x;
				int y = func_return_y;
				int cl = ( player[player_num].move[0] * 12 );
				ObjSetEx( C_EF_BALL_P000_00_K + player[player_num].move[0], PRI_EXP, x / DOT, y / DOT, 1.0f, 1.0f, 0, cl, cl, cl, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
			}
		}
		if ( player[player_num].move[0] == 0 )
		{
			int ei = 0;
			for ( ei = 0; ei < 32; ei++ )
			{
				DirMove( ( ( ei * 32 ) + ( player[player_num].timer ) ) % 1024 , 64 * DOT, player[player_num].pos.x, player[player_num].pos.y );
				int x = func_return_x;
				int y = func_return_y;
				int cl = 128 + ( ( funcSin( ( ( player[player_num].timer % 1024 ) * 20 ) ) * 127 ) >> 16 );
				ObjSetEx( C_EF_BALL_P000_00_K + 10 + ( ( funcSin( ( player[player_num].timer * ei ) ) * 4 ) >> 16 ), PRI_EXP, x / DOT, y / DOT, 1.0f, 1.0f, 0, cl, cl, cl, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
			}
		}
		else
		{
			DrawNumer( player[0].move[0], 3, ( player[player_num].pos.x / DOT ) - 20, ( player[player_num].pos.y / DOT ) + 60, 0, 0 );	
		}
		
		if ( player[player_num].move[1] > 0 )
		{
			ObjSetEx( C_MANDARA_P000_00_K, PRI_PLAYER, player[player_num].pos.x / DOT, player[player_num].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
		}
		if ( player[player_num].move[2] > 0 )
		{
			ObjSetEx( C_PLAYER_P000_00_K + 3, PRI_PLAYER, player[player_num].pos.x / DOT, player[player_num].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
		}
		else
		{
			if ( ( player[player_num].muteki == 0 ) || ( ( player[player_num].muteki % 2 ) == 0 ) )
			{
				ObjSetEx( C_PLAYER_P000_00_K + ( player[player_num].timer % 3 ), PRI_PLAYER, player[player_num].pos.x / DOT, player[player_num].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
			}
		}
	}
}
void SetplayerDamege( COLLISION my, COLLISION you )
{
	CHARCTAR mychar = GetCollisionType_to_CharctarBuffer( my.coltype, my.index );
	CHARCTAR youchar = GetCollisionType_to_CharctarBuffer( you.coltype, you.index );
	int player_num = my.index;
	int damege = you.damege;

	if ( player[player_num].muteki == 0 )
	{
		if ( g_DebugMuteki == 0 )
		{
//			player[player_num].life -= damege;
			player[player_num].life -= 40;
			soundPlaySe( EN_SE_DAMEGE );
			
			int ei;
			for ( ei = 0; ei < 64; ei++ )
			{
				int rdir = funcRand( 1024 );
				int rspd = funcRand( 10 ) * DOT;
				create_effect( EN_EFC_PLYSHT_TOUCH_02, player[player_num].pos.x, player[player_num].pos.y, rdir, rspd );
			}
			player[player_num].muteki = 30 * 1;
			player[player_num].move[0] = 0;
			player[player_num].move[1] = 0;
			player[player_num].move[2] = 0;
			BulletClaer( 30, 0 );
			set_HitStop( 30 );
			diffic_red_dec( 50 );
			create_effect( EN_EFC_MISSTIME, player[player_num].pos.x, player[player_num].pos.y, 0, 200 );
			
			AddSaveDate( EN_SAVEDATA_MISS, 1 );
		}
	}
/*	
	if ( g_DebugMuteki == 0 )
	{
		//ゲームオーバー
		if ( player[player_num].life <= 0 )
		{
			stage_info.scene_wait = 0;
			scene = END;
		}
	}
*/
}

void AddScoer( int player_num, int add )
{
	player[player_num].score += add;
}

//ゲーム難易度
void diffic( void )
{
	g_Rank = 100 + ( stage_info.stage * 15 ) + ( stage_info.round * 5 );
}

void diffic_red_add( int add )
{
	int red_max = 100;
	switch( GetSaveDate( EN_SAVEDATA_SELECTLEVEL ) )
	{
	case LEVEL_EASY:
		red_max = 40;
		break;
	case LEVEL_NOMAL:
	case LEVEL_HARD:
	case LEVEL_ENDRESS:
		break;
	}
	
	
	g_Rank_Red += add;
	if ( g_Rank_Red > red_max )
	{
		g_Rank_Red = red_max;
	}
}
void diffic_red_dec( int dec )
{
	int red_min = 0;
	switch( GetSaveDate( EN_SAVEDATA_SELECTLEVEL ) )
	{
	case LEVEL_EASY:
	case LEVEL_NOMAL:
	case LEVEL_ENDRESS:
		break;
	case LEVEL_HARD:
		red_min = 40;
		break;
	}

	g_Rank_Red -= dec;
	if ( g_Rank_Red < red_min )
	{
		g_Rank_Red = red_min;
	}
}

//敵関連----------------------------------------------
void create_enemy( int type, int x, int y, int dir, int spd, int wait, int movetype )
{
	int i;
	
	for ( i = 0; i < ENEMY_MAX; i++ )
	{
		if ( enemy[i].life <= 0 )
		{
			memset( &enemy[i], 0, sizeof( CHARCTAR ) );

			enemy[i].pos.x = x;
			enemy[i].pos.y = y;
//			enemy[i].life = 10 + ( stage_info.stage * 2 );
			enemy[i].life = 10;
			enemy[i].dispsize = 64;
			enemy[i].damege = 100;
			enemy[i].type = type;
			enemy[i].dir = dir;
			enemy[i].spd = spd;
			enemy[i].akirame = 60 * 10;
			enemy[i].muteki = 10;
			enemy[i].seq = 0;
			enemy[i].wait = wait;
			enemy[i].movetype = movetype;

			enemy[i].atk.rensya_inicount = 100;
			enemy[i].atk.rensya_iniinter = 200;
			enemy[i].atk.rensya_inter = 8;
			enemy[i].atk.rensya_count = enemy[i].atk.rensya_inicount;
			enemy[i].atk.rensou_count = 0;
			enemy[i].atk.loop_count = 0;

			enemy[i].collsize.x = 24 * DOT;
			enemy[i].collsize.y = 24 * DOT;


			switch( enemy[i].type )
			{
			case EN_ENEMY_01:
				enemy[i].score = 3;
				enemy[i].atk.rensya_iniinter = 230;
				break;
			case EN_ENEMY_02:
				enemy[i].score = 3;
				enemy[i].atk.rensya_iniinter = 330;
				break;
			case EN_ENEMY_03:
				enemy[i].score = 3;
				enemy[i].atk.rensya_iniinter = 280;
				break;
			case EN_ENEMY_MBOSS_01:
				enemy[i].score = 10;
				enemy[i].collsize.x = 32 * DOT;
				enemy[i].collsize.y = 32 * DOT;
				enemy[i].life = 80;

				enemy[i].atk.rensya_inicount = 200;
				enemy[i].atk.rensya_iniinter = 8;
				enemy[i].atk.rensya_inter = 40;
				enemy[i].atk.rensya_count = enemy[i].atk.rensya_inicount;
				enemy[i].atk.rensou_count = 9999;
				enemy[i].akirame = 60 * 60 * 10;
				
				enemy[i].targetpos.x = 540 * DOT;
				enemy[i].targetpos.y = 200 * DOT;
				break;
			case EN_ENEMY_MBOSS_02:
				enemy[i].score = 10;
				enemy[i].collsize.x = 32 * DOT;
				enemy[i].collsize.y = 32 * DOT;
				enemy[i].life = 80;

				enemy[i].atk.rensya_inicount = 200;
				enemy[i].atk.rensya_iniinter = 8;
				enemy[i].atk.rensya_inter = 40;
				enemy[i].atk.rensya_count = enemy[i].atk.rensya_inicount;
				enemy[i].atk.rensou_count = 9999;
				enemy[i].akirame = 60 * 60 * 10;
				
				enemy[i].targetpos.x = 540 * DOT;
				enemy[i].targetpos.y = 200 * DOT;
				break;
			case EN_ENEMY_BOSS_01:
				enemy[i].score = 30;
				enemy[i].collsize.x = 64 * DOT;
				enemy[i].collsize.y = 96 * DOT;
				enemy[i].life = 200;

				enemy[i].atk.rensya_inicount = 200;
				enemy[i].atk.rensya_iniinter = 8;
				enemy[i].atk.rensya_inter = 40;
				enemy[i].atk.rensya_count = enemy[i].atk.rensya_inicount;
				enemy[i].atk.rensou_count = 9999;
				enemy[i].akirame = 60 * 60 * 10;
				
				enemy[i].targetpos.x = 540 * DOT;
				enemy[i].targetpos.y = 200 * DOT;
				break;
			case EN_ENEMY_BOSS_02:
				enemy[i].score = 30;
				enemy[i].collsize.x = 78 * DOT;
				enemy[i].collsize.y = 78 * DOT;
				enemy[i].life = 200;

				enemy[i].atk.rensya_inicount = 200;
				enemy[i].atk.rensya_iniinter = 8;
				enemy[i].atk.rensya_inter = 40;
				enemy[i].atk.rensya_count = enemy[i].atk.rensya_inicount;
				enemy[i].atk.rensou_count = 9999;
				enemy[i].akirame = 60 * 60 * 10;
				
				enemy[i].targetpos.x = 540 * DOT;
				enemy[i].targetpos.y = 200 * DOT;
				break;
			case EN_ENEMY_BOSS_03:
				enemy[i].score = 30;
				enemy[i].collsize.x = 64 * DOT;
				enemy[i].collsize.y = 40 * DOT;
				enemy[i].life = 200;

				enemy[i].atk.rensya_inicount = 200;
				enemy[i].atk.rensya_iniinter = 8;
				enemy[i].atk.rensya_inter = 40;
				enemy[i].atk.rensya_count = enemy[i].atk.rensya_inicount;
				enemy[i].atk.rensou_count = 9999;
				enemy[i].akirame = 60 * 60 * 10;
				
				enemy[i].targetpos.x = 540 * DOT;
				enemy[i].targetpos.y = 200 * DOT;
				break;
			case EN_ENEMY_BOSS_04:
				enemy[i].score = 30;
				enemy[i].collsize.x = 64 * DOT;
				enemy[i].collsize.y = 76 * DOT;
				enemy[i].life = 200;

				enemy[i].atk.rensya_inicount = 200;
				enemy[i].atk.rensya_iniinter = 8;
				enemy[i].atk.rensya_inter = 40;
				enemy[i].atk.rensya_count = enemy[i].atk.rensya_inicount;
				enemy[i].atk.rensou_count = 9999;
				enemy[i].akirame = 60 * 60 * 10;
				
				enemy[i].targetpos.x = 540 * DOT;
				enemy[i].targetpos.y = 200 * DOT;
				break;
			case EN_ENEMY_BOSS_05:
				enemy[i].score = 30;
				enemy[i].collsize.x = 40 * DOT;
				enemy[i].collsize.y = 64 * DOT;
				enemy[i].life = 200;

				enemy[i].atk.rensya_inicount = 200;
				enemy[i].atk.rensya_iniinter = 8;
				enemy[i].atk.rensya_inter = 40;
				enemy[i].atk.rensya_count = enemy[i].atk.rensya_inicount;
				enemy[i].atk.rensou_count = 9999;
				enemy[i].akirame = 60 * 60 * 10;
				
				enemy[i].targetpos.x = 540 * DOT;
				enemy[i].targetpos.y = 200 * DOT;
				break;
			default:
				break;
			}
			break;
		}
	}
}

void move_enemy( void )
{
	int i;
	int figdir;
	int fig_saki_dir;
	int sax = 0;
	int say = 0;
	int spd_wari = 1;
	
	if ( stage_info.bg_change == 0 )
	{
		spd_wari = 2;
	}
	
	stage_info.enemy = 0;
	stage_info.boss = 0;
	
	
	{
		int ei = 0;
		for ( ei = 0; ei < SAKIYOMI_MAX; ei++ )
		{
			sax += sakiyomi[ei].x;
			say += sakiyomi[ei].y;
		}
		sax = sax / SAKIYOMI_MAX;
		say = say / SAKIYOMI_MAX;
	}
	
	for ( i = 0; i < ENEMY_MAX; i++ )
	{
		if ( enemy[i].life > 0 )
		{
			stage_info.enemy++;
			
			enemy[i].wait--;
			if ( enemy[i].wait > 0 )
			{
				//出現待ち
				continue;
			}
			else
			{
				enemy[i].wait = 0;
			}
			enemy[i].timer++;

			switch( enemy[i].movetype )
			{
			case 	EN_ENE_MOVETYPE_DEC_ADD_UE:		//減速後上に移動
				if ( enemy[i].movetype_flg < 30 )
				{
					if ( enemy[i].spd > ( 0 * DOT ) )
					{
						enemy[i].spd -= 20;
					}
					else
					{
						enemy[i].spd = ( 0 * DOT );
						enemy[i].movetype_flg++;
					}
				}
				else
				{
					enemy[i].dir = EndDirAddDir( 256, enemy[i].dir, 0x10 );
					if ( enemy[i].spd < ( 3 * DOT ) )
					{
						enemy[i].spd += 20;
					}
					else
					{
						enemy[i].spd = ( 3 * DOT );
					}
				}
				break;
			case EN_ENE_MOVETYPE_DEC_ADD_SITA:	//減速後下に移動
				if ( enemy[i].movetype_flg < 30 )
				{
					if ( enemy[i].spd > ( 0 * DOT ) )
					{
						enemy[i].spd -= 20;
					}
					else
					{
						enemy[i].spd = ( 0 * DOT );
						enemy[i].movetype_flg++;
					}
				}
				else
				{
					enemy[i].dir = EndDirAddDir( 768, enemy[i].dir, 0x08 );
					if ( enemy[i].spd < ( 3 * DOT ) )
					{
						enemy[i].spd += 20;
					}
					else
					{
						enemy[i].spd = ( 3 * DOT );
					}
				}
				break;
			case EN_ENE_MOVETYPE_HOMING:		//自機に向かって移動
			{
				if ( enemy[i].movetype_flg == 0 )
				{
					figdir = FigDir( enemy[i].pos.x, enemy[i].pos.y );
					enemy[i].dir = EndDirAddDir( figdir, enemy[i].dir, 0x08 );
					int kin = Kinjiti( player[0].pos.x / DOT, player[0].pos.y / DOT, enemy[i].pos.x / DOT, enemy[i].pos.y / DOT );
					if ( kin < ( 64 ) )
					{
						enemy[i].movetype_flg = 1;
					}
				}
				break;
			}
			case EN_ENE_MOVETYPE_DEC_ADD:		//減速＞加速
				if ( enemy[i].movetype_flg < 30 )
				{
					if ( enemy[i].spd > ( 0 * DOT ) )
					{
						enemy[i].spd -= 20;
					}
					else
					{
						enemy[i].spd = ( 0 * DOT );
						enemy[i].movetype_flg++;
					}
				}
				else
				{
					if ( enemy[i].spd < ( 3 * DOT ) )
					{
						enemy[i].spd += 20;
					}
					else
					{
						enemy[i].spd = ( 3 * DOT );
					}
				}
				break;
			case EN_ENE_MOVETYPE_DEC_STOP:		//減速＞停止
				if ( enemy[i].spd > ( 0 * DOT ) )
				{
					enemy[i].spd -= 20;
				}
				else
				{
					enemy[i].spd = ( 0 * DOT );
				}
				break;
			case EN_ENE_MOVETYPE_DEC_STOP_YURE:	//減速＞停止＞適当にゆれる
			{
				if ( enemy[i].spd > ( 1 * DOT ) )
				{
					enemy[i].spd -= 20;
				}
				else
				{
					enemy[i].spd = ( 1 * DOT );
				}

				int tergetdir = PointDir( enemy[i].pos.x, enemy[i].pos.y, enemy[i].targetpos.x, enemy[i].targetpos.y );
				enemy[i].dir = EndDirAddDir( tergetdir, enemy[i].dir, 0x08 );
				
				int kin = Kinjiti( enemy[i].pos.x, enemy[i].pos.y, enemy[i].targetpos.x, enemy[i].targetpos.y );

				if ( kin < ( 40 * DOT ) )
				{
					enemy[i].targetpos.x = ( funcRand( 200 ) + 420 ) * DOT;
					enemy[i].targetpos.y = ( funcRand( 200 ) + 80 ) * DOT;
				}
				break;
			}
			case EN_ENE_MOVETYPE_DEC_SONOMAMA:	//減速＞低速で移動
				if ( enemy[i].spd > ( 1 * DOT ) )
				{
					enemy[i].spd -= 30;
				}
				else
				{
					enemy[i].spd = ( 1 * DOT );
				}
				break;
			case EN_ENE_MOVETYPE_SONOMAMA:		//そのままの速度で移動
				break;
			default:
				break;
			}

			switch( enemy[i].type )
			{
			case EN_ENEMY_MBOSS_01:
			case EN_ENEMY_MBOSS_02:
			case EN_ENEMY_BOSS_01:
			case EN_ENEMY_BOSS_02:
			case EN_ENEMY_BOSS_03:
			case EN_ENEMY_BOSS_04:
			case EN_ENEMY_BOSS_05:
				stage_info.boss = 1;
				break;
			default:
				break;
			}


			DirMove( enemy[i].dir, enemy[i].spd, enemy[i].pos.x, enemy[i].pos.y );
			enemy[i].pos.x = func_return_x;
			enemy[i].pos.y = func_return_y;

			figdir = FigDir( enemy[i].pos.x, enemy[i].pos.y );
//			int kyori = Kinjiti( enemy[i].pos.x, enemy[i].pos.y, sax, say );
			int px = ( player[0].pos.x + sax );
			int py = ( player[0].pos.y + say );
			UINT32 kyori = getKyori( enemy[i].pos.x - px, enemy[i].pos.y - py );
			int sax2 = ( player[0].pos.x ) + ( ( kyori / 800 ) * sax );
			int say2 = ( player[0].pos.y ) + ( ( kyori / 800 ) * say );
			
			if ( kyori > ( 300 * DOT ) )
			{
				sax2 = ( player[0].pos.x ) + ( ( kyori / 1600 ) * sax );
				say2 = ( player[0].pos.y ) + ( ( kyori / 1600 ) * say );
			}
			if ( kyori > ( 500 * DOT ) )
			{
//				sax2 = ( player[0].pos.x ) + ( ( kyori / 3200 ) * sax );
//				say2 = ( player[0].pos.y ) + ( ( kyori / 3200 ) * say );
			}
			
			sax2 = sax + sax2;
			say2 = say + say2;
			fig_saki_dir = PointDir( enemy[i].pos.x, enemy[i].pos.y, sax2, say2 );
			
			if ( sax2 < 0 )
			{
				sax2 = 0;
				fig_saki_dir = PointDir( enemy[i].pos.x, enemy[i].pos.y, sax2, say2 );
			}
			if ( sax2 > getMapWidth( ) )
			{
				sax2 = getMapWidth( );
				fig_saki_dir = PointDir( enemy[i].pos.x, enemy[i].pos.y, sax2, say2 );
			}
			if ( say2 < 0 )
			{
				say2 = 0;
				fig_saki_dir = PointDir( enemy[i].pos.x, enemy[i].pos.y, sax2, say2 );
			}
			if ( say2 > getMapHeight( ) )
			{
				say2 = getMapHeight( );
				fig_saki_dir = PointDir( enemy[i].pos.x, enemy[i].pos.y, sax2, say2 );
			}
			
			if ( kyori > ( 400 * DOT ) )
			{
				fig_saki_dir = figdir;
			}
			if ( funcRand( 4 ) == 0 )
			{
				fig_saki_dir = figdir;
			}


			set_collision( EN_COLTYPE_ENEMY, i, enemy[i].pos.x, enemy[i].pos.y, enemy[i].collsize.x * 2, enemy[i].collsize.y * 2, EN_HITPARAM_TI, enemy[i].damege );
			set_collision( EN_COLTYPE_ENEMY, i, enemy[i].pos.x, enemy[i].pos.y, enemy[i].collsize.x, enemy[i].collsize.y, EN_HITPARAM_KU, enemy[i].damege );
/*			
			if ( ( enemy[i].timer % 60 ) == 0 )
			{
//				Bullet( EN_ENESHOT_02 , enemy[i].pos.x, enemy[i].pos.y, figdir, 160 );
//				BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir, 160, 5, 32 );
//				BulletWayWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir, 160, 3, 128, 4, 16 );
//				BulletTat( EN_ENESHOT_02 , enemy[i].pos.x, enemy[i].pos.y, figdir, 160, 4, 40 );

				create_effect( EN_EFC_ENEMZL_01, enemy[i].pos.x, enemy[i].pos.y, 0, 0 );				//マズル表示

				if ( enemy[i].type == EN_ENEMY_01 )
				{
					BulletWayTat( EN_ENESHOT_01 ,enemy[i].pos.x, enemy[i].pos.y, figdir, 160, 5, 32, 4, 40 );
				}
				if ( enemy[i].type == EN_ENEMY_02 )
				{
					BulletWayWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir, 160, 4, 128, 7, 16 );
				}
				if ( enemy[i].type == EN_ENEMY_03 )
				{
					BulletTat( EN_ENESHOT_02 , enemy[i].pos.x, enemy[i].pos.y, figdir, 160, 4, 40 );
				}

			}
*/
			if ( enemy[i].muteki > 0 )
			{
				enemy[i].muteki--; 
			}

			enemy[i].akirame--;
			if ( enemy[i].akirame > 0 ) 
			{
				if ( enemy[i].atk.rensya_count > 0 )				//連射数がある
				{
					enemy[i].atk.rensya_inter--;					//インターバルカウント
					if ( enemy[i].atk.rensya_inter <= 0 )			
					{
						if ( funcRand( 100 ) == 0 )
						{
							create_effect( EN_EFC_BOSSDEAD_SERIHU,
							               enemy[i].pos.x + ( ( 16 * DOT + funcRand( enemy[i].collsize.x / 2 ) ) ),
							               enemy[i].pos.y - ( ( 16 * DOT + funcRand( enemy[i].collsize.y / 2 ) ) ),
							               220 + funcRand( 20 ),
							               50 + funcRand( 100 )
							              );
						}
						
						//発射
						enemy[i].atk.rensya_inter = enemy[i].atk.rensya_iniinter;	//インターバル初期化
						//ランク
						enemy[i].atk.rensya_inter -= g_Rank;
						if ( enemy[i].atk.rensya_inter < 2 )
						{
							enemy[i].atk.rensya_inter = 2;
						}
						
						enemy[i].atk.rensya_count--;				//連射数カウント

						// 攻撃
						figdir = FigDir( enemy[i].pos.x, enemy[i].pos.y );
						switch( enemy[i].type )
						{
						case EN_ENEMY_01:
							if ( stage_info.bg_change == 0 )
							{
								Bullet( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, fig_saki_dir, 200 );
							}
							else
							{
								Bullet( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, fig_saki_dir, 800 );
							}
							break;
						case EN_ENEMY_02:
							if ( stage_info.bg_change == 0 )
							{
								BulletWayTat( EN_ENESHOT_01 ,enemy[i].pos.x, enemy[i].pos.y, figdir, 300, 3, 40, 3, 50 );
							}
							else
							{
								BulletWayTat( EN_ENESHOT_01 ,enemy[i].pos.x, enemy[i].pos.y, figdir, 600, 3, 40, 3, 100 );
							}
							break;
						case EN_ENEMY_03:
						{
							if ( stage_info.bg_change == 0 )
							{
								int bure = 0;
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir + bure, 100, 2, 32 );
								bure = funcRand( 60 ) - 30;
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir + bure, 150, 2, 32 );
								bure = funcRand( 60 ) - 30;
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir + bure, 200, 2, 32 );
							}
							else
							{
								int bure = 0;
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir + bure, 200, 2, 32 );
								bure = funcRand( 60 ) - 30;
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir + bure, 250, 2, 32 );
								bure = funcRand( 60 ) - 30;
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir + bure, 300, 2, 32 );
							}
							break;
						}
						case EN_ENEMY_MBOSS_01:
							if ( stage_info.bg_change == 0 )
							{
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir, 200, 2, 128 );
								if ( ( enemy[i].atk.rensya_count % 10 ) == 0 )
								{
									BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir, 300, 3, 20 * ( enemy[i].atk.rensya_count % 4 ) );
								}
							}
							else
							{
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir, 300, 2, 128 );
								if ( ( enemy[i].atk.rensya_count % 10 ) == 0 )
								{
									BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir, 600, 3, 20 * ( enemy[i].atk.rensya_count % 4 ) );
								}
							}
							break;
						case EN_ENEMY_MBOSS_02:
						{
							if ( stage_info.bg_change == 0 )
							{
								int rdir = figdir + ( funcRand( 512 ) - 256 );
								Bullet( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, rdir, 50 );
								if ( ( enemy[i].atk.rensya_count % 10 ) == 0 )
								{
									Bullet( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir, 400 );
								}
							}
							else
							{
								int rdir = figdir + ( funcRand( 512 ) - 256 );
								Bullet( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, rdir, 100 );
								if ( ( enemy[i].atk.rensya_count % 10 ) == 0 )
								{
									Bullet( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir, 800 );
								}
							}
							break;
						}
						case EN_ENEMY_BOSS_01:
							if ( ( enemy[i].atk.rensya_count % 40 ) == 10 )
							{
								enemy[i].move[0] = 0;
							}
							if ( ( enemy[i].atk.rensya_count % 40 ) < 10 )
							{
								enemy[i].move[0]++;
								int spd = 0;
								if ( stage_info.bg_change == 0 )
								{
									spd = ( enemy[i].move[0] * 20 ) + 50;
								}
								else
								{
									spd = ( enemy[i].move[0] * 100 ) + 100;
								}

								figdir = FigDir( enemy[i].pos.x, enemy[i].pos.y + ( 100 * DOT ) );
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y + ( 100 * DOT ), figdir, spd, 5, 64 );

								figdir = FigDir( enemy[i].pos.x, enemy[i].pos.y - ( 100 * DOT ) );
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y - ( 100 * DOT ), figdir, spd, 5, 64 );
							}
							break;
						case EN_ENEMY_BOSS_02:
							if ( ( enemy[i].atk.rensya_count % 20 ) == 0 )
							{
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir, 1000 / spd_wari, 16, 64 );
							}
							if ( ( enemy[i].atk.rensya_count % 10 ) == 0 )
							{
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir, 600 / spd_wari, 15, 40 );
							}
							break;
						case EN_ENEMY_BOSS_03:
						{
							if ( ( enemy[i].atk.rensya_count % 40 ) == 10 )
							{
								enemy[i].move[0] = 0;
							}
							if ( ( enemy[i].atk.rensya_count % 40 ) < 10 )
							{
								enemy[i].move[0]++;
								int spd = ( enemy[i].move[0] * 80 ) + 100;
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x + ( 54 * DOT ), enemy[i].pos.y - ( 14 * DOT ), ( enemy[i].move[0] * 50 ), spd / spd_wari, 3, 341 );
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x - ( 54 * DOT ), enemy[i].pos.y - ( 14 * DOT ), -( enemy[i].move[0] * 50 ), spd / spd_wari, 3, 341 );
							}
							if ( ( enemy[i].atk.rensya_count % 30 ) == 10 )
							{
								figdir = FigDir( enemy[i].pos.x + ( 54 * DOT ), enemy[i].pos.y + ( 14 * DOT ) );
								BulletTat( EN_ENESHOT_01 , enemy[i].pos.x + ( 54 * DOT ), enemy[i].pos.y - ( 14 * DOT ), figdir, 600 / spd_wari, 5, 100 );
								figdir = FigDir( enemy[i].pos.x - ( 54 * DOT ), enemy[i].pos.y + ( 14 * DOT ) );
								BulletTat( EN_ENESHOT_01 , enemy[i].pos.x - ( 54 * DOT ), enemy[i].pos.y - ( 14 * DOT ), figdir, 600 / spd_wari, 5, 100 );
							}
							break;
						
						}
						case EN_ENEMY_BOSS_04:
							enemy[i].move[0]++ ;
							BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, ( enemy[i].move[0] * 3 ), 600 / spd_wari, 3, 341 );
							BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, ( enemy[i].move[0] * 5 ), 800 / spd_wari, 3, 341 );
							break;
						case EN_ENEMY_BOSS_05:
							if ( ( enemy[i].atk.rensya_count % 40 ) < 20 )
							{
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir, 1000 / spd_wari, 32, 32 );
							}
							if ( ( enemy[i].atk.rensya_count % 20 ) < 3 )
							{
								BulletWay( EN_ENESHOT_01 , enemy[i].pos.x, enemy[i].pos.y, figdir + ( funcRand( 128 ) - 64 ), 600 / spd_wari, 15, 40 );
							}
							break;
						default:
							break;
						}
					}
				}
				else 
				{
					//ループ回数を設定
					if ( enemy[i].atk.rensou_count > 0 )
					{
						enemy[i].atk.loop_count++;
						enemy[i].atk.rensou_count--;
						enemy[i].atk.rensya_count = enemy[i].atk.rensya_inicount;
						enemy[i].atk.rensya_inter = enemy[i].atk.rensya_iniinter;
					}
					else 
					{
						//攻撃完全におわり
					}
				}
			}
			if ( 0 != limit( enemy[i].pos.x, enemy[i].pos.y, enemy[i].dispsize ) )
			{
				if ( enemy[i].seq != 0 )
				{
					enemy[i].life = 0;
					AddSaveDate( EN_SAVEDATA_OUT_ENEMY, 1 );
				}
			}
			else
			{
				enemy[i].seq = 1;
			}
		}
	}
}

void darw_enemy( void )
{
	int i;
	
	for ( i = 0; i < ENEMY_MAX; i++ )
	{
		if ( enemy[i].life > 0 )
		{
			int moza = ( enemy[i].life / 8 ) - 3;
			if ( moza < 0 )
			{
				moza = 0;
			}
			
			switch( enemy[i].type )
			{
			case EN_ENEMY_01:
				ObjSetEx( C_ENEMY_01_P000_00_K, PRI_ENEMY, enemy[i].pos.x / DOT, enemy[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_ENEMY_02:
				ObjSetEx( C_ENEMY_02_P000_00_K, PRI_ENEMY, enemy[i].pos.x / DOT, enemy[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_ENEMY_03:
				ObjSetEx( C_ENEMY_03_P000_00_K, PRI_ENEMY, enemy[i].pos.x / DOT, enemy[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_ENEMY_MBOSS_01:
				ObjSetEx( C_ENEMY_MBOSS_01_P000_00_K, PRI_ENEMY, enemy[i].pos.x / DOT, enemy[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_ENEMY_MBOSS_02:
				ObjSetEx( C_ENEMY_MBOSS_02_P000_00_K, PRI_ENEMY, enemy[i].pos.x / DOT, enemy[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_ENEMY_BOSS_01:
				SetMozaiku( moza );
				ObjSetEx( C_TEKI_HEEHAW3_P000_00_K, PRI_ENEMY, enemy[i].pos.x / DOT, enemy[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_ENEMY_BOSS_02:
				SetMozaiku( moza );
				ObjSetEx( C_TEKI_HEEHAW2_P000_00_K, PRI_ENEMY, enemy[i].pos.x / DOT, enemy[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_ENEMY_BOSS_03:
				SetMozaiku( moza );
				ObjSetEx( C_TEKI_HEEHAW1_P000_00_K, PRI_ENEMY, enemy[i].pos.x / DOT, enemy[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_ENEMY_BOSS_04:
				SetMozaiku( moza );
				ObjSetEx( C_TEKI_HEEHAW5_P000_00_K, PRI_ENEMY, enemy[i].pos.x / DOT, enemy[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_ENEMY_BOSS_05:
				SetMozaiku( moza );
				ObjSetEx( C_TEKI_HEEHAW4_P000_00_K, PRI_ENEMY, enemy[i].pos.x / DOT, enemy[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			}
			SetMozaiku( 0 );
		}
	}
}
void GetEnemyPos( int i )
{
	func_return_x = -1000 * DOT;
	func_return_y = -1000 * DOT;

	if ( enemy[i].life > 0 )
	{
		func_return_x = enemy[i].pos.x;
		func_return_y = enemy[i].pos.y;
	}
}
int GetEnemyLife( int i )
{
	int rc = enemy[i].life;
	return( rc );
}
void SetEnemyDamege( COLLISION my, COLLISION you )
{
	CHARCTAR mychar = GetCollisionType_to_CharctarBuffer( my.coltype, my.index );
	CHARCTAR youchar = GetCollisionType_to_CharctarBuffer( you.coltype, you.index );
	int i = my.index;
	int damege = you.damege;

	if ( ( enemy[i].muteki == 0 ) && ( enemy[i].life > 0 ) )
	{
		enemy[i].life = enemy[i].life - damege;
		//ショットタッチエフェクト
		if ( enemy[i].life <= 0 )
		{
			int item_type = EN_ITEM_NONE;
			int item_num = 1;
			int bul_clear_item_type = EN_ITEM_NONE;
			if ( youchar.movetype != EN_SHOT_TYPE_HYPER )	//マンダラショットではない
			{
				create_item( EN_ITEM_SCOER_S, enemy[i].pos.x, enemy[i].pos.y, 0, 0 );
			}
			else
			{
				bul_clear_item_type = EN_ITEM_SCOER_M;
				item_num = ( g_Rank_Red / RED_SCALE ) + 1;
				int eei = 0;
				for ( eei = 0; eei < item_num; eei++ )
				{
					int bure = item_num * 16;
					int rx = ( funcRand( bure ) - ( bure / 2 ) ) * DOT;
					int ry = ( funcRand( bure ) - ( bure / 2 ) ) * DOT;
					create_item( EN_ITEM_SCOER_M, enemy[i].pos.x + rx, enemy[i].pos.y + ry, 0, 0 );
				}
			}

			int ei = 0;
			switch( enemy[i].type )
			{
			case EN_ENEMY_01:
			case EN_ENEMY_02:
			case EN_ENEMY_03:
				soundPlaySe( EN_SE_BOM1 + funcRand( 2 ) );
				create_effect( EN_EFC_ENEBAKU_03, enemy[i].pos.x, enemy[i].pos.y, 768 + funcRand( 10 ) - 5, 1500 + funcRand( 500 ) );
				for ( ei = 0; ei < 4; ei++ )
				{
					int rx = ( funcRand( 64 ) - 32 ) * DOT;
					int ry = ( funcRand( 64 ) - 32 ) * DOT;
					create_effect( EN_EFC_ENEBAKU_01, enemy[i].pos.x + rx, enemy[i].pos.y + ry, 768 + funcRand( 10 ) - 5, 1500 + funcRand( 500 ) );

					int rdir = funcRand( 1024 );
					int rspd = funcRand( 10 ) * DOT;
					create_effect( EN_EFC_PLYSHT_TOUCH_02, enemy[i].pos.x + rx, enemy[i].pos.y + ry, rdir, rspd );
				}
				for ( ei = 0; ei < 2; ei++ )
				{
					int rx = ( funcRand( 64 ) - 32 ) * DOT;
					int ry = ( funcRand( 64 ) - 32 ) * DOT;
					create_effect( EN_EFC_ENEBAKU_04, enemy[i].pos.x + rx, enemy[i].pos.y + ry, 768 + funcRand( 10 ) - 5, 1500 + funcRand( 500 ) );
				}

				break;
			case EN_ENEMY_MBOSS_01:
			case EN_ENEMY_MBOSS_02:
				soundPlaySe( EN_SE_BOM3 );
				set_Shake( 30 );
				BulletClaer( 10, bul_clear_item_type );
				player[0].life += 10;
				stage_info.enxt_stage = 1;
				create_effect( EN_EFC_EXTEND01, player[0].pos.x, player[0].pos.y + ( -40 * DOT ), 0, 300 );
				for ( ei = 0; ei < 2; ei++ )
				{
					int rx = ( funcRand( 64 ) - 32 ) * DOT;
					int ry = ( funcRand( 64 ) - 32 ) * DOT;
					create_effect( EN_EFC_ENEBAKU_02, enemy[i].pos.x + rx, enemy[i].pos.y + ry, 768 + funcRand( 10 ) - 5, 1500 + funcRand( 500 ) );
				}
				for ( ei = 0; ei < 16; ei++ )
				{
					int rx = ( funcRand( 128 ) - 64 ) * DOT;
					int ry = ( funcRand( 128 ) - 64 ) * DOT;
					create_effect( EN_EFC_ENEBAKU_01, enemy[i].pos.x + rx, enemy[i].pos.y + ry, 768 + funcRand( 10 ) - 5, 1500 + funcRand( 500 ) );

					int rdir = funcRand( 1024 );
					int rspd = funcRand( 10 ) * DOT;
					create_effect( EN_EFC_PLYSHT_TOUCH_02, enemy[i].pos.x + rx, enemy[i].pos.y + ry, rdir, rspd );
				}
				create_effect( EN_EFC_BOSSDEAD, enemy[i].pos.x, enemy[i].pos.y, 270 + funcRand( 20 ), 50 + funcRand( 100 ) );
				break;
			case EN_ENEMY_BOSS_01:
			case EN_ENEMY_BOSS_02:
			case EN_ENEMY_BOSS_03:
			case EN_ENEMY_BOSS_04:
			case EN_ENEMY_BOSS_05:
				soundPlaySe( EN_SE_BOSS_DEAD );
				set_Shake( 40 );
				BulletClaer( 10, bul_clear_item_type );
				player[0].life += 20;
				stage_info.enxt_stage = 1;
				create_effect( EN_EFC_EXTEND02, player[0].pos.x, player[0].pos.y + ( -40 * DOT ), 0, 300 );
				for ( ei = 0; ei < 4; ei++ )
				{
					int rx = ( funcRand( 256 ) - 128 ) * DOT;
					int ry = ( funcRand( 256 ) - 128 ) * DOT;
					create_effect( EN_EFC_ENEBAKU_02, enemy[i].pos.x + rx, enemy[i].pos.y + ry, 768 + funcRand( 10 ) - 5, 1500 + funcRand( 500 ) );
				}
				for ( ei = 0; ei < 4; ei++ )
				{
					int rx = ( funcRand( 256 ) - 128 ) * DOT;
					int ry = ( funcRand( 256 ) - 128 ) * DOT;
					create_effect( EN_EFC_ENEBAKU_03, enemy[i].pos.x + rx, enemy[i].pos.y + ry, 768 + funcRand( 10 ) - 5, 1500 + funcRand( 500 ) );
				}
				for ( ei = 0; ei < 64; ei++ )
				{
					int rx = ( funcRand( 256 ) - 128 ) * DOT;
					int ry = ( funcRand( 256 ) - 128 ) * DOT;
					create_effect( EN_EFC_ENEBAKU_01, enemy[i].pos.x + rx, enemy[i].pos.y + ry, 768 + funcRand( 10 ) - 5, 1500 + funcRand( 500 ) );

					int rdir = funcRand( 1024 );
					int rspd = funcRand( 10 ) * DOT;
					create_effect( EN_EFC_PLYSHT_TOUCH_02, enemy[i].pos.x + rx, enemy[i].pos.y + ry, rdir, rspd );
				}
				create_effect( EN_EFC_BOSSDEAD, enemy[i].pos.x, enemy[i].pos.y, 270 + funcRand( 20 ), 50 + funcRand( 100 ) );
				break;
			}
			if ( enemy[i].type == EN_ENEMY_BOSS_05 )
			{
				//クリア
//				stage_info.scene_wait = 0;
//				scene = CLEAR;
			}
			if ( enemy[i].type == EN_ENEMY_02 )
			{
				//次の敵が出現
				stage_info.enxt_stage = 1;
			}
			if ( stage_info.bg_change != 0 )
			{
//				diffic_red_add( 5 );
			}
/*			
			if ( player[0].move[0] > 20 )
			{
				player[0].move[0] -= 10;
				if ( player[0].move[0] < 20 )
				{
					player[0].move[0] = 20;
				} 
			}
*/
			AddScoer( 0, enemy[i].score );
			AddSaveDate( EN_SAVEDATA_ENEMY, 1 );
		}
	}
}


void set_collision( int coltype, int index, int x, int y, int w, int h, int hitparam, int damege )
{
	int i;
	
	for ( i = 0; i < COLLISION_MAX; i++ )
	{
		if ( collision[i].life <= 0 )
		{
			memset( &collision[i], 0, sizeof( COLLISION ) );
			collision[i].life = 1;				//存在の有無
			collision[i].coltype = coltype;				//存在の有無
			collision[i].index = index;			//存在の有無
			collision[i].pos.x = x;				//位置   キャラクターの中央からのオフセット
			collision[i].pos.y = y;				//位置   キャラクターの中央からのオフセット
			collision[i].size.x = w;			//サイズ
			collision[i].size.y = h;			//サイズ
			collision[i].hitparam = hitparam;	//判定の種類
			collision[i].damege = damege;		//相手へのダメージ
			break;
		}
	}
}
void move_collision( void )
{
	int i;
	
	for ( i = 0; i < COLLISION_MAX; i++ )
	{
		if ( collision[i].life > 0 )
		{
			int ch1_x1 = 0;
			int ch1_x2 = 0;
			int ch1_y1 = 0;
			int ch1_y2 = 0;

			int ch2_x1 = 0;
			int ch2_x2 = 0;
			int ch2_y1 = 0;
			int ch2_y2 = 0;
			//自機ショットと敵
			if ( collision[i].coltype == EN_COLTYPE_PLAYER_SHOT )
			{
				int j = 0;
				for ( j = 0; j < COLLISION_MAX; j++ )
				{

					if ( ( collision[j].coltype == EN_COLTYPE_ENEMY ) && ( collision[j].life > 0 ) )
					{
						ch1_x1 = collision[i].pos.x - ( collision[i].size.x / 2 );
						ch1_x2 = collision[i].pos.x + ( collision[i].size.x / 2 );
						ch1_y1 = collision[i].pos.y - ( collision[i].size.y / 2 );
						ch1_y2 = collision[i].pos.y + ( collision[i].size.y / 2 );

						ch2_x1 = collision[j].pos.x - ( collision[j].size.x / 2 );
						ch2_x2 = collision[j].pos.x + ( collision[j].size.x / 2 );
						ch2_y1 = collision[j].pos.y - ( collision[j].size.y / 2 );
						ch2_y2 = collision[j].pos.y + ( collision[j].size.y / 2 );

						//当たり判定
						if ( 
						     ( ch1_x1 < ch2_x2 )
						  && ( ch2_x1 < ch1_x2 )
						  && ( ch1_y1 < ch2_y2 )
						  && ( ch2_y1 < ch1_y2 )
						   )
						{
							//ヒットコールバック
							SetplyshotDamege( collision[i], collision[j] );
							SetEnemyDamege( collision[j], collision[i] );
						}
					}
				}
			}
			//敵ショットと自機
			if ( collision[i].coltype == EN_COLTYPE_ENEMY_SHOT )
			{
				int j = 0;
				for ( j = 0; j < COLLISION_MAX; j++ )
				{

					if ( ( collision[j].coltype == EN_COLTYPE_PLAYER ) && ( collision[j].life > 0 ) )
					{
						ch1_x1 = collision[i].pos.x - ( collision[i].size.x / 2 );
						ch1_x2 = collision[i].pos.x + ( collision[i].size.x / 2 );
						ch1_y1 = collision[i].pos.y - ( collision[i].size.y / 2 );
						ch1_y2 = collision[i].pos.y + ( collision[i].size.y / 2 );

						ch2_x1 = collision[j].pos.x - ( collision[j].size.x / 2 );
						ch2_x2 = collision[j].pos.x + ( collision[j].size.x / 2 );
						ch2_y1 = collision[j].pos.y - ( collision[j].size.y / 2 );
						ch2_y2 = collision[j].pos.y + ( collision[j].size.y / 2 );

						//当たり判定
						if ( 
						     ( ch1_x1 < ch2_x2 )
						  && ( ch2_x1 < ch1_x2 )
						  && ( ch1_y1 < ch2_y2 )
						  && ( ch2_y1 < ch1_y2 )
						   )
						{
							//ヒットコールバック
							SeteneshotDamege( collision[i], collision[j] );
							SetplayerDamege( collision[j], collision[i] );
						}
					}
				}
			}
			//自機と敵
			if ( collision[i].coltype == EN_COLTYPE_PLAYER )
			{
				int j = 0;
				for ( j = 0; j < COLLISION_MAX; j++ )
				{

					if ( ( collision[j].coltype == EN_COLTYPE_ENEMY ) && ( collision[j].life > 0 ) )
					{
						if ( collision[j].hitparam == EN_HITPARAM_KU )
						{
							ch1_x1 = collision[i].pos.x - ( collision[i].size.x / 2 );
							ch1_x2 = collision[i].pos.x + ( collision[i].size.x / 2 );
							ch1_y1 = collision[i].pos.y - ( collision[i].size.y / 2 );
							ch1_y2 = collision[i].pos.y + ( collision[i].size.y / 2 );

							ch2_x1 = collision[j].pos.x - ( collision[j].size.x / 2 );
							ch2_x2 = collision[j].pos.x + ( collision[j].size.x / 2 );
							ch2_y1 = collision[j].pos.y - ( collision[j].size.y / 2 );
							ch2_y2 = collision[j].pos.y + ( collision[j].size.y / 2 );

							//当たり判定
							if ( 
							     ( ch1_x1 < ch2_x2 )
							  && ( ch2_x1 < ch1_x2 )
							  && ( ch1_y1 < ch2_y2 )
							  && ( ch2_y1 < ch1_y2 )
							   )
							{
								//ヒットコールバック
								SetplayerDamege( collision[i], collision[j] );
								SetEnemyDamege( collision[j], collision[i] );
							}
						}
					}
				}
			}
			//アイテムと自機
			if ( collision[i].coltype == EN_COLTYPE_ITEM )
			{
				int j = 0;
				for ( j = 0; j < COLLISION_MAX; j++ )
				{

					if ( ( collision[j].coltype == EN_COLTYPE_PLAYER ) && ( collision[j].life > 0 ) )
					{
						ch1_x1 = collision[i].pos.x - ( collision[i].size.x / 2 );
						ch1_x2 = collision[i].pos.x + ( collision[i].size.x / 2 );
						ch1_y1 = collision[i].pos.y - ( collision[i].size.y / 2 );
						ch1_y2 = collision[i].pos.y + ( collision[i].size.y / 2 );

						ch2_x1 = collision[j].pos.x - ( collision[j].size.x / 2 );
						ch2_x2 = collision[j].pos.x + ( collision[j].size.x / 2 );
						ch2_y1 = collision[j].pos.y - ( collision[j].size.y / 2 );
						ch2_y2 = collision[j].pos.y + ( collision[j].size.y / 2 );

						//当たり判定
						if ( 
						     ( ch1_x1 < ch2_x2 )
						  && ( ch2_x1 < ch1_x2 )
						  && ( ch1_y1 < ch2_y2 )
						  && ( ch2_y1 < ch1_y2 )
						   )
						{
							//ヒットコールバック
							SetItemDamege( collision[i], collision[j] );
//							SetplayerDamege( collision[j], collision[i] );	//アイテムはプレイヤー側でヒットを呼ばない
						}
					}
				}
			}
		}
	}
}

void move_delete_collision( void )
{
	int i;
	
	for ( i = 0; i < COLLISION_MAX; i++ )
	{
		//コリジョン消去
		collision[i].life = 0;
	}
}

void darw_collision( void )
{
	int i;
	int x;
	int y;
	int offset_y;
	
	for ( i = 0; i < COLLISION_MAX; i++ )
	{
		if ( collision[i].life > 0 )
		{
			if ( g_DebugDisp != 0 )
			{
				offset_y = 0;
				if ( collision[i].hitparam == EN_HITPARAM_TI )
				{
					offset_y = 64;
				}
				
				int r = 255;
				int g = 255;
				int b = 255;
				switch( collision[i].coltype )
				{
				case EN_COLTYPE_PLAYER:
					r = 255;
					g = 0;
					b = 0;
					break;
				case EN_COLTYPE_PLAYER_SHOT:
					r = 255;
					g = 255;
					b = 0;
					break;
				case EN_COLTYPE_ENEMY:
					r = 0;
					g = 0;
					b = 128;
					break;
				case EN_COLTYPE_ENEMY_SHOT:
					r = 0;
					g = 128;
					b = 128;
					break;
				case EN_COLTYPE_ITEM:
					r = 0;
					g = 255;
					b = 0;
					break;
				}
				//中央
				ObjSetEx( C_COL_02_P000_00_K, PRI_INFO, collision[i].pos.x / DOT, collision[i].pos.y / DOT, 1.0f, 1.0f, 0, r, g, b, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );

				//左上
				x = collision[i].pos.x - ( collision[i].size.x / 2 );
				y = collision[i].pos.y - ( collision[i].size.y / 2 );
				ObjSetEx( C_COL_01_P000_00_K, PRI_INFO, x / DOT, y / DOT, 1.0f, 1.0f, 0, r, g, b, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );

				//右下
				x = collision[i].pos.x + ( collision[i].size.x / 2 );
				y = collision[i].pos.y + ( collision[i].size.y / 2 );
				ObjSetEx( C_COL_01_P000_00_K + 2, PRI_INFO, x / DOT, y / DOT, 1.0f, 1.0f, 0, r, g, b, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );

				//左下
				x = collision[i].pos.x - ( collision[i].size.x / 2 );
				y = collision[i].pos.y + ( collision[i].size.y / 2 );
				ObjSetEx( C_COL_01_P000_00_K + 3, PRI_INFO, x / DOT, y / DOT, 1.0f, 1.0f, 0, r, g, b, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );

				//右上
				x = collision[i].pos.x + ( collision[i].size.x / 2 );
				y = collision[i].pos.y - ( collision[i].size.y / 2 );
				ObjSetEx( C_COL_01_P000_00_K + 1, PRI_INFO, x / DOT, y / DOT, 1.0f, 1.0f, 0, r, g, b, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
			}
		}
	}
}

CHARCTAR GetCollisionType_to_CharctarBuffer( int CollisionType, int index )
{
	CHARCTAR rc;
	
	memset( &rc , 0, sizeof( CHARCTAR ) );
	
	switch( CollisionType )
	{
	case EN_COLTYPE_PLAYER:
		rc = player[index];
		break;
	case EN_COLTYPE_PLAYER_SHOT:
		rc = plyshot[index];
		break;
	case EN_COLTYPE_ENEMY:
		rc = enemy[index];
		break;
	case EN_COLTYPE_ENEMY_SHOT:
		rc = eneshot[index];
		break;
	case EN_COLTYPE_ITEM:
		rc = item[index];
		break;
	default:
		break;
	}
	
	return ( rc );
}

void Bullet( int type , int x, int y, int dir, int spd )
{
	create_eneshot( type, x, y, dir, spd );
}
void BulletWay( int type , int x, int y, int dir, int spd, int waydir, int wayinter )
{
	int offset = 0;
	int offset2 = 0;
	int offset3 = 0;
	int i;
	
	for ( i = 0; i < waydir; i++ )
	{
		
		if ( ( waydir % 2 ) == 0 )
		{
			offset2 = ( wayinter * ( waydir / 2 ) ) - ( wayinter / 2 );
		}
		else
		{
			offset2 = ( wayinter * ( waydir / 2 ) );
		}
		
		offset = i * wayinter;
		offset3 = dir + offset - offset2;
		if ( offset3 < 0 )
		{
			offset3 = offset3 + MAX_DIR;
		}
		offset3 = offset3 % MAX_DIR;
		create_eneshot( type, x, y, offset3, spd );
	}
}
void BulletWayWay( int type , int x, int y, int dir, int spd, int waydir, int wayinter, int waydir2, int wayinter2 )
{
	int offset = 0;
	int offset2 = 0;
	int offset3 = 0;
	int i;
	
	for ( i = 0; i < waydir; i++ )
	{
		
		if ( ( waydir % 2 ) == 0 )
		{
			offset2 = ( wayinter * ( waydir / 2 ) ) - ( wayinter / 2 );
		}
		else
		{
			offset2 = ( wayinter * ( waydir / 2 ) );
		}
		
		offset = i * wayinter;
		offset3 = dir + offset - offset2;
		if ( offset3 < 0 )
		{
			offset3 = offset3 + MAX_DIR;
		}
		offset3 = offset3 % MAX_DIR;
		BulletWay( type , x, y, offset3, spd, waydir2, wayinter2 );
	}
}
void BulletTat( int type , int x, int y, int dir, int spd, int tatnum, int addspd )
{
	int i;
	for ( i = 0; i < tatnum; i++ )
	{
		create_eneshot( type, x, y, dir, spd + ( i * addspd ) );
	}
}
void BulletWayTat( int type , int x, int y, int dir, int spd, int waydir, int wayinter, int tatnum, int addspd )
{
	int offset = 0;
	int offset2 = 0;
	int offset3 = 0;
	int i;
	
	for ( i = 0; i < waydir; i++ )
	{
		
		if ( ( waydir % 2 ) == 0 )
		{
			offset2 = ( wayinter * ( waydir / 2 ) ) - ( wayinter / 2 );
		}
		else
		{
			offset2 = ( wayinter * ( waydir / 2 ) );
		}
		
		offset = i * wayinter;
		offset3 = dir + offset - offset2;
		if ( offset3 < 0 )
		{
			offset3 = offset3 + MAX_DIR;
		}
		offset3 = offset3 % MAX_DIR;
		BulletTat( type , x, y, offset3, spd, tatnum, addspd );
	}
}
//敵弾の全消去
void BulletClaer( int time, int item )
{
	Bullet_ClaerTimer = time;
	Bullet_ClaerItem = item;
}

//敵ショット関連----------------------------------------------
void create_eneshot( int type, int x, int y, int dir, int spd )
{
	int i;
	
	for ( i = 0; i < ENESHOT_MAX; i++ )
	{
		if ( eneshot[i].life <= 0 )
		{
			memset( &eneshot[i], 0, sizeof( CHARCTAR ) );

			eneshot[i].pos.x = x;
			eneshot[i].pos.y = y;
			eneshot[i].life = 1;
			eneshot[i].dispsize = 32;
			eneshot[i].damege = 1;
			eneshot[i].spd = spd;
			eneshot[i].dir = dir;
			eneshot[i].type = type;

			eneshot[i].collsize.x = 8 * DOT;
			eneshot[i].collsize.y = 8 * DOT;

			switch( eneshot[i].type )
			{
			case EN_ENESHOT_01:
				eneshot[i].anime_max = 4;	//アニメーションパターン数
				eneshot[i].anime_int = 2;	//アニメーションカウント
				break;
			case EN_ENESHOT_02:
				eneshot[i].anime_max = 32;	//アニメーションパターン数
				break;
			default:
				break;
			}

			create_effect( EN_EFC_ENEMZL_01, eneshot[i].pos.x, eneshot[i].pos.y, 0, 0 );				//マズル表示
			
			AddSaveDate( EN_SAVEDATA_BULLET, 1 );
			break;
		}
	}
}

void move_eneshot( void )
{
	stage_info.eneshot = 0;
	
	int i;
	for ( i = 0; i < ENESHOT_MAX; i++ )
	{
		if ( eneshot[i].life > 0 )
		{
			stage_info.eneshot++;
			
			eneshot[i].timer++;

			DirMove( eneshot[i].dir, eneshot[i].spd, eneshot[i].pos.x, eneshot[i].pos.y );
			eneshot[i].pos.x = func_return_x;
			eneshot[i].pos.y = func_return_y;

			switch( eneshot[i].type )
			{
			case EN_ENESHOT_01:
				//通常アニメーション
				eneshot[i].anime_cnt++;
				if ( eneshot[i].anime_cnt > eneshot[i].anime_int )
				{
					eneshot[i].anime_cnt = 0;
					eneshot[i].anime++;
					if ( eneshot[i].anime >= eneshot[i].anime_max ) 
					{
						eneshot[i].anime = 0;
					}
				}
				break;
			case EN_ENESHOT_02:
				//方角
				eneshot[i].anime = DirToAnim( eneshot[i].dir, eneshot[i].anime_max );
				break;
			default:
				break;
			}

			if ( Bullet_ClaerTimer > 0 )
			{
				eneshot[i].life = 0;
				create_effect( EN_EFC_BULCLEAR, eneshot[i].pos.x, eneshot[i].pos.y, eneshot[i].dir, eneshot[i].spd );	//弾けしエフェクト
				//全画面弾けしが行われた
				if ( Bullet_ClaerItem != EN_ITEM_NONE )
				{
					//弾けし時にアイテム出現リクエストがある
					create_item( Bullet_ClaerItem, eneshot[i].pos.x, eneshot[i].pos.y, 0, 0 );
				}
			}

			if ( 0 != limit( eneshot[i].pos.x, eneshot[i].pos.y, eneshot[i].dispsize ) )
			{
				//画面外
				eneshot[i].life = 0;

				AddSaveDate( EN_SAVEDATA_OUT_BULLET, 1 );
			}
			set_collision( EN_COLTYPE_ENEMY_SHOT, i, eneshot[i].pos.x, eneshot[i].pos.y, eneshot[i].collsize.x, eneshot[i].collsize.y, 0, eneshot[i].damege );
		}
	}
	
	//弾けし時間のカウント
	if ( Bullet_ClaerTimer > 0 )
	{
		Bullet_ClaerTimer--;
		if ( Bullet_ClaerTimer == 0 )
		{
			Bullet_ClaerItem = 0;
		}
	}
}

void darw_eneshot( void )
{
	int i;
	
	int char_ofs = 1;
	if ( stage_info.bg_change != 0 )
	{
		char_ofs = 2;
	}
	for ( i = 0; i < ENESHOT_MAX; i++ )
	{
		if ( eneshot[i].life > 0 )
		{
//			BltRectEx( 17, eneshot[i].pos.x / DOT, eneshot[i].pos.y / DOT,
//			               eneshot[i].anime * eneshot[i].dispsize, eneshot[i].type * eneshot[i].dispsize,
//			               eneshot[i].dispsize, eneshot[i].dispsize );
//			ObjSetEx( C_BULLET_01_P000_00_K + eneshot[i].anime, PRI_ENESHOT, eneshot[i].pos.x / DOT, eneshot[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
			ObjSetEx( C_BULLET_01_P000_00_K + char_ofs, PRI_ENESHOT, eneshot[i].pos.x / DOT, eneshot[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
		}
	}
}
void SeteneshotDamege( COLLISION my, COLLISION you )
{
	CHARCTAR mychar = GetCollisionType_to_CharctarBuffer( my.coltype, my.index );
	CHARCTAR youchar = GetCollisionType_to_CharctarBuffer( you.coltype, you.index );
	int i = my.index;
	int damege = you.damege;

	//貫通の場合はダメージを減算しない
	eneshot[i].life = eneshot[i].life - damege;
	//ショットタッチエフェクト
}

//自機ショット関連----------------------------------------------
void create_plyshot( int type, int x, int y, int dir, int spd )
{
	int i;
	
	for ( i = 0; i < PLYSHOT_MAX; i++ )
	{
		if ( plyshot[i].life <= 0 )
		{
//			soundPlaySe( EN_SE_ATTACK );

			memset( &plyshot[i], 0, sizeof( CHARCTAR ) );

			plyshot[i].pos.x = x;
			plyshot[i].pos.y = y;
			plyshot[i].life = 1;
			plyshot[i].dispsize = 32;
			plyshot[i].damege = 1;
			plyshot[i].spd = spd;
			plyshot[i].dir = dir;
			plyshot[i].anime_max = 32;
			plyshot[i].collsize.x = 32 * DOT;
			plyshot[i].collsize.y = 16 * DOT;
			plyshot[i].seq = 0;
			plyshot[i].movetype = type;
			create_effect( EN_EFC_PLYMZL_01, plyshot[i].pos.x, plyshot[i].pos.y, 0, 0 );

			AddSaveDate( EN_SAVEDATA_SHOT, 1 );
			break;
		}
	}
}

void move_plyshot( void )
{
	stage_info.plyshot = 0;

	int i;
	for ( i = 0; i < PLYSHOT_MAX; i++ )
	{
		if ( plyshot[i].life > 0 )
		{
			stage_info.plyshot++;
			
			DirMove( plyshot[i].dir, plyshot[i].spd, plyshot[i].pos.x, plyshot[i].pos.y );
			plyshot[i].pos.x = func_return_x;
			plyshot[i].pos.y = func_return_y;

			if ( 0 != limit( plyshot[i].pos.x, plyshot[i].pos.y, plyshot[i].dispsize ) )
			{
				//画面外
				plyshot[i].life = 0;
				AddSaveDate( EN_SAVEDATA_OUT_SHOT, 1 );
			}

			set_collision( EN_COLTYPE_PLAYER_SHOT, i, plyshot[i].pos.x, plyshot[i].pos.y, plyshot[i].collsize.x, plyshot[i].collsize.y, 0, plyshot[i].damege );
			plyshot[i].timer++;
		}
	}
}

void darw_plyshot( void )
{
	int i;
	
	for ( i = 0; i < PLYSHOT_MAX; i++ )
	{
		if ( plyshot[i].life > 0 )
		{
//			plyshot[i].anime = DirToAnim( plyshot[i].dir, plyshot[i].anime_max );
//			BltRectEx( 2, plyshot[i].pos.x / DOT, plyshot[i].pos.y / DOT, plyshot[i].anime * plyshot[i].dispsize, 0, plyshot[i].dispsize, plyshot[i].dispsize );
			plyshot[i].anime = 0;
			ObjSetEx( C_MANDARA_SHOT_P000_00_K + plyshot[i].anime, PRI_PLYSHOT, plyshot[i].pos.x / DOT, plyshot[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
		}
		if ( g_DebugDisp != 0 )
		{
		}
	}
}
void SetplyshotDamege( COLLISION my, COLLISION you )
{
	CHARCTAR mychar = GetCollisionType_to_CharctarBuffer( my.coltype, my.index );
	CHARCTAR youchar = GetCollisionType_to_CharctarBuffer( you.coltype, you.index );
	int i = my.index;
	int damege = you.damege;
	
	//貫通の場合はダメージを減算しない
	plyshot[i].life = plyshot[i].life - damege;
	//ショットタッチエフェクト
	if ( plyshot[i].life <= 0 )
	{
		soundPlaySe( EN_SE_HIT );
		create_effect( EN_EFC_PLYSHT_TOUCH_01, plyshot[i].pos.x, plyshot[i].pos.y, 0, 0 );
		
//		int rdir = funcRand( 256 ) + 128;
//		int rspd = funcRand( 10 ) * DOT;
//		create_effect( EN_EFC_PLYSHT_TOUCH_02, plyshot[i].pos.x + ( 10 * DOT ), plyshot[i].pos.y, rdir, rspd );

		if ( plyshot[i].timer <= 14 )	//接近射撃でアイテム出現
		{
			if ( plyshot[i].movetype != EN_SHOT_TYPE_HYPER )	//マンダラショットではない
			{
				create_item( EN_ITEM_ENELGY_S, plyshot[i].pos.x, plyshot[i].pos.y, 0, 0 );	//回復アイテム出現
			}
		}
	}
	
}

//アイテム関連----------------------------------------------
void create_item( int type, int x, int y, int dir, int spd )
{
	int i;
	
	for ( i = 0; i < ITEM_MAX; i++ )
	{
		if ( item[i].life <= 0 )
		{
			memset( &item[i], 0, sizeof( CHARCTAR ) );

			item[i].pos.x = x;
			item[i].pos.y = y;
			item[i].life = 1;
			item[i].dispsize = 128;
			item[i].damege = 0;
			item[i].spd = spd;
			item[i].dir = dir;
			item[i].collsize.x = 32 * DOT;
			item[i].collsize.y = 32 * DOT;
			item[i].type = type;
			
			switch( item[i].type )
			{
			case EN_ITEM_SCOER_S:
			case EN_ITEM_SCOER_M:
			case EN_ITEM_SCOER_L:
			case EN_ITEM_ENELGY_S:
			case EN_ITEM_ENELGY_M:
			case EN_ITEM_ENELGY_L:
				item[i].spd = 200 + funcRand( 100 );
				item[i].dir = 768;
				item[i].move[0] = -400;
				break;
			default:
				break;
			}
			break;
		}
	}
}
void move_item( void )
{
	int i;

	for ( i = 0; i < ITEM_MAX; i++ )
	{
		if ( item[i].life > 0 )
		{
			
			DirMove( item[i].dir, item[i].spd, item[i].pos.x, item[i].pos.y );
			item[i].pos.x = func_return_x;
			item[i].pos.y = func_return_y;

			switch( item[i].type )
			{
			case EN_ITEM_SCOER_S:
			case EN_ITEM_SCOER_M:
			case EN_ITEM_SCOER_L:
			case EN_ITEM_ENELGY_S:
			case EN_ITEM_ENELGY_M:
			case EN_ITEM_ENELGY_L:
				if ( item[i].timer >= 30 )	//出現から30フレームは回収しない
				{
					int kin = Kinjiti( player[0].pos.x / DOT, player[0].pos.y / DOT, item[i].pos.x / DOT, item[i].pos.y / DOT );
					if ( kin < ( 256 ) )	//回収範囲
					{
						item[i].seq = 2;	//回収
					}
				}
				if ( player[0].life == 0 )
				{
					item[i].seq = 3;	//落下
				}
				
				switch ( item[i].seq )
				{
				case 0:
					//出現
//					item[i].spd -= 10;
//					if ( item[i].spd < 0 )
//					{
//						item[i].spd = 0;
//						item[i].seq = 1;
//						item[i].dir = 768;	//したに移動
//					}
					item[i].move[0] += 10;
					item[i].pos.y += item[i].move[0];
					break;
				case 1:
					//移動
					item[i].spd += 10;
					if ( item[i].spd > 300 )
					{
						item[i].spd = 300;
						item[i].dir = 768;	//したに移動
					}
					break;
				case 2:
					{
						//回収
						item[i].dir = FigDir( item[i].pos.x, item[i].pos.y );
						item[i].spd += 400;
						if ( item[i].spd > 1600 )
						{
							item[i].spd = 1600;
						}
						break;
					}
				case 3:
					{
						//回収
						item[i].dir = 512;
						item[i].spd += 400;
						if ( item[i].spd > 1600 )
						{
							item[i].spd = 1600;
						}
						break;
					}
				}
				break;
			default:
				break;
			}

			//アイテムにコリジョンをつける
			set_collision( EN_COLTYPE_ITEM, i, item[i].pos.x, item[i].pos.y, item[i].collsize.x, item[i].collsize.y, EN_HITPARAM_TI, item[i].damege );

			if ( 0 != limit( item[i].pos.x, item[i].pos.y, item[i].dispsize ) )
			{
				//画面外
				item[i].life = 0;
			}

			item[i].timer++;
		}
	}
	
}
void SetItemDamege( COLLISION my, COLLISION you )
{
	CHARCTAR mychar = GetCollisionType_to_CharctarBuffer( my.coltype, my.index );
	CHARCTAR youchar = GetCollisionType_to_CharctarBuffer( you.coltype, you.index );
	int i = my.index;
	int damege = you.damege;

	if ( item[i].life > 0 )
	{
//		item[i].life = item[i].life - damege;
		item[i].life = 0;	//取得後必ず死亡で
		//ショットタッチエフェクト
		if ( item[i].life <= 0 )
		{
			switch( item[i].type )
			{
			case EN_ITEM_SCOER_S:
				create_effect( EN_EFC_RED_ADD, player[0].pos.x, player[0].pos.y, 0, 0 );
				if ( stage_info.bg_change != 0 )
				{
					diffic_red_add( 5 );
				}
				else
				{
					diffic_red_add( 2 );
				}
				break;
			case EN_ITEM_SCOER_M:
				create_effect( EN_EFC_SCOER_PLATE, player[0].pos.x + ( -40 * DOT ), player[0].pos.y + ( -20 * DOT ), 900, 200 );
				AddScoer( 0, g_Rank_Red );
				break;
			case EN_ITEM_SCOER_L:
				break;
			case EN_ITEM_ENELGY_S:
				if ( player[0].move[0] > 2 )
				{
					player[0].move[0] -= 2;		//マンダラ時間回復
					if ( player[0].move[0] < 2 )
					{
						player[0].move[0] = 2;
					}
				}
				break;
			case EN_ITEM_ENELGY_M:
				break;
			case EN_ITEM_ENELGY_L:
				break;
			default:
				break;
			}
		}
	}
}
void darw_item( void )
{
	int i;
	
	for ( i = 0; i < PLYSHOT_MAX; i++ )
	{
		if ( item[i].life > 0 )
		{
			int x = item[i].pos.x;
			int y = item[i].pos.y;
			
			switch( item[i].type )
			{
			case EN_ITEM_SCOER_S:
				ObjSetEx( C_SCOER_S_P000_00_K, PRI_EXP, x / DOT, y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_ITEM_SCOER_M:
				ObjSetEx( C_SCOER_M_P000_00_K, PRI_EXP, x / DOT, y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_ITEM_SCOER_L:
				DrawNumer( 3, 1, ( item[i].pos.x / DOT ), ( item[i].pos.y / DOT ), 0, 1 );	
				break;
			case EN_ITEM_ENELGY_S:
				ObjSetEx( C_ENELGY_S_P000_00_K, PRI_EXP, x / DOT, y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_ITEM_ENELGY_M:
				DrawNumer( 11, 2, ( item[i].pos.x / DOT ), ( item[i].pos.y / DOT ), 0, 1 );	
				break;
			case EN_ITEM_ENELGY_L:
				DrawNumer( 12, 2, ( item[i].pos.x / DOT ), ( item[i].pos.y / DOT ), 0, 1 );	
				break;
			default:
				break;
			}

		}
		if ( g_DebugDisp != 0 )
		{
		}
	}
}


//エフェクト関連----------------------------------------------
void create_effect( int type, int x, int y, int dir, int spd )
{
	int i;

	if ( g_ScoerPlateInter > 0 )
	{
		if ( type == EN_EFC_SCOER_PLATE )
		{
			return;
		}
	}
	
	for ( i = 0; i < EFFECT_MAX; i++ )
	{
		if ( effect[i].life <= 0 )
		{
			memset( &effect[i], 0, sizeof( CHARCTAR ) );

			effect[i].pos.x = x;
			effect[i].pos.y = y;
			effect[i].life = 1;				//アニメーションループ回数
			effect[i].dispsize = 32;
			effect[i].damege = 1;
			effect[i].spd = spd;
			effect[i].dir = dir;
			effect[i].type = type;
			switch( effect[i].type )
			{
			case EN_EFC_PLYMZL_01:
			case EN_EFC_ENEMZL_01:
				effect[i].anime_max = 1;	//アニメーションパターン数
				effect[i].anime_int = 1;	//アニメーションカウント
				break;
			case EN_EFC_PLYSHT_TOUCH_01:
				effect[i].anime_max = 3;	//アニメーションパターン数
				effect[i].anime_int = 2;	//アニメーションカウント
				break;
			case EN_EFC_PLYSHT_TOUCH_02:
				effect[i].anime_max = 20;	//アニメーションパターン数
				effect[i].anime_int = 1;	//アニメーションカウント
				effect[i].dispsize = 16;
				break;
			case EN_EFC_ENEBAKU_01:
				effect[i].anime_max = 4;	//アニメーションパターン数
				effect[i].anime_int = 2;	//アニメーションカウント
				effect[i].dispsize = 40;
				break;
			case EN_EFC_ENEBAKU_02:
				effect[i].anime_max = 8;	//アニメーションパターン数
				effect[i].anime_int = 1;	//アニメーションカウント
				effect[i].dispsize = 40;
				break;
			case EN_EFC_ENEBAKU_03:
				effect[i].anime_max = 4;	//アニメーションパターン数
				effect[i].anime_int = 1;	//アニメーションカウント
				effect[i].dispsize = 40;
				break;
			case EN_EFC_ENEBAKU_04:
				effect[i].anime_max = 4;	//アニメーションパターン数
				effect[i].anime_int = 1;	//アニメーションカウント
				effect[i].dispsize = 40;
				break;
			case EN_EFC_BULCLEAR:
				effect[i].anime_max = 20;	//アニメーションパターン数
				effect[i].anime_int = 1;	//アニメーションカウント
				effect[i].dispsize = 16;
				break;
			case EN_EFC_EFBALL:
				effect[i].anime_max = 20;	//アニメーションパターン数
				effect[i].anime_int = 1;	//アニメーションカウント
				effect[i].dispsize = 16;
				break;
			case EN_EFC_BOSSDEAD:
				effect[i].anime_max = 6;	//アニメーションパターン数
				effect[i].anime_int = 10;	//アニメーションカウント
				break;
			case EN_EFC_BOSSDEAD_SERIHU:
				effect[i].anime_max = 1;	//アニメーションパターン数
				effect[i].anime_int = 30;	//アニメーションカウント
				break;
			case EN_EFC_EXTEND01:
			case EN_EFC_EXTEND02:
			case EN_EFC_MISSTIME:
				effect[i].anime_max = 60;	//アニメーションパターン数
				effect[i].anime_int = 1;	//アニメーションカウント
				break;
			case EN_EFC_SCOER_PLATE:
				g_ScoerPlateInter = 3;
				effect[i].anime_max = 40;	//アニメーションパターン数
				effect[i].anime_int = 1;	//アニメーションカウント
				effect[i].move[0] = g_Rank_Red;	//目的地
				effect[i].move[1] = 0;		//カウント
				break;
			case EN_EFC_RED_ADD:
				break;
			default:
				break;
			}
			break;
		}
	}
}
void move_effect( void )
{
	g_ScoerPlateInter--;
	if ( g_ScoerPlateInter < 0 )
	{
		g_ScoerPlateInter = 0;
	}
	
	int i;
	for ( i = 0; i < EFFECT_MAX; i++ )
	{
		if ( effect[i].life > 0 )
		{
			DirMove( effect[i].dir, effect[i].spd, effect[i].pos.x, effect[i].pos.y );
			effect[i].pos.x = func_return_x;
			effect[i].pos.y = func_return_y;

			switch( effect[i].type )
			{
			case EN_EFC_PLYMZL_01:
			case EN_EFC_ENEMZL_01:
			case EN_EFC_PLYSHT_TOUCH_01:
			case EN_EFC_PLYSHT_TOUCH_02:
			case EN_EFC_ENEBAKU_01:
			case EN_EFC_ENEBAKU_02:
			case EN_EFC_ENEBAKU_03:
			case EN_EFC_ENEBAKU_04:
			case EN_EFC_BULCLEAR:
			case EN_EFC_EFBALL:
			case EN_EFC_BOSSDEAD:
			case EN_EFC_BOSSDEAD_SERIHU:
				//通常アニメーション
				effect[i].anime_cnt++;
				if ( effect[i].anime_cnt > effect[i].anime_int )
				{
					effect[i].anime_cnt = 0;
					effect[i].anime++;
					if ( effect[i].anime >= effect[i].anime_max ) 
					{
						effect[i].anime = 0;
						effect[i].life = effect[i].life - 1;
					}
				}
				break;
			case EN_EFC_EXTEND01:
			case EN_EFC_EXTEND02:
			case EN_EFC_MISSTIME:
				//通常アニメーション
				effect[i].spd -= 10;
				if ( effect[i].spd < 0 )
				{
					effect[i].spd = 0;
				}
				
				effect[i].anime_cnt++;
				if ( effect[i].anime_cnt > effect[i].anime_int )
				{
					effect[i].anime_cnt = 0;
					effect[i].anime++;
					if ( effect[i].anime >= effect[i].anime_max ) 
					{
						effect[i].anime = 0;
						effect[i].life = effect[i].life - 1;
					}
				}
				break;
			case EN_EFC_SCOER_PLATE:
				effect[i].spd -= 1;
				if ( effect[i].spd < 0 )
				{
					effect[i].spd = 0;
				}

				{
					int add = ( effect[i].move[0] / 20 );
					if ( add == 0 )
					{
						add = 1;
					}
					effect[i].move[1] += add;		//カウント
					if ( effect[i].move[1] > effect[i].move[0] )
					{
						effect[i].move[1] = effect[i].move[0];
					}
				}

				effect[i].anime_cnt++;
				if ( effect[i].anime_cnt > effect[i].anime_int )
				{
					effect[i].anime_cnt = 0;
					effect[i].anime++;
					if ( effect[i].anime >= effect[i].anime_max ) 
					{
						effect[i].anime = 0;
						effect[i].life = effect[i].life - 1;
					}
				}
				break;
			case EN_EFC_RED_ADD:
				{
					effect[i].spd += 20;
					if ( effect[i].spd > 3000 )
					{
						effect[i].spd = 3000;
					}
					
					int ex = 640;
					int ey = 360;
					effect[i].dir = PointDir( effect[i].pos.x / DOT, effect[i].pos.y / DOT, ex, ey );
					int kin = Kinjiti( effect[i].pos.x / DOT, effect[i].pos.y / DOT, ex, ey );
					if ( kin < ( 32 ) )	//回収範囲
					{
						effect[i].life = 0;
					}
					
				}
				break;
			default:
				break;
			}

			if ( 0 != limit( effect[i].pos.x, effect[i].pos.y, effect[i].dispsize ) )
			{
				//画面外
				effect[i].life = 0;
			}

			effect[i].timer++;
		}
	}
}

void darw_effect( void )
{
	int i;
	
	for ( i = 0; i < EFFECT_MAX; i++ )
	{
		if ( effect[i].life > 0 )
		{
			switch ( effect[i].type )
			{
			case EN_EFC_PLYMZL_01:
				ObjSetEx( C_EF_BALL_P000_00_K + 14 - effect[i].anime, PRI_EXP, effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1.0f, 1.0f, 0, 100, 100, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_EFC_ENEMZL_01:
				ObjSetEx( C_EF_BALL_P000_00_K + 14 - effect[i].anime, PRI_EXP, effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1.0f, 1.0f, 0, 100, 255, 100, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_EFC_PLYSHT_TOUCH_01:
				ObjSetEx( C_MANDARA_SHOT_TUCH_P000_00_K + effect[i].anime, PRI_PLYSHOT_TUCH, effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_EFC_PLYSHT_TOUCH_02:
			{
				int cl = effect[i].anime * 10;
				ObjSetEx( C_EF_BALL_P000_00_K + 20 - effect[i].anime, PRI_EXP, effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1.0f, 1.0f, 0, cl, cl, cl, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			}
			case EN_EFC_ENEBAKU_01:
				ObjSetEx( C_EXP_01_P000_00_K + effect[i].anime, PRI_EXP, effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_EFC_ENEBAKU_02:
				ObjSetEx( C_EXP_02_P000_00_K + effect[i].anime, PRI_EXP, effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_EFC_ENEBAKU_03:
				ObjSetEx( C_EXP_03_P000_00_K + effect[i].anime, PRI_EXP, effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_EFC_ENEBAKU_04:
				ObjSetEx( C_EXP_04_P000_00_K + effect[i].anime, PRI_EXP, effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_EFC_BULCLEAR:
			{
				int cl = effect[i].anime * 10;
				ObjSetEx( C_EF_BALL_P000_00_K + 20 - effect[i].anime, PRI_EXP, effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1.0f, 1.0f, 0, cl, cl, cl, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			
			}
			case EN_EFC_EXTEND01:
				DarwFont( "+ 10 SEC", effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1 );
				break;
			case EN_EFC_EXTEND02:
				DarwFont( "+ 20 SEC", effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1 );
				break;
			case EN_EFC_MISSTIME:
				DarwFont( "- 40 SEC", effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1 );
				break;
			case EN_EFC_BOSSDEAD:
				ObjSetEx( C_BOSS_DEAD_P000_00_K + effect[i].anime, PRI_EXP, effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_EFC_BOSSDEAD_SERIHU:
				ObjSetEx( C_BOSS_DEAD_SERIHU_P000_00_K + effect[i].anime, PRI_EXP, effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_EFC_SCOER_PLATE:
				DarwFont( "+", effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1 );
				DrawNumer( effect[i].move[1], 9, ( effect[i].pos.x / DOT ) + 10, effect[i].pos.y / DOT, 0, 0 );
				break;
			case EN_EFC_RED_ADD:
				ObjSetEx( C_EF_BALL_P000_00_K + 10, PRI_EXP, effect[i].pos.x / DOT, effect[i].pos.y / DOT, 1.0f, 1.0f, 0, 255, 100, 100, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
				break;
			case EN_EFC_EFBALL:
			default:
				break;
			}
		}
	}
}

//BG関連----------------------------------------------
void move_BG( void )
{
	stage_info.BG_Count++;
//	stage_info.BG_Count = stage_info.BG_Count % 640;

	if ( stage_info.bg_change != 0 )
	{
		int i = 0;
		for ( i = 0; i < 640; i++ )
		{
			bg_change_scroll[i].y += ( funcRand( 20 ) + 4 );
			if ( bg_change_scroll[i].y > 360 )
			{
				bg_change_scroll[i].y = 360;
			}
		}
	}

}
void darw_BG( void )
{
//	Blt( 31, 0 - stage_info.BG_Count, 0 );
//	Blt( 31, 640 - stage_info.BG_Count, 0 );
	int i = 0;

	int char_ofs = 0;
	if ( stage_info.bg_change != 0 )
	{
		char_ofs = 1;
	}

	if ( stage_info.bg_change == 0 )
	{
		for ( i = 0; i < ( 21 ) * ( 8 ); i++ )
		{
			ObjSetEx( C_BETA_P000_00_K, PRI_BG, ( ( i % 21 ) * 32 ), ( ( i / 21 ) * 32 ), 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
		}
	}

	int spd = stage_info.BG_Count;
	for ( i = 0; i < 66; i++ )
	{
		ObjSetEx( C_TOWER4_P000_00_K + char_ofs, PRI_BG, 0 + ( i * 10 ) - ( spd % 10 ), 140, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
	}

	spd = stage_info.BG_Count * 2;
	for ( i = 0; i < 22; i++ )
	{
		ObjSetEx( C_TOWER3_P000_00_K + char_ofs, PRI_BG, 0 + ( i * 32 ) - ( spd % 32 ), 140, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
	}

	spd = stage_info.BG_Count * 4;
	for ( i = 0; i < 9; i++ )
	{
		ObjSetEx( C_TOWER2_P000_00_K + char_ofs, PRI_BG, 0 + ( i * 96 ) - ( spd % 96 ), 140, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
	}

	spd = stage_info.BG_Count * 8;
	for ( i = 0; i < 5; i++ )
	{
		ObjSetEx( C_TOWER1_P000_00_K + char_ofs, PRI_BG, 0 + ( i * 256 ) - ( spd % 256 ), 140, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0.0f, GM_SPR_NONE, 0, -1, -1 );
	}


	if ( stage_info.bg_change != 0 )
	{
		int i = 0;
		for ( i = 0; i < 640; i++ )
		{
			if ( bg_change_scroll[i].y < 360 )
			{
				BltFunction( FID_WORK01, i, bg_change_scroll[i].y, i, 0, 1, 360, 1.0f, 1.0f, 0, 255, 255, 255, 255, 0, PRI_BG, GM_SPR_NONE );
			}
		}
	}
}





void BltRectEx( int bno, int dx, int dy, int sx, int sy, int w, int h )
{
	dx = dx - ( w / 2 );
	dy = dy - ( w / 2 );
	BltRect( bno, dx, dy, sx, sy, w, h );
}

// stg用関数郡--------------------------------------------------
//角度を足す
int AddDir( int dir, int add )
{
	int rc = 0;
	
	rc = dir + add;
	if ( rc < 0 )
	{
		rc = rc + MAX_DIR;
	}
	rc = rc % MAX_DIR;
	
	return( rc );
}
//目的の方向に角度を進める
int EndDirAddDir( int end_dir, int dir, int add_dir )
{
	int rc = 0;
	int jdir = end_dir - dir;
	
	if ( jdir < 0 )
	{
		jdir = jdir + MAX_DIR;
	}

	if ( jdir == 0 ) 
	{
		rc = end_dir;
	}
	else if ( add_dir >= jdir ) 
	{
		rc = end_dir;
	}
	else 
	{
		if ( jdir > ( MAX_DIR / 2 ) )
		{
			rc = dir - add_dir;
		}
		else if ( ( jdir < ( MAX_DIR / 2 ) ) && ( jdir > 0 ) )
		{
			rc = dir + add_dir;
		}
		else 
		{
			rc = dir + add_dir;
		}
	}
	if ( rc < 0 )
	{
		rc = rc + MAX_DIR;
	}
	rc = rc % MAX_DIR;
	
	return( rc );
}
//目的地の角度を取得
int PointDir( int sx, int sy, int ex, int ey )
{
	int rc = 0;

	int px = sx - ex;
	int py = sy - ey;
	rc = funcTan2( px, py ) ;
	
	return( rc );
}
//自機の方向を取得
int FigDir( int sx, int sy )
{
	int rc = 0;

	int px = sx - player[0].pos.x;
	int py = sy - player[0].pos.y;
	rc = funcTan2( px, py );
	
	return( rc );
}
//方向の精度を落とす
int DirChange( int dir, int seido )
{
	int rc = 0;
	int bai = MAX_DIR / seido;
	
	rc = ( ( dir / bai ) * bai );
	
	return( rc );
}
//移動後の座標を取得
void DirMove( int dir, int spd, int x, int y )
{
	int move_x;
	int move_y;

	move_x = ( funcSin( dir ) * spd ) >> 16;
	move_y = ( funcCos( dir ) * spd ) >> 16;

	func_return_x = x + move_x;
	func_return_y = y + move_y;
	
}
//画面外か？
int limit( int x, int y, int size )
{
	int rc = 0;
	size = size * DOT;
	
	if ( ( x + size ) < 0 )
	{
		rc = 1;
	}
	if ( ( x - size ) > getMapWidth( ) )
	{
		rc = 1;
	}
	if ( ( y + size ) < 0 )
	{
		rc = 1;
	}
	if ( ( y - size ) > getMapHeight( ) )
	{
		rc = 1;
	}
	
	return( rc );
}
int DirToAnim( int dir, int max_anim )
{
	int rc = 0;
	rc = ( dir + ( ( MAX_DIR / max_anim ) / 2 ) ) / ( MAX_DIR / max_anim );
	if ( rc >= max_anim )
	{
		rc = rc- max_anim;
	}
	
	return( rc );
}
int Kinjiti( int x1, int y1, int x2, int y2 )
{
	int rc = 0;

	int absx = abs( x1 - x2 );
	int absy = abs( y1 - y2 );
	
	if ( absx > absy )
	{
		rc = absy + ( absx / 2 );
	}
	else
	{
		rc = absx + ( absy / 2 );
	}
	
	return( rc );
}



void debugdisp( void )
{
	if ( g_DebugDisp != 0 )
	{
		DrawNumer( stage_info.enemy, 8, 10, 10 + ( 20 * 0 ), 0, 0 );	
		DrawNumer( stage_info.eneshot, 8, 10, 10 + ( 20 * 1 ), 0, 0 );	
		DrawNumer( stage_info.scoeritem, 8, 10, 10 + ( 20 * 2 ), 0, 0 );	
		DrawNumer( player[0].move[0], 8, 10, 10 + ( 20 * 3 ), 0, 0 );	
		DrawNumer( player[0].move[1], 8, 10, 10 + ( 20 * 4 ), 0, 0 );	
		DrawNumer( g_Rank_Red, 8, 10, 10 + ( 20 * 5 ), 0, 0 );	
		DrawNumer( stage_info.plyshot, 8, 10, 10 + ( 20 * 6 ), 0, 0 );	
		DrawNumer( player[0].dir, 8, 10, 10 + ( 20 * 7 ), 0, 0 );	
		DrawNumer( sakiyomi[3].x, 8, 10, 10 + ( 20 * 8 ), 0, 0 );	
		DrawNumer( sakiyomi[4].x, 8, 10, 10 + ( 20 * 9 ), 0, 0 );	
	}
	// 線の描画
}







