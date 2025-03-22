#pragma once

#include <map>
#include <raylib.h>
#include <string>

namespace MK {

enum class FontVariant {
    Regular,
    Bold,
    RegularItalic,
    BoldItalic,
};
std::string to_string(FontVariant const&);

class FontFamily {
public:
    void load_variants();

    std::string const& name() const { return m_name; }
    std::map<FontVariant, Font> const& variants() const { return m_variants; }

private:
    std::string m_name { "SF-Pro-Text" };
    std::map<FontVariant, Font> m_variants;
};
}
