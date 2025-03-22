#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>

#include "Browser.hh"
#include "Config.hh"
#include "Forward.hh"
#include "HtmlParser.hh"
#include "HtmlToken.hh"
#include "Layout.hh"

namespace LibBrowser {
Browser::Browser()
{
    InitWindow(WIDTH, HEIGHT, "MK Browser");
    SetTargetFPS(60);
}

Browser::~Browser() { delete m_html_parser; }

void Browser::load(URL const& url)
{
    auto maybe_body = url.request();
    m_html_parser = new HtmlParser(maybe_body.value_or(""));
    auto tokens = html_parser().lex();
    m_layout = new Layout(tokens);
}

void Browser::draw_layout() const
{
    for (auto const& layout_text : layout().display_list()) {
        bool is_offscreen_content = layout_text.position.y + VSTEP < m_scroll
            || layout_text.position.y > m_scroll + HEIGHT;

        if (is_offscreen_content)
            continue;

        DrawTextEx(layout_text.font, layout_text.content.c_str(),
            Vector2Add(layout_text.position, { 0, -m_scroll }),
            layout().font_size(), layout().spacing(), BLACK);
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
} // namespace LibBrowser
