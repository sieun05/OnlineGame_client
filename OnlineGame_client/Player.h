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

map<string, Player> otherPlayers; //key = "ip:port"
std::mutex playersMutex;

void HandleStateMessage(const string& msg) 
{
	//형식: STATE|ip:port|x|y|width|height|r|g|b

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

	if(tokens.size()!=4)
		return;
	if (tokens[0] != "STATE")
		return;

	string id = tokens[1];
	float x = (float)atof(tokens[2].c_str());
	float y = (float)atof(tokens[3].c_str());

	std::lock_guard<std::mutex> lock(playersMutex);

	auto it = otherPlayers.find(id);
	if (it == otherPlayers.end()) {
		//새 플레이어 추가
		otherPlayers[id] = Player{ Vec2{x, y}, 0.5f, 0.5f, RGB{0.0f, 0.0f, 1.0f} };
	}
	else {
		//기존 플레이어 위치 업데이트
		it->second.pos.x = x;
		it->second.pos.y = y;
	}
}