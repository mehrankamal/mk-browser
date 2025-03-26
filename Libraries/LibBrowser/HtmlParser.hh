#pragma once

#include <string>
#include <vector>

#include "HtmlNode.hh"

namespace LibBrowser {
class HtmlParser {
public:
    HtmlParser(std::string const);
    HtmlNode* parse();

private:
    void parse_tag(std::string const&);
    void parse_text(std::string const&);
    HtmlNode* finish();

    std::string m_text { "" };
    std::vector<HtmlNode*> m_unfinished {};
};
}
