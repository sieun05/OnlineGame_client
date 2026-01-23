#pragma once
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