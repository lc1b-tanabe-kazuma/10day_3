#pragma once
#include "KamataEngine.h"
#include "input/Input.h"
struct Ray {
	KamataEngine::Vector3 origin;
	KamataEngine::Vector3 direction;
};

class Aim {
public:
	// 初期化
	void Initialize(KamataEngine::Camera* camera);

	// 更新
	void Update();

	// 描画
	void Draw();

	KamataEngine::Vector3 GetForward();

	// 攻撃のゲッターとセッター
	bool IsAttac() const { return isAttac_; }

	Ray GetRayFromMouse();

	// 座標のゲッター
	KamataEngine::Vector2 GetWorldPosition() const { return mousePos_; }

	~Aim();

private:
	// テクスチャーハンドル
	uint32_t textureHandle_ = 0u;

	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Sprite* sprite_ = nullptr;

	uint32_t spriteGH_ = 0;

	// キーボード入力
	KamataEngine::Input* input_ = nullptr;

	// マウスの座標
	KamataEngine::Vector2 mousePos_;

	// ウィンドウの幅と高さ
	float width = 1280.0f;
	float height = 720.0f;

	// 攻撃
	bool isAttac_ = false;

	Ray ray_;

	// クールタイム
	float coolTime_ = 0.3f;
};