#pragma once
#include <map>
#include <mutex>
#include <vector>
#include "head.h"


struct Player {
	Player() = default;
	Player(Vec2 pos, float width, float height, RGB color) 
		: pos(pos), width(width), height(height), color(color) {}
	~Player() = default;
	Player(const Player& other) = default;
	Player& operator=(const Player& other) = default;

	Vec2 pos;
	float width, height;
	RGB color;
};

int myPlayerId = -1;
Player player{ Vec2{0.0f, 0.0f}, 0.5f, 0.5f, RGB{0.0f, 0.0f, 1.0f} };
map<string, Player> otherPlayers; //key = "playerId"
std::mutex playersMutex;

void Set_MyPlayerID(int id)
{
	myPlayerId = id;
}

void HandleStateMessage(const string& msg) 
{
	//형식: STATE|playerId|x|y|width|height|r|g|b

	vector<string> tokens;
	string temp;
	for (char c : msg) {
		if (c == '|') {
			tokens.push_back(temp);
			temp.clear();
		}
		else {
			temp.push_back(c);
		}
	}
	tokens.push_back(temp);

	if(tokens.size()<4)
		return;
	if (tokens[0] != "STATE")
		return;

	int id = atoi(tokens[1].c_str());
	float x = (float)atof(tokens[2].c_str());
	float y = (float)atof(tokens[3].c_str());
	

	if (id == myPlayerId){
		std::lock_guard<std::mutex> lock(playersMutex);
		player.pos = Vec2{ x, y }; //내 플레이어 위치 업데이트
		return; //내 플레이어는 무시
	}

	std::lock_guard<std::mutex> lock(playersMutex);

	auto it = otherPlayers.find(tokens[1]);
	if (it == otherPlayers.end()) {
		//새 플레이어 추가
		cout << "New player joined: " << id << " at (" << x << ", " << y << ")\n";
		otherPlayers[tokens[1]] = Player{ Vec2{x, y}, 0.5f, 0.5f, RGB{0.0f, 0.0f, 1.0f} };
	}
	else {
		//기존 플레이어 위치 업데이트
		it->second.pos = Vec2{ x, y };
	}
}