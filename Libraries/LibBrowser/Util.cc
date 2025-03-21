#include <iostream>
#include <sstream>
#include <string_view>

#include "Url.hh"

namespace MK {
std::string lex(std::string const body)
{
    std::stringstream text_contetnt;
    auto in_tag = false;
    for (auto const c : body) {
        if (c == '<')
            in_tag = true;
        else if (c == '>')
            in_tag = false;
        else if (!in_tag)
            text_contetnt << c;
    }

    return text_contetnt.str();
}

} // namespace MK
