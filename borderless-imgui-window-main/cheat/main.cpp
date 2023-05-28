#include "gui.h"
#include <thread>
#include "globals.h"
#include "hacks.h"




int __stdcall wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow)
{
	Memory mem{ "csgo.exe" };

	globals::clientAddress = mem.GetModuleAddress("client.dll");

	globals::engineAddress = mem.GetModuleAddress("engine.dll");

	std::thread(hacks::VisualsThread, mem).detach();

	std::thread(hacks::Movement, mem).detach();

	std::thread(hacks::Triggerbot, mem).detach();

	std::thread(hacks::Aimbot, mem).detach();

	std::thread(hacks::NoRecoil, mem).detach();

	// create gui
	gui::CreateHWindow("Cheat Menu");
	gui::CreateDevice();
	gui::CreateImGui();

	while (gui::isRunning)
	{
		gui::BeginRender();
		gui::Render();
		gui::EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	// destroy gui
	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();

	return EXIT_SUCCESS;
}
