#pragma once

#include <cstddef>
#include <map>
#include <raylib.h>
#include <vector>

#include "FontFamily.hh"
#include "HtmlParser.hh"
#include "HtmlToken.hh"
#include "Layout.hh"
#include "Url.hh"

namespace LibBrowser {
class Browser {
public:
    Browser();
    void load(URL const& url);
    void run();

    HtmlParser& html_parser() const { return *m_html_parser; }
    std::vector<HtmlToken> const& tokens() const { return m_tokens; }

private:
    void update_draw_frame();
    void close_window() const;
    void draw_layout() const;
    void update_state();

    // FIXIT: Move fully to Layout or some common lib
    float m_font_size { 13.0f };
    float m_spacing { 0.0f };
    HtmlParser* m_html_parser = nullptr;
    std::vector<Layout::Text> m_display_list;
    std::vector<HtmlToken> m_tokens;
    float m_scroll { 0.0f };
};
} // namespace LibBrowser
