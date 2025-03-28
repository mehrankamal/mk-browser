#include <iostream>
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>

#include "Browser.hh"
#include "Config.hh"
#include "Forward.hh"
#include "HtmlNode.hh"
#include "HtmlParser.hh"
#include "Layout.hh"

namespace LibBrowser {
Browser::Browser()
{
    InitWindow(WIDTH, HEIGHT, "MK Browser");
    SetTargetFPS(60);
}

void Browser::load(URL const& url)
{
    auto maybe_body = url.request();
    m_html_parser = std::make_unique<HtmlParser>(maybe_body.value_or(""));
    auto tree = html_parser().parse();
    m_layout = std::make_unique<Layout>(tree);
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
            layout_text.size, layout().spacing(), BLACK);
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
