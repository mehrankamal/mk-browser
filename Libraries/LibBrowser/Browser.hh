#pragma once

#include <raylib.h>
#include <vector>

#include "Url.hh"

namespace MK {
class Browser {
public:
    struct LayoutText {
        Vector2 position;
        std::string character;
    };

    Browser();
    void load(URL const& url);
    void run();

    std::string const& text_content() const { return m_text_content; }

private:
    void update_draw_frame();
    void close_window() const;
    void compute_layout(std::string const& text_content);
    void draw_layout() const;
    void update_state();

    std::vector<LayoutText> m_display_list;
    std::string m_text_content;
    Font m_font;
    float m_scroll { 0.0f };
    float m_font_size { 13.0f };
    float m_spacing { 0.0f };
};
} // namespace MK
