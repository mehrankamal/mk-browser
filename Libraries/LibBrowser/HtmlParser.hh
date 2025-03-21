#pragma once

#include <string>

namespace MK {
class HtmlParser {
public:
    HtmlParser(std::string const);
    std::string lex();

private:
    std::string m_text { "" };
};
}
