#include "polygon.h"
#include "input.h"
#include "renderer.h"
#include <chrono>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME	"data/TEXTURE/444.jpeg"	//�e�N�X�`����
#define TEXTURE_NAME2	"data/TEXTURE/333.jpg"	//�e�N�X�`����
#define MAX_TEX (2)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void SetVertex(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[MAX_TEX] = { NULL };			// �e�N�X�`�����

static int						g_TexNo;					// �e�N�X�`���ԍ�

static RANDOM random;
static GLITCH glitch;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPolygon(void)
{
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
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

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// ���_�o�b�t�@�X�V
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
	//�����_���̏�����
	srand(time(NULL));
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPolygon(void)
{
	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	// �e�N�X�`���̉��
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
// �O���b�`�pCB�X�V����
//=============================================================================
void UpdatePolygon(void)
{
	//RGB�J���[�V�t�g
	if (GetKeyboardTrigger(DIK_1))
	{
		if (glitch.shake_enable == 0.0f)
			glitch.shake_enable = 1.0f;
		else
			glitch.shake_enable = 0.0f;
	}

	//�m�C�Y
	if (GetKeyboardTrigger(DIK_2))
	{
		if (glitch.grain_enable == 0.0f)
			glitch.grain_enable = 1.0f;
		else
			glitch.grain_enable = 0.0f;
	}
	//������
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

	//���������p
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
// �`�揈��
//=============================================================================
void DrawPolygon(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �e�N�X�`���ݒ�
	for (int i = 0; i < MAX_TEX; i++)
	{
		GetDeviceContext()->PSSetShaderResources(i, 1, &g_Texture[i]);
	}


	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);
}


//=============================================================================
// ���_�f�[�^�ݒ�
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
