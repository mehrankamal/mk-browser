#include <cstdlib>
#include <cstring>
#include <optional>

#include "forward.h"
#include "url-parser.h"

namespace MK {
UrlParser::UrlParser(char *const url_str) {
  u32 len = strlen(url_str);
  m_url_str = static_cast<char *>(malloc((len + 1) * sizeof(char)));
  strcpy(m_url_str, url_str);
}

std::optional<URL> UrlParser::parse() {
    std::string scheme;
    std::string host;
    std::string path;

    if (strncmp(m_url_str, "http://", 7) == 0) {
        scheme = "http";
        m_url_str += 7;
    } else if (strncmp(m_url_str, "https://", 8) == 0) {
        scheme = "https";
        m_url_str += 8;
    } else {
        // Handle error or unsupported scheme
        return std::optional<URL>();
    }

    char *host_start = m_url_str;
    char *path_start = strchr(m_url_str, '/');

    if (path_start) {
        host = std::string(host_start, path_start - host_start);
        path = path_start;
    } else {
        host = host_start;
        path = "/";
    }



    return URL::create(scheme, host, path);
}
} // namespace MK