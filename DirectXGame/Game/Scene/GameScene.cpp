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
	delete modelEnemy_;
	delete modelBullet_;
	delete bossModel_;
	delete railCameraController_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete enemySpawner_;
	delete drawNumber_;
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

	// 敵モデル
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	modelBullet_ = Model::CreateFromOBJ("playerBullet", true);


	// ボスの初期化
	boss_ = new Boss();
	bossModel_ = Model::CreateFromOBJ("boss", true);
	boss_->Initialize(bossModel_, &camera_);

	player_->SetBoss(boss_);

	boss_->SetPlayer(player_);

	// レールカメラコントローラーの初期化
	railCameraController_ = new RailCameraController();
	railCameraController_->Initialize(&camera_, player_, boss_);
	railCameraController_->SetParent(&player_->GetWorldTransform());

	// 敵スポナーの初期化
	enemySpawner_ = new EnemySpawner();
	enemySpawner_->Initialize(modelEnemy_, modelBullet_, &camera_, player_, enemies_);
	enemySpawner_->LoadPopData("Resources/enemy/EnemyPopData.csv");

	// 数字描画の初期化
	drawNumber_ = new DrawNumber();
	drawNumber_->Initialize(TextureManager::Load("UI/number.png"), Vector2(1000.0f, 32.0f));

}

void GameScene::Update() {

	// 敵が消えたら削除
	enemies_.remove_if([this](Enemy* enemy) {
		// 画面外に出たかどうかの判定
		enemy->OutFlag();

		// 死亡しているか、画面外に出たら削除
		if (enemy->IsDead() || enemy->IsOut()) {
			delete enemy;
			return true;
		}
		return false;
	});

	// 照準の更新
	aim_->Update();

	// プレイヤーの更新
	player_->Update();

	// ボスの更新
	if (boss_) {
		boss_->Update();
	}

	// 敵スポナーの更新
	enemySpawner_->Update();

	// 敵キャラの更新
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	// 攻撃する時
	if (aim_->IsAttac()) {
		player_->Attack();
	}

	// 当たり判定
	OnCollision();

	// 数字描画の更新
	drawNumber_->Update(static_cast<int>(player_->GetCombo()));

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("GameClear");
	}

	// カメラ処理
	// レールカメラコントローラーの更新
	railCameraController_->Update();
	// レールカメラコントローラーのカメラを取得して適用
	camera_.matView = railCameraController_->GetCamera().matView;
	camera_.matProjection = railCameraController_->GetCamera().matProjection;
	// ビュープロジェクション行列の更新と転送
	camera_.TransferMatrix();
}

void GameScene::OnCollision() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自機の弾リスト取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

#pragma region プレイヤーの弾と敵の当たり判定
	// 敵の座標を取得
	for (Enemy* enemy : enemies_) {
		posA = enemy->GetWorldPosition();

		// プレイヤーの弾の座標を取得
		for (PlayerBullet* bullet : playerBullets) {
			posB = bullet->GetPosition();
			if (IsCollision(posA, enemy->GetRadius(), posB, bullet->GetRadius())) {

				// ---- 通常弾 ----
				bullet->OnCollision();
				enemy->OnCollision();

				// ---- コンボ ----
				player_->OnEnemyHit();
			}
		}
	}

	// 敵の弾とプレイヤーの当たり判定
	for (Enemy* enemy : enemies_) {
		posA = player_->GetWorldPosition();

		// プレイヤーの弾の座標を取得
		for (EnemyBullet* enemyBullet : enemy->GetBullets()) {
			posB = enemyBullet->GetPosition();
			if (IsCollision(posA, player_->GetRadius(), posB, enemyBullet->GetRadius())) {

				// ---- 敵の弾 ----
				enemyBullet->OnCollision();
				player_->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::Draw() {

	// コマンドリストの取得
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 3Dオブジェクト描画前処理
	Model::PreDraw();

	// プレイヤーの描画
	player_->Draw();

	// ボスの描画
	if (boss_) {
		boss_->Draw();
	}

	// 敵の描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// 3Dオブジェクト後処理
	Model::PostDraw();

	// UI描画前処理
	Sprite::PreDraw(commandList);

	// 照準描画
	aim_->Draw();

	// コンボ描画
	drawNumber_->Draw();

	// UI描画後処理
	Sprite::PostDraw();
}