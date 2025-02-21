import url
from pathlib import Path

def show(body):
    in_tag = False
    for c in body:
        if c == '<':
            in_tag = True
        elif c == '>':
            in_tag = False
        elif not in_tag:
            print(c, end='')
            
def load(url: url.URL):
    body = url.request()
    show(body)
    
if __name__ == "__main__":
    import sys
    if len(sys.argv) > 1:
        load(url.URL(sys.argv[1]))
    else:
        # Load UA/index.html in this directory (use pathlib to calculate path) using the file:// scheme
        load(url.URL(f'file:///{Path(__file__).parent}/UA/index.html'))