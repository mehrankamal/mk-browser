#pragma once

#include <cstddef>
#include <map>
#include <raylib.h>
#include <vector>

#include "FontFamily.hh"
#include "HtmlParser.hh"
#include "HtmlToken.hh"
#include "Url.hh"

namespace LibBrowser {
class Browser {
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
    struct LayoutText {
        Vector2 position;
        std::string content;
        TextStyle style;
        // FIXIT: Use font reference if possible rather than copying font into
        // the LayoutText
        Font font;
    };

    Browser();
    void load(URL const& url);
    void run();
    void load_font_family();

    HtmlParser& html_parser() const { return *m_html_parser; }
    std::vector<HtmlToken> const& tokens() const { return m_tokens; }
    FontFamily const& font_family() const { return m_font_family; }

private:
    void update_draw_frame();
    void close_window() const;
    void compute_layout();
    void draw_layout() const;
    void update_state();

    HtmlParser* m_html_parser = nullptr;
    FontFamily m_font_family;
    std::vector<LayoutText> m_display_list;
    std::vector<HtmlToken> m_tokens;
    float m_scroll { 0.0f };
    float m_font_size { 13.0f };
    float m_spacing { 0.0f };
};
} // namespace LibBrowser
