"""
Program to gzip the final html file to webpage.h file
"""
import binascii
import gzip
import os
import subprocess

def compress_and_convert_to_byte_array(filename):
    """
    Open and compress file and return byte array and array length
    """
    # Compress the file
    with open(filename, 'rb') as f_in:
        compressed_data = gzip.compress(f_in.read())

    # Convert the compressed data to a byte array
    byte_array = binascii.hexlify(compressed_data).decode('utf-8')

    # Format the byte array with 0x as a delimiter and separate each byte with a comma
    formatted_byte_array = ', '.join([
        f'0x{byte_array[i:i+2]}' 
        for i in range(0, len(byte_array), 2)
            ])

    return formatted_byte_array, len(byte_array) // 2 # Divide by 2 to get the number of bytes

subprocess.run("pnpm build",shell=True, universal_newlines=True, check=False)
html_filename = os.path.join('./dist', 'index.html')
out_byte_array, out_byte_length = compress_and_convert_to_byte_array(html_filename)

output_filename = os.path.join('../src','webpage.h')

with open(output_filename, 'w',encoding="utf-8") as f_out:
    f_out.write("#include <Arduino.h>\n\n")
    f_out.write(f"const size_t gzip_html_length = {out_byte_length};\n")
    f_out.write(f"const char webpage_html[] PROGMEM = {{{out_byte_array}}};\n")

print("The contents and length have been written to webpage.h.")
