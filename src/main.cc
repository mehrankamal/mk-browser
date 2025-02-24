#include <iostream>

#include "Browser.hh"
#include "UrlParser.hh"
#include "Util.hh"

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

  MK::Browser browser;

  return 0;
}

// #include "Browser.hh"

// int main(int argc, char **argv) {
//   MK::Browser browser = MK::Browser();

//   return 0;
// }
