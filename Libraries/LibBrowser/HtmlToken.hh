#pragma once

#include <string>

namespace MK {
class HtmlToken {
public:
    enum class Type { Tag, Text };

    HtmlToken(Type const&, std::string);

    std::string const& text_content() { return m_text_content; }
    Type const& type() { return m_type; }

private:
    Type m_type;
    std::string m_text_content { "" };
};
}
