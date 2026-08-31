#pragma once
#include "KamataEngine.h"
#include "SceneBase.h"

class GameClear : public SceneBase {
public:
	GameClear(KamataEngine::Input* input) : SceneBase(input) {};

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	~GameClear() override;

private:
	// カメラ
	KamataEngine::Camera camera_;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
};