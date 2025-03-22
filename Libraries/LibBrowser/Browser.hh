#pragma once

#include <cassert>
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
    ~Browser();

    void load(URL const& url);
    void run();

    HtmlParser& html_parser() const
    {
        assert(m_html_parser);
        return *m_html_parser;
    }
    Layout& layout() const
    {
        assert(m_layout);
        return *m_layout;
    }

private:
    void update_draw_frame();
    void close_window() const;
    void draw_layout() const;
    void update_state();

    HtmlParser* m_html_parser = nullptr;
    Layout* m_layout = nullptr;
    float m_scroll { 0.0f };
};
} // namespace LibBrowser
