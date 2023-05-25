#!/usr/bin/python3
import os
import sys
import urllib.parse

query_string = os.environ.get('QUERY_STRING', '')
params = urllib.parse.parse_qs(query_string)
name = params.get('name', [''])[0]
age = params.get('age', [''])[0]

print('Content-type: text/plain')
print(f'Content-length: {sys.getsizeof(name+age)}')
print()

print(f'Name: {name}')
print(f'Age: {age}')

sys.stdout.close()