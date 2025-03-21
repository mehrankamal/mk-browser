#include <iostream>
#include <string>

#include <raylib.h>
#include <raymath.h>

#include "Browser.hh"
#include "Forward.hh"
#include "Util.hh"

#define HSTEP 13u
#define VSTEP 18u
#define SCROLL_STEP 100u

namespace MK {
Browser::Browser()
{
    InitWindow(WIDTH, HEIGHT, "MK Browser");
    SetTargetFPS(60);
}

void Browser::load(URL const& url)
{
    auto maybe_body = url.request();
    m_text_content = lex(maybe_body.value_or(""));
    compute_layout(m_text_content);
}

void Browser::compute_layout(std::string const& text_content)
{
    auto cursor_x = HSTEP;
    auto cursor_y = VSTEP;

    for (size_t i = 0; i < text_content.length(); ++i) {
        if (text_content[i] == '\n') {
            cursor_x = HSTEP;
            cursor_y += VSTEP;
            continue;
        }

        LayoutText layout_text = {
            .position
            = { static_cast<float>(cursor_x), static_cast<float>(cursor_y) },
            .character = std::string(1, text_content[i]),
        };

        m_display_list.push_back(layout_text);
        cursor_x += HSTEP;

        if (cursor_x >= WIDTH - HSTEP) {
            cursor_x = HSTEP;
            cursor_y += VSTEP;
        }
    }
}

void Browser::draw_layout() const
{
    for (auto const& layout_text : m_display_list) {
        bool is_offscreen_content = layout_text.position.y + VSTEP < m_scroll
            || layout_text.position.y > m_scroll + HEIGHT;

        if (is_offscreen_content)
            continue;

        DrawTextEx(m_font, layout_text.character.c_str(),
            Vector2Add(layout_text.position, { 0, -m_scroll }), 13, 0, BLACK);
    }
}

void Browser::run()
{
    m_font = LoadFont("resources/fonts/SF-Pro-Text-Regular.ttf");
    while (!WindowShouldClose()) {
        update_draw_frame();
    }

    close_window();
}

void Browser::update_state()
{
    if (IsKeyDown(KEY_DOWN)) {
        m_scroll += SCROLL_STEP;
    }

    if (IsKeyDown(KEY_UP)) {
        m_scroll -= SCROLL_STEP;
    }
}

void Browser::update_draw_frame()
{
    update_state();

    BeginDrawing();

    ClearBackground(RAYWHITE);

    draw_layout();

    EndDrawing();
}

void Browser::close_window() const { CloseWindow(); }
} // namespace MK
