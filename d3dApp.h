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

    HINSTANCE AppInst()const;    //�C���X�^���X�擾              
    HWND      MainWnd()const;    //�n���h���擾              
    float     AspectRatio()const;//��ʃA�X�y�N�g��擾              

    int Run();                                 

    //�C���^�[�t�F�[�X
    virtual bool Init();                       
    virtual void OnResize();                   
    virtual void UpdateScene(float dt) = 0;    
    virtual void DrawScene() = 0;              
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
 
protected:
    bool InitMainWindow();       //Windows������
    bool InitDirect3D();         //D3D11������
    
    void CalculateFrameStats();  //fps�v�Z
    bool InitImGui();            //ImGui���n��

protected:

    HINSTANCE m_hAppInst;        //�C���X�^���X
    HWND      m_hMainWnd;        //�n���h��
    bool      m_AppPaused;       //���~���H
    bool      m_Minimized;       //�ŏ����H
    bool      m_Maximized;       //�ő剻�H
    bool      m_Resizing;        //�T�C�Y�������H
    bool      m_Enable4xMsaa;    //MSAA�g�p�H
    UINT      m_4xMsaaQuality;   //MSAA�N�I���e�B

    Timer m_Timer;               //�^�C�}�[


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

    ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;       //�[�x�o�b�t�@
    ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;  //PTV
    ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;  //DSV
    D3D11_VIEWPORT m_ScreenViewport;                     //VP

    std::string m_MainWndCaption;                        //Window��
    int m_ClientWidth;                                   //Window��
    int m_ClientHeight;                                  //Window��

};

#endif 