#include <iostream>

#include "url.h"

namespace MK {
void show(std::string const body) {
  auto in_tag = false;
  for (auto const c : body) {
    if (c == '<')
      in_tag = true;
    else if (c == '>')
      in_tag = false;
    else if (!in_tag)
      std::cout << c;
  }
}

void load(MK::URL const &url) {
  auto maybe_body = url.request();
  if (maybe_body.has_value())
    show(maybe_body.value());
  else
    std::cerr << "Error in getting request content" << std::endl;
}
} // namespace MK