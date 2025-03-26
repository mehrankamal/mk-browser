
#include <string>
#include <vector>

#include "Types.hh"

namespace LibBrowser {
std::vector<std::string> split_to_chunks(std::string const& text_content,
    std::string const& separators = "\n ", i32 n_splits = -1);
std::string casefold(std::string const& text);
}
