#include "polygon.h"
#include "input.h"
#include "renderer.h"
#include <chrono>

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME	"data/TEXTURE/444.jpeg"	//テクスチャ名
#define TEXTURE_NAME2	"data/TEXTURE/333.jpg"	//テクスチャ名
#define MAX_TEX (2)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetVertex(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[MAX_TEX] = { NULL };			// テクスチャ情報

static int						g_TexNo;					// テクスチャ番号

static RANDOM random;
static GLITCH glitch;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPolygon(void)
{
	ID3D11Device* pDevice = GetDevice();

	//テクスチャ生成
	g_Texture[0] = NULL;
	D3DX11CreateShaderResourceViewFromFile(
		GetDevice(),
		TEXTURE_NAME,
		NULL,
		NULL,
		&g_Texture[0],
		NULL);

	g_Texture[1] = NULL;
	D3DX11CreateShaderResourceViewFromFile(
		GetDevice(),
		TEXTURE_NAME2,
		NULL,
		NULL,
		&g_Texture[1],
		NULL);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// 頂点バッファ更新
	SetVertex();


	random.minValue = 0.0f;
	random.maxValue = 1.0f;
	random.time = 0.0f;

	glitch.shake_power = 0.4f;
	glitch.shake_rate = 0.7f;
	glitch.shake_speed = 5.0f;
	glitch.shake_block_size = 300.0f;
	glitch.shake_color_rate = 0.02f;
	glitch.shake_enable = 0.0f;
	glitch.grain_enable = 0.0f;
	glitch.scanline_enbale = 0.0f;
	glitch.scanline_opacity = 0.8f;
	glitch.scanline_noise = 0.1f;
	glitch.flick_enable = 0.0f;
	glitch.flick_speed = 200.0f;
	glitch.flick_str = 0.4f;
	//ランダムの初期化
	srand(time(NULL));
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPolygon(void)
{
	// 頂点バッファの解放
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	// テクスチャの解放
	for (int i = 0; i < MAX_TEX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

}

//=============================================================================
// グリッチ用CB更新処理
//=============================================================================
void UpdatePolygon(void)
{
	//RGBカラーシフト
	if (GetKeyboardTrigger(DIK_1))
	{
		if (glitch.shake_enable == 0.0f)
			glitch.shake_enable = 1.0f;
		else
			glitch.shake_enable = 0.0f;
	}

	//ノイズ
	if (GetKeyboardTrigger(DIK_2))
	{
		if (glitch.grain_enable == 0.0f)
			glitch.grain_enable = 1.0f;
		else
			glitch.grain_enable = 0.0f;
	}
	//走査線
	if (GetKeyboardTrigger(DIK_3))
	{
		if (glitch.scanline_enbale == 0.0f)
			glitch.scanline_enbale = 1.0f;
		else
			glitch.scanline_enbale = 0.0f;
	}
	//
	if (GetKeyboardTrigger(DIK_4))
	{
		if (glitch.flick_enable == 0.0f)
			glitch.flick_enable = 1.0f;
		else
			glitch.flick_enable = 0.0f;
	}

	//乱数生成用
	{
		static auto startTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsed = currentTime - startTime;
		random.time = elapsed.count();
	}
	SetRandom(random);
	SetGlitch(glitch);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPolygon(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// テクスチャ設定
	for (int i = 0; i < MAX_TEX; i++)
	{
		GetDeviceContext()->PSSetShaderResources(i, 1, &g_Texture[i]);
	}


	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}


//=============================================================================
// 頂点データ設定
//=============================================================================
void SetVertex(void)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
}
