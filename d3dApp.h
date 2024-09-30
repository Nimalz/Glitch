#pragma once
#ifndef D3DAPP_H
#define D3DAPP_H

#include <wrl/client.h>
#include <string>
#include "WinMin.h"
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "Timer.h"
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_dx11.h>
#include <Imgui/imgui_impl_win32.h>

class D3DApp
{
public:
    D3DApp(HINSTANCE hInstance, const std::string& windowName, int initWidth, int initHeight);
    virtual ~D3DApp();

    HINSTANCE AppInst()const;    //インスタンス取得              
    HWND      MainWnd()const;    //ハンドル取得              
    float     AspectRatio()const;//画面アスペクト比取得              

    int Run();                                 

    //インターフェース
    virtual bool Init();                       
    virtual void OnResize();                   
    virtual void UpdateScene(float dt) = 0;    
    virtual void DrawScene() = 0;              
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
 
protected:
    bool InitMainWindow();       //Windows初期化
    bool InitDirect3D();         //D3D11初期化
    
    void CalculateFrameStats();  //fps計算
    bool InitImGui();            //ImGui初始化

protected:

    HINSTANCE m_hAppInst;        //インスタンス
    HWND      m_hMainWnd;        //ハンドル
    bool      m_AppPaused;       //中止中？
    bool      m_Minimized;       //最小化？
    bool      m_Maximized;       //最大化？
    bool      m_Resizing;        //サイズ調整中？
    bool      m_Enable4xMsaa;    //MSAA使用？
    UINT      m_4xMsaaQuality;   //MSAAクオリティ

    Timer m_Timer;               //タイマー


    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
    // Direct3D 11
    ComPtr<ID3D11Device> m_pd3dDevice;                   
    ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;  
    ComPtr<IDXGISwapChain> m_pSwapChain;          

    // Direct3D 11.1
    ComPtr<ID3D11Device1> m_pd3dDevice1;                  
    ComPtr<ID3D11DeviceContext1> m_pd3dImmediateContext1; 
    ComPtr<IDXGISwapChain1> m_pSwapChain1;                

    ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;       //深度バッファ
    ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;  //PTV
    ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;  //DSV
    D3D11_VIEWPORT m_ScreenViewport;                     //VP

    std::string m_MainWndCaption;                        //Window名
    int m_ClientWidth;                                   //Window幅
    int m_ClientHeight;                                  //Window高

};

#endif 