#include "MenuManager.hpp"
#include <cstdio>
#include <citro2d.h>
#include <vector>
#include <3ds.h>
#include <functional>
#include <memory>

MenuManager::MenuManager() {
  this->textBuffer = C2D_TextBufNew(4096);
  this->menuElements = new std::vector<MenuEntry>;
}

void MenuManager::AddMenuItem(const char* text, std::function<void (void)> callback) {
  C2D_Text currentText;
  MenuEntry newMenuEntry;
  C2D_TextParse(&currentText, this->textBuffer, text);
  C2D_TextOptimize(&currentText);
  newMenuEntry.text = currentText;
  newMenuEntry.callback = callback;
  this->menuElements->push_back(newMenuEntry);
}

void MenuManager::ClearMenu() {
  C2D_TextBufClear(this->textBuffer);
  this->cursorIndex = 0;
  this->menuElements = new std::vector<MenuEntry>;
  //this->menuElements.reserve(1000);
}

void MenuManager::Render() {
  float outerPadding = 5.0f;
  float innerPadding = 5.0f;

  float yOffset = outerPadding + 0.0f;
  for(uint i = 0; i < this->menuElements->size(); i++) {
    float height;
    float width;

    C2D_TextGetDimensions(&this->menuElements->at(i).text, 0.5f, 0.5f, &width, &height);

    C2D_DrawRectSolid(outerPadding, yOffset, 0.5f, 320.0f - outerPadding * 2, height + innerPadding * 2, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));

    if (i == this->cursorIndex) {
      C2D_DrawText(&this->menuElements->at(i).text, C2D_WithColor, outerPadding + innerPadding, yOffset + innerPadding, 0.5f, 0.5f, 0.5f, C2D_Color32f(1.0f,0.0f,0.0f,1.0f));
    } else {
      C2D_DrawText(&this->menuElements->at(i).text, C2D_WithColor, outerPadding + innerPadding, yOffset + innerPadding, 0.5f, 0.5f, 0.5f, C2D_Color32f(0.0f,0.0f,0.0f,1.0f));
    }
    yOffset += height + outerPadding + (innerPadding * 2);
  }
}

void MenuManager::Update(u32 kDown) {
  if (kDown & KEY_DOWN) {
    this->cursorIndex += 1;
    if (this->cursorIndex >= this->menuElements->size()) {
      this->cursorIndex = 0;
    }
  }

  if (kDown & KEY_UP) {
    if (this->cursorIndex == 0) {
      this->cursorIndex = this->menuElements->size() - 1;
    } else {
      this->cursorIndex -= 1;
    }
  }

  if (kDown & KEY_A) {
    if (this->menuElements->at(this->cursorIndex).callback != nullptr) {
      this->menuElements->at(this->cursorIndex).callback();
    }
  }
}
