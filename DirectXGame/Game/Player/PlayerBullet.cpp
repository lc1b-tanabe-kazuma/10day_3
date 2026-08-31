#include "PlayerBullet.h"
#include "MYMath.h"
#include "imgui.h"

using namespace KamataEngine;

void PlayerBullet::Initialize(Model* model, const Vector3& position, Vector3& velocity) {

	// NULLポインタのチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// ワールド変換の初期化
	worldTranseform_.Initialize();

	worldTranseform_.translation_ = position;

	velocity_ = velocity;

	// 向き
	worldTranseform_.rotation_.y = 3.14f / 2.0f;

	WorldTransformUpdate(worldTranseform_);
}

void PlayerBullet::Update() {

	// 時間経過で弾を消す
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 移動
	worldTranseform_.translation_ += velocity_;

	// 行列を定数バッファに転送
	WorldTransformUpdate(worldTranseform_);

#ifdef DEBUG
	// デバッグ用表示
	ImGui::Begin("Player Bullet");
	ImGui::Text("position");
	ImGui::Text("x: %.2f", worldTranseform_.translation_.x);
	ImGui::Text("y: %.2f", worldTranseform_.translation_.y);
	ImGui::Text("z: %.2f", worldTranseform_.translation_.z);
	ImGui::End();
#endif // DEBUG
}

void PlayerBullet::Draw(const Camera& camera) {

	// 描画
	model_->Draw(worldTranseform_, camera);
}

Vector3 PlayerBullet::GetPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTranseform_.matWorld_.m[3][0];
	worldPos.y = worldTranseform_.matWorld_.m[3][1];
	worldPos.z = worldTranseform_.matWorld_.m[3][2];
	return worldPos;
}