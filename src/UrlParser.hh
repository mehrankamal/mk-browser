#include <sys/types.h>

#include "Forward.hh"
#include "Url.hh"

namespace MK {
class UrlParser {
public:
  UrlParser(char *const url_str);
  std::optional<URL> parse();

private:
  u32 m_start;
  u32 m_current;
  char *m_url_str;
};
} // namespace MK