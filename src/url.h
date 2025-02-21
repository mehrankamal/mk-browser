#pragma once

class URL {
public:
    URL(char* const);

    void request();

    char *const scheme(){ return m_scheme; };
    char *const host(){ return m_host; };
    char *const path(){ return m_path; };

    ~URL();
private:
    char* m_scheme;
    char* m_host;
    char* m_path;
};