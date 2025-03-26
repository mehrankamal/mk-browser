#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Types.hh"

namespace LibBrowser {
class HtmlNode {
public:
    enum class Type { Tag, Text };

    static std::string to_string(Type const& type)
    {
        switch (type) {
        case Type::Tag:
            return "Tag";
        case Type::Text:
            return "Text";
        }
    }

    HtmlNode(Type const&, std::string, HtmlNode* parent = nullptr);

    std::string const& text_content() { return m_text_content; }
    Type const& type() { return m_type; }
    void add_child(HtmlNode* child) { m_children.push_back(child); }

    std::string to_string(u32 ident = 0) const;

private:
    Type m_type;
    std::string m_text_content { "" };
    HtmlNode* m_parent { nullptr };
    std::vector<HtmlNode*> m_children {};
};
}
