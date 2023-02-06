#ifndef __FUNCTION
#define __FUNCTION

#include "include/general.h"
#include "charcode/charcode.h"

extern char basicpath[128];
extern char configPath[128];
extern char homepath[128];

extern void SetSaveDate( int index, int data );
extern int GetSaveDate( int index );
extern void AddSaveDate( int index, int data );
extern void SetScene( int Scene );
extern void ResetGameFlag( void );
extern void ResetGameFlag2( void );
extern int LoadGameFlag( char *fn );
extern int SaveGameFlag( char *fn );
extern int LoadGameFlag2( char *fn );
extern int SaveGameFlag2( char *fn );
extern int SaveFile( char *fn, int32_t *buff, int32_t size );
extern int LoadFile( char *fn, int32_t *buff, int32_t size );
extern int32_t GetConfig( char* fn, char* cParam );
extern int32_t LogFileWrite( char* fn, char* cParam );

extern void SetColorKey(int bmpindex, int flag );
extern int32_t LoadBitmap( char *fname, int bmpindex, int flag );
extern void ReleaseBitmap( int bmpindex );
extern void Blt( int bmpindex, int dstX, int dstY );
extern void BltRect(int bmpindex, int srcX, int srcY, int dstX, int dstY, int width, int height);
extern void BltFunction( int bmpindex, int dstx, int dsty, int TexPosX, int TexPosY, int Width, int Height, float zoomx, float zoomy, int func, int r, int g, int b, int a, int rot, int pri, int flip );
extern void BltFunctionEx( int bmpindex, int dstx, int dsty, int TexPosX, int TexPosY, int Width, int Height, float zoomx, float zoomy, int func, int r, int g, int b, int a, int rot, int pri, int flip );
extern void BltOffset( int offset_x, int offset_y );
extern void SetObjSetClip( int flg );
extern void ClearSecondary( void );
extern void DrawNumer( int32_t value, int32_t length, int32_t x, int32_t y, int32_t center, int32_t zero );
extern void BltNumericImage( int32_t value, int32_t length, int32_t x, int32_t y, int32_t plane, int32_t num_stpos_x, int32_t num_stpos_y, int32_t num_width, int32_t num_height, int32_t width_offset, int32_t zero );
extern void SetGscreenPalette( SDL_Surface *surface );
extern void SetPalette(int getbmpindex, int setbmpindex);
extern void BltRectRotZoom( int bmpindex, int dstX, int dstY, int srcX, int srcY, int width, int height, double angle, double zoom, int smooth);
extern void CreateSurface( int bmpindex, int size_x, int size_y  );
extern void SwapToSecondary( int bmpindex );
extern void GScreenToSecondary( int bmpindex );
extern void SaveBmp( int bmpindex, char *fn );
extern void drawGRPline(f32 x1, f32 y1, f32 x2, f32 y2, Uint32 color);
extern void BltGSreface( int rr );
extern void halt( void );
extern void BltFunctionBGR(int index, int srcX, int srcY, int dstX, int dstY, int width, int height, int ScaleX, int ScaleY, int func, int blend, int flip, int rot, UINT32 bgr );
extern void ObjSetEx( UINT32 charcode, INT32 pri, INT32 dstx, INT32 dsty, float zoomx, float zoomy, INT32 func, INT32 r, INT32 g, INT32 b, INT32 a, float rot, INT32 flip, INT32 center, INT32 w, INT32 h );
extern void DarwFont( char *value, int32_t x, int32_t y, int32_t center );
extern void SetMozaiku( int size );
extern void BltMozaiku(SDL_Surface *s_surf, SDL_Surface *d_surf, Sint16 osx, Sint16 osy, Sint16 sx, Sint16 sy, Sint16 ex, Sint16 ey, int size );

extern int IsPushKey( int player_num, int keycode );
extern int IsPressKey( int player_num, int keycode );
extern void KeyInput( void );
extern int initPAD(void);
extern void closePAD(void);
extern int getPAD(void);
extern int IsPushOKKey( void );
extern int IsPushCancelKey( void );

extern void FunctionInit( void );
extern void FPSWait( void );
extern int system_keys( void );
extern int Set_Volume( int vol );
extern int GetIntCnt( void );

extern int32_t funcSin( int32_t rdo );
extern int32_t funcCos( int32_t rdo );
extern int32_t funcTan2( int32_t posX, int32_t posY );
extern int32_t get2keta( int32_t val, int32_t st );
extern int32_t funcRand( int32_t max );
extern int32_t getKyori( int32_t val1, int32_t val2 );

#endif
