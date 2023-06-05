#!/usr/bin/env python

import os
import sys
body = "<!DOCTYPE html> <html> <head>   <title>Hello Page</title> </head> <body>   <h1>Hello, World!</h1>   <p>Welcome to my hello page.</p> </body> </html>"
# Print the autoindex HTML as the CGI response
print('HTTP/1.1 200 OK\r')
print('Content-Type: text/html\r')
print(f'Content-Length: {len(body)}\r')
print('\r')
print(body, end='')