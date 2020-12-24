#!/usr/bin/env python3

from http.server import HTTPServer, BaseHTTPRequestHandler
import os

class Dispatcher:
    def dispatch(self, path):
        _, effect_name, effect_color, effect_arg = path.split("/")
        
        os.system("echo '" + effect_name + " " + effect_color + " " + effect_arg + "' > ../schedule.cmd")
        
        return effect_name + " " + effect_color + " " + effect_arg
    
class Handler(BaseHTTPRequestHandler):
    def do_GET(self):
        verb = self.command
        path = self.path
        
        if path == "/favicon.ico":
            return
        
        result = Dispatcher().dispatch(path)
        self.wfile.write(result.encode("utf8"))

def main():
    HTTPServer(("", 80), Handler).serve_forever()

main()
