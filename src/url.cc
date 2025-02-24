
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "Url.hh"

namespace MK {

URL::URL(std::string const scheme, std::string const host,
         std::string const path)
    : m_scheme(scheme), m_host(host), m_path(path) {}

URL URL::create(std::string const scheme, std::string const host,
                std::string const path) {
  return URL(scheme, host, path);
}

std::optional<std::string> URL::request() const {
  auto sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1) {
    std::cerr << "Failed to create socket" << std::endl;
    return nullptr;
  }

  struct hostent *server = gethostbyname(m_host.c_str());

  if (server == nullptr) {
    std::cerr << "Failed to resolve hostname " << std::endl;
    close(sock);
    return std::optional<std::string>();
  }

  struct sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(80);
  hint.sin_addr.s_addr = INADDR_ANY;
  memcpy(&hint.sin_addr.s_addr, server->h_addr, server->h_length);

  if (connect(sock, (sockaddr *)&hint, sizeof(hint)) == -1) {
    std::cerr << "Failed to connect to server " << host() << std::endl;
    return std::optional<std::string>();
  }

  std::string request =
      "GET / HTTP/1.1\r\nHost: " + m_host + "\r\nConnection: close\r\n\r\n";

  if (send(sock, request.c_str(), request.length(), 0) == -1) {
    std::cerr << "Failed to send request" << std::endl;
    close(sock);
    return "";
  }

  std::string response;
  char buf[4096];

  int bytes_received;
  while ((bytes_received = recv(sock, buf, 4095, 0)) > 0) {
    buf[bytes_received] = '\0';
    response += buf;
  }

  if (bytes_received == -1) {
    std::cerr << "Error in recv()" << std::endl;
  }

  // strip response headers from response

  size_t header_end = response.find("\r\n\r\n");
  if (header_end != std::string::npos) {
    response = response.substr(header_end + 4);
  } else {
    std::cerr << "Failed to find end of headers" << std::endl;
    return std::optional<std::string>();
  }

  close(sock);

  return response;
}
} // namespace MK