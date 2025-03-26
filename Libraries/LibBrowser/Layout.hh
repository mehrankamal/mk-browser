#pragma once

#include <raylib.h>
#include <vector>

#include "Config.hh"
#include "FontFamily.hh"
#include "HtmlNode.hh"

namespace LibBrowser {
class Layout {
public:
    enum class FontStyle {
        Normal,
        Italic,
    };
    enum class FontWeight {
        Normal,
        Bold,
    };
    struct TextStyle {
        FontStyle style;
        FontWeight weight;
    };
    struct Text {
        Vector2 position;
        std::string content;
        TextStyle style;
        // FIXIT: Use font reference if possible rather than copying font into
        // the LayoutText
        Font font;
        float size;
    };

    Layout(std::vector<HtmlNode> const&);
    FontFamily const& font_family() const { return m_font_family; }
    std::vector<Text> display_list() const { return m_display_list; }
    float font_size() const { return m_font_size; }
    float spacing() const { return m_spacing; }

private:
    void layout_content(std::vector<HtmlNode> const&);
    void layout_text(std::string const&);
    void flush();

    std::vector<Text> m_display_list {};
    std::vector<Text> m_line {};
    FontFamily m_font_family;
    float m_font_size { 13.0f };
    float m_spacing { 0.0f };
    float m_cursor_x { HSTEP };
    float m_cursor_y = { VSTEP };
    FontWeight m_weight = FontWeight::Normal;
    FontStyle m_style = FontStyle::Normal;
};
}
