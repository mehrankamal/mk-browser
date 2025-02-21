#include "url.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <string>

URL::URL(char *const url) {
  std::string url_str(url);
  std::vector<std::string> url_parts;
  std::stringstream ss(url_str);
  std::string item;

  while (std::getline(ss, item, ':')) {
    url_parts.push_back(item);
  }

  if (url_parts.size() != 2) {
    throw std::invalid_argument("Invalid URL");
  }

  this->m_scheme = new char[url_parts[0].length() + 1];
  strcpy(this->m_scheme, url_parts[0].c_str());

  if (url_str.find("/") == std::string::npos) {
    url_str += "/";
  }

  std::string host;
  std::string path;
  std::string delimiter = "/";
  size_t pos = 0;
  std::string token;
  int i = 0;

  while ((pos = url_str.find(delimiter)) != std::string::npos) {
    token = url_str.substr(0, pos);
    if (i == 0) {
      host = token;
    } else {
      path += "/" + token;
    }
    url_str.erase(0, pos + delimiter.length());
    i++;
  }

  this->m_host = new char[host.length() + 1];
  strcpy(this->m_host, host.c_str());

  std::cout << "Host: " << this->m_host << std::endl;
  std::cout << "Path: " << path << std::endl;
  std::cout << "Scheme: " << this->m_scheme << std::endl;
}

URL::~URL() {
  delete[] this->m_scheme;
  delete[] this->m_host;
  delete[] this->m_path;
}

void URL::request() {
    auto sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    struct sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(80);

    if (connect(sock, (sockaddr*)&hint, sizeof(hint)) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
        return;
    }

    char buf[4096];
    std::string request = "GET / HTTP/1.1\r\nHost: " + std::string(this->m_host) + "\r\n\r\n";
    send(sock, request.c_str(), request.size() + 1, 0);

    int bytesReceived = recv(sock, buf, 4096, 0);
    if (bytesReceived == -1) {
        std::cerr << "Error in recv()" << std::endl;
        return;
    }

    std::cout << std::string(buf, bytesReceived) << std::endl;

    close(sock);

    return;
}