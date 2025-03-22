#include "FontFamily.hh"
#include "Types.hh"

namespace LibBrowser {

std::string to_string(LibBrowser::FontVariant const& variant)
{
    switch (variant) {
    case FontVariant::Regular:
        return "Regular";
    case FontVariant::Bold:
        return "Bold";
    case FontVariant::RegularItalic:
        return "RegularItalic";
    case FontVariant::BoldItalic:
        return "BoldItalic";
    }
}

void FontFamily::load_variants()
{
    for (auto variant = static_cast<u32>(FontVariant::Regular);
        variant <= static_cast<u32>(FontVariant::BoldItalic); ++variant) {
        std::string path = "Resources/Fonts/" + m_name + "-"
            + to_string(from_underlying<FontVariant>(variant)) + ".ttf";
        Font font = LoadFont(path.c_str());
        m_variants.insert(
            std::make_pair(from_underlying<FontVariant>(variant), font));
    }
}
}
