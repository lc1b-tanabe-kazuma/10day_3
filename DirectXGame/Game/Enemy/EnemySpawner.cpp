#define NOMINMAX
#include "EnemySpawner.h"
#include <cassert>
#include <cstdlib>
#include <fstream>

using namespace std;
using namespace KamataEngine;

void EnemySpawner::Initialize(Model* modelEnemy, Model* modelBullet, Camera* camera, Player* player, list<Enemy*>& enemies) {
	modelEnemy_ = modelEnemy;
	modelBullet_ = modelBullet;
	camera_ = camera;
	player_ = player;
	enemies_ = &enemies;
}

void EnemySpawner::LoadPopData(const string& filePath) {

	ifstream file;
	file.open(filePath);

#ifdef DEBUG
	assert(file.is_open());
#endif

	popCommands_ << file.rdbuf();

	file.close();
}

void EnemySpawner::Update() {

	// 待機処理
	if (isWaiting_) {

		waitTimer_--;

		if (waitTimer_ <= 0) {
			isWaiting_ = false;
		}

		return;
	}

	string line;

	while (getline(popCommands_, line)) {

		stringstream lineStream(line);

		string word;

		getline(lineStream, word, ',');

		// コメント
		if (word.find("//") == 0) {
			continue;
		}

		// POP
		if (word.find("POP") == 0) {
			ExecutePop(lineStream);
		}

		// WAIT
		else if (word.find("WAIT") == 0) {
			ExecuteWait(lineStream);
			break;
		}
	}
}

void EnemySpawner::ExecutePop(stringstream& lineStream) {

	string word;

	// X
	getline(lineStream, word, ',');
	float x = static_cast<float>(atof(word.c_str()));

	// Y
	getline(lineStream, word, ',');
	float y = static_cast<float>(atof(word.c_str()));

	// Z
	getline(lineStream, word, ',');
	float z = static_cast<float>(atof(word.c_str()));

	// 移動ベクトル X
	getline(lineStream, word, ',');
	float vx = static_cast<float>(atof(word.c_str()));

	// 移動ベクトル Y
	getline(lineStream, word, ',');
	float vy = static_cast<float>(atof(word.c_str()));

	// 移動ベクトル Z
	getline(lineStream, word, ',');
	float vz = static_cast<float>(atof(word.c_str()));

	// 敵生成
	Enemy* newEnemy = new Enemy();

	newEnemy->Initialize(modelEnemy_, camera_, modelBullet_, player_);

	newEnemy->SetPosition(Vector3(x, y, z));
	newEnemy->SetMoveVector(Vector3(vx, vy, vz));

	enemies_->push_back(newEnemy);
}

void EnemySpawner::ExecuteWait(stringstream& lineStream) {

	string word;

	getline(lineStream, word, ',');

	int32_t waitFrame = atoi(word.c_str());

	isWaiting_ = true;
	waitTimer_ = waitFrame;
}