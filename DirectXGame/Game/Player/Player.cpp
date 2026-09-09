#define NOMINMAX
#include "Player.h"
#include "Aim/Aim.h"
#include "Boss/Boss.h"
#include "MyMath.h"
#include <algorithm>

using namespace KamataEngine;

void Player::Initialize(Model* model, Camera* camera, Model* bulletModel, Aim* aim) {
	model_ = model;
	camera_ = camera;
	bulletModel_ = bulletModel;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 64.0f};
	worldTransform_.scale_ = {1.3f, 1.3f, 1.3f};
	worldTransform_.rotation_.y = 3.14f / 2.0f;

	input_ = Input::GetInstance();

	aim_ = aim;

	playerHP_ = maxPlayerHP_;

	// 射撃音のロード
	shotSoundHandle_ = KamataEngine::Audio::GetInstance()->LoadWave("sound/SE/shot.wav");

	// チャージ音の読み込み
	chargeSoundHandle_ = KamataEngine::Audio::GetInstance()->LoadWave("sound/SE/charge.wav");

}

void Player::OnCollision() {

	// 無敵時間中は当たり判定を無効化
	if (isHit_) {
		return;
	}

	// プレイヤーの体力を減らす
	playerHP_--;

	// 被弾フラグを立てる
	isHit_ = true;
}

Player::~Player() {
	// 弾の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	bullets_.clear();

	if (chargeBullet_) {
		delete chargeBullet_;
		chargeBullet_ = nullptr;
	}

	if (isChargingSoundPlaying_) {
		KamataEngine::Audio::GetInstance()->StopWave(chargeVoiceHandle_);
	}

}

void Player::Update() {

	// 移動処理
	Move();

	attackCoolTime_ -= 1.0f / 30.0f;

	switch (chargeState_) {

	case ChargeState::None:

		// クールタイムが残っている場合は攻撃できない
		if (attackCoolTime_ > 0.0f) {
			break;
		}

		// 攻撃ボタンが押されているか
		if (aim_->IsAttackHold()) {

			chargeState_ = ChargeState::Charging;
			chargeTime_ = 0.0f;

			// チャージ弾を生成
			chargeBullet_ = new PlayerBullet();

			chargeBullet_->Initialize(bulletModel_, GetWorldPosition(), forward_, 1.0f, 0);

			// 音がまだ鳴っていなければループ再生を開始
			if (!isChargingSoundPlaying_) {
				// 第2引数を true にしてループ再生
				chargeVoiceHandle_ = KamataEngine::Audio::GetInstance()->PlayWave(chargeSoundHandle_, true, 0.5f);
				isChargingSoundPlaying_ = true;
			}

		}

		break;

	case ChargeState::Charging:

		if (aim_->IsAttackHold()) {

			// チャージ
			chargeTime_ += 1.0f / 30.0f;

			if (chargeTime_ > kMaxChargeTime) {
				chargeTime_ = kMaxChargeTime;
			}

			// チャージ率
			float chargeRate = GetChargeRate();

			// 弾の大きさ
			const float kMinBulletScale = 1.0f;
			const float kMaxBulletScale = 3.0f;

			float bulletScale = kMinBulletScale + (kMaxBulletScale - kMinBulletScale) * chargeRate;

			// プレイヤーの前に表示
			chargeBullet_->SetPosition(GetWorldPosition() + forward_ * 3.0f);
			chargeBullet_->setScale({bulletScale, bulletScale, bulletScale});

			// 向きをプレイヤーの向きに合わせる
			chargeBullet_->GetWorldTransform().rotation_.y = worldTransform_.rotation_.y;

			// ワールド変換行列を更新
			WorldTransformUpdate(chargeBullet_->GetWorldTransform());

		} else {

			// ボタンを離した瞬間に発射
			Attack();

			// 射撃音を再生（単発再生: false）
			KamataEngine::Audio::GetInstance()->PlayWave(shotSoundHandle_, false, 0.4f);

			//発射時にチャージ音を停止
			if (isChargingSoundPlaying_) {
				KamataEngine::Audio::GetInstance()->StopWave(chargeVoiceHandle_);
				isChargingSoundPlaying_ = false;
			}


			// チャージ弾を削除
			delete chargeBullet_;
			chargeBullet_ = nullptr;

			// 攻撃のクールタイムをリセット
			attackCoolTime_ = 1.0f;

			// チャージ時間をリセット
			chargeTime_ = 0.0f;
			chargeState_ = ChargeState::None;
		}

		break;
	}

	// 弾の更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// コンボタイマー
	if (combo_ > 0) {

		// コンボ継続時間を数える
		comboTimer_ -= 1.0f / 30.0f;

		// コンボ継続時間を過ぎたら
		if (comboTimer_ <= 0.0f) {

			// コンボが途切れたらコンボ数をリセット
			combo_ = 0;
			comboTimer_ = 0.0f;
		}
	}

	// 無敵時間の処理
	if (isHit_) {
		kInvincibleTime -= 1.0f / 30.0f; // 30FPSで減算
	}

	// 無敵時間をリセット
	if (kInvincibleTime <= 0.0f) {
		isHit_ = false;
		kInvincibleTime = 2.0f;
	}

	// ワールド座標を取得
	Vector3 worldPos = GetWorldPosition();

#ifdef _DEBUG
	// キャラクターの座標を画面表示する
	ImGui::Begin("Player");
	ImGui::DragFloat3("Position", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("Rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("WorldPosition", &worldPos.x, 0.01f);
	ImGui::DragFloat3("angle", &angleZ, 0.01f);
	ImGui::End();
#endif // _DEBUG
}

void Player::OnEnemyHit() {

	// コンボを増やす
	combo_++;

	// コンボ時間をリセット
	comboTimer_ = kComboTime;
}

void Player::Draw() {

	// 弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(*camera_);
	}

	// チャージ中の弾
	if (chargeBullet_) {
		chargeBullet_->Draw(*camera_);
	}

	// 無敵時間中
	if (isHit_) {

		// 30フレーム周期で点滅
		if (static_cast<int>(kInvincibleTime * 30.0f) % 10 < 5) {
			return;
		}
	}

	// プレイヤーの描画
	model_->Draw(worldTransform_, *camera_);
}

// 移動処理
void Player::Move() {

	// ===== 上下移動 =====
	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.y += verticalSpeed_;
	}
	if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= verticalSpeed_;
	}

	// Y制限
	worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, minY_, maxY_);

	// ===== 回転入力 =====
	if (input_->PushKey(DIK_A)) {
		angleZ -= kRotateSpeed;
	} else if (input_->PushKey(DIK_D)) {
		angleZ += kRotateSpeed;
	}

	// ===== XZ 円運動 =====
	worldTransform_.translation_.x = std::cos(angleZ) * radius;

	worldTransform_.translation_.z = std::sin(angleZ) * radius;

	// ===== 向き更新 =====
	Vector3 bossPos = boss_->GetWorldPosition();
	Vector3 playerPos = worldTransform_.translation_;

	Vector3 toBoss = bossPos - playerPos;

	if (Length(toBoss) > 0.0001f) {
		toBoss = Normalize(toBoss);

		forward_ = toBoss;

		// Y軸だけ回転（地面に沿って向く）
		worldTransform_.rotation_.y = std::atan2(toBoss.x, toBoss.z) + 3.14159265f / 2.0f;
	}

	// ワールド変換行列の更新
	WorldTransformUpdate(worldTransform_);
}

void Player::Attack() {

	PlayerBullet* newBullet = new PlayerBullet();

	Ray ray = aim_->GetRayFromMouse();

	// プレイヤー位置
	Vector3 playerPos = GetWorldPosition();

	// ボス位置
	Vector3 bossPos = boss_->GetWorldPosition();

	// プレイヤー → ボス方向
	Vector3 normal = Normalize(bossPos - playerPos);

	// プレイヤーとボスの距離
	float kAimDistance = Length(bossPos - playerPos);

	// プレイヤーの前に照準平面を作る
	Vector3 planePoint = playerPos + normal * kAimDistance;

	// Rayと平面の交点
	float denominator = Dot(ray.direction, normal);

	if (std::abs(denominator) < 0.0001f) {
		delete newBullet;
		return;
	}

	float t = Dot(planePoint - ray.origin, normal) / denominator;

	if (t < 0.0f) {
		delete newBullet;
		return;
	}

	// 照準位置
	Vector3 targetPos = ray.origin + ray.direction * t;

	// プレイヤー → 照準位置
	Vector3 bulletDir = Normalize(targetPos - playerPos);

	// チャージ率
	float chargeRate = chargeTime_ / kMaxChargeTime;

	// 弾のサイズ
	const float kMinBulletScale = 1.0f;
	const float kMaxBulletScale = 3.0f;

	float bulletScale = kMinBulletScale + (kMaxBulletScale - kMinBulletScale) * chargeRate;

	// 弾速
	Vector3 bulletVelocity = bulletDir * kBulletSpeed;

	const int kMinDamage = 3;
	const int kMaxDamage = 20;

	int damage = static_cast<int>(kMinDamage + combo_ + (kMaxDamage - kMinDamage) * GetChargeRate());

	// 弾生成
	newBullet->Initialize(bulletModel_, playerPos, bulletVelocity, bulletScale, damage);

	// 向きをプレイヤーの向きに合わせる
	newBullet->GetWorldTransform().rotation_.y = worldTransform_.rotation_.y;

	bullets_.push_back(newBullet);
}

Vector3 Player::GetWorldPosition() const { return worldTransform_.translation_; }