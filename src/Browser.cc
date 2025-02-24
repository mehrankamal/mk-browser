#include <iostream>

#include "raylib.h"

#include "Browser.hh"
#include "Forward.hh"
#include "Util.hh"

namespace MK {
Browser::Browser() {
  InitWindow(WIDTH, HEIGHT, "MK Browser");

  SetTargetFPS(60);
}

void Browser::load(URL const &url) {
  auto maybe_body = url.request();
  m_text_content = lex(maybe_body.value_or(""));
  std::cerr << "Body text: \n" << m_text_content << std::endl;
}

void Browser::run() const {
  while (!WindowShouldClose()) {
    update_draw_frame();
  }

  close_window();
}

void Browser::update_draw_frame() const {

  BeginDrawing();

  ClearBackground(RAYWHITE);

  DrawText(m_text_content.c_str(), 10, 10, 12, BLACK);

  EndDrawing();
}

void Browser::close_window() const { CloseWindow(); }
} // namespace MK
