#pragma once
#include <Scene.h>
#include <string>

class LevelScene final : public ngenius::Scene
{
public:
	explicit LevelScene(const std::string& name) : Scene(name) {}
	~LevelScene() override = default;
	LevelScene(const LevelScene& other) = delete;
	LevelScene(LevelScene&& other) = delete;
	LevelScene& operator=(const LevelScene& other) = delete;
	LevelScene& operator=(LevelScene&& other) = delete;

	void Initialise() override;
	void Activate() override {}
	void Deactivate() override {}
};

