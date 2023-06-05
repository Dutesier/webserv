#!/usr/bin/python

# Import modules for CGI handling
import cgi, cgitb

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')

body = f'<html> <head> <title>Hello - Second CGI Program</title> </head> <body> <h2>Hello {first_name} {last_name}</h2> </body> </html>'
print('HTTP/1.1 200 OK\r')
print('Content-Type: text/html\r')
print(f'Content-Length: {len(body)}\r')
print('\r')
print(body, end='')