#include "LoggingSoundService.h"

#include <iostream>

void LoggingSoundService::PlaySound(const std::string& soundFile, int)
{
	std::cout << "Playing: " << soundFile << std::endl;
}

void LoggingSoundService::Update()
{}
