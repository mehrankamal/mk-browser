#pragma once

#include <raylib.h>
#include <vector>

#include "FontFamily.hh"
#include "HtmlToken.hh"

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
    };

    Layout(std::vector<HtmlToken> const&);
    FontFamily const& font_family() const { return m_font_family; }
    std::vector<Text> display_list() const { return m_display_list; }

private:
    void layout_content(std::vector<HtmlToken> const&);

    std::vector<Text> m_display_list {};
    FontFamily m_font_family;
    float m_font_size { 13.0f };
    float m_spacing { 0.0f };
};
}
