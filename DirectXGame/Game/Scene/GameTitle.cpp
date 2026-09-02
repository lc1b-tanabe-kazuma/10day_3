#include "GameTitle.h"
#include "MyMath.h"
#include "SceneManager.h"

void GameTitle::Initialize() {
	//
	camera_.Initialize();

	worldTransform_.Initialize();

	// 照準の初期化
	aim_ = new Aim();
	aim_->Initialize(&camera_);

	// 進むボタンの初期化
	startButtonSpriteTH_ = TextureManager::Load("uvChecker.png");
	startButtonSprite_ = Sprite::Create(
	    startButtonSpriteTH_, startButtonPos, // 初期位置
	    Vector4(1.0f, 1.0f, 1.0f, 1.0f)       // 色
	);
	startButtonSprite_->SetSize(startButtonSize);
	startButtonSprite_->SetAnchorPoint({0.5f, 0.5f});
}

void GameTitle::Update() {

	// 照準の更新
	aim_->Update();

	Vector2 mousePos_ = aim_->GetWorldPosition();

	// スタートボタンの上にマウスがあるか
	bool isStartHover = IsMouseOver(mousePos_, startButtonPos, startButtonSize);

	if (isStartHover) {

		// ボタンの色を点滅させる
		buttonTimer_ += 1.0f / 30.0f; // タイマーを進める
		float alpha = (sin(buttonTimer_ * 3.14159f * 2.0f) + 1.0f) / 2.0f * 0.5f + 0.5f;
		startButtonSprite_->SetColor({1.0f, 1.0f, 1.0f, alpha});

		// 左クリック
		if (input_->IsTriggerMouse(0)) {

			//
			SceneManager::GetInstance()->ChangeScene("Game");
		}
	} else {
		// 色を元に戻す
		startButtonSprite_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	}
}

bool GameTitle::IsMouseOver(Vector2 mouse, Vector2 pos, Vector2 size) {

	return (mouse.x >= pos.x - size.x / 2 && mouse.x <= pos.x + size.x / 2 && mouse.y >= pos.y - size.y / 2 && mouse.y <= pos.y + size.y / 2);
}

void GameTitle::Draw() {

	// コマンドリストの取得
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw();

	Model::PostDraw();

	// UI描画前処理
	Sprite::PreDraw(commandList);

	// スタートボタンの描画
	startButtonSprite_->Draw();

	// 照準の描画
	aim_->Draw();

	// UI描画後処理
	Sprite::PostDraw();
}

GameTitle::~GameTitle() {
	delete aim_;
	delete startButtonSprite_;
}