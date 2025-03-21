#include <cstdlib>
#include <cstring>
#include <optional>

#include "UrlParser.hh"

namespace MK {
UrlParser::UrlParser(char* const url_str)
{
    u32 len = strlen(url_str);
    m_url_str = static_cast<char*>(malloc((len + 1) * sizeof(char)));
    strcpy(m_url_str, url_str);
}

std::optional<URL> UrlParser::parse()
{
    std::string scheme;
    std::string host;
    std::string path;
    u32 port;

    if (strncmp(m_url_str, "http://", 7) == 0) {
        scheme = "http";
        m_url_str += 7;
        port = 80;
    } else if (strncmp(m_url_str, "https://", 8) == 0) {
        scheme = "https";
        m_url_str += 8;
        port = 443;
    } else {
        // Handle error or unsupported scheme
        return std::optional<URL>();
    }

    char* host_start = m_url_str;
    char* path_start = strchr(m_url_str, '/');

    if (path_start) {
        host = std::string(host_start, path_start - host_start);
        path = path_start;
    } else {
        host = host_start;
        path = "/";
    }

    // if host has : then parse port
    char* port_start = strchr(host_start, ':');
    if (port_start) {
        host = std::string(host_start, port_start - host_start);
        port = atoi(port_start + 1);
    }

    return URL::create(scheme, host, port, path);
}
} // namespace MK
