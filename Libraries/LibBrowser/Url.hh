#pragma once

#include <optional>
#include <string>

#include "Forward.hh"
namespace MK {
class URL {
public:
  static URL create(std::string const scheme, std::string const host, u32 port,
                    std::string const path);

  std::optional<std::string> request() const;

  std::string const &scheme() const { return m_scheme; };
  std::string const &host() const { return m_host; };
  std::string const &path() const { return m_path; };

private:
  URL(std::string const, std::string const, u32 const, std::string const);
  std::optional<std::string> request_http() const;
  std::optional<std::string> request_https() const;

  std::string m_scheme;
  std::string m_host;
  std::string m_path;
  u32 m_port;
};
} // namespace MK