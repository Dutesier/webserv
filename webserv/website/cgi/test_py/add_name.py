#!/usr/bin/env python3

import os
import cgi

# Read existing names from file
with open(os.path.join(os.environ['DOCUMENT_ROOT'], 'name_list.txt')) as f:
  names = set(line.strip() for line in f)

# Add new name
form = cgi.FieldStorage()
new_name = form.getvalue('name', '').strip()
if new_name:
  names.add(new_name)

# Write updated list to file
with open(os.path.join(os.environ['DOCUMENT_ROOT'], 'name_list.txt'), 'w') as f:
  f.write('\n'.join(sorted(names)))

# Send response
print('Content-type: text/plain')
print()
print('OK')
