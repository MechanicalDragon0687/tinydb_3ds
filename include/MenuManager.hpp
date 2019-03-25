#pragma once

#include <citro2d.h>
#include <vector>
#include <functional>
#include <memory>

class MenuManager {
private:
  C2D_TextBuf textBuffer;
  std::vector<C2D_Text> menuElementTexts;
  std::vector<std::function<void (void)>> menuElementCallbacks;

  uint cursorIndex = 0;
public:
  MenuManager();
  void Render();
  void Update(u32 kDown);
  void ClearMenu();
  void AddMenuItem(const char* text);
  void AddMenuItem(const char* text, std::function<void (void)>);
};
