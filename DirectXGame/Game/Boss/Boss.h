#pragma once
#include "KamataEngine.h"
#include "input/Input.h"
#include <list>

class Player;

class Boss {
public:
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);

	// 更新
	void Update();

	// 描画
	void Draw();

	//void Attack();

	// 当たり判定の時の敵の位置を取得
	KamataEngine::Vector3 GetPosition();

	// 衝突時処理
	void Oncollosion(int damage);

	// ボスの半径
	const float& GetRadius() const { return kRadius; }

	// デスフラグ取得
	bool IsDead() const { return isDead_; }

	int GetHP() { return bossHp_; }

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition() const;

	// 弾リストを取得
	//const std::list<BossBullet*>& GetBullets() const { return bullets_; }

	// 柱のリストを取得
	//const std::list<Pillar*>& GetPillars() const { return pillars_; }

	// プレイヤーのセッター
	void SetPlayer(Player* player) { player_ = player; }

	// デストラクタ
	~Boss();

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTranseform_;
	// モデル
	KamataEngine::Model* model_ = nullptr;

	// テクスチャーハンドル
	uint32_t textureHandle_ = 0u;
	KamataEngine::Camera* camera_ = nullptr;

	// ボスの出現処理
	bool isPop_ = false;

	// 半径
	const float kRadius = 5.0f;

	// ボスの体力
	int32_t bossHp_ = 102;
	int32_t maxBossHp_ = 102;

	// デフラグ
	bool isDead_ = false;

	// 柱のリスト
	//std::list<Pillar*> pillars_;

	float speed_ = 0.0125f;

	float rotateAngle_ = 0.0f;

	// ボスの弾
	//std::list<BossBullet*> bullets_;

	//  弾モデル
	//KamataEngine::Model* bulletModel_ = nullptr;

	// キーボード入力
	KamataEngine::Input* input_ = nullptr;

	// 弾の生成クールタイム
	float bulletCoolTime_ = 1.0f;
	// static constexpr int32_t kAttackInterval_ = 120;

	// プレイヤー
	Player* player_ = nullptr;

	// 柱のモデル
	//KamataEngine::Model* pillarModel_ = nullptr;
};