#include "KamataEngine.h"
#include "engine/sceneEngine/SceneManager.h"
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"");

	// DirectXCommonのインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// sceneManager作成
	SceneManager::GetInstance()->Initialize();

#ifdef _DEBUG
	// ImguiManagerのインスタンスを取得
	ImGuiManager* imguiManager_ = ImGuiManager::GetInstance();
#endif

	// カーソルを隠す
	ShowCursor(FALSE);

	// メインループ
	while (true) {

		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

#ifdef _DEBUG
		// Imgui受付開始
		imguiManager_->Begin();
#endif
		// sceneの更新
		SceneManager::GetInstance()->Update();

		HWND hwnd = WinApp::GetInstance()->GetHwnd();

		// マウスのスクリーン座標取得
		POINT mousePos;
		GetCursorPos(&mousePos);

		// クライアント座標に変換
		POINT clientPos = mousePos;
		ScreenToClient(hwnd, &clientPos);

		RECT rect;
		GetClientRect(hwnd, &rect);

		bool isInsideClient = (clientPos.x >= 0 && clientPos.x < rect.right && clientPos.y >= 0 && clientPos.y < rect.bottom);

#ifdef _DEBUG
		ImGuiIO& io = ImGui::GetIO();
		bool isImGui = io.WantCaptureMouse;
#else
		bool isImGui = false;
#endif

		if (isImGui || !isInsideClient) {
			while (ShowCursor(TRUE) < 0)
				;
		} else {
			while (ShowCursor(FALSE) >= 0)
				;
		}

#ifdef _DEBUG
		// Imgui受付終了
		imguiManager_->End();
#endif
		// 描画開始
		dxCommon->PreDraw();

		// sceneの描画
		SceneManager::GetInstance()->Draw();

#ifdef _DEBUG
		// Imguiの描画
		imguiManager_->Draw();
#endif

		// 描画終了
		dxCommon->PostDraw();
	}

	// sceneManager解放
	SceneManager::GetInstance()->Finalize();

	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}