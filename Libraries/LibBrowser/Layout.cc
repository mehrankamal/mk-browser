#include <cassert>

#include "Config.hh"
#include "FontFamily.hh"
#include "HtmlToken.hh"
#include "Layout.hh"

namespace LibBrowser {

Layout::Layout(std::vector<HtmlToken> const& tokens)
{
    m_font_family.load_variants();
    layout_content(tokens);
    flush();
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
    Layout::FontWeight const& m_weight, Layout::FontStyle const& m_style)
{
    if (m_weight == Layout::FontWeight::Normal
        && m_style != Layout::FontStyle::Italic) {
        return FontVariant::Regular;
    } else if (m_weight == Layout::FontWeight::Bold
        && m_style != Layout::FontStyle::Italic) {
        return FontVariant::Bold;
    } else if (m_weight == Layout::FontWeight::Normal
        && m_style == Layout::FontStyle::Italic) {
        return FontVariant::RegularItalic;
    } else {
        return FontVariant::BoldItalic;
    }
}

void Layout::flush()
{
    if (m_line.empty()) {
        return;
    }

    auto text_measures = std::vector<Vector2>();
    for (auto const& text : m_line) {
        auto text_measure = MeasureTextEx(
            text.font, text.content.c_str(), text.size, m_spacing);
        text_measures.push_back(text_measure);
    }

    float max_ascent = 0.0f;
    for (auto const& measure : text_measures) {
        max_ascent = std::max(max_ascent, -measure.y);
    }

    auto baseline = m_cursor_y + 1.25 * max_ascent;

    for (auto i = 0; i < m_line.size(); ++i) {
        m_line[i].position.y = baseline - text_measures[i].y;
        m_display_list.push_back(m_line[i]);
    }

    float max_descent = 0.0f;
    for (auto const& measure : text_measures) {
        max_descent = std::max(max_descent, measure.y);
    }

    m_cursor_y = baseline + 1.25 * max_descent;

    m_cursor_x = HSTEP;
    m_line.clear();
}

void Layout::layout_text(std::string const& text)
{
    auto font_variant = calculate_variant(m_weight, m_style);
    auto current_font = font_family().variants().at(font_variant);
    for (auto chunk : split_to_chunks(text)) {
        assert(chunk.find(" ") == std::string::npos);

        auto width
            = MeasureTextEx(current_font, chunk.c_str(), m_font_size, m_spacing)
                  .x;

        if (m_cursor_x + width > WIDTH - HSTEP) {
            flush();
        }

        auto layout_text = Text { .position = { m_cursor_x, 0 },
            .content = chunk,
            .style = { .style = m_style, .weight = m_weight },
            .font = current_font,
            .size = m_font_size };

        m_line.push_back(layout_text);
        m_cursor_x += width
            + MeasureTextEx(current_font, " ", m_font_size, m_spacing).x;
    }
}

void Layout::layout_content(std::vector<HtmlToken> const& tokens)
{

    for (auto token : tokens) {
        auto text_content = token.text_content();

        if (token.type() == HtmlToken::Type::Text) {
            layout_text(text_content);
        } else {
            if (text_content == "i") {
                m_style = FontStyle::Italic;
            } else if (text_content == "/i") {
                m_style = FontStyle::Normal;
            } else if (text_content == "b") {
                m_weight = FontWeight::Bold;
            } else if (text_content == "/b") {
                m_weight = FontWeight::Normal;
            } else if (text_content == "small") {
                m_font_size -= 2;
            } else if (text_content == "/small") {
                m_font_size += 2;
            } else if (text_content == "big") {
                m_font_size += 4;
            } else if (text_content == "/big") {
                m_font_size -= 4;
            }
        }
    }
}
}
