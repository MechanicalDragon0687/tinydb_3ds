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
	fsInit();
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	uint8_t currentPathBuffer[256] = {"\0"};
	FS_Path archivePath = fsMakePath(PATH_UTF16, "/");

	MenuManager menuManager;
	std::map<std::string, std::function<void (void)>> menuHandlers;

	menuHandlers.insert(std::make_pair("MainMenu", [&menuManager, &menuHandlers, &currentPathBuffer, &archivePath]() {
		menuManager.ClearMenu();
		currentPathBuffer[0] = 0;
		archivePath = fsMakePath(PATH_UTF16, "/");
		menuManager.AddMenuItem("Browse TinyDB", menuHandlers["BrowseMenu"]);
		menuManager.AddMenuItem("Browse SD", menuHandlers["SDMenu"]);
		menuManager.AddMenuItem("Settings", menuHandlers["SettingMenu"]);
	}));

	menuHandlers.insert(std::make_pair("SDMenu", [&menuManager, &menuHandlers, &archivePath, &currentPathBuffer]() {
		menuManager.ClearMenu();

		FS_ArchiveID archiveId = ARCHIVE_SDMC;
		FS_Archive archive;
		Handle handle;
		u32 resultOpen;

		if(R_FAILED(FSUSER_OpenArchive(&archive, archiveId, archivePath))) {
			printf("Failed to open file listing archive.\n");
			return;
		}

		FSUSER_OpenDirectory(&handle, archive, archivePath);
		u32 result;
    do {
        FS_DirectoryEntry item;
        FSDIR_Read(handle, &result, 1, &item);

        if (result == 1)
        {
					uint8_t utf8Name[256] = {"\0"};
					utf16_to_utf8(utf8Name, item.name, 256);

					if (item.attributes & FS_ATTRIBUTE_DIRECTORY) {
						menuManager.AddMenuItem((char*)utf8Name, [&item, &archivePath, &currentPathBuffer, &menuHandlers, utf8Name]() {
							snprintf((char*)currentPathBuffer, 256, "%s/%s", currentPathBuffer, utf8Name);
							printf("%s\n", currentPathBuffer);

							uint8_t utf16Path[512] = {"\0"};
							utf8_to_utf16((uint16_t*)utf16Path, currentPathBuffer, 256);

							archivePath = fsMakePath(PATH_UTF16, &utf16Path);
							menuHandlers["SDMenu"]();
						});
					} else {
						menuManager.AddMenuItem((char*)utf8Name, []() {
							printf("file clicked \n");
						});
					}
        } else {
					break;
				}
    } while(result);

		menuManager.AddMenuItem("Back to Main Menu", menuHandlers["MainMenu"]);
		FSDIR_Close(handle);
		FSUSER_CloseArchive(archive);
	}));

	menuHandlers.insert(std::make_pair("BrowseMenu", [&menuManager, &menuHandlers]() {
		menuManager.ClearMenu();
		menuManager.AddMenuItem("Back to Main Menu", menuHandlers["MainMenu"]);
		menuManager.AddMenuItem("Back 12315656", menuHandlers["MainMenu"]);
		menuManager.AddMenuItem("Back lel", menuHandlers["MainMenu"]);
		menuManager.AddMenuItem("Back to Main Menu", menuHandlers["MainMenu"]);
		menuManager.AddMenuItem("Back to Main Menu", menuHandlers["MainMenu"]);
	}));

	menuHandlers.insert(std::make_pair("SettingMenu", [&menuManager, &menuHandlers]() {
		menuManager.ClearMenu();
		menuManager.AddMenuItem("teest", menuHandlers["MainMenu"]);
		menuManager.AddMenuItem("Back to Main Menu", menuHandlers["MainMenu"]);
		menuManager.AddMenuItem("Back to Main Menu", menuHandlers["MainMenu"]);
		menuManager.AddMenuItem("Back to Main Menu", menuHandlers["MainMenu"]);
	}));

	menuHandlers["MainMenu"]();

	while (aptMainLoop())
	{
		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		if (kDown & KEY_B)
			menuHandlers["MainMenu"]();

		menuManager.Update(kDown);

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		C2D_TargetClear(bottom, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_SceneBegin(bottom);

		menuManager.Render();

		C3D_FrameEnd(0);
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	fsExit();

		menuHandlers["MainMenu"]();
			menuHandlers["SDMenu"]();
				menuHandlers["SettingMenu"]();
	return 0;
}
