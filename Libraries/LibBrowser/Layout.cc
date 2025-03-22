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
    Layout::FontWeight const& weight, Layout::FontStyle const& style)
{
    if (weight == Layout::FontWeight::Normal
        && style != Layout::FontStyle::Italic) {
        return FontVariant::Regular;
    } else if (weight == Layout::FontWeight::Bold
        && style != Layout::FontStyle::Italic) {
        return FontVariant::Bold;
    } else if (weight == Layout::FontWeight::Normal
        && style == Layout::FontStyle::Italic) {
        return FontVariant::RegularItalic;
    } else {
        return FontVariant::BoldItalic;
    }
}

void Layout::layout_content(std::vector<HtmlToken> const& tokens)
{
    auto cursor_x = HSTEP;
    auto cursor_y = VSTEP;

    auto weight = Layout::FontWeight::Normal;
    auto style = Layout::FontStyle::Normal;

    for (auto token : tokens) {
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

                auto layout_text
                    = Layout::Text { .position = { cursor_x, cursor_y },
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
                style = Layout::FontStyle::Italic;
            } else if (text_content == "/i") {
                style = Layout::FontStyle::Normal;
            } else if (text_content == "b") {
                weight = Layout::FontWeight::Bold;
            } else if (text_content == "/b") {
                weight = Layout::FontWeight::Normal;
            }
        }
    }
}
}
