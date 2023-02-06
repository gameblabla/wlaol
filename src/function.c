#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "SDL.h"
#include "SDL_mixer.h"
#include "define.h"
#include "function.h"
#include "extern.h"
#include "util_snd.h"
#include "include/general.h"
#include "include/dconv.h"
#include "SDL_rotozoom.h"

void FunctionInit( void );
void ResetGameFlag( void );
void ResetGameFlag2( void );
int LoadGameFlag( char *fn );
int SaveGameFlag( char *fn );
int LoadGameFlag2( char *fn );
int SaveGameFlag2( char *fn );
int SaveFile( char *fn, int32_t *buff, int32_t size );
int LoadFile( char *fn, int32_t *buff, int32_t size );
int32_t GetConfig( char* fn, char* cParam );
int32_t LogFileWrite( char* fn, char* cParam );

int32_t LoadBitmap( char *fname , int bmpindex, int flag );
void ReleaseBitmap( int bmpindex );
void BltRect(int bmpindex, int srcX, int srcY, int dstX, int dstY, int width, int height);
void Blt( int bmpindex, int dstX, int dstY );
void BltFunction( int bmpindex, int dstx, int dsty, int TexPosX, int TexPosY, int Width, int Height, float zoomx, float zoomy, int func, int r, int g, int b, int a, int rot, int pri, int flip );
void BltFunctionEx( int bmpindex, int dstx, int dsty, int TexPosX, int TexPosY, int Width, int Height, float zoomx, float zoomy, int func, int r, int g, int b, int a, int rot, int pri, int flip );
void BltOffset( int offset_x, int offset_y );
void SetObjSetClip( int flg );
void SetColorKey(int bmpindex, int flag );
void SetColorKeyPos(int bank, Uint32 color);
void ClearSecondary( void );
void DrawNumer( int32_t value, int32_t length, int32_t x, int32_t y, int32_t zero, int32_t center );
void BltNumericImage( int32_t value, int32_t length, int32_t x, int32_t y, int32_t plane, int32_t num_stpos_x, int32_t num_stpos_y, int32_t num_width, int32_t num_height, int32_t width_offset, int32_t zero );
int32_t get2keta( int32_t val, int32_t st );
void SetGscreenPalette( SDL_Surface *surface );
void SetPalette(int getbmpindex, int setbmpindex);
void BltRectRotZoom( int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height, double angle, double zoom, int smooth);
void CreateSurface( int bmpindex, int size_x, int size_y  );
void SwapToSecondary( int bmpindex );
void SaveBmp( int bmpindex, char *fn );
void drawGRPline(f32 x1, f32 y1, f32 x2, f32 y2, Uint32 color);
void pointSDLsurface( f32 px, f32 py, Uint32 color);
static inline void putSDLpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

void BltFunctionBGR(int index, int srcX, int srcY, int dstX, int dstY, int width, int height, int ScaleX, int ScaleY, int func, int blend, int flip, int rot, UINT32 bgr );
void rgb_blend(SDL_Surface *s_surf, SDL_Surface *d_surf, Sint16 osx, Sint16 osy, Sint16 sx, Sint16 sy, Sint16 ex, Sint16 ey, float rblend, float gblend, float bblend, float ablend);
Uint32 SDL_wheel_GetPixel(SDL_Surface *surf, int x, int y);
void SDL_wheel_PutPixel(SDL_Surface *surf, Sint16 x, Sint16 y, Uint32 color);
Uint32 SDL_wheel_MapRgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void BltRectZoom( int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height, int zoom );
void SetMozaiku( int size );
void BltMozaiku(SDL_Surface *s_surf, SDL_Surface *d_surf, Sint16 osx, Sint16 osy, Sint16 sx, Sint16 sy, Sint16 ex, Sint16 ey, int size );

void DarwFont( char *value, int32_t x, int32_t y, int32_t center );
void BltTextImage( char *value, int32_t bmplen, int32_t x, int32_t y, int32_t plane, int32_t num_stpos_x, int32_t num_stpos_y, int32_t num_width, int32_t num_height, int32_t width_offset, int32_t font_scale );

void BltGSreface( int rr );
void halt( void );

int IsPushKey( int player_num, int keycode );
int IsPressKey( int player_num, int keycode );
void KeyInit( void );
void KeyInput( void );
int initPAD(void);
void closePAD(void);
int getPAD(void);
int IsPushOKKey( void );
int IsPushCancelKey( void );

void FPSWait( void );
int system_keys( void );
int Set_Volume( int vol );

void soundInitBuffer(void);
void soundRelease(void);
void soundLoadBuffer(Sint32 num, Uint8 *fname, int loop);

int32_t funcSin( int32_t rdo );
int32_t funcCos( int32_t rdo );
int32_t funcTan2( int32_t posX, int32_t posY );
int32_t funcRand( int32_t max );

// キー取得用
static int key_eventPress[GP2X_BUTTON_MAX];
static int key_eventPress_old[GP2X_BUTTON_MAX];
static int key_eventPush[GP2X_BUTTON_MAX];
int	pad_type;
int	pads;
int	trgs;
int	reps;

SDL_Joystick *joys;
Uint8 *keys;

//static int pads_old;
static int g_intcnt;
static int rep_cnt;
// 画像表示用
static SDL_Surface* bitmap[BMPBUFF_MAX];
static SDL_Surface* g_surface_bakup;
int disp_offset_x;
int disp_offset_y;
int g_IsClip;
int g_Mozaike;

// 定時処理用
static int32_t prvTickCount;
static int32_t nowTick;
//static int frame;
#ifdef GP2X
//static const int INTERVAL_BASE = 8;
static const int INTERVAL_BASE = 16;
#else
static const int INTERVAL_BASE = 16;
#endif
SDL_Event event;

// 音楽再生
static int32_t sound_vol;


void FunctionInit( void )
{
	int i;
	
	prvTickCount = 0;
	
	for ( i = 0; i < BMPBUFF_MAX; i++ )
	{
		bitmap[i] = NULL;
	}
	KeyInit( );
	soundInitBuffer();
	g_DebugKeyStat = EN_DEBUGFUNC_NONE;
	g_intcnt = 0;
	
	disp_offset_x = 0;
	disp_offset_y = 0;
	SetMozaiku( 0 );
	SetObjSetClip( true );

}

void SetScene( int Scene )
{
	g_scene_req = Scene;
}

void SetSaveDate( int index, int data )
{
	gameflag[index] = data;
}
int GetSaveDate( int index )
{
	return( gameflag[index] );
}
void AddSaveDate( int index, int data )
{
	gameflag[index] += data;
}


void ResetGameFlag( void )
{
	memset( &gameflag[0], 0, sizeof( gameflag ) );
}
int LoadGameFlag( char *fn )
{
	int rc = 0;
	FILE *fp;	/* (1)ファイルポインタの宣言 */
	int size;
	char tmp[128];
	snprintf(tmp, sizeof(tmp), "%s/%s", homepath, fn);
	
	/* (2)ファイルのオープン */
	/*  ここで、ファイルポインタを取得する */
	if ( ( fp = fopen( tmp, "rb" ) ) == NULL )
	{
//		printf("file open error!!\n");
		rc = -1;	/* (3)エラーの場合は通常、異常終了する */
	}
	else 
	{
		/* (4)ファイルの読み（書き）*/
		size = fread( &gameflag[0], 1, sizeof( gameflag ), fp );  /* 5000バイト分読み込む */
		fclose( fp );	/* (5)ファイルのクローズ */
#ifdef GP2X
		sync( );
#endif
	}
	
	return ( rc );
}
int SaveGameFlag( char *fn )
{
	int rc = 0;
	FILE *fp;	/* (1)ファイルポインタの宣言 */
	int size;
	char tmp[128];
	snprintf(tmp, sizeof(tmp), "%s/%s", homepath, fn);

	/* (2)ファイルのオープン */
	/*  ここで、ファイルポインタを取得する */
	if ( ( fp = fopen( tmp, "wb" ) ) == NULL )
	{
//		printf("file open error!!\n");
		rc = -1;	/* (3)エラーの場合は通常、異常終了する */
	}
	else 
	{
		/* (4)ファイルの読み（書き）*/
		size = fwrite( &gameflag[0], 1, sizeof( gameflag ), fp );  /* 5000バイト分読み込む */
		fclose( fp );	/* (5)ファイルのクローズ */
#ifdef GP2X
		sync( );
#endif
	}
	
	return ( rc );
}
void ResetGameFlag2( void )
{
	memset( &gameflag2[0], 0, sizeof( gameflag ) );
}
int LoadGameFlag2( char *fn )
{
	int rc = 0;
	FILE *fp;	/* (1)ファイルポインタの宣言 */
	int size;
	char tmp[128];
	snprintf(tmp, sizeof(tmp), "%s/%s", homepath, fn);
	
	/* (2)ファイルのオープン */
	/*  ここで、ファイルポインタを取得する */
	if ( ( fp = fopen( tmp, "rb" ) ) == NULL )
	{
//		printf("file open error!!\n");
		rc = -1;	/* (3)エラーの場合は通常、異常終了する */
	}
	else 
	{
		/* (4)ファイルの読み（書き）*/
		size = fread( &gameflag2[0], 1, sizeof( gameflag ), fp );  /* 5000バイト分読み込む */
		fclose( fp );	/* (5)ファイルのクローズ */
#ifdef GP2X
		sync( );
#endif
	}
	
	return ( rc );
}
int SaveGameFlag2( char *fn )
{
	int rc = 0;
	FILE *fp;	/* (1)ファイルポインタの宣言 */
	int size;
	char tmp[128];
	snprintf(tmp, sizeof(tmp), "%s/%s", homepath, fn);
	
	/* (2)ファイルのオープン */
	/*  ここで、ファイルポインタを取得する */
	if ( ( fp = fopen( tmp, "wb" ) ) == NULL )
	{
//		printf("file open error!!\n");
		rc = -1;	/* (3)エラーの場合は通常、異常終了する */
	}
	else 
	{
		/* (4)ファイルの読み（書き）*/
		size = fwrite( &gameflag2[0], 1, sizeof( gameflag ), fp );  /* 5000バイト分読み込む */
		fclose( fp );	/* (5)ファイルのクローズ */
#ifdef GP2X
		sync( );
#endif
	}
	
	return ( rc );
}
int SaveFile( char *fn, int32_t *buff, int32_t size )
{
	int rc = 0;
	FILE *fp;	/* (1)ファイルポインタの宣言 */
	
	/* (2)ファイルのオープン */
	/*  ここで、ファイルポインタを取得する */
	if ( ( fp = fopen( fn, "wb" ) ) == NULL )
	{
//		printf("file open error!!\n");
		rc = -1;	/* (3)エラーの場合は通常、異常終了する */
	}
	else 
	{
		/* (4)ファイルの読み（書き）*/
		size = fwrite( buff, 1, size, fp );  /* 5000バイト分読み込む */
		fclose( fp );	/* (5)ファイルのクローズ */
#ifdef GP2X
		sync( );
#endif
	}
	
	return ( rc );
}
int LoadFile( char *fn, int32_t *buff, int32_t size )
{
	int rc = 0;
	FILE *fp;	/* (1)ファイルポインタの宣言 */
	
	/* (2)ファイルのオープン */
	/*  ここで、ファイルポインタを取得する */
	if ( ( fp = fopen( fn, "rb" ) ) == NULL )
	{
//		printf("file open error!!\n");
		rc = -1;	/* (3)エラーの場合は通常、異常終了する */
	}
	else 
	{
		/* (4)ファイルの読み（書き）*/
		size = fread( buff, 1, size, fp );  /* 5000バイト分読み込む */
		fclose( fp );	/* (5)ファイルのクローズ */
#ifdef GP2X
		sync( );
#endif
	}
	
	return ( rc );
}

int32_t GetConfig( char* fn, char* cParam )
{
	FILE *fp;	/* (1)ファイルポインタの宣言 */
	char *sp;
	char s[256];
	char s2[256];
	memset( s, '\0', sizeof( s ) );
	int32_t rc;
	
	rc = 0;
	/* (2)ファイルのオープン */
	/*  ここで、ファイルポインタを取得する */
	if ( ( fp = fopen( fn, "r" ) ) == NULL )
	{
		rc = 0;	/* (3)エラーの場合は通常、異常終了する */
	}
	else 
	{
		/* (4)ファイルの読み（書き）*/
		while ( fgets( s, 256, fp ) != NULL) 
		{
			/* ここではfgets()により１行単位で読み出し */
			if ( strstr( s, cParam ) != NULL )
			{
				sp = NULL;
				//文字列が存在する
				sp = strstr( s, "=" );
				if ( sp != NULL )
				{
					sp++;
					memset( s2, '\0', sizeof( s ) );
					if ( ! ( ( *sp >= '0' && *sp <= '9' ) || ( *sp =='-' ) ) )
					{
						return 0;	//	だめびゃん...
					}
					while ( *sp >= '0' && *sp <= '9' )
					{
						rc = rc * 10 + ( *sp - '0' );
						sp++;
					}
				}
				break;
			}
			memset( s, '\0', sizeof( s ) );
		}
#ifdef GP2X
		sync( );
#endif
		fclose(fp);
	}
	return( rc );
}

int32_t LogFileWrite( char* fn, char* cParam )
{
	FILE *fp;	/* (1)ファイルポインタの宣言 */
	int32_t rc;
	
	rc = 0;
	/* (2)ファイルのオープン */
	/*  ここで、ファイルポインタを取得する */
	if ( ( fp = fopen( fn, "a" ) ) == NULL )
	{
		rc = 0;	/* (3)エラーの場合は通常、異常終了する */
	}
	else 
	{
		fputs( cParam, fp);
#ifdef GP2X
		sync( );
#endif
		fclose(fp);
	}
	return( rc );
}

int32_t LoadBitmap( char *fname , int bmpindex, int flag )
{
	int32_t rc;
	SDL_Surface* tmp;

	rc = 0;
	
	ReleaseBitmap( bmpindex );


	tmp = SDL_LoadBMP( fname );
	if(tmp)
	{
//		if(flag != 0)
//		{
//			// パレット０番が透過色
//			SDL_SetColorKey(tmp, SDL_SRCCOLORKEY, SDL_MapRGBA(g_screen->format,0,0,0,0) );
//		}
		bitmap[bmpindex] = SDL_DisplayFormat(tmp);
//		bitmap[bmpindex] = tmp;
		SDL_FreeSurface(tmp);
	}
	else 
	{
		rc = -1;
	}

//	bitmap[bmpindex] = SDL_LoadBMP( fname );
	if(bitmap[bmpindex])
	{
		if(flag != 0)
		{
			SDL_Surface *surface;
//		    Uint8 bpp;
//			SDL_Palette *pal;
//			Uint8 r;
//			Uint8 g;
//			Uint8 b;

			surface = bitmap[bmpindex];
		    if(surface){
//			    bpp = surface->format->BytesPerPixel;
//				if(bpp <= 1){
//					pal = surface->format->palette;
//					if(pal){
//						r = pal->colors->r;
//						g = pal->colors->g;
//						b = pal->colors->b;
//						// パレット０番が透過色
//						SDL_SetColorKey(bitmap[bmpindex], SDL_SRCCOLORKEY, SDL_MapRGBA(g_screen->format,r,g,b,0) );
//					}
//				}
				SDL_SetColorKey(bitmap[bmpindex], SDL_SRCCOLORKEY, SDL_MapRGBA(g_screen->format,0,255,0,0) );
			}
		}
	}
	else 
	{
		rc = -1;
	}


	return ( rc );
}
void SetColorKey(int bmpindex, int flag )
{
	if(flag != 0)
	{
		SDL_Surface *surface;
	    Uint8 bpp;
		SDL_Palette *pal;
		Uint8 r;
		Uint8 g;
		Uint8 b;

		surface = bitmap[bmpindex];
	    if(surface){
		    bpp = surface->format->BytesPerPixel;
			if(bpp <= 1){
				pal = surface->format->palette;
				if(pal){
					r = pal->colors->r;
					g = pal->colors->g;
					b = pal->colors->b;
					// パレット０番が透過色
					SDL_SetColorKey(bitmap[bmpindex], SDL_SRCCOLORKEY, SDL_MapRGBA(g_screen->format,r,g,b,0) );
				}
			}
		}
	}
	else 
	{
		SDL_SetColorKey(bitmap[bmpindex], 0, SDL_MapRGBA(g_screen->format,0,0,0,0) );
	}
}
// 未使用
void SetColorKeyPos(int bmpindex, Uint32 color)
{
//	SDL_GetRGBA(Uint32 pixel, SDL_PixelFormat *fmt, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);
	SDL_SetColorKey(bitmap[bmpindex], SDL_SRCCOLORKEY, color);
}
// BMPの開放
void ReleaseBitmap( int bmpindex )
{
	if ( bitmap[bmpindex] != NULL )
	{
		SDL_FreeSurface(bitmap[bmpindex]);
		bitmap[bmpindex] = NULL;
	}
}


void SetMozaiku( int size )
{
	g_Mozaike = size;
}

// ＢＭＰ範囲指定表示
void BltRect( int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height)
{
	SDL_Rect srcRect;
	srcRect.x = srcX;
	srcRect.y = srcY;
	srcRect.w = width;
	srcRect.h = height;

	SDL_Rect dstRect;
	dstRect.x = dstX + disp_offset_x;
	dstRect.y = dstY + disp_offset_y;
	dstRect.w = width;
	dstRect.h = height;

	if(bitmap[bmpindex])
	{
		SDL_BlitSurface(bitmap[bmpindex], &srcRect, g_screen, &dstRect);
	}
}
// ＢＭＰ範囲指定表示
void Blt( int bmpindex, int dstX, int dstY )
{
	SDL_Rect dstRect;
	dstRect.x = dstX + disp_offset_x;
	dstRect.y = dstY + disp_offset_y;
	dstRect.w = 0;
	dstRect.h = 0;

	if(bitmap[bmpindex])
	{
		SDL_BlitSurface(bitmap[bmpindex], NULL, g_screen, &dstRect);
	}
}

void BltFunction( int bmpindex, int dstx, int dsty, int TexPosX, int TexPosY, int Width, int Height, float zoomx, float zoomy, int func, int r, int g, int b, int a, int rot, int pri, int flip )
{
//	BltRect( bmpindex, dstx, dsty, TexPosX, TexPosY, Width, Height );
	UINT32 bgr = ( b << 16 ) + ( g << 8 ) + ( r );
	BltFunctionBGR( bmpindex, dstx, dsty, TexPosX, TexPosY, Width, Height, 100, 100, func, a, flip, rot, bgr );
}
void BltFunctionEx( int bmpindex, int dstx, int dsty, int TexPosX, int TexPosY, int Width, int Height, float zoomx, float zoomy, int func, int r, int g, int b, int a, int rot, int pri, int flip )
{
	BltFunction( bmpindex, dstx - ( Width / 2 ), dsty - ( Height / 2 ), TexPosX, TexPosY, Width, Height, zoomx, zoomy, func, r, g, b, a, rot, pri, flip );
}

void SetObjSetClip( int flg )
{
	g_IsClip = flg;
}

void ObjSetEx( UINT32 charcode, INT32 pri, INT32 dstx, INT32 dsty, float zoomx, float zoomy, INT32 func, INT32 r, INT32 g, INT32 b, INT32 a, float rot, INT32 flip, INT32 center, INT32 w, INT32 h )
{
	INT32 clip = 0;

	if ( charcode == 0 ) 
	{
		return;
	}
	INT32 num = 0;
	while( 1 )
	{
		CODEDATA rCodet = g_CharCodeData[ charcode + num ];
		if ( ( flip == GM_SPR_V ) || ( flip == GM_SPR_HV ) )
		{
			rCodet.m_OfsX *= -1;
			rCodet.m_CutOfsX *= -1;
		}
		if ( ( flip == GM_SPR_H ) || ( flip == GM_SPR_HV ) )
		{
			rCodet.m_OfsY *= -1;
			rCodet.m_CutOfsY *= -1;
		}
	
		clip = 0;
		INT32 min_x = ( dstx + rCodet.m_OfsX + rCodet.m_CutOfsX ) + ( INT32 )( ( ( float )( rCodet.m_SizeX ) * zoomx ) / 2.0f );
		INT32 max_x = ( dstx + rCodet.m_OfsX + rCodet.m_CutOfsX ) - ( INT32 )( ( ( float )( rCodet.m_SizeX ) * zoomx ) / 2.0f );
		INT32 min_y = ( dsty - rCodet.m_OfsY - rCodet.m_CutOfsY ) + ( INT32 )( ( ( float )( rCodet.m_SizeY ) * zoomy ) / 2.0f );
		INT32 max_y = ( dsty - rCodet.m_OfsY - rCodet.m_CutOfsY ) - ( INT32 )( ( ( float )( rCodet.m_SizeY ) * zoomy ) / 2.0f );

		if (
			 ( ( min_x < 0 ) || ( max_x > 640 ) )
		  || ( ( min_y < 0 ) || ( max_y > 360 ) )
//			 ( ( min_x < 0 ) || ( max_x > 1000 ) )
//		  || ( ( min_y < 0 ) || ( max_y > 1000 ) )
		   )
		{
			//画面外
			if ( g_IsClip == true )
			{
				clip = 1;
			}
		}
		if ( rot != 0.0f )
		{
			clip = 0;
			if (
				 ( ( min_x < -100 ) || ( max_x > 740 ) )
			  || ( ( min_y < -100 ) || ( max_y > 460 ) )
	//			 ( ( min_x < 0 ) || ( max_x > 1000 ) )
	//		  || ( ( min_y < 0 ) || ( max_y > 1000 ) )
			   )
			{
				//画面外
				if ( g_IsClip == true )
				{
					clip = 1;
				}
			}
		}
		if ( clip == 0 )
		{
			INT32	Width = rCodet.m_SizeX;		// 横幅
			INT32	Height = rCodet.m_SizeY;		// 縦幅
			if ( w > -1 )
			{
				Width = w;	//幅指定あり
			}
			if ( h > -1 )
			{
				Height = h;	//高さ指定あり
			}
/*
char str[512];
memset( str, '\0', sizeof( str ) );
sprintf( str, "log %d %d %d %d %d %d %d", r, g, b, rCodet.m_PosX, rCodet.m_PosY, rCodet.m_SizeX, h );
LogFileWrite( "log.txt", str );
*/
			if ( center != 0 )
			{
				//左上基準
				BltFunction( rCodet.m_ResID, dstx + rCodet.m_OfsX + rCodet.m_CutOfsX, dsty - rCodet.m_OfsY - rCodet.m_CutOfsY, rCodet.m_PosX, rCodet.m_PosY, Width, Height, zoomx, zoomy, func, r, g, b, a, rot, pri, flip );
			}
			else
			{
				//中央基準
				BltFunctionEx( rCodet.m_ResID, dstx + rCodet.m_OfsX + rCodet.m_CutOfsX, dsty - rCodet.m_OfsY - rCodet.m_CutOfsY, rCodet.m_PosX, rCodet.m_PosY, Width, Height, zoomx, zoomy, func, r, g, b, a, rot, pri, flip );
			}
		}

		num++;	//分割画像あり
		rCodet = g_CharCodeData[ charcode + num ];	//次の行の分割番号が連番か？
		if ( rCodet.m_Bunkatu != ( num + 1 ) )
		{
			//分割画像終了
			break;
		}


/*
		CSpriteDrawObj* p = &CSDKCompGetInstance().SpriteDrawObj();
		
		if(p)
		{
			uColor = r + ( g << 8 ) + ( b << 16 );

			p->ObjSet( charcode + i, dstx*_DOT, dsty*_DOT, pri);
			p->SetSpColor( uColor );
			switch( func )
			{
			case EN_MINI_KASA:
				p->SetBlendKasan( a );
				break;
			case EN_MINI_ZYOU:
				p->SetBlendJyozan( a );
				break;
			case EN_MINI_KASA2:
				p->SetBlendMusiKasan( a );
				break;
			case EN_MINI_BLEND:
			default:
				p->SetBlendHantoumei( a );
				break;
			}
			p->SetSpZoom( zoomx * 100, zoomy * 100 );
			p->SetSpCenter( SP_CC );
	//		p->SetSpFlip();
*/
	/*
		GM_SPR_NONE,
		GM_SPR_V,
		GM_SPR_H,
		GM_SPR_HV,
	*/
//		}
	}
}


void SetGscreenPalette( SDL_Surface *surface )
{
//	SDL_Surface *surface;
    Uint8 bpp;
	SDL_Palette *pal;

//	surface = bitmap[bmpindex];
    if(surface){
	    bpp = surface->format->BytesPerPixel;
		if(bpp <= 1){
			pal = surface->format->palette;
			if(pal){
				SDL_SetPalette(g_screen, SDL_LOGPAL|SDL_PHYSPAL, pal->colors, 0, 256);
			}
		}
	}
}
void SetPalette(int getbmpindex, int setbmpindex)
{
	SDL_Surface *surface;
    Uint8 bpp;
	SDL_Palette *pal;

	surface = bitmap[getbmpindex];
    if(surface){
	    bpp = surface->format->BytesPerPixel;
		if(bpp <= 8){
			pal = surface->format->palette;
			if(pal){
				SDL_SetPalette(bitmap[setbmpindex], SDL_LOGPAL|SDL_PHYSPAL, pal->colors, 0, 256);
			}
		}
	}
}
void SaveBmp( int bmpindex, char *fn )
{
	if ( bmpindex >= 0 )
	{
		SDL_SaveBMP(bitmap[bmpindex], fn);
	}
	else 
	{
		SDL_SaveBMP(g_screen, fn);
	}
}
void CreateSurface( int bmpindex, int size_x, int size_y  )
{
	SDL_Surface* tmp;
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

	ReleaseBitmap( bmpindex );
	tmp = SDL_CreateRGBSurface( SDL_SWSURFACE, size_x, size_y, 0, rmask, gmask, bmask, amask );
	if(tmp)
	{
		bitmap[bmpindex] = SDL_DisplayFormat(tmp);
		SDL_FreeSurface(tmp);
	}
}

void SwapToSecondary( int bmpindex )
{
	if ( g_surface_bakup != NULL )
	{
		g_screen = g_surface_bakup;
		g_surface_bakup = NULL;
	}
	else 
	{
		g_surface_bakup = g_screen;
		g_screen = bitmap[bmpindex];
	}
} 

void GScreenToSecondary( int bmpindex )
{
	SDL_Rect dstRect;
	dstRect.x = 0;
	dstRect.y = 0;
	dstRect.w = 0;
	dstRect.h = 0;

	if(bitmap[bmpindex])
	{
		SDL_BlitSurface(g_screen, NULL, bitmap[bmpindex], &dstRect);
	}
} 

// ＢＭＰ範囲指定表示
void BltRectRotZoom( int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height, double angle, double zoom, int smooth)
{
/*
	SDL_Surface *temp_Surface;
	
	temp_Surface = rotozoomSurface(bitmap[bmpindex], angle, zoom, smooth);
	SDL_Surface *surface;
    Uint8 bpp;
	SDL_Palette *pal;
	Uint8 r;
	Uint8 g;
	Uint8 b;

	surface = bitmap[bmpindex];
    if(surface){
	    bpp = surface->format->BytesPerPixel;
		if(bpp <= 8){
			pal = surface->format->palette;
			if(pal){
				SDL_SetPalette(temp_Surface, SDL_LOGPAL|SDL_PHYSPAL, pal->colors, 0, 256);
				r = pal->colors->r;
				g = pal->colors->g;
				b = pal->colors->b;
				// パレット０番が透過色
				SDL_SetColorKey(temp_Surface, SDL_SRCCOLORKEY, SDL_MapRGBA(g_screen->format,r,g,b,0) );
			}
		}
	}
	
//	SDL_SaveBMP(SDL_DisplayFormat(temp_Surface), "tempbmp.bmp");
	
	
	SDL_Rect srcRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = temp_Surface->w;
	srcRect.h = temp_Surface->h;

	SDL_Rect dstRect;
	dstRect.x = dstX - ( temp_Surface->w / 2 );
	dstRect.y = dstY - ( temp_Surface->h / 2 );
	dstRect.w = temp_Surface->w;
	dstRect.h = temp_Surface->h;

	SDL_BlitSurface( temp_Surface , &srcRect, g_screen, &dstRect);
	SDL_FreeSurface(temp_Surface);
*/
}

//画面表示位置オフセット
void BltOffset( int offset_x, int offset_y )
{
	disp_offset_x = offset_x;
	disp_offset_y = offset_y;
}

void BltFunctionBGR(int index, int srcX, int srcY, int dstX, int dstY, int width, int height, int ScaleX, int ScaleY, int func, int blend, int flip, int rot, UINT32 bgr )
{
	int x;
	int y;

	//モザイクのテスト
	if ( g_Mozaike > 1 )
	{
		BltMozaiku( bitmap[index], g_screen, srcX + disp_offset_x, srcY + disp_offset_y, dstX, dstY, width, height, g_Mozaike );
		return; 
	}
	
	if ( ScaleX != 100 )	//拡大する
	{
		ScaleY = ScaleX;
		BltRectZoom( index, srcX, srcY, dstX, dstY, width, height, ScaleX );
	}
	else 
	{
		//ブレンドを行う
		if ( ( 0x00ffffff != bgr ) || ( blend < 255 ) )
		{
/*
			SDL_Surface* temp_Surface2;
			SDL_Surface* temp_Surface;
		    Uint32 rmask, gmask, bmask, amask;
		    SDL_PixelFormat fmt = *bitmap[index]->format;
		    rmask = bgr & 0x000000ff;
		    gmask = bgr & 0x0000ff00;
		    bmask = bgr & 0x00ff0000;
		    amask = bgr & 0xff000000;
		    
		    fmt.Rmask = rmask;
		    fmt.Gmask = gmask;
		    fmt.Bmask = bmask;
		    fmt.Amask = amask;

//			temp_Surface = SDL_ConvertSurface( bitmap[index], &fmt, SDL_SWSURFACE);
			temp_Surface2 = SDL_CreateRGBSurface(SDL_SWSURFACE, bitmap[index]->w, bitmap[index]->h, 32, rmask, gmask, bmask, 0xff000000);
			temp_Surface = SDL_DisplayFormatAlpha( temp_Surface2 ); 
			SDL_FreeSurface(temp_Surface2);
			
			SDL_Rect srcRect;
			srcRect.x = 0;
			srcRect.y = 0;
			srcRect.w = bitmap[index]->w;
			srcRect.h = bitmap[index]->h;

			SDL_Rect dstRect;
			dstRect.x = 0;
			dstRect.y = 0;
			dstRect.w = bitmap[index]->w;
			dstRect.h = bitmap[index]->h;

			SDL_BlitSurface( bitmap[index] , NULL, temp_Surface, NULL);

//			SDL_Rect srcRect;
			srcRect.x = 0;
			srcRect.y = 0;
			srcRect.w = width;
			srcRect.h = height;

//			SDL_Rect dstRect;
			dstRect.x = srcX;
			dstRect.y = srcY;
			dstRect.w = width;
			dstRect.h = height;
			SDL_BlitSurface( temp_Surface , &srcRect, g_screen, &dstRect);
			SDL_FreeSurface(temp_Surface);
*/
/*
//			SDL_Surface *originalImage;
			SDL_Surface *image;
			SDL_Surface *dummy;
		    Uint32 rmask, gmask, bmask, amask;
		    rmask = bgr & 0x000000ff;
		    gmask = bgr & 0x0000ff00;
		    bmask = bgr & 0x00ff0000;
		    amask = bgr & 0xff000000;
			
//			originalImage = SDL_LoadBMP("koyo.bmp");
//			screen = SDL_SetVideoMode(bitmap[index]->w, bitmap[index]->h, 0, SDL_SWSURFACE);

			image = SDL_CreateRGBSurface(SDL_SWSURFACE,
					bitmap[index]->w, bitmap[index]->h,
					32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
			dummy = SDL_CreateRGBSurface(SDL_SWSURFACE,
					1, 1,
					32, 0x00000080, 0x00008000, 0x00ff0000, 0xff000000);
			SDL_BlitSurface(bitmap[index], NULL, image, NULL);

			image->format->Rloss = dummy->format->Rloss;
			image->format->Gloss = dummy->format->Gloss;
			image->format->Bloss = dummy->format->Bloss;
			image->format->Rshift = dummy->format->Rshift;
			image->format->Gshift = dummy->format->Gshift;
			image->format->Bshift = dummy->format->Bshift;
			image->format->Rmask = dummy->format->Rmask;
			image->format->Gmask = dummy->format->Gmask;
			image->format->Bmask = dummy->format->Bmask;

			SDL_Rect srcRect;
			srcRect.x = dstX;
			srcRect.y = dstY;
			srcRect.w = width;
			srcRect.h = height;

			SDL_Rect dstRect;
			dstRect.x = srcX + disp_offset_x;
			dstRect.y = srcY + disp_offset_x;
			dstRect.w = width;
			dstRect.h = height;

			SDL_BlitSurface(image, &srcRect, g_screen, &dstRect );

			SDL_FreeSurface(image);
			SDL_FreeSurface(dummy);
*/

		    float rmask, gmask, bmask, amask;
		    rmask = ( float )( bgr & 0x000000ff ) / 255.0f;
		    gmask = ( float )( ( bgr & 0x0000ff00 ) >> 8 ) / 255.0f;
		    bmask = ( float )( ( bgr & 0x00ff0000 ) >> 16 ) / 255.0f;
		    amask = ( float )( blend ) / 255.0f;

//			SDL_wheel_Blend( bitmap[index], g_screen, srcX, srcY, width, height, rmask, gmask, bmask);
/*
			SDL_Surface* temp_Surface2;
			SDL_Surface* temp_Surface;
			temp_Surface2 = SDL_CreateRGBSurface(SDL_SWSURFACE,
					width, height,
					32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
			temp_Surface = SDL_DisplayFormatAlpha( temp_Surface2 ); 
			SDL_Rect srcRect;
			srcRect.x = 0;
			srcRect.y = 0;
			srcRect.w = width;
			srcRect.h = height;

			SDL_Rect dstRect;
			dstRect.x = srcX;
			dstRect.y = srcY;
			dstRect.w = width;
			dstRect.h = height;
			SDL_BlitSurface(bitmap[index], NULL, temp_Surface, &dstRect );
*/

//			rgb_blend(bitmap[index], g_screen, 32, 32, 0, 0, 100, 100, 0.5f, 0.5f, 1.0f, 1.0f);
//			rgb_blend(bitmap[index], g_screen, srcX, srcY, 32, 32, 32, 32, 0.5f, 0.5f, 1.0f, 2.0f);
//			rgb_blend(bitmap[index], g_screen, srcX, srcY, dstX, dstY, width, height, rmask, gmask, bmask, amask);
//			rgb_blend(bitmap[index], temp_Surface, 0, 0, dstX, dstY, width, height, 0.5f, 0.5f, 1.0f, 2.0f);
/*
//			SDL_Rect srcRect;
			srcRect.x = 0;
			srcRect.y = 0;
			srcRect.w = width;
			srcRect.h = height;

//			SDL_Rect dstRect;
			dstRect.x = 0;
			dstRect.y = 0;
			dstRect.w = width;
			dstRect.h = height;
			SDL_BlitSurface(temp_Surface, &srcRect, temp_Surface, &dstRect );

			SDL_FreeSurface(temp_Surface2);
			SDL_FreeSurface(temp_Surface);
*/

			switch( flip )
			{
			case 1:
				for ( x = 0; x < width; x++ )
				{
					rgb_blend(bitmap[index], g_screen, srcX + x + disp_offset_x, srcY + disp_offset_y, dstX + ( ( width - 1 ) - x ), dstY, 1, height, rmask, gmask, bmask, amask);
//					BltRect( index, srcX + x, srcY    , dstX + ( ( width - 1 ) - x ) , dstY                         , 1, height);
				}
				break;
			case 2:
				for ( y = 0; y < height; y++ )
				{
					rgb_blend(bitmap[index], g_screen, srcX + disp_offset_x, srcY + y + disp_offset_y, dstX, dstY + ( ( height - 1 ) - y ) , width, 1, rmask, gmask, bmask, amask);
//					BltRect( index, srcX    , srcY + y, dstX,                         dstY + ( ( height - 1 ) - y ) , width, 1);
				}
				break;
			case 3:
				for ( x = 0; x < width; x++ )
				{
					for ( y = 0; y < height; y++ )
					{
						rgb_blend(bitmap[index], g_screen, srcX + x + disp_offset_x, srcY + y + disp_offset_y, dstX + ( ( width - 1 ) - x ), dstY + ( ( height - 1 ) - y ) , 1, 1, rmask, gmask, bmask, amask);
	//					BltRect( index, srcX    , srcY + y, dstX,                         dstY + ( ( height - 1 ) - y ) , width, 1);
					}
				}
				break;
			default:
				rgb_blend(bitmap[index], g_screen, srcX + disp_offset_x, srcY + disp_offset_y, dstX, dstY, width, height, rmask, gmask, bmask, amask);
//				BltRect( index, srcX, srcY, dstX, dstY, width, height);
				break;
			}

		}
		else 
		{
			switch( flip )
			{
			case 1:
				for ( x = 0; x < width; x++ )
				{
					BltRect( index, srcX + x, srcY    , dstX + ( ( width - 1 ) - x ) , dstY                         , 1, height);
				}
				break;
			case 2:
				for ( y = 0; y < height; y++ )
				{
					BltRect( index, srcX    , srcY + y, dstX,                         dstY + ( ( height - 1 ) - y ) , width, 1);
				}
				break;
			case 3:
				for ( x = 0; x < width; x++ )
				{
					for ( y = 0; y < height; y++ )
					{
						BltRect( index, srcX + x + disp_offset_x, srcY + y + disp_offset_y, dstX + ( ( width - 1 ) - x ), dstY + ( ( height - 1 ) - y ) , 1, 1 );
	//					BltRect( index, srcX    , srcY + y, dstX,                         dstY + ( ( height - 1 ) - y ) , width, 1);
					}
				}
//				for ( y = 0; y < height; y++ )
//				{
//					BltRect( index, srcX    , srcY + y, dstX,                         dstY + ( ( height - 1 ) - y ) , width, 1);
//				}
				break;
			default:
				BltRect( index, srcX + disp_offset_x, srcY + disp_offset_y, dstX, dstY, width, height);
				break;
			}
		}
	}
	
}

static Uint8 r1, g1, b1, a1;
static Uint8 r2, g2, b2, a2;
void rgb_blend(SDL_Surface *s_surf, SDL_Surface *d_surf, Sint16 osx, Sint16 osy, Sint16 sx, Sint16 sy, Sint16 ex, Sint16 ey, float rblend, float gblend, float bblend, float ablend)
{
	int i, j;
	Uint32 col1, col2;
	Uint32 r, g, b, a;

	switch(d_surf->format->BytesPerPixel){
		case 2:
		case 3:
		case 4:
			break;
		case 1:
		default:
			return;
	}

	if(SDL_LockSurface(d_surf) != -1){
		for(i = 0; i < ey; i++){
			if(( i + sy ) >= s_surf->h) continue;
			if(( i + osy ) >= d_surf->h) continue;
			if(( i + sy ) < 0) continue;
			if(( i + osy ) < 0) continue;
			for(j = 0; j < ex; j++){
				if(( j + sx ) >= s_surf->w) continue;
				if(( j + osx ) >= d_surf->w) continue;
				if(( j + sx ) < 0) continue;
				if(( j + osx ) < 0) continue;
				col1 = SDL_wheel_GetPixel(s_surf, j + sx, i + sy);
				col2 = SDL_wheel_GetPixel(d_surf, j + osx, i + osy);
				SDL_GetRGBA(col1, s_surf->format, &r1, &g1, &b1, &a1);
				SDL_GetRGBA(col2, d_surf->format, &r2, &g2, &b2, &a2);
				if ( 
				     ( r1 == 0 )
				  && ( g1 == 255 )
				  && ( b1 == 0 )
				   )
				{
					//黒は抜き色
					 continue;
				}
				if ( ablend < 1.0f )
				{
					//ブレンドあり
					r = (Uint32)( ( rblend * r1 * ablend ) + ( r2 * ( 1 - ablend ) ) );
					g = (Uint32)( ( gblend * g1 * ablend ) + ( g2 * ( 1 - ablend ) ) );
					b = (Uint32)( ( bblend * b1 * ablend ) + ( b2 * ( 1 - ablend ) ) );
//					a = (Uint32)(ablend * a1 * a2 ) / 255.0f);
//					(R,G,B,A)= (Rs*As + Rd*(1 - As), Gs*As + Gd*(1 - As), Bs*As + Bd*(1 - As), As*As + Ad*(1 - As)) 
					a = 255;
				}
				else 
				{
					//ブレンドなし
					r = (Uint32)(rblend * r1);
					g = (Uint32)(gblend * g1);
					b = (Uint32)(bblend * b1);
					a = 255;
				}
				if(r > 255) r = 255;
				if(g > 255) g = 255;
				if(b > 255) b = 255;
				if(a > 255) a = 255;
				col1 = SDL_wheel_MapRgba(r, g, b, a);
				SDL_wheel_PutPixel(d_surf, j + osx, i + osy, col1);
			}
		}
		SDL_UnlockSurface(d_surf);
	}
}

Uint32 SDL_wheel_GetPixel(SDL_Surface *surf, int x, int y)
{
	int bpp = surf->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;

	switch(bpp){
		case 1:
			return *p;
		case 2:
			return *(Uint16 *)p;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
				return p[0] << 16 | p[1] << 8 | p[2];
			}
			return p[0] | p[1] << 8 | p[2] << 16;
		case 4:
			return *(Uint32 *)p;
		default:
			return 0;
	}
}
void SDL_wheel_PutPixel(SDL_Surface *surf, Sint16 x, Sint16 y, Uint32 color)
{
	int bpp = surf->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;

	switch(bpp){
		case 1:
			*p = (Uint8)color;
			break;
		case 2:
			*(Uint16 *)p = (Uint16)color;
			break;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
				p[0] = (color >> 16) & 0xff;
				p[1] = (color >> 8) & 0xff;
				p[2] = color & 0xff;
			}else{
				p[0] = color & 0xff;
				p[1] = (color >> 8) & 0xff;
				p[2] = (color >> 16) & 0xff;
			}
			break;
		case 4:
			*(Uint32 *)p = color;
			break;
		default:
			break;
	}
}
Uint32 SDL_wheel_MapRgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	return SDL_MapRGBA(g_screen->format, r, g, b, a);
}

void BltMozaiku(SDL_Surface *s_surf, SDL_Surface *d_surf, Sint16 osx, Sint16 osy, Sint16 sx, Sint16 sy, Sint16 ex, Sint16 ey, int size )
{
	int i, j;
	Uint32 col1;

	if(SDL_LockSurface(d_surf) != -1){
		for(i = 0; i < ey; i++){
			if(( i + sy ) >= s_surf->h) continue;
			if(( i + osy ) >= d_surf->h) continue;
			if(( i + sy ) < 0) continue;
			if(( i + osy ) < 0) continue;
			for(j = 0; j < ex; j++){
				if(( j + sx ) >= s_surf->w) continue;
				if(( j + osx ) >= d_surf->w) continue;
				if(( j + sx ) < 0) continue;
				if(( j + osx ) < 0) continue;
				
				int tj = ( ( j / size ) * size );
				if ( tj == 0 )
				{
					tj = 1;
				}
				int ti = ( ( i / size ) * size );
				if ( ti == 0 )
				{
					ti = 1;
				}
				col1 = SDL_wheel_GetPixel(s_surf, tj + sx, ti + sy );
				SDL_GetRGBA(col1, s_surf->format, &r1, &g1, &b1, &a1);
				if ( 
				     ( r1 == 0 )
				  && ( g1 == 255 )
				  && ( b1 == 0 )
				   )
				{
					//黒は抜き色
					 continue;
				}
				//ブレンドなし
				SDL_wheel_PutPixel(d_surf, j + osx, i + osy, col1);
			}
		}
		SDL_UnlockSurface(d_surf);
	}
}

void BltRectZoom( int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height, int zoom )
{
//	SDL_Surface *surface;
	/*Uint8 r;
	Uint8 g;
	Uint8 b;
	float zoom_offset = ( float )( ( float )zoom / ( float )100 );
	SDL_Surface *temp_Surface;
	
	temp_Surface = rotozoomSurface(bitmap[bmpindex], 0, zoom_offset, 0 );


	r = 0;
	g = 0;
	b = 0;
	// パレット０番が透過色
	SDL_SetColorKey(temp_Surface, SDL_SRCCOLORKEY, SDL_MapRGBA(g_screen->format,r,g,b,0) );
	
//	SDL_SaveBMP(SDL_DisplayFormat(temp_Surface), "tempbmp.bmp");
	
	
	SDL_Rect srcRect;
	srcRect.x = srcX * zoom_offset;
	srcRect.y = srcY * zoom_offset;
	srcRect.w = width * zoom_offset;
	srcRect.h = height * zoom_offset;

	SDL_Rect dstRect;
//	dstRect.x = 320 - ( temp_Surface->w / 2 );
//	dstRect.y = 240 - ( temp_Surface->h / 2 );
	dstRect.x = dstX;
	dstRect.y = dstY;
	dstRect.w = width * zoom_offset;
	dstRect.h = height * zoom_offset;


	SDL_BlitSurface( temp_Surface , &srcRect, g_screen, &dstRect);
	SDL_FreeSurface(temp_Surface);*/

}

/*[ BltTextImage ]***************************************************/
/*	文字画像表示関数												*/
/*-[引数]-----------------------------------------------------------*/
/*	value			(i )	表示する文字列							*/
/*	bmplen			(i )	画像内に並ぶ１行の文字数				*/
/*	x				(i )	画像を表示する位置ｘ座標				*/
/*	y				(i )	画像を表示する位置ｙ座標				*/
/*	plane			(i )	文字画像が読み込まれているプレーン番号	*/
/*	num_stpos_x		(i )	プレーン内での数値画像の開始位置ｘ座標	*/
/*	num_stpos_y		(i )	プレーン内での数値画像の開始位置ｙ座標	*/
/*	num_width		(i )	数値画像１文字の幅（ドット数）			*/
/*	num_height		(i )	数値画像１文字の高さ（ドット数）		*/
/*	width_offset	(i )	隣の文字までの間隔（ドット数）			*/
/*	font_scale		(i )	拡大率（n倍）							*/
/*-[戻り値]---------------------------------------------------------*/
/*	無し															*/
/********************************************************************/
void DarwFont( char *value, int32_t x, int32_t y, int32_t center )
{
	int len;
	len = strlen( value );
	
	if ( center != 0 )
	{
		//中ぞろえ
		x -= ( len * 18 ) / 2;
	}
	y -= 16 / 2;
	
	CODEDATA rCodet = g_CharCodeData[ C_FONTS3 ];
	BltTextImage( value, 16, x, y, rCodet.m_ResID, rCodet.m_PosX, rCodet.m_PosY, 32, 32, 18, 1 );

}
void BltTextImage( char *value, int32_t bmplen, int32_t x, int32_t y, int32_t plane, int32_t num_stpos_x, int32_t num_stpos_y, int32_t num_width, int32_t num_height, int32_t width_offset, int32_t font_scale )
{
	char decoder[255]   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ.()[]#$%'\"!?+-*/=:oxst0123456789\0";
	int len;
	int i;
	int blt_num;
	char *ans;
	int offset_x;
	int offset_y;
	
	len = strlen( value );
	
	for ( i = 0; i < len; i++ )
	{
		ans = strchr( decoder, value[i] );
		if ( ans != NULL )
		{
			blt_num = (int)(ans - decoder);
			offset_x = blt_num % bmplen;
			offset_y = blt_num / bmplen;
			
			// 数字画像転送
			BltRect( plane, x + ( width_offset * font_scale * i), y, num_stpos_x + (num_width * offset_x), num_stpos_y + ( num_height * offset_y ), num_width, num_height );
//			BltRect( plane, x + (width_offset * i), y, num_stpos_x + (num_width * offset_x), num_stpos_y + ( num_height * offset_y ), num_width, num_height );
		}
	}
	return;
}

// 背景のクリア
void ClearSecondary( void )
{
	SDL_Rect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = DISPLY_WIDTH;
	rect.h = DISPLY_HEIGHT;
	
	
	SDL_FillRect(g_screen, &rect, SDL_MapRGBA(g_screen->format,0,0,0,255));
}

// ラインの描画
void drawGRPline(f32 x1, f32 y1, f32 x2, f32 y2, Uint32 color)
{
	f32 x, y, dx, dy, s, step;
	int i, j;

	dx = abs((x2 >> 16) - (x1 >> 16)) * WP;
	dy = abs((y2 >> 16) - (y1 >> 16)) * WP;

	x = x1;
	y = y1;
/*
	if(x >= DISPLY_WIDTH_WP) x -= DISPLY_WIDTH_WP;
	if(x <  0) x += DISPLY_WIDTH_WP;
	if(y >= DISPLY_HEIGHT_WP) y -= DISPLY_HEIGHT_WP;
	if(y <  0) y += DISPLY_HEIGHT_WP;
*/
	if(dx > dy){
		if(x1 > x2){
			step = (y1 > y2) ? +1 * WP : -1 * WP;
			s = x1;
			x1 = x2;
			x2 = s;
			y1 = y2;
		}else{
			step = (y1 < y2) ? +1 * WP : -1 * WP;
		}
		pointSDLsurface( x, y, color);
		s = dx / 2 * WP;
		i = x1 >> 16;
		j = x2 >> 16;
		while(++i <= j){
			x1 += 1 * WP;
//			if(x1 >= DISPLY_WIDTH_WP) x1 -= DISPLY_WIDTH_WP;
//			if(x1 <  0) x1 += DISPLY_WIDTH_WP;
			s -= dy;
			if(s < 0){
				s += dx;
				y1 += step;
			}
//			if(y1 >= DISPLY_HEIGHT_WP) y1 -= DISPLY_HEIGHT_WP;
//			if(y1 <  0) y1 += DISPLY_HEIGHT_WP;
			pointSDLsurface( x1 + WP, y1,      color);
			pointSDLsurface( x1 + WP, y1 + WP, color);
			pointSDLsurface( x1 + WP, y1 - WP, color);

			pointSDLsurface( x1,      y1,      color);
			pointSDLsurface( x1,      y1 + WP, color);
			pointSDLsurface( x1,      y1 - WP, color);

			pointSDLsurface( x1 - WP, y1,      color);
			pointSDLsurface( x1 - WP, y1 + WP, color);
			pointSDLsurface( x1 - WP, y1 - WP, color);
		}
	}else{
		if(y1 > y2){
			step = (x1 > x2) ? +1 * WP : -1 * WP;
			s = y1;
			y1 = y2;
			y2 = s;
			x1 = x2;
		}else{
			step = (x1 < x2)? +1 * WP : -1 * WP;
		}
		pointSDLsurface( x, y, color);
		s = dy / 2 * WP;
		i = y1 >> 16;
		j = y2 >> 16;
		while(++i <= j){
			y1 += 1 * WP;
//			if(y1 >= DISPLY_HEIGHT_WP) y1 -= DISPLY_HEIGHT_WP;
//			if(y1 <  0) y1 += DISPLY_HEIGHT_WP;
			s -= dx;
			if(s < 0){
				s += dy;
				x1 += step;
			}
//			if(x1 >= DISPLY_WIDTH_WP) x1 -= DISPLY_WIDTH_WP;
//			if(x1 <  0) x1 += DISPLY_WIDTH_WP;
			pointSDLsurface( x1 + WP, y1,      color);
			pointSDLsurface( x1 + WP, y1 + WP, color);
			pointSDLsurface( x1 + WP, y1 - WP, color);

			pointSDLsurface( x1,      y1,      color);
			pointSDLsurface( x1,      y1 + WP, color);
			pointSDLsurface( x1,      y1 - WP, color);

			pointSDLsurface( x1 - WP, y1,      color);
			pointSDLsurface( x1 - WP, y1 + WP, color);
			pointSDLsurface( x1 - WP, y1 - WP, color);
		}
	}
}


static inline void putSDLpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + (y >> 16) * surface->pitch + (x >> 16) * bpp;

    switch(bpp){
	    case 1:
	        *p = pixel;
	        break;
	    case 2:
	        *(Uint16 *)p = pixel;
	        break;
	    case 3:
	        if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
	            p[0] = (pixel >> 16) & 0xff;
	            p[1] = (pixel >> 8) & 0xff;
	            p[2] = pixel & 0xff;
	        }else{
	            p[0] = pixel & 0xff;
	            p[1] = (pixel >> 8) & 0xff;
	            p[2] = (pixel >> 16) & 0xff;
	        }
	        break;
	    case 4:
	        *(Uint32 *)p = pixel;
	        break;
    }
}

void pointSDLsurface( f32 px, f32 py, Uint32 color)
{
	if ( px < 0 || px >= ( DISPLY_WIDTH_WP ) || py < 0 || py >= ( DISPLY_HEIGHT_WP ) ){
		return;
	}

	putSDLpixel(g_screen, px, py, color);
}

void KeyInit( void )
{
	int i;
	
	/* initialize */
/*
	SDL_Init(SDL_INIT_JOYSTICK|SDL_INIT_VIDEO);
	SDL_JoystickOpen(0);
*/
	sound_vol = 128;
	 
	for ( i = 0; i < GP2X_BUTTON_MAX; i++ )
	{
		key_eventPress[i] = 0;
		key_eventPress_old[i] = 0;
		key_eventPush[i] = 0;
	}

	initPAD( );
}
int initPAD(void)
{
	if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0){
		return 0;
    }

	if(SDL_NumJoysticks() > 0){
		joys = SDL_JoystickOpen(0);
		SDL_JoystickEventState(SDL_ENABLE);
	}else{
		joys = NULL;
	}

	pad_type = 0;
	trgs = 0;
	reps = 0;

	rep_cnt = 0;

	return 1;
}
void closePAD(void)
{
	if(SDL_JoystickOpened(0)){
		SDL_JoystickClose(joys);
	}
}

void KeyInput( void )
{
	int i;
#ifdef GP2X
	int pad = 0;

	keys = SDL_GetKeyState(NULL);

	if(joys){
		if(SDL_JoystickGetButton(joys,  0)) pad |= PAD_UP;
		if(SDL_JoystickGetButton(joys,  1)) pad |= PAD_UP | PAD_LEFT;
		if(SDL_JoystickGetButton(joys,  2)) pad |= PAD_LEFT;
		if(SDL_JoystickGetButton(joys,  3)) pad |= PAD_DOWN | PAD_LEFT;
		if(SDL_JoystickGetButton(joys,  4)) pad |= PAD_DOWN;
		if(SDL_JoystickGetButton(joys,  5)) pad |= PAD_DOWN | PAD_RIGHT;
		if(SDL_JoystickGetButton(joys,  6)) pad |= PAD_RIGHT;
		if(SDL_JoystickGetButton(joys,  7)) pad |= PAD_UP | PAD_RIGHT;
		if(SDL_JoystickGetButton(joys, 12)) pad |= PAD_BUTTON1;
		if(SDL_JoystickGetButton(joys, 15)) pad |= PAD_BUTTON2;
		if(SDL_JoystickGetButton(joys, 14)) pad |= PAD_BUTTON3;
		if(SDL_JoystickGetButton(joys, 13)) pad |= PAD_BUTTON4;
		if(SDL_JoystickGetButton(joys, 11)) pad |= PAD_BUTTON5;
		if(SDL_JoystickGetButton(joys, 10)) pad |= PAD_BUTTON6;
		if(SDL_JoystickGetButton(joys, 17)) pad |= PAD_BUTTON7;
		if(SDL_JoystickGetButton(joys, 16)) pad |= PAD_BUTTON8;
		if(SDL_JoystickGetButton(joys,  9)) pad |= PAD_BUTTON9;
		if(SDL_JoystickGetButton(joys,  8)) pad |= PAD_BUTTONA;
		if(SDL_JoystickGetButton(joys, 18)) pad |= PAD_BUTTONB;
	}
#else
	int x = 0, y = 0;
	int pad = 0;

	keys = SDL_GetKeyState(NULL);

	if(joys){
		x = SDL_JoystickGetAxis(joys, 0);
		y = SDL_JoystickGetAxis(joys, 1);
	}
	if(pad_type == 0){
		if(keys[SDLK_RIGHT] == SDL_PRESSED || keys[SDLK_KP6] == SDL_PRESSED || x > JOYSTICK_AXIS){
			pad |= PAD_RIGHT;
		}
		if(keys[SDLK_LEFT] == SDL_PRESSED || keys[SDLK_KP4] == SDL_PRESSED || x < -JOYSTICK_AXIS){
			pad |= PAD_LEFT;
		}
		if(keys[SDLK_DOWN] == SDL_PRESSED || keys[SDLK_KP2] == SDL_PRESSED || y > JOYSTICK_AXIS){
			pad |= PAD_DOWN;
		}
		if(keys[SDLK_UP] == SDL_PRESSED || keys[SDLK_KP8] == SDL_PRESSED || y < -JOYSTICK_AXIS){
			pad |= PAD_UP;
		}
	}
	if(pad_type == 1){
		if(keys[SDLK_d] == SDL_PRESSED || keys[SDLK_KP6] == SDL_PRESSED || x > JOYSTICK_AXIS){
			pad |= PAD_RIGHT;
		}
		if(keys[SDLK_a] == SDL_PRESSED || keys[SDLK_KP4] == SDL_PRESSED || x < -JOYSTICK_AXIS){
			pad |= PAD_LEFT;
		}
		if(keys[SDLK_s] == SDL_PRESSED || keys[SDLK_KP2] == SDL_PRESSED || y > JOYSTICK_AXIS){
			pad |= PAD_DOWN;
		}
		if(keys[SDLK_w] == SDL_PRESSED || keys[SDLK_KP8] == SDL_PRESSED || y < -JOYSTICK_AXIS){
			pad |= PAD_UP;
		}
	}

	int	btn1 = 0, btn2 = 0, btn3 = 0, btn4 = 0, btn5 = 0, btn6 = 0, btn7 = 0, btn8 = 0, btn9 = 0, btnA = 0;

	if(joys){
		btn1 = SDL_JoystickGetButton(joys, 0);
		btn2 = SDL_JoystickGetButton(joys, 1);
		btn3 = SDL_JoystickGetButton(joys, 2);
		btn4 = SDL_JoystickGetButton(joys, 3);
		btn5 = SDL_JoystickGetButton(joys, 4);
		btn6 = SDL_JoystickGetButton(joys, 5);
		btn7 = SDL_JoystickGetButton(joys, 6);
		btn8 = SDL_JoystickGetButton(joys, 7);
		btn9 = SDL_JoystickGetButton(joys, 8);
		btnA = SDL_JoystickGetButton(joys, 9);
	}
	if(pad_type == 0){
		if(keys[SDLK_LCTRL] == SDL_PRESSED || btn1){
			pad |= PAD_BUTTON1;
		}
		if(keys[SDLK_LALT] == SDL_PRESSED || btn2){
			pad |= PAD_BUTTON2;
		}
		if(keys[SDLK_LSHIFT] == SDL_PRESSED || btn3){
			pad |= PAD_BUTTON3;
		}
		if(keys[SDLK_SPACE] == SDL_PRESSED || btn4){
			pad |= PAD_BUTTON4;
		}
	}
	if(pad_type == 1){
		if(keys[SDLK_BACKSLASH] == SDL_PRESSED || btn1){
			pad |= PAD_BUTTON1;
		}
		if(keys[SDLK_RSHIFT] == SDL_PRESSED || btn2){
			pad |= PAD_BUTTON2;
		}
		if(keys[SDLK_p] == SDL_PRESSED || btn3){
			pad |= PAD_BUTTON3;
		}
		if(keys[SDLK_o] == SDL_PRESSED || btn4){
			pad |= PAD_BUTTON4;
		}
	}
	//音量
	if(keys[SDLK_F1] == SDL_PRESSED || btn7){
		pad |= PAD_BUTTON7;
	}
	if(keys[SDLK_F2] == SDL_PRESSED || btn8){
		pad |= PAD_BUTTON8;
	}

	if(btn4){
		pad |= PAD_BUTTON4;
	}
	if(btn5){
		pad |= PAD_BUTTON5;
	}
	if(btn6){
		pad |= PAD_BUTTON6;
	}
//	if(btn7){
//		pad |= PAD_BUTTON7;
//	}
//	if(btn8){
//		pad |= PAD_BUTTON8;
//	}
	if(btn9){
		pad |= PAD_BUTTON9;
	}
	if(btnA){
		pad |= PAD_BUTTONA;
	}
	
	
#endif

	for ( i = 0; i < GP2X_BUTTON_MAX; i++ )
	{
		key_eventPress[i] = 0;
	}
	
	if ( pad & PAD_UP )
	{
		key_eventPress[GP2X_BUTTON_UP] = 1;
	}
	if ( pad & PAD_DOWN )
	{
		key_eventPress[GP2X_BUTTON_DOWN] = 1;
	}
	if ( pad & PAD_LEFT )
	{
		key_eventPress[GP2X_BUTTON_LEFT] = 1;
	}
	if ( pad & PAD_RIGHT )
	{
		key_eventPress[GP2X_BUTTON_RIGHT] = 1;
	}
	if ( pad & PAD_BUTTON1 )
	{
		key_eventPress[GP2X_BUTTON_A] = 1;
	}
	if ( pad & PAD_BUTTON2 )
	{
		key_eventPress[GP2X_BUTTON_X] = 1;
	}
	if ( pad & PAD_BUTTON3 )
	{
		key_eventPress[GP2X_BUTTON_Y] = 1;
	}
	if ( pad & PAD_BUTTON4 )
	{
		key_eventPress[GP2X_BUTTON_B] = 1;
	}
	if ( pad & PAD_BUTTON5 )
	{
		key_eventPress[GP2X_BUTTON_R] = 1;
	}
	if ( pad & PAD_BUTTON6 )
	{
		key_eventPress[GP2X_BUTTON_L] = 1;
	}
	if ( pad & PAD_BUTTON7 )
	{
		key_eventPress[GP2X_BUTTON_VOLDOWN] = 1;
	}
	if ( pad & PAD_BUTTON8 )
	{
		key_eventPress[GP2X_BUTTON_VOLUP] = 1;
	}
	if ( pad & PAD_BUTTON9 )
	{
		key_eventPress[GP2X_BUTTON_SELECT] = 1;
	}
	if ( pad & PAD_BUTTONA )
	{
		key_eventPress[GP2X_BUTTON_START] = 1;
	}
	if ( pad & PAD_BUTTONB )
	{
		key_eventPress[GP2X_BUTTON_CLICK] = 1;
	}
#ifndef GP2X
	if ( keys[SDLK_ESCAPE] )	// 終了
	{
		key_eventPress[GP2X_BUTTON_EXIT] = 1;
	}
#endif
	for ( i = 0; i < GP2X_BUTTON_MAX; i++ )
	{
		if ( ( key_eventPress_old[i] == 0 ) && ( key_eventPress[i] != 0 ) )
		{
			key_eventPush[i] = 1;
		}
		else 
		{
			key_eventPush[i] = 0;
		}
		key_eventPress_old[i] = key_eventPress[i];
		
	}
}

int IsPushKey( int player_num, int keycode )
{
	int rc = 0;
	int chkey = -1;

	switch( player_num )
	{
	default:
	case EN_PLAYERNUM_01:
		switch( keycode )
		{
		case EN_INPUT_UP:
			chkey = EN_SAVEDATA_1P_UP;
			break;
		case EN_INPUT_DOWN:
			chkey = EN_SAVEDATA_1P_DOWN;
			break;
		case EN_INPUT_LEFT:
			chkey = EN_SAVEDATA_1P_LEFT;
			break;
		case EN_INPUT_RIGHT:
			chkey = EN_SAVEDATA_1P_RIGHT;
			break;
		case EN_INPUT_1:
			chkey = EN_SAVEDATA_1P_1;
			break;
		case EN_INPUT_2:
			chkey = EN_SAVEDATA_1P_2;
			break;
		case EN_INPUT_3:
			chkey = EN_SAVEDATA_1P_3;
			break;
		case EN_INPUT_4:
			chkey = EN_SAVEDATA_1P_4;
			break;
		case EN_INPUT_5:
			chkey = EN_SAVEDATA_1P_5;
			break;
		case EN_INPUT_6:
			chkey = EN_SAVEDATA_1P_6;
			break;
		case EN_INPUT_START:
			chkey = EN_SAVEDATA_1P_START;
			break;
		case EN_INPUT_SELECT:
			chkey = EN_SAVEDATA_1P_SELECT;
			break;
		default:
			break;
		}
		break;
	case EN_PLAYERNUM_02:
		switch( keycode )
		{
		case EN_INPUT_UP:
			chkey = EN_SAVEDATA_2P_UP;
			break;
		case EN_INPUT_DOWN:
			chkey = EN_SAVEDATA_2P_DOWN;
			break;
		case EN_INPUT_LEFT:
			chkey = EN_SAVEDATA_2P_LEFT;
			break;
		case EN_INPUT_RIGHT:
			chkey = EN_SAVEDATA_2P_RIGHT;
			break;
		case EN_INPUT_1:
			chkey = EN_SAVEDATA_2P_1;
			break;
		case EN_INPUT_2:
			chkey = EN_SAVEDATA_2P_2;
			break;
		case EN_INPUT_3:
			chkey = EN_SAVEDATA_2P_3;
			break;
		case EN_INPUT_4:
			chkey = EN_SAVEDATA_2P_4;
			break;
		case EN_INPUT_5:
			chkey = EN_SAVEDATA_2P_5;
			break;
		case EN_INPUT_6:
			chkey = EN_SAVEDATA_2P_6;
			break;
		case EN_INPUT_START:
			chkey = EN_SAVEDATA_2P_START;
			break;
		case EN_INPUT_SELECT:
			chkey = EN_SAVEDATA_2P_SELECT;
			break;
		default:
			break;
		}
		break;
	case EN_PLAYERNUM_03:
		switch( keycode )
		{
		case EN_INPUT_UP:
			chkey = EN_SAVEDATA_3P_UP;
			break;
		case EN_INPUT_DOWN:
			chkey = EN_SAVEDATA_3P_DOWN;
			break;
		case EN_INPUT_LEFT:
			chkey = EN_SAVEDATA_3P_LEFT;
			break;
		case EN_INPUT_RIGHT:
			chkey = EN_SAVEDATA_3P_RIGHT;
			break;
		case EN_INPUT_1:
			chkey = EN_SAVEDATA_3P_1;
			break;
		case EN_INPUT_2:
			chkey = EN_SAVEDATA_3P_2;
			break;
		case EN_INPUT_3:
			chkey = EN_SAVEDATA_3P_3;
			break;
		case EN_INPUT_4:
			chkey = EN_SAVEDATA_3P_4;
			break;
		case EN_INPUT_5:
			chkey = EN_SAVEDATA_3P_5;
			break;
		case EN_INPUT_6:
			chkey = EN_SAVEDATA_3P_6;
			break;
		case EN_INPUT_START:
			chkey = EN_SAVEDATA_3P_START;
			break;
		case EN_INPUT_SELECT:
			chkey = EN_SAVEDATA_3P_SELECT;
			break;
		default:
			break;
		}
		break;
	case EN_PLAYERNUM_04:
		switch( keycode )
		{
		case EN_INPUT_UP:
			chkey = EN_SAVEDATA_4P_UP;
			break;
		case EN_INPUT_DOWN:
			chkey = EN_SAVEDATA_4P_DOWN;
			break;
		case EN_INPUT_LEFT:
			chkey = EN_SAVEDATA_4P_LEFT;
			break;
		case EN_INPUT_RIGHT:
			chkey = EN_SAVEDATA_4P_RIGHT;
			break;
		case EN_INPUT_1:
			chkey = EN_SAVEDATA_4P_1;
			break;
		case EN_INPUT_2:
			chkey = EN_SAVEDATA_4P_2;
			break;
		case EN_INPUT_3:
			chkey = EN_SAVEDATA_4P_3;
			break;
		case EN_INPUT_4:
			chkey = EN_SAVEDATA_4P_4;
			break;
		case EN_INPUT_5:
			chkey = EN_SAVEDATA_4P_5;
			break;
		case EN_INPUT_6:
			chkey = EN_SAVEDATA_4P_6;
			break;
		case EN_INPUT_START:
			chkey = EN_SAVEDATA_4P_START;
			break;
		case EN_INPUT_SELECT:
			chkey = EN_SAVEDATA_4P_SELECT;
			break;
		default:
			break;
		}
		break;
	}
	if ( ( key_eventPush[gameflag[chkey]] == 1 ) && ( chkey != -1 ) )
	{
		rc = 1;
	}

	return( rc );
}

int IsPressKey( int player_num, int keycode )
{
	int rc = 0;
	int chkey = -1;

	switch( player_num )
	{
	default:
	case EN_PLAYERNUM_01:
		switch( keycode )
		{
		case EN_INPUT_UP:
			chkey = EN_SAVEDATA_1P_UP;
			break;
		case EN_INPUT_DOWN:
			chkey = EN_SAVEDATA_1P_DOWN;
			break;
		case EN_INPUT_LEFT:
			chkey = EN_SAVEDATA_1P_LEFT;
			break;
		case EN_INPUT_RIGHT:
			chkey = EN_SAVEDATA_1P_RIGHT;
			break;
		case EN_INPUT_1:
			chkey = EN_SAVEDATA_1P_1;
			break;
		case EN_INPUT_2:
			chkey = EN_SAVEDATA_1P_2;
			break;
		case EN_INPUT_3:
			chkey = EN_SAVEDATA_1P_3;
			break;
		case EN_INPUT_4:
			chkey = EN_SAVEDATA_1P_4;
			break;
		case EN_INPUT_5:
			chkey = EN_SAVEDATA_1P_5;
			break;
		case EN_INPUT_6:
			chkey = EN_SAVEDATA_1P_6;
			break;
		case EN_INPUT_START:
			chkey = EN_SAVEDATA_1P_START;
			break;
		case EN_INPUT_SELECT:
			chkey = EN_SAVEDATA_1P_SELECT;
			break;
		default:
			break;
		}
		break;
	case EN_PLAYERNUM_02:
		switch( keycode )
		{
		case EN_INPUT_UP:
			chkey = EN_SAVEDATA_2P_UP;
			break;
		case EN_INPUT_DOWN:
			chkey = EN_SAVEDATA_2P_DOWN;
			break;
		case EN_INPUT_LEFT:
			chkey = EN_SAVEDATA_2P_LEFT;
			break;
		case EN_INPUT_RIGHT:
			chkey = EN_SAVEDATA_2P_RIGHT;
			break;
		case EN_INPUT_1:
			chkey = EN_SAVEDATA_2P_1;
			break;
		case EN_INPUT_2:
			chkey = EN_SAVEDATA_2P_2;
			break;
		case EN_INPUT_3:
			chkey = EN_SAVEDATA_2P_3;
			break;
		case EN_INPUT_4:
			chkey = EN_SAVEDATA_2P_4;
			break;
		case EN_INPUT_5:
			chkey = EN_SAVEDATA_2P_5;
			break;
		case EN_INPUT_6:
			chkey = EN_SAVEDATA_2P_6;
			break;
		case EN_INPUT_START:
			chkey = EN_SAVEDATA_2P_START;
			break;
		case EN_INPUT_SELECT:
			chkey = EN_SAVEDATA_2P_SELECT;
			break;
		default:
			break;
		}
		break;
	case EN_PLAYERNUM_03:
		switch( keycode )
		{
		case EN_INPUT_UP:
			chkey = EN_SAVEDATA_3P_UP;
			break;
		case EN_INPUT_DOWN:
			chkey = EN_SAVEDATA_3P_DOWN;
			break;
		case EN_INPUT_LEFT:
			chkey = EN_SAVEDATA_3P_LEFT;
			break;
		case EN_INPUT_RIGHT:
			chkey = EN_SAVEDATA_3P_RIGHT;
			break;
		case EN_INPUT_1:
			chkey = EN_SAVEDATA_3P_1;
			break;
		case EN_INPUT_2:
			chkey = EN_SAVEDATA_3P_2;
			break;
		case EN_INPUT_3:
			chkey = EN_SAVEDATA_3P_3;
			break;
		case EN_INPUT_4:
			chkey = EN_SAVEDATA_3P_4;
			break;
		case EN_INPUT_5:
			chkey = EN_SAVEDATA_3P_5;
			break;
		case EN_INPUT_6:
			chkey = EN_SAVEDATA_3P_6;
			break;
		case EN_INPUT_START:
			chkey = EN_SAVEDATA_3P_START;
			break;
		case EN_INPUT_SELECT:
			chkey = EN_SAVEDATA_3P_SELECT;
			break;
		default:
			break;
		}
		break;
	case EN_PLAYERNUM_04:
		switch( keycode )
		{
		case EN_INPUT_UP:
			chkey = EN_SAVEDATA_4P_UP;
			break;
		case EN_INPUT_DOWN:
			chkey = EN_SAVEDATA_4P_DOWN;
			break;
		case EN_INPUT_LEFT:
			chkey = EN_SAVEDATA_4P_LEFT;
			break;
		case EN_INPUT_RIGHT:
			chkey = EN_SAVEDATA_4P_RIGHT;
			break;
		case EN_INPUT_1:
			chkey = EN_SAVEDATA_4P_1;
			break;
		case EN_INPUT_2:
			chkey = EN_SAVEDATA_4P_2;
			break;
		case EN_INPUT_3:
			chkey = EN_SAVEDATA_4P_3;
			break;
		case EN_INPUT_4:
			chkey = EN_SAVEDATA_4P_4;
			break;
		case EN_INPUT_5:
			chkey = EN_SAVEDATA_4P_5;
			break;
		case EN_INPUT_6:
			chkey = EN_SAVEDATA_4P_6;
			break;
		case EN_INPUT_START:
			chkey = EN_SAVEDATA_4P_START;
			break;
		case EN_INPUT_SELECT:
			chkey = EN_SAVEDATA_4P_SELECT;
			break;
		default:
			break;
		}
		break;
	}
	
	if ( ( key_eventPress[gameflag[chkey]] == 1 ) && ( chkey != -1 ) )
	{
		rc = 1;
	}

	return( rc );
}

int IsPushOKKey( void )
{
	int rc = 0;
#ifdef GP2X
	if ( key_eventPush[GP2X_BUTTON_B] == 1 )
#else
	if ( key_eventPush[GP2X_BUTTON_A] == 1 )
#endif
	{
		rc = 1;
	}

	return( rc );
}
int IsPushCancelKey( void )
{
	int rc = 0;
	
#ifdef GP2X
	if ( key_eventPush[GP2X_BUTTON_X] == 1 )
#else
	if ( key_eventPush[GP2X_BUTTON_X] == 1 )
#endif
	{
		rc = 1;
	}

	return( rc );
}

void FPSWait( void )
{
	int interval = INTERVAL_BASE;
	Uint32 leftTick;

	//サウンドの制御
	soundPlayCtrl( );

	SDL_PollEvent(&event);
/*
	nowTick = SDL_GetTicks();
	frame = (nowTick - prvTickCount) / interval;
	if(frame <= 0){
#ifdef GP2X
		wait(prvTickCount + interval - nowTick);
#else
		SDL_Delay(prvTickCount + interval - nowTick);
#endif
	}
	prvTickCount = SDL_GetTicks();
*/

	if(prvTickCount == 0) prvTickCount = SDL_GetTicks();
	
	while (true)
	{
	 	nowTick = SDL_GetTicks();
 		leftTick = prvTickCount + interval - nowTick;
 		if(leftTick < 1 || leftTick > 9999)
 		{
			break;
		}
#ifdef GP2X
 	 	wait(1);
#else
		SDL_Delay(1);
#endif
	}
	prvTickCount = nowTick;

	gameflag[EN_SAVEDATA_PLAYTIME_F] = gameflag[EN_SAVEDATA_PLAYTIME_F] + 1;
	if ( gameflag[EN_SAVEDATA_PLAYTIME_F] >= 60 )
	{
		gameflag[EN_SAVEDATA_PLAYTIME_F] = 0;
		gameflag[EN_SAVEDATA_PLAYTIME_S] = gameflag[EN_SAVEDATA_PLAYTIME_S] + 1;
		if ( gameflag[EN_SAVEDATA_PLAYTIME_S] >= 60 )
		{
			gameflag[EN_SAVEDATA_PLAYTIME_S] = 0;
			gameflag[EN_SAVEDATA_PLAYTIME_M] = gameflag[EN_SAVEDATA_PLAYTIME_M] + 1;
			if ( gameflag[EN_SAVEDATA_PLAYTIME_M] >= 60 )
			{
				gameflag[EN_SAVEDATA_PLAYTIME_M] = 0;
				gameflag[EN_SAVEDATA_PLAYTIME_H] = gameflag[EN_SAVEDATA_PLAYTIME_H] + 1;
				if ( gameflag[EN_SAVEDATA_PLAYTIME_H] >= 999 )
				{
					gameflag[EN_SAVEDATA_PLAYTIME_H] = 999;
				}
			}
		}
	}

}

int GetIntCnt( void )
{
	return ( g_intcnt );
}

int system_keys( void )
{
	int rc;
	
	g_intcnt++;
	
	rc = 1;
#ifdef GP2X
	// 終了
//	if ( ( IsPressKey( GP2X_BUTTON_START ) ) && ( IsPressKey( GP2X_BUTTON_SELECT ) ) )
	if ( ( IsPressKey( EN_PLAYERNUM_01, GP2X_BUTTON_START ) ) && ( IsPressKey( EN_PLAYERNUM_01, GP2X_BUTTON_L ) ) && ( IsPressKey( EN_PLAYERNUM_01, GP2X_BUTTON_R ) ) )
#else
	if ( ( event.type == SDL_QUIT ) || ( key_eventPress[GP2X_BUTTON_EXIT] != 0 ) )
#endif
	{
		rc = 0;
		SetScene( EN_SN_EXIT );
	}
	// 音量調整
	if ( key_eventPush[GP2X_BUTTON_VOLUP] != 0 )
	{
		gameflag[EN_SAVEDATA_MASTERVOLUME] = gameflag[EN_SAVEDATA_MASTERVOLUME] + 10;
		if ( gameflag[EN_SAVEDATA_MASTERVOLUME] > VOL_MAX )
		{
			gameflag[EN_SAVEDATA_MASTERVOLUME] = VOL_MAX;
		}
		Set_Volume( gameflag[EN_SAVEDATA_MASTERVOLUME] );
	}
	if ( key_eventPush[GP2X_BUTTON_VOLDOWN] != 0 )
	{
		gameflag[EN_SAVEDATA_MASTERVOLUME] = gameflag[EN_SAVEDATA_MASTERVOLUME] - 10;
		if ( gameflag[EN_SAVEDATA_MASTERVOLUME] < 0 )
		{
			gameflag[EN_SAVEDATA_MASTERVOLUME] = 0;
		}
		Set_Volume( gameflag[EN_SAVEDATA_MASTERVOLUME] );
	}
	
	//デバッグ用キー操作
	if ( g_DebugKey != 0 )
	{
		keys = SDL_GetKeyState(NULL);
		if(keys[SDLK_q] == SDL_PRESSED )
		{
			g_DebugKeyStat = EN_DEBUGFUNC_PAUSE;
			if(keys[SDLK_w] == SDL_PRESSED )
			{
				g_DebugKeyStat = EN_DEBUGFUNC_SLOW;
			}
		}
		else
		{
			if(keys[SDLK_w] == SDL_PRESSED )
			{
				g_DebugKeyStat = EN_DEBUGFUNC_NONE;
			}
		}
		if(keys[SDLK_e] == SDL_PRESSED )
		{
			g_DebugKeyStat = EN_DEBUGFUNC_FAST1;
		}
		else
		{
			if ( g_DebugKeyStat == EN_DEBUGFUNC_FAST1 )
			{
				g_DebugKeyStat = EN_DEBUGFUNC_NONE;
			}
		}
		if(keys[SDLK_r] == SDL_PRESSED )
		{
			SetScene( EN_SN_TITLE );
		}
	}
	
	
	return( rc );
}
int Set_Volume( int vol )
{
	int rc = 0;
	
	soundSetVolumeBgm( vol, 0 );
	soundSetVolumeAll( vol );
	
	return( rc );
}

/*[ BltNumericImage ]************************************************/
/*	数値画像表示関数												*/
/*-[引数]-----------------------------------------------------------*/
/*	value			(i )	画像表示する数値						*/
/*	length			(i )	表示する桁数（桁数以上は表示されない）	*/
/*	x				(i )	画像を表示する位置ｘ座標				*/
/*	y				(i )	画像を表示する位置ｙ座標				*/
/*	plane			(i )	数値画像が読み込まれているプレーン番号	*/
/*	num_stpos_x		(i )	プレーン内での数値画像の開始位置ｘ座標	*/
/*	num_stpos_y		(i )	プレーン内での数値画像の開始位置ｙ座標	*/
/*	num_width		(i )	数値画像１文字の幅（ドット数）			*/
/*	num_height		(i )	数値画像１文字の高さ（ドット数）		*/
/*-[戻り値]---------------------------------------------------------*/
/*	無し															*/
/********************************************************************/
void DrawNumer( int32_t value, int32_t length, int32_t x, int32_t y, int32_t zero, int32_t center )
{
	int len;
	len = length;
	CODEDATA rCodet = g_CharCodeData[ C_FONTS3 ];
	
	if ( center != 0 )
	{
		//中ぞろえ
		x -= ( len * 18 ) / 2;
	}
	y -= 16 / 2;
	BltNumericImage( value, 9, x, y, rCodet.m_ResID, rCodet.m_PosX, rCodet.m_PosY + 96, 32, 32, 18, zero );
}

void BltNumericImage( int32_t value, int32_t length, int32_t x, int32_t y, int32_t plane, int32_t num_stpos_x, int32_t num_stpos_y, int32_t num_width, int32_t num_height, int32_t width_offset, int32_t zero )
{
	int32_t blt_num;	// １桁の数値を格納する
	int32_t i;			// 桁数分のforループで使用
	int32_t dv;		// 割り算で使用する値

	// value が負の値の場合、正の値に置き換える
	if ( value < 0 )
	{
		value = value * -1;
	}

	// 最初の割り算で使用する値を求める
	dv = 1;
	for( i = 1; i < length; i++ )
	{
		dv = dv * 10;
	}

	// 指定された桁数分の数字画像を転送する
	for( i = 0; i < length; i++ )
	{
		// 表示する数字を求める
		blt_num = value / dv;
		value = value - blt_num * dv;
		if ( blt_num > 9 )
		{	// 表示したい１桁の数値にならなければ、１桁にする。
			blt_num = blt_num % 10;
		}
		
		if ( ( blt_num != 0 ) || ( dv == 1 ) )
		{
			zero = 1;
		}
		
		// 数字画像転送
		if ( zero == 1 )
		{
			BltRect( plane, x, y, num_stpos_x + (num_width * blt_num), num_stpos_y, num_width, num_height );
			x = x + width_offset;
		}
		// 割り算で使用する値を10で割る
		dv = dv / 10;
	}

	return;
}

int32_t funcSin( int32_t rdo )
{
	int32_t ang = 0;
	int32_t rc = 0;
	
	if ( rdo < 0 )
	{
		rdo = rdo * -1;		//正にする
		rdo = rdo % FUNC_MAX_DIR;	//360度に置き換える
		rdo = FUNC_MAX_DIR - rdo;	//正の角度で設定
	}
	else 
	{
		rdo = rdo % FUNC_MAX_DIR;
	}
	if ( ( rdo >= 0 ) && ( rdo < FUNC_MAX_DIR ) )
	{
		ang = ( ( 65535 * rdo ) / FUNC_MAX_DIR );
		rc = MOTsin( ang );
	}
	if ( ( rdo >= FUNC_MAX_DIR ) && ( rdo < FUNC_MAX_DIR ) )
	{
		rdo = rdo - FUNC_MAX_DIR;
		ang = ( ( 65535 * rdo ) / FUNC_MAX_DIR );
		rc = MOTsin( ang );
		rc = rc * -1;
	}
	
	return( rc );
}
int32_t funcCos( int32_t rdo )
{
	int32_t ang = 0;
	int32_t rc = 0;
	
	if ( rdo < 0 )
	{
		rdo = rdo * -1;		//正にする
		rdo = rdo % FUNC_MAX_DIR;	//360度に置き換える
		rdo = FUNC_MAX_DIR - rdo;	//正の角度で設定
	}
	else 
	{
		rdo = rdo % FUNC_MAX_DIR;
	}
	if ( ( rdo >= 0 ) && ( rdo < FUNC_MAX_DIR ) )
	{
		ang = ( ( 65535 * rdo ) / FUNC_MAX_DIR );
		rc = MOTcos( ang );
	}
	if ( ( rdo >= FUNC_MAX_DIR ) && ( rdo < FUNC_MAX_DIR ) )
	{
		rdo = rdo - FUNC_MAX_DIR;
		ang = ( ( 65535 * rdo ) / FUNC_MAX_DIR );
		rc = MOTcos( ang );
		rc = rc * -1;
	}
	rc = rc * -1;
	
	return( rc );
}


int32_t funcTan2( int32_t posX, int32_t posY )
{
	int32_t rc = 0;
	
	rc = MOTatan( ( posX ) * 256, ( posY * -1 ) * 256 );
	rc = rc * -1;
	rc = rc / 64;
	if ( rc < 0 )
	{
		rc = 512 + ( 512 + rc );
	}
	
	return( rc );
}

int32_t funcRand( int32_t max )
{
	int32_t rc = 0;

	max = abs( max );
	if ( max == 0 )
	{
		rc = 0;
	}
	else
	{
		rc = rand( ) % max;
	}
	
	return( rc );
}
int32_t get2keta( int32_t val, int32_t st )
{
	int32_t rc = 0;
	
	val = val / st;
	rc = val % 100;
	
	return( rc );
}

int32_t getKyori( int32_t val1, int32_t val2 )
{
	int32_t rc = 0;
	
	rc = sqrt( ( val1 * val1 ) + ( val2 * val2 ) );
	
	return( rc );
}

void BltGSreface( int rr )
{
	SDL_Surface *temp_Surface;
	
	//temp_Surface = rotozoomSurface(g_screen, 0, rr, 0);
	temp_Surface = g_screen;
	SDL_Surface *surface;
    Uint8 bpp;
	SDL_Palette *pal;
	Uint8 r;
	Uint8 g;
	Uint8 b;

	surface = g_screen;
    if(surface){
	    bpp = surface->format->BytesPerPixel;
		if(bpp <= 8){
			pal = surface->format->palette;
			if(pal){
				SDL_SetPalette(temp_Surface, SDL_LOGPAL|SDL_PHYSPAL, pal->colors, 0, 256);
				r = pal->colors->r;
				g = pal->colors->g;
				b = pal->colors->b;
				// パレット０番が透過色
				SDL_SetColorKey(temp_Surface, false, SDL_MapRGBA(g_screen->format,r,g,b,0) );
			}
		}
	}
	
//	SDL_SaveBMP(SDL_DisplayFormat(temp_Surface), "tempbmp.bmp");
	
	
	SDL_Rect srcRect;
	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = temp_Surface->w;
	srcRect.h = temp_Surface->h;

	SDL_Rect dstRect;
//	dstRect.x = 320 - ( temp_Surface->w / 2 );
//	dstRect.y = 240 - ( temp_Surface->h / 2 );
	dstRect.x = 0;
	dstRect.y = 0;
	dstRect.w = temp_Surface->w;
	dstRect.h = temp_Surface->h;


	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = DISPLY_WIDTH * rr;
	rect.h = DISPLY_HEIGHT * rr;
	SDL_FillRect(g_screen, &rect, SDL_MapRGBA(g_screen->format,0,0,0,255));

	SDL_BlitSurface( temp_Surface , &srcRect, g_screen, &dstRect);
	SDL_FreeSurface(temp_Surface);

}

void halt( void )
{
	if ( gameflag[61] > 1 )
	{
		BltGSreface( gameflag[61] );	//拡大表示
	}
	SDL_Flip( g_screen );	// 描画
}


