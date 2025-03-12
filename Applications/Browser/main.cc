#include <iostream>

#include "../../Libraries/LibBrowser/Browser.hh"
#include "../../Libraries/LibBrowser/UrlParser.hh"
#include "../../Libraries/LibBrowser/Util.hh"

int main(int argc, char **argv) {
  if (argc > 1) {
    MK::UrlParser url_parser = MK::UrlParser(argv[1]);
    auto maybe_url = url_parser.parse();

    MK::Browser browser;

    if (maybe_url.has_value()) {
      browser.load(maybe_url.value());
      browser.run();
    } else {
      std::cerr << "Error parsing URL" << std::endl;
    }
  } else {
    std::cerr << "Exected URL, none found" << std::endl;
  }

  return 0;
}
