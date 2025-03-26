#include <cstdlib>
#include <string>

#include "HtmlNode.hh"

namespace LibBrowser {
HtmlNode::HtmlNode(Type const& type, std::string text_content, HtmlNode* parent,
    std::map<std::string, std::string> attributes)
    : m_type(type)
    , m_text_content(text_content)
    , m_parent(parent)
    , m_attributes(attributes)
{
}

std::string HtmlNode::to_string(u32 ident) const
{
    std::string result = "";
    for (u32 i = 0; i < ident; ++i) {
        result += " ";
    }
    if (m_type == Type::Tag) {
        result += "Tag <" + m_text_content + ">";
    } else {
        result += "Text \"" + m_text_content + "\"";
    }

    result += "\n";

    for (auto child : m_children) {
        result += child->to_string(ident + 2);
    }
    return result;
}
}
