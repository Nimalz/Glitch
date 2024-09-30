#include "GameApp.h"
#include "Vertex.h"
#include "WICTextureLoader.h"
#include <random> 
#include <ctime> 

using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance, const std::string& windowName, int initWidth, int initHeight)
	: D3DApp(hInstance, windowName, initWidth, initHeight)
{
	//Glitch用パラメーターの初期化
	random.minValue = 0.0f;
	random.maxValue = 1.0f;
	random.time = 0.0f;

	glitch.shake_power = 0.4f;
	glitch.shake_rate = 0.7f;
	glitch.shake_speed = 5.0f;
	glitch.shake_block_size = 300.0f;
	glitch.shake_color_rate = 0.02f;
	glitch.shake_enable = 1.0f;
	glitch.rgb_shift = 1.0f;
	glitch.grain_enable = 0.0f;
	glitch.grain_blockSize = 300.0f;
	glitch.grain_alpha = 0.4f;
	glitch.scanline_enbale = 0.0f;
	glitch.scanline_opacity = 0.8f;
	glitch.scanline_noise = 10.0f;
	glitch.flick_enable = 0.0f;
	glitch.flick_speed = 200.0f;
	glitch.flick_str = 0.4f;
}

GameApp::~GameApp()
{
}


bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	if (!InitEffect())
		return false;

	if (!InitResource())
		return false;

	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();

	if (m_pImage)
		ResizeRect();
}

void GameApp::UpdateScene(float dt)
{

	random.time = m_Timer.TotalTime();

	//定数バッファを更新
	XMMATRIX WVP;
	WVP = XMMatrixOrthographicOffCenterLH(0.0f, (float)m_ClientWidth, (float)m_ClientHeight, 0.0f, 0.0f, 1.0f);
	WVP = XMMatrixTranspose(WVP);
	D3D11_MAPPED_SUBRESOURCE mappedData;

	m_pd3dImmediateContext->Map(m_pWVPBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	memcpy_s(mappedData.pData, sizeof(WVPBuffer), &WVP, sizeof(WVPBuffer));
	m_pd3dImmediateContext->Unmap(m_pWVPBuffer.Get(), 0);
	m_pd3dImmediateContext.Get()->UpdateSubresource(m_pRandomBuffer.Get(), 0, nullptr, &random, 0, 0);
	m_pd3dImmediateContext.Get()->UpdateSubresource(m_pGlitchBuffer.Get(), 0, nullptr, &glitch, 0, 0);



}


void GameApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);

	static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//四角を描く
	m_pd3dImmediateContext->Draw(4, 0);

	DrawImGui();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	m_pSwapChain->Present(0, 0);
}

bool GameApp::InitEffect()
{
	ComPtr<ID3DBlob> blob;

	//VSの生成
	CreateShaderFromFile(L"HLSL\\Triangle_VS.cso", L"HLSL\\shader.hlsl", "VertexShaderPolygon", "vs_4_0", blob.ReleaseAndGetAddressOf());
	m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf());

	//入力レイアウトの生成
	m_pd3dDevice->CreateInputLayout(VertexPosTex::inputLayout, ARRAYSIZE(VertexPosTex::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf());

	//PSの生成
	CreateShaderFromFile(L"HLSL\\Triangle_PS.cso", L"HLSL\\shader.hlsl", "PixelShaderPolygon", "ps_4_0", blob.ReleaseAndGetAddressOf());
	m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf());

	return true;

}

bool GameApp::InitResource()
{
	//WVP　CB
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(WVPBuffer);
	hBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	m_pd3dDevice->CreateBuffer(&hBufferDesc, nullptr, m_pWVPBuffer.GetAddressOf());
	m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, m_pWVPBuffer.GetAddressOf());
	m_pd3dImmediateContext->PSSetConstantBuffers(0, 1, m_pWVPBuffer.GetAddressOf());

	//グリッチ用CB
	hBufferDesc.ByteWidth = sizeof(RandomBuffer);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	m_pd3dDevice->CreateBuffer(&hBufferDesc, nullptr, m_pRandomBuffer.GetAddressOf());
	m_pd3dImmediateContext->VSSetConstantBuffers(1, 1, m_pRandomBuffer.GetAddressOf());
	m_pd3dImmediateContext->PSSetConstantBuffers(1, 1, m_pRandomBuffer.GetAddressOf());

	//乱数CB
	hBufferDesc.ByteWidth = sizeof(GlitchBuffer);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	m_pd3dDevice->CreateBuffer(&hBufferDesc, nullptr, m_pGlitchBuffer.GetAddressOf());
	m_pd3dImmediateContext->VSSetConstantBuffers(2, 1, m_pGlitchBuffer.GetAddressOf());
	m_pd3dImmediateContext->PSSetConstantBuffers(2, 1, m_pGlitchBuffer.GetAddressOf());

	// サンプラーステート設定
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.MipLODBias = 0;
	sampDesc.MaxAnisotropy = 16;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	m_pd3dDevice->CreateSamplerState(&sampDesc, m_pSamplerState.GetAddressOf());
	m_pd3dImmediateContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());

	//テクスチャの読み込み

	CreateWICTextureFromFile(m_pd3dDevice.Get(), L"data\\TEXTURE\\444.jpeg", nullptr, m_pImage.GetAddressOf());
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pImage.GetAddressOf());


	//テクスチャのサイズ調整によって、四角形の頂点を調整
	ID3D11Resource* resource = nullptr;
	m_pImage.Get()->GetResource(&resource);

	ID3D11Texture2D* texture = nullptr;
	resource->QueryInterface(&texture);


	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);

	UINT width = m_ClientWidth;
	UINT height = m_ClientHeight;
	width = desc.Width;
	height = desc.Height;

	texture->Release();
	resource->Release();

	float Aspect = (float)m_ClientWidth / (float)width > (float)m_ClientHeight / (float)height ? (float)m_ClientHeight / (float)height : (float)m_ClientWidth / (float)width;

	VertexPosTex vertices[] =
	{
		{ XMFLOAT3(m_ClientWidth * 0.5f - width * Aspect * 0.5f, m_ClientHeight * 0.5f - height * Aspect * 0.5f, 0.0f),XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(m_ClientWidth * 0.5f + width * Aspect * 0.5f, m_ClientHeight * 0.5f - height * Aspect * 0.5f, 0.0f),XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(m_ClientWidth * 0.5f - width * Aspect * 0.5f, m_ClientHeight * 0.5f + height * Aspect * 0.5f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(m_ClientWidth * 0.5f + width * Aspect * 0.5f, m_ClientHeight * 0.5f + height * Aspect * 0.5f, 0.0f), XMFLOAT2(1.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof vertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	m_pd3dDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf());

	UINT stride = sizeof(VertexPosTex);
	UINT offset = 0;

	//バインド
	m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());

	m_pd3dImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);


	return true;
}

HRESULT GameApp::CreateShaderFromFile(
	const WCHAR* csoFileNameInOut,
	const WCHAR* hlslFileName,
	LPCSTR entryPoint,
	LPCSTR shaderModel,
	ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	//プリコンパイルのバイナリファイル存在する場合
	if (csoFileNameInOut && D3DReadFileToBlob(csoFileNameInOut, ppBlobOut) == S_OK)
	{
		return hr;
	}
	else
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG

		dwShaderFlags |= D3DCOMPILE_DEBUG;


		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		ID3DBlob* errorBlob = nullptr;
		hr = D3DCompileFromFile(hlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel,
			dwShaderFlags, 0, ppBlobOut, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
			}
			if (errorBlob)
			{
				errorBlob->Release();
				errorBlob = nullptr;
			}
			return hr;
		}


		if (csoFileNameInOut)
		{
			return D3DWriteBlobToFile(*ppBlobOut, csoFileNameInOut, FALSE);
		}
	}

	return hr;
}

void GameApp::DrawImGui()
{
	static bool shake;
	static bool rgb;
	static bool noise;
	static bool scanline;
	static bool blackout;

	ImGui::Begin("Glitching!");
	if (ImGui::BeginTable("Glitching", 4))
	{
		//uv+rgbシフト
		ImGui::TableNextColumn();
		ImGui::Checkbox("UV Shift", &shake);
		ImGui::SliderFloat("Shake Power", &glitch.shake_power, 0.0f, 2.0f);
		ImGui::SliderFloat("Shake Rate", &glitch.shake_rate, 0.0f, 1.0f);
		ImGui::SliderFloat("Shake Speed", &glitch.shake_speed, 0.0f, 20.0f);
		ImGui::SliderFloat("Block Size", &glitch.shake_block_size, 0.0f, 500.0f);
		ImGui::Checkbox("RGB Shift", &rgb);
		ImGui::SliderFloat("Color Scattering", &glitch.shake_color_rate, -0.15f, 0.15f);

		//ノイズ効果
		ImGui::TableNextColumn();
		ImGui::Checkbox("Noise", &noise);
		ImGui::SliderFloat("Noise Size", &glitch.grain_blockSize, 0.0f, 500.0f);
		ImGui::SliderFloat("Noise Alpha", &glitch.grain_alpha, 0.0f, 1.0f);

		//走査線
		ImGui::TableNextColumn();
		ImGui::Checkbox("Scanline", &scanline);
		ImGui::SliderFloat("Scanline Opacity", &glitch.scanline_opacity, 0.0f, 1.0f);
		ImGui::SliderFloat("Scanline Width", &glitch.scanline_noise, 0.0f, 20.0f);

		//点滅効果
		ImGui::TableNextColumn();
		ImGui::Checkbox("Blackout", &blackout);
		ImGui::SliderFloat("Blackout Speed", &glitch.flick_speed, 0.0f, 1000.0f);
		ImGui::SliderFloat("Blackout Strength", &glitch.flick_str, 0.0f, 1.0f);
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::Text("Read Image From File:");
		if (ImGui::Button("Read!"))
		{
			HRESULT hr = S_OK;
			ID3D11ShaderResourceView* temp;

			wchar_t* directory = ReadFileDiectory();
			hr = CreateWICTextureFromFile(m_pd3dDevice.Get(), directory, nullptr, &temp);
			if (hr == 0x88982f50)
			{
				MessageBox(m_hMainWnd, "画像ファイルを選択してください！", "読み込みエラー", MB_OK | MB_ICONWARNING);
			}
			else if (hr == HRESULT_FROM_WIN32(ERROR_INVALID_NAME))

			{
				MessageBox(m_hMainWnd, "ファイルパスに英字、数字以外の文字が含まれたので、使用出来ません。", "Invalid Path", MB_OK | MB_ICONWARNING);
			}
			else
			{
				temp->Release();
				m_pImage.Reset();
				CreateWICTextureFromFile(m_pd3dDevice.Get(), directory, nullptr, m_pImage.GetAddressOf());
				m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pImage.GetAddressOf());
				ResizeRect();
			}

			delete[] directory;
		}

		ImGui::EndTable();
	}

	ImGui::End();

	if (shake)
	{
		glitch.shake_enable = 1.0f;

	}
	else
	{
		glitch.shake_enable = 0.0f;
	}

	if (rgb)
	{

		glitch.rgb_shift = 1.0f;

	}
	else
	{
		glitch.rgb_shift = 0.0f;
	}

	if (noise)
	{

		glitch.grain_enable = 1.0f;

	}
	else
	{
		glitch.grain_enable = 0.0f;
	}

	if (scanline)
	{

		glitch.scanline_enbale = 1.0f;

	}
	else
	{
		glitch.scanline_enbale = 0.0f;
	}

	if (blackout)
	{

		glitch.flick_enable = 1.0f;

	}
	else
	{
		glitch.flick_enable = 0.0f;
	}

}

//ファイルの読み込み
wchar_t* GameApp::ReadFileDiectory()
{
	OPENFILENAME ofn;
	char szFile[260];
	HWND hwnd = NULL;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;

	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0Images\0*.PNG;*.JPG;*.JPEG\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{

		int size_needed = MultiByteToWideChar(CP_ACP, 0, ofn.lpstrFile, -1, NULL, 0);
		wchar_t* wFilePath = new wchar_t[size_needed];

		MultiByteToWideChar(CP_ACP, 0, ofn.lpstrFile, -1, wFilePath, size_needed);

		return wFilePath;
	}

	return nullptr;
}

void GameApp::ResizeRect()
{

	//テクスチャのサイズ調整によって、四角形の頂点を調整
	ID3D11Resource* resource = nullptr;
	m_pImage.Get()->GetResource(&resource);

	ID3D11Texture2D* texture = nullptr;
	resource->QueryInterface(&texture);


	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);

	UINT width = m_ClientWidth;
	UINT height = m_ClientHeight;
	width = desc.Width;
	height = desc.Height;

	texture->Release();
	resource->Release();

	float Aspect = (float)m_ClientWidth / (float)width > (float)m_ClientHeight / (float)height ? (float)m_ClientHeight / (float)height : (float)m_ClientWidth / (float)width;

	//画像サイズをもとに頂点バッファの調整
	VertexPosTex vertices[] =
	{
		{ XMFLOAT3(m_ClientWidth * 0.5f - width * Aspect * 0.5f, m_ClientHeight * 0.5f - height * Aspect * 0.5f, 0.0f),XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(m_ClientWidth * 0.5f + width * Aspect * 0.5f, m_ClientHeight * 0.5f - height * Aspect * 0.5f, 0.0f),XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(m_ClientWidth * 0.5f - width * Aspect * 0.5f, m_ClientHeight * 0.5f + height * Aspect * 0.5f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(m_ClientWidth * 0.5f + width * Aspect * 0.5f, m_ClientHeight * 0.5f + height * Aspect * 0.5f, 0.0f), XMFLOAT2(1.0f, 1.0f) }
	};

	m_pd3dImmediateContext.Get()->UpdateSubresource(m_pVertexBuffer.Get(), 0, nullptr, &vertices, 0, 0);


}