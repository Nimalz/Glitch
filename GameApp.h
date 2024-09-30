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

    // 乱数用構造体
    struct RandomBuffer
    {
        float		minValue;
        float		maxValue;
        float		time;
        float		Dummy;
    };

    // Glitch用構造体
    struct GlitchBuffer
    {
        // 振動の強さ
        float shake_power;
        // 振動確率
        float shake_rate;
        // 振動速度
        float shake_speed;
        // 振動ブロックサイズ(分割数)
        float shake_block_size;
        // 色の分離率
        float shake_color_rate;

        // UVシフトするか
        float shake_enable;

        // RGBVシフトするか
        float rgb_shift;

        // ノイズ使用するか
        float grain_enable;

        float grain_blockSize;

        float grain_alpha;

        //走査線
        float scanline_enbale;

        float scanline_opacity;

        float scanline_noise;

        //点滅
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
// [In]csoFileNameInOut プリコンパイルのバイナリファイル(.cso)を読み込む
// [In]hlslFileName     プリコンパイルのファイルがない場合はシェーダーファイルから
// [In]entryPoint       エントリーポイント
// [In]shaderModel      シェーダーモデル（種類＋バージョン）
// [Out]ppBlobOut       輸出用Blob
    HRESULT CreateShaderFromFile(
        const WCHAR* csoFileNameInOut,
        const WCHAR* hlslFileName,
        LPCSTR entryPoint,
        LPCSTR shaderModel,
        ID3DBlob** ppBlobOut);

    wchar_t* ReadFileDiectory();

    void ResizeRect();
private:
    ComPtr<ID3D11InputLayout> m_pVertexLayout;	//入力レイアウト
    ComPtr<ID3D11Buffer> m_pVertexBuffer;		//頂点バッファ
    ComPtr<ID3D11VertexShader> m_pVertexShader;	//VS
    ComPtr<ID3D11PixelShader> m_pPixelShader;	//PS

    ComPtr<ID3D11Buffer> m_pWVPBuffer;          //WVP用定数バッファ
    ComPtr<ID3D11Buffer> m_pRandomBuffer;       //乱数用定数バッファ
    ComPtr<ID3D11Buffer> m_pGlitchBuffer;       //グリッチ関連定数バッファ

    ComPtr<ID3D11SamplerState> m_pSamplerState; //サンプラー状態
    //テクスチャ
    ComPtr<ID3D11ShaderResourceView> m_pImage;
    //定数バッファ用
    RandomBuffer random;
    GlitchBuffer glitch;
};


#endif