#pragma once

#include <raylib.h>

#include "Url.hh"

namespace MK {
class Browser {
public:
  Browser();
  void load(URL const &url);
  void run();

  std::string const &text_content() const { return m_text_content; }

private:
  void update_draw_frame() const;
  void close_window() const;

  std::string m_text_content;
  Font m_font;
};
} // namespace MK