#include "SceneManager.h"
#include "GameClear.h"
#include "GameScene.h"
#include "GameTitle.h"

using namespace std;
using namespace KamataEngine;

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance() {
	if (instance == nullptr) {
		instance = new SceneManager;
	}
	return instance;
}

void SceneManager::Initialize() {
	// Inputのインスタンスを取得
	input_ = Input::GetInstance();

	// ===== 修正ポイント：タイトルとゲーム、両方のシーンを最初に登録する =====
	// ※Title.h 内のクラス名が「Title」の場合
	RegisterScene("Title", std::make_unique<GameTitle>(input_));
	RegisterScene("Game", std::make_unique<GameScene>(input_));
	RegisterScene("GameClear", std::make_unique<GameClear>(input_));

	transition_ = SceneTransition::GetInstance();
	transition_->Initialize();

	// 最初のシーンにタイトルを設定 (ChangeSceneで設定しないことでフェードをスキップ)
	auto it = scenes_.find("Title");
	if (it != scenes_.end()) {
		currentScene_ = it->second.get();
		currentScene_->Initialize();
	}
}

void SceneManager::RegisterScene(const std::string& name, std::unique_ptr<SceneBase> scene) { scenes_[name] = std::move(scene); }

void SceneManager::ChangeScene(const std::string& name) {

	// すでにトランジション中ならシーン切り替えを受け付けない（多重遷移防止）
	if (transition_ && transition_->IsTransitioning()) {
		return;
	}

	// フェードトランジション開始
	transition_->StartTransition("Fade", [this, name]() {
		// ===== シーンを毎回新しく生成する =====
		if (name == "Game") {
			scenes_[name] = std::make_unique<GameScene>(input_);
		} else if (name == "Title") {
			// ※Title.hのクラス名に合わせて「Title」または「GameTitle」にしてください
			scenes_[name] = std::make_unique<GameTitle>(input_);
		} else if (name == "GameClear") {
			scenes_[name] = std::make_unique<GameClear>(input_);
		}

		// ===== 新しいシーンに切り替え =====
		currentScene_ = scenes_[name].get();

		// 初期化
		currentScene_->Initialize();

		// 1回Updateを通して状態を確定させる
		currentScene_->Update();
	});
}

void SceneManager::Update() {
	bool isTransitioning = transition_ && transition_->IsTransitioning();

	if (currentScene_) {
		currentScene_->SetActive(!isTransitioning);
		currentScene_->Update();
	}

	// トランジション進行中はトランジションのUpdateのみ
	if (transition_ && transition_->IsTransitioning()) {
		transition_->Update();
	}
}

void SceneManager::Draw() {
	if (currentScene_) {
		currentScene_->Draw();
	}

	if (transition_ && transition_->IsTransitioning()) {
		transition_->Draw();
	}
}

void SceneManager::Finalize() {
	delete instance;
	instance = nullptr;
}