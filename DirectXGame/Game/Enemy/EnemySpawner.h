#pragma once
#include "Enemy.h"
#include "KamataEngine.h"
#include <list>
#include <sstream>
#include <string>

class EnemySpawner {
public:
	// 初期化
	void Initialize(KamataEngine::Model* modelEnemy, KamataEngine::Model* modelBullet, KamataEngine::Camera* camera, Player* player, std::list<Enemy*>& enemies);

	// CSV読み込み
	void LoadPopData(const std::string& filePath);

	// スクリプト更新
	void Update();

	// 生成された敵を取得
	std::list<Enemy*>& GetEnemies() { return *enemies_; }

private:
	// POPコマンド
	void ExecutePop(std::stringstream& lineStream);

	// WAITコマンド
	void ExecuteWait(std::stringstream& lineStream);

private:
	// 敵
	std::list<Enemy*>* enemies_;

	// CSVデータ
	std::stringstream popCommands_;

	// 待機中か
	bool isWaiting_ = false;

	// 待機時間
	int32_t waitTimer_ = 0;

	// Enemy生成に必要なもの
	KamataEngine::Model* modelEnemy_ = nullptr;
	KamataEngine::Model* modelBullet_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	Player* player_ = nullptr;
};