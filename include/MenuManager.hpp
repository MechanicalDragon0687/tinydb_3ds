#pragma once

#include <citro2d.h>
#include <vector>
#include <functional>
#include <memory>

struct MenuEntry {
  C2D_Text text;
  std::function<void()> callback;
};

class MenuManager {
private:
  C2D_TextBuf textBuffer;
  std::vector<MenuEntry>* menuElements;

  uint cursorIndex = 0;
public:
  MenuManager();
  void Render();
  void Update(u32 kDown);
  void ClearMenu();
  void AddMenuItem(const char* text);
  void AddMenuItem(const char* text, std::function<void (void)>);
};
