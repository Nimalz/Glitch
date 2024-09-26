#include "main.h"
#include "renderer.h"
#include "camera.h"
#include "polygon.h"


D3DXMATRIX				g_CameraViewMatrix;
D3DXMATRIX				g_CameraInvViewMatrix;
D3DXMATRIX				g_CameraProjectionMatrix;

D3DXVECTOR3				g_CameraPosition;
D3DXVECTOR3				g_CameraTarget;
D3DXVECTOR3				g_CameraRotation;



D3DXMATRIX GetCameraViewMatrix()
{
	return g_CameraViewMatrix;
}

D3DXMATRIX GetCameraInvViewMatrix()
{
	return g_CameraInvViewMatrix;
}

D3DXMATRIX GetCameraProjectionMatrix()
{
	return g_CameraProjectionMatrix;
}

D3DXVECTOR3 GetCameraPosition()
{
	return g_CameraPosition;
}

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	g_CameraPosition = D3DXVECTOR3( 0.0f, 50.0f, -100.0f );
	g_CameraTarget = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	g_CameraRotation = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitCamera(void)
{


}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateCamera(void)
{

}


//=============================================================================
// 描画処理
//=============================================================================
void SetCamera(void)
{
	// ビュー行列設定
	D3DXMatrixLookAtLH( &g_CameraViewMatrix, &g_CameraPosition, &g_CameraTarget, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );

	SetViewMatrix( &g_CameraViewMatrix );


	// ビュー行列の逆行列を作っておく（オプション）
	float det;
	D3DXMatrixInverse(&g_CameraInvViewMatrix, &det, &g_CameraViewMatrix);


	// プロジェクション行列設定
	D3DXMatrixPerspectiveFovLH( &g_CameraProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 1000.0f);

	SetProjectionMatrix( &g_CameraProjectionMatrix );
}
