#pragma once

#include <cstddef>
#include <map>
#include <raylib.h>
#include <vector>

#include "HtmlParser.hh"
#include "HtmlToken.hh"
#include "Url.hh"

namespace MK {
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

    enum class FontVariant {
        Regular,
        Bold,
        RegularItalic,
        BoldItalic,
    };

    class FontFamily {
    public:
        void load_variants();

        std::string const& name() const { return m_name; }
        std::map<FontVariant, Font> const& variants() const
        {
            return m_variants;
        }

    private:
        std::string m_name { "SF-Pro-Text" };
        std::map<FontVariant, Font> m_variants;
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
} // namespace MK
