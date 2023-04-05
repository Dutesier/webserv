#!/usr/bin/python3
import os
import urllib.parse

print("Content-Type: text/html")
print()

# Get the query string from the environment
query_string = os.environ.get("QUERY_STRING", "")

# Parse the query string into a dictionary
params = urllib.parse.parse_qs(query_string)

# Print the parameters as HTML
print("<html><body>")
print("<h1>Query Parameters:</h1>")
print("<ul>")
for name, values in params.items():
    for value in values:
        print("<li>{0}={1}</li>".format(name, value))
print("</ul>")
print("</body></html>")
