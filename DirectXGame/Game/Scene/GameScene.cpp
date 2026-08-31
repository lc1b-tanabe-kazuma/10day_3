#define NOMINMAX
#include "Scene/GameScene.h"
#include "MyMath.h"
#include "SceneManager.h"
#include <algorithm>
#include <array>
#include <fstream>
#include <numbers>

using namespace std;
using namespace KamataEngine;

GameScene::~GameScene() {

}

void GameScene::Initialize() {

	// カメラの初期化
	camera_.Initialize();

	//
	worldTransform_.Initialize();
}

void GameScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("GameClear");
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 3Dオブジェクト描画前処理
	Model::PreDraw();

	// 3Dオブジェクト後処理
	Model::PostDraw();

	// UI描画前処理
	Sprite::PreDraw(commandList);

	// UI描画後処理
	Sprite::PostDraw();
}