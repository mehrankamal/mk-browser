#include <iostream>
#include <sstream>
#include <string>

#include "HtmlParser.hh"
#include "HtmlToken.hh"

namespace LibBrowser {

HtmlParser::HtmlParser(std::string const text)
    : m_text(text)
{
}

std::vector<HtmlToken> HtmlParser::lex()
{
    std::vector<HtmlToken> tokens;
    std::stringstream text_buffer;
    auto in_tag = false;
#ifdef HTML_PARSER_DEBUG
    std::cerr << "[HtmlParser] Parser Text: " << m_text << std::endl;
#endif
    for (auto const c : m_text) {
        if (c == '<') {
            in_tag = true;
            auto text_content = text_buffer.str();
            if (!text_content.empty()) {
                auto token = HtmlToken(HtmlToken::Type::Text, text_content);
                tokens.push_back(token);
            }
            text_buffer.str("");
        } else if (c == '>') {
            in_tag = false;
            auto text_content = text_buffer.str();
            tokens.push_back(HtmlToken(HtmlToken::Type::Tag, text_content));
            text_buffer.str("");
        } else {
            text_buffer << c;
        }
    }

    auto text_content = text_buffer.str();
    if (!in_tag && !text_content.empty()) {
        tokens.push_back(HtmlToken(HtmlToken::Type::Text, text_content));
    }

#ifdef HTML_PARSER_DEBUG
    std::cerr << "Parsed no. of tokens: " << tokens.size() << std::endl;
#endif

    return tokens;
}

}
