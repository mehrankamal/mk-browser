#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

#include "HtmlNode.hh"
#include "HtmlParser.hh"
#include "String.hh"

namespace LibBrowser {

static std::vector<std::string> void_tags { "area", "base", "br", "col",
    "embed", "hr", "img", "input", "link", "meta", "param", "source", "track",
    "wbr" };

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
    if (std::all_of(text.begin(), text.end(), isspace)) {
        return;
    }
    assert(!m_unfinished.empty());
    auto parent = m_unfinished.back();
    auto node = new HtmlNode(HtmlNode::Type::Text, text, parent);
    parent->add_child(node);
}

std::pair<std::string, std::map<std::string, std::string>>
HtmlParser::parse_attributes(std::string const& text)
{
    auto parts = split_to_chunks(text);
    auto tag = casefold(parts[0]);

    std::map<std::string, std::string> attributes;

    for (auto i = 1; i < parts.size(); ++i) {
        auto attribute = parts[i];
        if (attribute.find("=") != std::string::npos) {
            auto attribute_pair = split_to_chunks(attribute, "=", 1);

            auto key = casefold(attribute_pair[0]);
            auto value = attribute_pair[1];

            if ((value.front() == '"' && value.back() == '"')
                || (value.front() == '\'' && value.back() == '\'')) {
                value = value.substr(1, value.size() - 2);
            }
            attributes.insert(std::make_pair(key, value));
        } else {
            auto key = casefold(attribute);
            attributes.insert(std::make_pair(key, ""));
        }
    }

    return std::make_pair(tag, attributes);
}

void HtmlParser::parse_tag(std::string const& tag)
{
#ifdef HTML_PARSER_DEBUG
    std::cerr << "[HtmlParser] Parsing tag: " << tag << std::endl;
#endif
    auto [tag_name, attributes] = parse_attributes(tag);

    auto is_void = std::any_of(void_tags.begin(), void_tags.end(),
        [tag_name](
            std::string const& void_tag) { return tag_name == void_tag; });

    if (tag_name.starts_with("!")) {
        return;
    } else if (tag_name.starts_with("/")) {
        if (m_unfinished.size() == 1) {
            return;
        }
        auto node = m_unfinished.back();
        m_unfinished.pop_back();
        m_unfinished.back()->add_child(node);
    } else if (is_void) {
        auto parent = m_unfinished.empty() ? nullptr : m_unfinished.back();
        auto node
            = new HtmlNode(HtmlNode::Type::Tag, tag_name, parent, attributes);
        parent->add_child(node);
        return;
    } else {
        auto parent = m_unfinished.empty() ? nullptr : m_unfinished.back();
        auto node
            = new HtmlNode(HtmlNode::Type::Tag, tag_name, parent, attributes);
        m_unfinished.push_back(node);
    }
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
        if (c == '<') {
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
