#include <iostream>

#include <Browser.hh>
#include <UrlParser.hh>

int main(int argc, char** argv)
{
    if (argc > 1) {
        LibBrowser::UrlParser url_parser = LibBrowser::UrlParser(argv[1]);
        auto maybe_url = url_parser.parse();

        LibBrowser::Browser browser;

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
