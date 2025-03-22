
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <optional>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "Forward.hh"
#include "Url.hh"

namespace LibBrowser {

std::optional<std::string> strip_headers(std::string const& response)
{
    size_t header_end = response.find("\r\n\r\n");

    if (header_end != std::string::npos) {
        return response.substr(header_end + 4);
    } else {
        std::cerr << "Failed to find end of headers" << std::endl;
        return nullptr;
    }
}

URL::URL(std::string const scheme, std::string const host, u32 const port,
    std::string const path)
    : m_scheme(scheme)
    , m_host(host)
    , m_port(port)
    , m_path(path)
{
}

URL URL::create(std::string const scheme, std::string const host, u32 port,
    std::string const path)
{
    return URL(scheme, host, port, path);
}

std::optional<std::string> URL::request_http() const
{
    auto sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return nullptr;
    }

    struct hostent* server = gethostbyname(m_host.c_str());

    if (server == nullptr) {
        std::cerr << "Failed to resolve hostname " << std::endl;
        close(sock);
        return std::optional<std::string>();
    }

    struct sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(m_port);
    hint.sin_addr.s_addr = INADDR_ANY;
    memcpy(&hint.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sock, (sockaddr*)&hint, sizeof(hint)) == -1) {
        std::cerr << "Failed to connect to server " << host() << std::endl;
        return std::optional<std::string>();
    }

#ifdef URL_DEBUG
    std::cout << "Requesting Path: " + m_path << std::endl;
#endif

    std::string request = "GET " + m_path + " HTTP/1.1\r\nHost: " + m_host
        + "\r\nConnection: close\r\n\r\n";

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

    close(sock);

#ifdef URL_DEBUG
    std::cerr << "----------Response from request----------" << std::endl
              << response << std::endl
              << "-----------------------------------------" << std::endl;
#endif

    return strip_headers(response);
}

std::optional<std::string> URL::request_https() const
{
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    std::unique_ptr<SSL_CTX, decltype(&SSL_CTX_free)> ctx(
        SSL_CTX_new(TLS_client_method()), SSL_CTX_free);
    if (!ctx)
        return std::nullopt;

    SSL_CTX_set_verify(ctx.get(), SSL_VERIFY_PEER, nullptr);
    SSL_CTX_set_default_verify_paths(ctx.get());

    std::unique_ptr<BIO, decltype(&BIO_free_all)> bio(
        BIO_new_ssl_connect(ctx.get()), BIO_free_all);
    if (!bio)
        return std::nullopt;

    std::string const host_port = m_host + ":" + std::to_string(m_port);
    BIO_set_conn_hostname(bio.get(), host_port.c_str());

    SSL* ssl = nullptr;
    BIO_get_ssl(bio.get(), &ssl);
    if (!ssl)
        return std::nullopt;

    SSL_set_tlsext_host_name(ssl, m_host.c_str());

    if (BIO_do_connect(bio.get()) <= 0) {
        ERR_print_errors_fp(stderr);
        return std::nullopt;
    }

    if (SSL_get_verify_result(ssl) != X509_V_OK) {
        return std::nullopt;
    }

    std::string const request = "GET " + m_path
        + " HTTP/1.1\r\n"
          "Host: "
        + m_host
        + "\r\n"
          "Connection: close\r\n\r\n";

    if (BIO_write(bio.get(), request.data(), request.size()) <= 0) {
        return std::nullopt;
    }

    std::string response;
    char buffer[4096];
    int bytes_read = 0;

    while ((bytes_read = BIO_read(bio.get(), buffer, sizeof(buffer))) > 0) {
        response.append(buffer, bytes_read);
    }

    if (bytes_read < 0) {
        return std::nullopt;
    }

    return strip_headers(response);
}

std::optional<std::string> URL::request() const
{
    if (m_scheme == "http") {
        return request_http();
    } else if (m_scheme == "https") {
        return request_https();
    } else {
        std::cerr << "Unsupported scheme " << m_scheme << std::endl;
        return std::optional<std::string>();
    }
}
} // namespace LibBrowser
