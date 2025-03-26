#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

#include "HtmlNode.hh"
#include "HtmlParser.hh"

namespace LibBrowser {

HtmlParser::HtmlParser(std::string const text)
    : m_text(text)
{
}

HtmlNode* HtmlParser::finish()
{
    while (m_unfinished.size() > 1) {
        auto node = m_unfinished.back();
        m_unfinished.pop_back();
        m_unfinished.back()->add_child(node);
    }

    return m_unfinished.back();
}

void HtmlParser::parse_text(std::string const& text)
{
#ifdef HTML_PARSER_DEBUG
    std::cerr << "[HtmlParser] Parsing text: " << text << std::endl;
#endif
    assert(!m_unfinished.empty());
    auto parent = m_unfinished.back();
    auto node = new HtmlNode(HtmlNode::Type::Text, text, parent);
    parent->add_child(node);
}

void HtmlParser::parse_tag(std::string const& tag)
{
#ifdef HTML_PARSER_DEBUG
    std::cerr << "[HtmlParser] Parsing tag: " << tag << std::endl;
#endif
    if (tag.starts_with("/")) {
        if (m_unfinished.size() == 1) {
            return;
        }

        auto node = m_unfinished.back();
        m_unfinished.pop_back();
        m_unfinished.back()->add_child(node);
    } else {
        auto parent = m_unfinished.empty() ? nullptr : m_unfinished.back();
        auto node = new HtmlNode(HtmlNode::Type::Tag, tag, parent);
        m_unfinished.push_back(node);
    }
}

static bool is_whitespace(char const c)
{
    return c == ' ' || c == '\n' || c == '\t';
}

HtmlNode* HtmlParser::parse()
{
    std::vector<HtmlNode> tokens;
    std::string text_content = "";
    auto in_tag = false;
#ifdef HTML_PARSER_DEBUG
    std::cerr << "[HtmlParser] Parser Text: " << m_text << std::endl;
#endif
    for (auto const c : m_text) {
        if (is_whitespace(c)) {
            continue;
        } else if (c == '<') {
            in_tag = true;
            if (!text_content.empty()) {
                parse_text(text_content);
            }
            text_content = "";
        } else if (c == '>') {
            in_tag = false;
            parse_tag(text_content);
            text_content = "";
        } else {
            text_content += c;
        }
    }

    if (!in_tag && !text_content.empty()) {
        parse_text(text_content);
    }

#ifdef HTML_PARSER_DEBUG
    std::cerr << "Parsed no. of tokens: " << tokens.size() << std::endl;
#endif

    return finish();
}

}
