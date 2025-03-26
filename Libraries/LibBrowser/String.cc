#include "String.hh"

namespace LibBrowser {
std::vector<std::string> split_to_chunks(std::string const& text_content,
    std::string const& separators, i32 n_splits)
{
    std::vector<std::string> chunks;
    std::string current_chunk;

    i32 splits = 0;

    for (char c : text_content) {
        auto is_separator = separators.find(c) != std::string::npos;
        if (is_separator) {
            if (n_splits > 0 && chunks.size() == n_splits) {
                current_chunk += c;
            } else if (!current_chunk.empty()) {
                chunks.push_back(current_chunk);
                current_chunk.clear();
            }
        } else {
            current_chunk += c;
        }
    }

    if (!current_chunk.empty()) {
        chunks.push_back(current_chunk);
    }

    return chunks;
}

std::string casefold(std::string const& text)
{
    auto result = text;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });

    return result;
}
}
