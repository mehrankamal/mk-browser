#pragma once

#include <raylib.h>
#include <vector>

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

private:
};
}
