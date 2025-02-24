// #include <iostream>

// #include "url-parser.h"
// #include "util.h"

// int main(int argc, char **argv) {
//   if (argc > 1) {
//     MK::UrlParser url_parser = MK::UrlParser(argv[1]);
//     auto maybe_url = url_parser.parse();

//     if (maybe_url.has_value()) {
//       MK::load(maybe_url.value());
//     } else {
//       std::cerr << "Error parsing URL" << std::endl;
//     }
//   } else {
//     std::cerr << "Exected URL, none found" << std::endl;
//   }
//   return 0;
// }

#include <raylib.h>

#define WIDTH 800u
#define HEIGHT 600u

void UpdateDrawFrame(void);
int main() {
  InitWindow(WIDTH, HEIGHT, "MK Browser");

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    UpdateDrawFrame();
  }

  CloseWindow();

  return 0;
}

void UpdateDrawFrame(void) {
  BeginDrawing();

  ClearBackground(RAYWHITE);

  DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
  DrawRectangle(0, 0, 100, 100, BLACK);

  EndDrawing();
}