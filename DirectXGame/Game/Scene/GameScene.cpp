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
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
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

	// 敵が全て消えたら最初から出現し直す
	if (enemies_.empty()) {
		// 既存のコマンドストリームをクリアしてファイルから再読み込みする
		LoadEnemyPopData(); // Resources/enemyPopData.csv を再読み込み

		// 待機状態を解除して即実行できるようにする
		waitTimer_ = 0;
	}

	// 照準の更新
	aim_->Update();

	// プレイヤーの更新
	player_->Update();

	// 敵のスクリプト実行
	UpdateEnemyPopcomand();

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

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("GameClear");
	}
}

// 敵のスクリプトファイル読み込み
void GameScene::LoadEnemyPopData() {

	// ファイルを開く
	ifstream file;
	file.open("Resources/enemy/enemyPopData.csv");

#ifdef DEBUG
	assert(file.is_open());
#endif // DEBUG

	// ファイルの内容を文字列ストリームにコピー
	enemyPopComands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

// 敵のスクリプト実行
void GameScene::UpdateEnemyPopcomand() {

	// 待機処理
	if (isWaiting_) {
		// 待機タイマーをデクリメント
		waitTimer_--;
		// タイマーが0になったら待機終了
		if (waitTimer_ <= 0) {
			isWaiting_ = false;
		}
		// 待機中は他の処理をしない
		return;
	}

	// 1行分の文字列を入れる変数
	string line;

	// コマンド実行ループ
	while (getline(enemyPopComands, line)) {
		// 1行分の文字列を文字列ストリームに変換
		stringstream line_stream(line);

		string word;
		// カンマ区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始める行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// 敵の生成
			// x座標
			getline(line_stream, word, ',');
			float x = (float)atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)atof(word.c_str());

			// 移動ベクトルの取得
			// 移動ベクトルX
			getline(line_stream, word, ',');
			float vx = (float)atof(word.c_str());

			// 移動ベクトルY
			getline(line_stream, word, ',');
			float vy = (float)atof(word.c_str());

			// 移動ベクトルZ
			getline(line_stream, word, ',');
			float vz = (float)atof(word.c_str());

			// 敵を発生させる
			Enemy* newEnemies = new Enemy();
			newEnemies->Initialize(modelEnemy_, &camera_, modelBullet_, player_);
			newEnemies->SetPosition(Vector3(x, y, z));
			newEnemies->SetMoveVector(Vector3(vx, vy, vz));

			enemies_.push_back(newEnemies);

			// デバッグ出力
			// OutputDebugStringA(("Enemy Spawned at: " + to_string(x) + "," + to_string(y) + "," + to_string(z) + "\n").c_str());
		}

		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');
			// 待ち時間
			int32_t waitFrame = atoi(word.c_str());

			// 待機開始
			isWaiting_ = true;

			// 待機タイマーをセット
			waitTimer_ = waitFrame;

			// ループを抜ける
			break;
		}
	}
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

	// 敵の描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// 3Dオブジェクト後処理
	Model::PostDraw();

	// UI描画前処理
	Sprite::PreDraw(commandList);

	aim_->Draw();

	// UI描画後処理
	Sprite::PostDraw();
}