#include <iostream>

#include "raylib.h"

#include "Browser.hh"
#include "Forward.hh"
#include "Util.hh"

#define HSTEP 13u
#define VSTEP 18u

namespace MK {
Browser::Browser() {
  InitWindow(WIDTH, HEIGHT, "MK Browser");

  SetTargetFPS(60);
}

void Browser::load(URL const &url) {
  auto maybe_body = url.request();
  m_text_content = lex(maybe_body.value_or(""));
}

void Browser::run() {
  m_font = LoadFont("resources/fonts/SF-Pro-Text-Regular.ttf");
  while (!WindowShouldClose()) {
    update_draw_frame();
  }

  close_window();
}

void Browser::update_draw_frame() const {

  BeginDrawing();

  ClearBackground(RAYWHITE);

  auto cursor_x = HSTEP;
  auto cursor_y = VSTEP;
  auto c_str = m_text_content.c_str();
  auto start_c_str = 0;

  // TODO: Fix this, dont skip initial spaces, newlines and tabs
  while (c_str[start_c_str] == '\n' || c_str[start_c_str] == '\t' ||
         c_str[start_c_str] == ' ')
    start_c_str += 1;

  while (c_str[start_c_str] != '\0') {
    if (c_str[start_c_str] == '\n') {
      cursor_x = HSTEP;
      cursor_y += VSTEP;
      start_c_str += 1;
      continue;
    }

    auto current_char = (char *)malloc(2);

    strncpy(current_char, c_str + start_c_str, 1);
    current_char[1] = '\0';

    DrawTextEx(m_font, current_char,
               {static_cast<float>(cursor_x), static_cast<float>(cursor_y)}, 16,
               1.0, BLACK);

    cursor_x += HSTEP;
    start_c_str += 1;

    if (cursor_x >= WIDTH - HSTEP) {
      cursor_x = HSTEP;
      cursor_y += VSTEP;
    }

    free(current_char);
  }

  EndDrawing();
}

void Browser::close_window() const { CloseWindow(); }
} // namespace MK
