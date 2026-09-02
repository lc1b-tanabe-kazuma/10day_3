#pragma once
#include "Aim/Aim.h"
#include "KamataEngine.h"
#include "Player.h"
#include "SceneBase.h"
#include "Enemy.h"

using namespace KamataEngine;

// ゲームシーン
class GameScene : public SceneBase {
public:
	GameScene(KamataEngine::Input* input) : SceneBase(input) {}

	~GameScene() override;

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 敵のスクリプトファイル読み込み
	void LoadEnemyPopData();

	// 敵のスクリプト実行
	void UpdateEnemyPopcomand();

	// 敵の出現待機中
	bool IsWaiting() const { return isWaiting_; }

	// 敵が全員WAITコマンドの時間が0になっていてかつ全員デリートされたか
	bool IsAllEnemiesWaited() const { return enemies_.empty() && !isWaiting_ && enemyPopComands.eof(); }

	void OnCollision();

	// 描画
	void Draw() override;

private:

	// カメラ
	KamataEngine::Camera camera_;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// プレイヤー
	Player* player_ = nullptr;
	KamataEngine::Model* playerModel_ = nullptr;
	KamataEngine::Model* playerBulletModel_ = nullptr;

	// 照準
	Aim* aim_ = nullptr;

	// 敵
	std::list<Enemy*> enemies_;
	KamataEngine::Model* modelEnemy_ = nullptr;
	KamataEngine::Model* modelBullet_ = nullptr;

	// 敵の発生コマンド
	std::stringstream enemyPopComands;

	// 敵の出現の待機中フラグ
	bool isWaiting_ = false;

	// 敵の出現の待機タイマー
	int32_t waitTimer_ = 0;
};