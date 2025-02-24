#include "raylib.h"

#include "Browser.hh"
#include "Forward.hh"

namespace MK {
Browser::Browser() {
  InitWindow(WIDTH, HEIGHT, "MK Browser");

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    update_draw_frame();
  }

  close_window();
}

void Browser::update_draw_frame() {
  BeginDrawing();

  ClearBackground(RAYWHITE);

  DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
  DrawRectangle(0, 0, 100, 100, BLACK);

  EndDrawing();
}

void Browser::close_window() { CloseWindow(); }
} // namespace MK
