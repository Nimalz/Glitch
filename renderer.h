#pragma once

#include "main.h"

//*********************************************************
// �\����
//*********************************************************

// ���_�\����
struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXCOLOR  Diffuse;
    D3DXVECTOR2 TexCoord;
};


// �}�e���A���\����
struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];//bit���E�p
};


// �����p�\����
struct RANDOM
{
	float		minValue;
	float		maxValue;
	float		time;
	float		Dummy;
};

// Glitch�p�\����
struct GLITCH
{
	//================
	// RGB�J���[�V�t�g
	//================
	// �U���̋���
	float shake_power;
	// �U����
	float shake_rate;
	// �U�����x
	float shake_speed;
	// �U���u���b�N�T�C�Y
	float shake_block_size;
	// �F�̕�����
	float shake_color_rate;

	// �U���g�p���邩
	float shake_enable;

	//================
	// �m�C�Y����
	//================
	// �m�C�Y�g�p���邩
	float grain_enable;

	//================
	// ������
	//================
	// �g�p���邩
	float scanline_enbale;

	float scanline_opacity;

	float scanline_noise;



	//================
	// �_�Ō���
	//================
	float flick_enable;

	float flick_speed;

	float flick_str;

	float dummy[3];
};
//*****************************************************************************
// �v���g�^�C�v�錾
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

