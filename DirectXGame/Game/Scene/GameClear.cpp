#include "scene/GameClear.h"
#include "MyMath.h"
#include "SceneManager.h"

using namespace KamataEngine;

void GameClear::Initialize() {

	//
	camera_.Initialize();
}

void GameClear::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}
}

void GameClear::Draw() {
	// コマンドリストの取得
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw();

	Model::PostDraw();

	// UI描画前処理
	Sprite::PreDraw(commandList);

	// UI描画後処理
	Sprite::PostDraw();
}

GameClear::~GameClear() {
	
}