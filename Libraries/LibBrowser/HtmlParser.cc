#include <sstream>
#include <string>

#include "HtmlParser.hh"

namespace MK {

HtmlParser::HtmlParser(std::string const text)
    : m_text(text)
{
}

std::string HtmlParser::lex()
{
    std::stringstream text_contetnt;
    auto in_tag = false;
    for (auto const c : m_text) {
        if (c == '<')
            in_tag = true;
        else if (c == '>')
            in_tag = false;
        else if (!in_tag)
            text_contetnt << c;
    }

    return text_contetnt.str();
}

}
