#include "Boss.h"
#include "MyMath.h"
#include "Player/Player.h"
#include <numbers>

using namespace KamataEngine;

void Boss::Initialize(Model* model, KamataEngine::Camera* camera) {

#ifdef DEBUG
	// NULLポインタのチェック
	assert(model);
#endif // DEBUG

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// bulletモデル
	//bulletModel_ = bulletModel;

	// 柱のモデル
	//pillarModel_ = pillarModel;

	// ワールド変換の初期化
	worldTranseform_.Initialize();
	worldTranseform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTranseform_.scale_ = {2.5f, 2.5f, 2.5f};

	// テクスチャーハンドル
	// textureHandle_ = textureHandle;

	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	input_ = Input::GetInstance();

	//const int kCount = 8;

	//for (int i = 0; i < kCount; i++) {
	//
	//	float baseAngle = (2.0f * std::numbers::pi_v<float> / kCount) * i;
	//
	//	// 上
	//	Pillar* upper = new Pillar();
	//	upper->Initialize(pillarModel_, PillarType::Upper, i, kCount);
	//	upper->SetAngle(baseAngle);
	//	pillars_.push_back(upper);
	//
	//	// 下
	//	Pillar* lower = new Pillar();
	//	lower->Initialize(pillarModel_, PillarType::Lower, i, kCount);
	//	lower->SetAngle(baseAngle);
	//	pillars_.push_back(lower);
	//}
}

void Boss::Update() {

	// ボス自身をゆっくり回転させる（演出）
	rotateAngle_ += 0.005f;
	worldTranseform_.rotation_.y = rotateAngle_;

	// 行列を定数バッファに転送
	WorldTransformUpdate(worldTranseform_);

	// 柱に「中心座標」を渡す
	//for (Pillar* pillar : pillars_) {
	//	pillar->SetCenter(worldTranseform_.translation_);
	//	pillar->Update();
	//
	//	// ボスのHPが半分以下になったらスピードアップ
	//	if (bossHp_ <= maxBossHp_ / 2) {
	//		pillar->setSpeed(-0.0225f);
	//	} else {
	//		speed_ = 0.0225f;
	//	}
	//}

	// 弾のクールタイムが0以下なら攻撃可能
	//if (bulletCoolTime_ > 0) {
	//	bulletCoolTime_ -= 1.0f / 60.0f;
	//}

	//Attack();

	// 弾の更新
	//for (BossBullet* bullet : bullets_) {
	//	bullet->Update();
	//}

	// デスフラグが立った弾を削除
	//bullets_.remove_if([](BossBullet* bullet) {
	//	if (bullet->IsDead()) {
	//		delete bullet;
	//		return true;
	//	}
	//	return false;
	//});

#ifdef _DEBUG
	// ボスの座標を画面表示する
	ImGui::Begin("Boss Position");
	ImGui::Text("x: %.2f", worldTranseform_.translation_.x);
	ImGui::Text("y: %.2f", worldTranseform_.translation_.y);
	ImGui::Text("z: %.2f", worldTranseform_.translation_.z);

	// ボスの体力を画面表示する
	ImGui::Text("HP: %d", bossHp_);
	ImGui::End();
#endif // DEBUG
}

void Boss::Draw() {

	// 描画
	model_->Draw(worldTranseform_, *camera_);
	// 柱の描画
	//for (Pillar* pillar : pillars_) {
	//	pillar->Draw(*camera_);
	//}
	//// 弾の描画
	//for (BossBullet* bullet : bullets_) {
	//	bullet->Draw(*camera_);
	//}
}

//void Boss::Attack() {
//
//	if (bulletCoolTime_ > 0) {
//		return;
//	}
//
//	// 弾を生成
//	BossBullet* newBullet = new BossBullet();
//
//	// 弾の発射位置(自機のワールド座標)
//	Vector3 bulletStartPos = GetWorldPosition();
//
//	// ボス位置
//	Vector3 bossPos = worldTranseform_.translation_;
//
//	// プレイヤー位置（取得方法はプロジェクトに合わせて）
//Vector3 playerPos = player_->GetWorldPosition();
//
//	// プレイヤーへの方向ベクトル
//	Vector3 bulletDir = playerPos - bulletStartPos;
//
//	// 正規化
//	bulletDir = Normalize(bulletDir);
//
//	// 弾速
//	const float kBulletSpeed = 1.0f;
//	Vector3 bulletVelocity = bulletDir * kBulletSpeed;
//
//	// 弾の初期化
//	newBullet->Initialize(bulletModel_, textureHandle_, bulletStartPos, bulletVelocity);
//
//	// 弾をセット
//	bullets_.push_back(newBullet);
//
//	bulletCoolTime_ = 1.0f;
//}

Vector3 Boss::GetPosition() { return worldTranseform_.translation_; }

void Boss::Oncollosion() {
	// ボスの体力を減らす
	bossHp_ -= 3;
	// 体力が0以下ならデスフラグを立てる
	if (bossHp_ <= 0) {
		isDead_ = true;
	}
}

Boss::~Boss() {

	//for (Pillar* pillar : pillars_) {
	//	delete pillar;
	//}
	//pillars_.clear();
	//for (BossBullet* bullet : bullets_) {
	//	delete bullet;
	//}
}

Vector3 Boss::GetWorldPosition() const {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTranseform_.matWorld_.m[3][0];
	worldPos.y = worldTranseform_.matWorld_.m[3][1];
	worldPos.z = worldTranseform_.matWorld_.m[3][2];
	return worldPos;
}