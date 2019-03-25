#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <map>

#include <citro2d.h>
#include <string>

#include "MenuManager.hpp"

int main(int argc, char* argv[])
{
	romfsInit();
	gfxInitDefault();

	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	MenuManager menuManager;
	std::map<std::string, std::function<void (void)>> menuHandlers;
	menuHandlers.insert(std::make_pair("MainMenu", [&menuManager, &menuHandlers]() {
		menuManager.ClearMenu();
		menuManager.AddMenuItem("Browse1234 Category", menuHandlers["BrowseMenu"]);
	}));

	menuHandlers.insert(std::make_pair("BrowseMenu", [&menuManager, &menuHandlers]() {
		menuManager.ClearMenu();
		menuManager.AddMenuItem("Category 1");
		menuManager.AddMenuItem("Category 2");
		menuManager.AddMenuItem("Browsee", menuHandlers["MainMenu"]);
	}));

	menuHandlers["MainMenu"]();

	while (aptMainLoop())
	{
		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		menuManager.Update(kDown);

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		C2D_TargetClear(bottom, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_SceneBegin(bottom);

		menuManager.Render();

		C2D_TargetClear(top, C2D_Color32f(255.0f, 0.0f, 0.0f, 1.0f));
		C3D_FrameDrawOn(top);
		C2D_SceneBegin(top);

		C3D_FrameEnd(0);
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}
