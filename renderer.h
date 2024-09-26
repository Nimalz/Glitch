#pragma once

#include "main.h"

//*********************************************************
// 構造体
//*********************************************************

// 頂点構造体
struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXCOLOR  Diffuse;
    D3DXVECTOR2 TexCoord;
};


// マテリアル構造体
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];//bit境界用
};


// 乱数用構造体
struct RANDOM
{
	float		minValue;
	float		maxValue;
	float		time;
	float		Dummy;
};

// Glitch用構造体
struct GLITCH
{
	//================
	// RGBカラーシフト
	//================
	// 振動の強さ
	float shake_power;
	// 振動率
	float shake_rate;
	// 振動速度
	float shake_speed;
	// 振動ブロックサイズ
	float shake_block_size;
	// 色の分離率
	float shake_color_rate;

	// 振動使用するか
	float shake_enable;

	//================
	// ノイズ効果
	//================
	// ノイズ使用するか
	float grain_enable;

	//================
	// 走査線
	//================
	// 使用するか
	float scanline_enbale;

	float scanline_opacity;

	float scanline_noise;



	//================
	// 点滅効果
	//================
	float flick_enable;

	float flick_speed;

	float flick_str;

	float dummy[3];
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void UninitRenderer(void);

void Clear(void);
void Present(void);

ID3D11Device *GetDevice( void );
ID3D11DeviceContext *GetDeviceContext( void );

void SetDepthEnable( bool Enable );

void SetWorldViewProjection2D( void );
void SetWorldMatrix( D3DXMATRIX *WorldMatrix );
void SetViewMatrix( D3DXMATRIX *ViewMatrix );
void SetProjectionMatrix( D3DXMATRIX *ProjectionMatrix );


void SetMaterial(MATERIAL Material);
void SetRandom(RANDOM rand);
void SetGlitch(GLITCH glitch);

