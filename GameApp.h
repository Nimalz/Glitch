#pragma once
#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
#include <d3dcompiler.h>
#include "Vertex.h"

class GameApp : public D3DApp
{
public:
    struct WVPBuffer
    {
        DirectX::XMMATRIX WVP;

    };

    // �����p�\����
    struct RandomBuffer
    {
        float		minValue;
        float		maxValue;
        float		time;
        float		Dummy;
    };

    // Glitch�p�\����
    struct GlitchBuffer
    {
        // �U���̋���
        float shake_power;
        // �U���m��
        float shake_rate;
        // �U�����x
        float shake_speed;
        // �U���u���b�N�T�C�Y(������)
        float shake_block_size;
        // �F�̕�����
        float shake_color_rate;

        // UV�V�t�g���邩
        float shake_enable;

        // RGBV�V�t�g���邩
        float rgb_shift;

        // �m�C�Y�g�p���邩
        float grain_enable;

        float grain_blockSize;

        float grain_alpha;

        //������
        float scanline_enbale;

        float scanline_opacity;

        float scanline_noise;

        //�_��
        float flick_enable;

        float flick_speed;

        float flick_str;

    };

public:
    GameApp(HINSTANCE hInstance, const std::string& windowName, int initWidth, int initHeight);
    ~GameApp();

    bool Init();
    void OnResize();
    void UpdateScene(float dt);
    void DrawScene();


private:
    bool InitEffect();
    bool InitResource();

    void DrawImGui();

// ------------------------------
// CreateShaderFromFile
// ------------------------------
// [In]csoFileNameInOut �v���R���p�C���̃o�C�i���t�@�C��(.cso)��ǂݍ���
// [In]hlslFileName     �v���R���p�C���̃t�@�C�����Ȃ��ꍇ�̓V�F�[�_�[�t�@�C������
// [In]entryPoint       �G���g���[�|�C���g
// [In]shaderModel      �V�F�[�_�[���f���i��ށ{�o�[�W�����j
// [Out]ppBlobOut       �A�o�pBlob
    HRESULT CreateShaderFromFile(
        const WCHAR* csoFileNameInOut,
        const WCHAR* hlslFileName,
        LPCSTR entryPoint,
        LPCSTR shaderModel,
        ID3DBlob** ppBlobOut);

    wchar_t* ReadFileDiectory();

    void ResizeRect();
private:
    ComPtr<ID3D11InputLayout> m_pVertexLayout;	//���̓��C�A�E�g
    ComPtr<ID3D11Buffer> m_pVertexBuffer;		//���_�o�b�t�@
    ComPtr<ID3D11VertexShader> m_pVertexShader;	//VS
    ComPtr<ID3D11PixelShader> m_pPixelShader;	//PS

    ComPtr<ID3D11Buffer> m_pWVPBuffer;          //WVP�p�萔�o�b�t�@
    ComPtr<ID3D11Buffer> m_pRandomBuffer;       //�����p�萔�o�b�t�@
    ComPtr<ID3D11Buffer> m_pGlitchBuffer;       //�O���b�`�֘A�萔�o�b�t�@

    ComPtr<ID3D11SamplerState> m_pSamplerState; //�T���v���[���
    //�e�N�X�`��
    ComPtr<ID3D11ShaderResourceView> m_pImage;
    //�萔�o�b�t�@�p
    RandomBuffer random;
    GlitchBuffer glitch;
};


#endif