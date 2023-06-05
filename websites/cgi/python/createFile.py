import os
import sys
import cgi, cgitb

# Create instance of FieldStorage
form = cgi.FieldStorage()

def printFieldStorage():
    # Get data from fields
    for key in form.keys():
        print(f'{key} = {form.getvalue(key)}')

def handle_post_request():
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    post_data = sys.stdin.read(content_length)
    print(post_data)

    try:
        # Process the POST data and attempt to create a file
        # Modify this section to suit your specific needs
        file_name = 'example.txt'
        with open(file_name, 'w') as file:
            file.write(post_data)
        response = f"File '{file_name}' created successfully."
    except Exception as e:
        response = f"Failed to create file: {str(e)}"

    # Prepare the response headers
    response_headers = [
        ('Content-Type', 'text/plain'),
        ('Content-Length', str(len(response)))
    ]

    # Send the response headers
    for header in response_headers:
        print(f'{header[0]}: {header[1]}')

    # Send the blank line to indicate the end of the headers
    print()

    # Send the response content
    print(response)

# Entry point of the script
if __name__ == '__main__':
    if os.environ.get('REQUEST_METHOD') == 'POST':
        printFieldStorage()
        # handle_post_request()