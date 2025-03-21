#include <cassert>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>

#include "Browser.hh"
#include "Forward.hh"
#include "Util.hh"

#define HSTEP 13.0f
#define VSTEP 18.0f
#define SCROLL_STEP 100u

namespace MK {
Browser::Browser()
{
    InitWindow(WIDTH, HEIGHT, "MK Browser");
    SetTargetFPS(60);
    m_font = LoadFont("resources/fonts/SF-Pro-Text-Regular.ttf");
}

void Browser::load(URL const& url)
{
    auto maybe_body = url.request();
    m_text_content = lex(maybe_body.value_or(""));
    compute_layout(m_text_content);
}

static std::vector<std::string> split_to_chunks(std::string const& text_content)
{
    std::vector<std::string> chunks;
    std::string current_chunk;

    for (char c : text_content) {
        if (c == ' ' || c == '\n') {
            if (!current_chunk.empty()) {
                chunks.push_back(current_chunk);
                current_chunk.clear();
            }
        } else {
            current_chunk += c;
        }
    }

    if (!current_chunk.empty()) {
        chunks.push_back(current_chunk);
    }

    return chunks;
}

void Browser::compute_layout(std::string const& text_content)
{
    auto cursor_x = HSTEP;
    auto cursor_y = VSTEP;

    for (auto chunk : split_to_chunks(text_content)) {
        assert(chunk.find(" ") == std::string::npos);

        auto width
            = MeasureTextEx(m_font, chunk.c_str(), m_font_size, m_spacing).x;

        LayoutText layout_text = {
            .position = { cursor_x, cursor_y },
            .character = chunk,
        };

        m_display_list.push_back(layout_text);
        cursor_x
            += width + MeasureTextEx(m_font, " ", m_font_size, m_spacing).x;

        if (cursor_x + width >= WIDTH - HSTEP) {
            cursor_x = HSTEP;
            // cursor_y += (m_font.baseSize + m_font.glyphPadding) * 1.25;
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
            Vector2Add(layout_text.position, { 0, -m_scroll }), m_font_size,
            m_spacing, BLACK);
    }
}

void Browser::run()
{
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
