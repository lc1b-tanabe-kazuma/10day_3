#pragma once
#include "Aim/Aim.h"
#include "DrawNumber.h"
#include "Enemy.h"
#include "EnemySpawner.h"
#include "KamataEngine.h"
#include "Player.h"
#include "SceneBase.h"

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

	// 当たり判定
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
	EnemySpawner* enemySpawner_ = nullptr;

	// 数字描画
	DrawNumber* drawNumber_;
	uint32_t numberTH_ = 0;
};