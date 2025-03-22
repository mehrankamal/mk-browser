#pragma once

#include <string>
#include <vector>

#include "HtmlToken.hh"

namespace LibBrowser {
class HtmlParser {
public:
    HtmlParser(std::string const);
    std::vector<HtmlToken> lex();

private:
    std::string m_text { "" };
};
}
