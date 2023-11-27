#ifndef DTO_H
#define DTO_H

#include <string>

struct SpawnerDTO {
	std::string id = "";
	float posX = 0.f;
	float posY = 0.f;
	float velX = 0.f;
	float velY = 0.f;
	float interval = 0.f;
	bool active = false;
	bool visible = false;
};

#endif
