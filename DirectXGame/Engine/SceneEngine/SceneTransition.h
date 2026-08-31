#pragma once
#include "TransitionBase.h"
#include <functional>
#include <map>
#include <memory>
#include <string>

class SceneTransition {
public:
	// シングルトンインスタンス取得
	static SceneTransition* GetInstance();

	void Initialize();
	void RegisterTransition(const std::string& name, std::unique_ptr<TransitionBase> transition);
	void StartTransition(const std::string& name, std::function<void()> onTransition = nullptr);

	// 更新
	void Update();

	// 描画
	void Draw();

	// トランジションしているか
	bool IsTransitioning() const;

private:
	std::map<std::string, std::unique_ptr<TransitionBase>> transitions_;
	std::unique_ptr<TransitionBase> currentTransition_;
	static SceneTransition* instance;
	SceneTransition() = default;
	~SceneTransition() = default;
	SceneTransition(const SceneTransition&) = delete;
	SceneTransition& operator=(const SceneTransition&) = delete;
};