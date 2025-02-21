import socket
import ssl

class URL:
    def __init__(self, url):
        self.scheme, url = url.split('://', 1)
        assert self.scheme in ['http', 'https']
        
        if '/' not in url:
            url = url + '/'
        
        self.host, url = url.split('/', 1)
        self.path = '/' + url
        
        if ":" in self.host:
            self.host, port = self.host.split(':', 1)
            self.port = int(port)
        elif self.scheme == 'https':
            self.port = 443
        elif self.scheme == 'http':
            self.port = 80
        
    def request(self):
        s = socket.socket(
            family=socket.AF_INET,
            type=socket.SOCK_STREAM,
            proto=socket.IPPROTO_TCP
        )
        
        s.connect((self.host, self.port))
        
        if self.scheme == 'https':
            s = ssl.create_default_context().wrap_socket(s, server_hostname=self.host)
        
        req = "GET {} HTTP/1.0\r\n".format(self.path)
        req += "Host: {}\r\n".format(self.host)
        req += "\r\n"
        
        s.send(req.encode('utf8'))
        
        res = s.makefile('r', encoding='utf8', newline='\r\n')
        
        statusline = res.readline()
        version, status, explaination = statusline.split(' ', 2)
        
        response_headers = {}
        while True:
            line = res.readline()
            if line == '\r\n':
                break
            
            header, value = line.split(":", 1)
            response_headers[header.casefold()] = value.strip()
                
        assert 'transfer-encoding' not in response_headers
        assert 'content-encoding' not in response_headers
        
        content = res.read()
        s.close()
        return content
