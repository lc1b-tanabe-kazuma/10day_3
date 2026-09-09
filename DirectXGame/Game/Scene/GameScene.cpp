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
	delete bossHPBar_;
	delete bossHPBackBar_;
	delete bossHPLogo_;
	delete playerHPBar_;
	delete playerHPBackBar_;
	delete playerHPLogo_;
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
	modelBullet_ = Model::CreateFromOBJ("bullet", true);

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
	drawNumber_->Initialize(TextureManager::Load("UI/number.png"), Vector2(968.0f, 32.0f));

	// コンボの文字描画の初期化
	comboTH_ = TextureManager::Load("UI/combo.png");
	comboSprite_ = Sprite::Create(comboTH_, {1128.0f, 32.0f}, {1, 1, 1, 1});

	// ボスのHPテクスチャの生成
	bossHPBarTH_ = TextureManager::Load("UI/UI_bossHP.png");
	bossHPBar_ = Sprite::Create(bossHPBarTH_, {64.0f, 50.0f}, {1, 1, 1, 1});
	bossHPBar_->SetAnchorPoint({0.0f, 0.5f});

	// 横長にする
	bossHPBar_->SetSize({563.0f, 32.0f});

	// ボスのHPロゴの生成
	bossHPLogoTH_ = TextureManager::Load("UI/UI_bossHPLogo.png");
	bossHPLogo_ = Sprite::Create(bossHPLogoTH_, {32.0f, 82.0f}, {1, 1, 1, 1});
	bossHPLogo_->SetAnchorPoint({0.0f, 0.5f});

	// 横長にする
	bossHPLogo_->SetSize({600.0f, 64.0f});

	// ボスのHPバーの背景の生成
	bossHPBackBarTH_ = TextureManager::Load("UI/UI_bossHPBarBack.png");
	bossHPBackBar_ = Sprite::Create(bossHPBackBarTH_, {64.0f, 50.0f}, {1, 1, 1, 1});
	bossHPBackBar_->SetAnchorPoint({0.0f, 0.5f});

	// 横長にする
	bossHPBackBar_->SetSize({563.0f, 32.0f});

	// プレイヤーのHPバーの生成
	playerHPBarTH_ = TextureManager::Load("UI/UI_playerHP.png");
	playerHPBar_ = Sprite::Create(playerHPBarTH_, {64.0f, 700.0f}, {1, 1, 1, 1});
	playerHPBar_->SetAnchorPoint({0.0f, 0.5f});

	// 横長にする
	playerHPBar_->SetSize({563.0f, 32.0f});

	// プレイヤーのHPバーの背景の生成
	playerHPBackBarTH_ = TextureManager::Load("UI/UI_playerHPBarBack.png");
	playerHPBackBar_ = Sprite::Create(playerHPBackBarTH_, {64.0f, 700.0f}, {1, 1, 1, 1});
	playerHPBackBar_->SetAnchorPoint({0.0f, 0.5f});

	// 横長にする
	playerHPBackBar_->SetSize({563.0f, 32.0f});

	// プレイヤーのHPロゴの生成
	playerHPLogoTH_ = TextureManager::Load("UI/UI_playerHPLogo.png");
	playerHPLogo_ = Sprite::Create(playerHPLogoTH_, {32.0f, 695.0f}, {1, 1, 1, 1});
	playerHPLogo_->SetAnchorPoint({0.0f, 0.5f});

	// 横長にする
	playerHPLogo_->SetSize({600.0f, 64.0f});

	// スカイドームの初期化
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	// 地面の初期化
	modelGround_ = Model::CreateFromOBJ("Ground", true);
	ground_ = new Ground();
	ground_->Initialize(modelGround_, &camera_);
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

	// エフェクトが終了したら削除
	for (auto it = effects_.begin(); it != effects_.end();) {
		if ((*it)->IsFinished()) {
			delete *it;
			it = effects_.erase(it);
		} else {
			++it;
		}
	}

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

	// 当たり判定
	OnCollision();

	// エフェクトの更新
	for (Effect* effect : effects_) {
		effect->Update();
	}

	// 数字描画の更新
	drawNumber_->Update(static_cast<int>(player_->GetCombo()));

	// ゲームクリア判定
	if (boss_->IsDead()) {
		SceneManager::GetInstance()->ChangeScene("GameClear");
	}

	// ゲームオーバー判定
	if (player_->GetHP() <= 0) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

	// カメラ処理
	// レールカメラコントローラーの更新
	railCameraController_->Update();
	// レールカメラコントローラーのカメラを取得して適用
	camera_.matView = railCameraController_->GetCamera().matView;
	camera_.matProjection = railCameraController_->GetCamera().matProjection;
	// ビュープロジェクション行列の更新と転送
	camera_.TransferMatrix();

	// スカイドームの更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();
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

				// 撃破エフェクト生成
				Effect* effect = new Effect();
				effect->Initialize(posA);
				effects_.push_back(effect);

				// ---- コンボ ----
				player_->OnEnemyHit();
			}
		}
	}

	// ボスの座標を取得
	posA = boss_->GetWorldPosition();

	// プレイヤーの弾の座標を取得
	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetPosition();
		if (IsCollision(posA, boss_->GetRadius(), posB, bullet->GetRadius())) {
			// ---- 通常弾 ----
			bullet->OnCollision();
			boss_->Oncollosion(bullet->GetDamage());

			// HPバーの更新
			float hpRate = (float)boss_->GetHP() / boss_->GetMaxHP();

			// 横長にする
			bossHPBar_->SetSize({563.0f * hpRate, 32.0f});

			// テクスチャの左上から必要な幅だけ使う
			bossHPBar_->SetTextureRect({0.0f, 0.0f}, {(563.0f * hpRate), 32});
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

				// ---- プレイヤーのHPバーの更新 ----
				float hpRate = (float)player_->GetHP() / player_->GetMaxHP();

				// 横長にする
				playerHPBar_->SetSize({563.0f * hpRate, 32.0f});

				// テクスチャの左上から必要な幅だけ使う
				playerHPBar_->SetTextureRect({0.0f, 0.0f}, {(563.0f * hpRate), 32});
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

	for (Effect* effect : effects_) {
		effect->Draw(&camera_);
	}

	// スカイドームの描画
	skydome_->Draw();

	// 地面の描画
	ground_->Draw();

	// 3Dオブジェクト後処理
	Model::PostDraw();

	// UI描画前処理
	Sprite::PreDraw(commandList);

	// 照準描画
	aim_->Draw();

	// コンボ描画
	drawNumber_->Draw();

	// コンボの文字描画
	comboSprite_->Draw();

	// ボスのHPバーの背景の描画
	bossHPBackBar_->Draw();

	// ボスのHPバーの描画
	bossHPBar_->Draw();

	// ボスのHPロゴの描画
	bossHPLogo_->Draw();

	// プレイヤーのHPバーの背景の描画
	playerHPBackBar_->Draw();

	// プレイヤーのHPバーの描画
	playerHPBar_->Draw();

	// プレイヤーのHPロゴの描画
	playerHPLogo_->Draw();

	// UI描画後処理
	Sprite::PostDraw();
}