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
	delete playerModel_;
	delete playerBulletModel_;
	delete player_;
	delete aim_;
}

void GameScene::Initialize() {

	// カメラの初期化
	camera_.Initialize();

	//
	worldTransform_.Initialize();

	// 照準の初期化
	aim_ = new Aim();
	aim_->Initialize(&camera_);

	// プレイヤーの初期化
	player_ = new Player();
	playerModel_ = Model::CreateFromOBJ("player", true);
	playerBulletModel_ = Model::CreateFromOBJ("playerBullet", true);
	player_->Initialize(playerModel_, &camera_, playerBulletModel_, aim_);
}

void GameScene::Update() {

	// 照準の更新
	aim_->Update();

	// プレイヤーの更新
	player_->Update();

	// 攻撃する時
	if (aim_->IsAttac()) {
		player_->Attack();
	}

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

	player_->Draw();

	// 3Dオブジェクト後処理
	Model::PostDraw();

	// UI描画前処理
	Sprite::PreDraw(commandList);

	aim_->Draw();

	// UI描画後処理
	Sprite::PostDraw();
}