#pragma once
#include <Scene.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


class MainMenu : public ngenius::Scene
{
public:
	explicit MainMenu(const std::string& name) : Scene(name) {}
	~MainMenu() override = default;
	MainMenu(const MainMenu& other) = delete;
	MainMenu(MainMenu&& other) = delete;
	MainMenu& operator=(const MainMenu& other) = delete;
	MainMenu& operator=(MainMenu&& other) = delete;

	void Initialise() override;
	void Activate() override {}
	void Deactivate() override {}
};

