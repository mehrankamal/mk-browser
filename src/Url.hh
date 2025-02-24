#pragma once

#include <string>
namespace MK {
class URL {
public:
  static URL create(std::string const scheme, std::string const host,
                    std::string const path);

  std::optional<std::string> request() const;

  std::string const &scheme() const { return m_scheme; };
  std::string const &host() const { return m_host; };
  std::string const &path() const { return m_path; };

private:
  URL(std::string const, std::string const, std::string const);

  std::string m_scheme;
  std::string m_host;
  std::string m_path;
};
} // namespace MK