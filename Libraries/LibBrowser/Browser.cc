#include <cassert>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>

#include "Browser.hh"
#include "Forward.hh"
#include "HtmlParser.hh"
#include "HtmlToken.hh"

#define HSTEP 13.0f
#define VSTEP 18.0f
#define SCROLL_STEP 100u

#define LAYOUT_DEBUG 1

namespace MK {
Browser::Browser()
{
    InitWindow(WIDTH, HEIGHT, "MK Browser");
    SetTargetFPS(60);
    load_font_family();
}

void Browser::load_font_family() { m_font_family.load_variants(); }

void Browser::load(URL const& url)
{
    auto maybe_body = url.request();
    m_html_parser = new HtmlParser(maybe_body.value_or(""));
    m_tokens = html_parser().lex();
    compute_layout();
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

static FontVariant calculate_variant(
    Browser::FontWeight const& weight, Browser::FontStyle const& style)
{
    if (weight == Browser::FontWeight::Normal
        && style != Browser::FontStyle::Italic) {
        return FontVariant::Regular;
    } else if (weight == Browser::FontWeight::Bold
        && style != Browser::FontStyle::Italic) {
        return FontVariant::Bold;
    } else if (weight == Browser::FontWeight::Normal
        && style == Browser::FontStyle::Italic) {
        return FontVariant::RegularItalic;
    } else {
        return FontVariant::BoldItalic;
    }
}

void Browser::compute_layout()
{
    auto cursor_x = HSTEP;
    auto cursor_y = VSTEP;

    auto weight = FontWeight::Normal;
    auto style = FontStyle::Normal;

    for (auto token : tokens()) {
        auto text_content = token.text_content();
        auto font_variant = calculate_variant(weight, style);
        auto current_font = font_family().variants().at(font_variant);
#ifdef LAYOUT_DEBUG
        std::cerr << "Getting font variant: " << to_string(font_variant)
                  << std::endl;
        std::cerr << "Token Text: " << text_content << std::endl;
        std::cerr << "Token Type: " << static_cast<u32>(token.type())
                  << std::endl;
#endif

        if (token.type() == HtmlToken::Type::Text) {
            for (auto chunk : split_to_chunks(text_content)) {
                assert(chunk.find(" ") == std::string::npos);

                auto width = MeasureTextEx(
                    current_font, chunk.c_str(), m_font_size, m_spacing)
                                 .x;

                LayoutText layout_text = { .position = { cursor_x, cursor_y },
                    .content = chunk,
                    .style = { .style = style, .weight = weight },
                    .font = current_font };

                m_display_list.push_back(layout_text);
                cursor_x += width
                    + MeasureTextEx(current_font, " ", m_font_size, m_spacing)
                          .x;

                if (cursor_x + width >= WIDTH - HSTEP) {
                    cursor_x = HSTEP;
                    cursor_y
                        += (current_font.baseSize + current_font.glyphPadding)
                        * 1.25;
                }
            }
        } else {

            if (text_content == "i") {
                style = FontStyle::Italic;
            } else if (text_content == "/i") {
                style = FontStyle::Normal;
            } else if (text_content == "b") {
                weight = FontWeight::Bold;
            } else if (text_content == "/b") {
                weight = FontWeight::Normal;
            }
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

        DrawTextEx(layout_text.font, layout_text.content.c_str(),
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
