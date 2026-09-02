#include "RailCameraController.h"
#include "Boss/Boss.h"
#include "MyMath.h"
#include "imgui.h"
#include "player.h"

using namespace KamataEngine;

// 初期化
void RailCameraController::Initialize(Camera* camera, const Player* player, const Boss* boss) {

	// 引数の内容をメンバ変数に記録
	camera_ = camera;
	player_ = player;
	boss_ = boss;

	worldTransform_.Initialize();

	// 初期位置（角度0）
	worldTransform_.translation_ = {radius_, height_, 0.0f};

	worldTransform_.rotation_ = {0, 0, 0};

	angle_ = 4.5f; // 初期化時
}

void RailCameraController::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

// 更新
void RailCameraController::Update() {

	Vector3 playerPos = player_->GetWorldPosition();
	Vector3 bossPos = boss_->GetWorldPosition();

	// ボス → プレイヤー方向
	Vector3 baseDir = Normalize(playerPos - bossPos);

	// ボス基準の右方向（右手系）
	Vector3 right = Normalize(Cross(Vector3{0, 1, 0}, baseDir));

	// ★ 左右固定
	Vector3 sideOffset = right * fixedSide_ * sideRadius_ * std::cos(angle_);

	// 後ろ + 高さ
	Vector3 backOffset = -baseDir * radius_ * std::sin(angle_);
	Vector3 heightOffset = Vector3{0, height_, 0};

	// カメラ位置
	Vector3 cameraPos = playerPos + backOffset + sideOffset + heightOffset;

	// 適用
	worldTransform_.translation_ = cameraPos;
	WorldTransformUpdate(worldTransform_);

	// 注視点（プレイヤーの前方）
	Vector3 targetPos = playerPos + baseDir * lookAhead_;

	camera_->matView = MakeLookAtMatrix(cameraPos, targetPos, Vector3{0, 1, 0});

	// 射影更新
	camera_->UpdateProjectionMatrix();

	#ifdef _DEBUG
	// ===== ImGui =====
	ImGui::Begin("RailCameraController");
	ImGui::DragFloat("Angle", &angle_, 0.01f);  // 角度
	ImGui::DragFloat("Radius", &radius_, 0.1f); // 中心からの距離
	ImGui::DragFloat("Height", &height_, 0.1f); // 高さ
	ImGui::End();
#endif // DEBUG
}

// ワールド座標を取得
Vector3 RailCameraController::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}