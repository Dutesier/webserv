#!/usr/bin/python3

import os
 
# printing environment variables
for item, value in os.environ.items():
    print('{}: {}'.format(item, value))