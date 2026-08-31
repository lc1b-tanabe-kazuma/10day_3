#include "GameTitle.h"
#include "SceneManager.h"

void GameTitle::Initialize() {
	//
	camera_.Initialize();
}

void GameTitle::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}
}

void GameTitle::Draw() {

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

GameTitle::~GameTitle() {
	
}