#include <iostream>

#include "url-parser.h"
#include "util.h"

int main(int argc, char **argv) {
  if (argc > 1) {
    MK::UrlParser url_parser = MK::UrlParser(argv[1]);
    auto maybe_url = url_parser.parse();

    if (maybe_url.has_value()) {
      MK::load(maybe_url.value());
    } else {
      std::cerr << "Error parsing URL" << std::endl;
    }
  } else {
    std::cerr << "Exected URL, none found" << std::endl;
  }
  return 0;
}