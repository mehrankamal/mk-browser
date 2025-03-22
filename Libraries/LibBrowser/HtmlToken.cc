#include "HtmlToken.hh"

namespace LibBrowser {
HtmlToken::HtmlToken(Type const& type, std::string text_content)
    : m_type(type)
    , m_text_content(text_content)
{
}
}
