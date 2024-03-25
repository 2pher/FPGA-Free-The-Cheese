import struct

def read_bitmap(filename):
    with open(filename, 'rb') as f:
        # Read the bitmap header
        header = f.read(54)

        # Get image width and height from the header
        width = struct.unpack('<I', header[18:22])[0]
        height = struct.unpack('<I', header[22:26])[0]

        # Move the file pointer to the beginning of pixel data
        f.seek(54)

        # Read pixel data
        pixel_data = f.read()

    return width, height, pixel_data

def extract_rgb565(pixel_data, width, height):
    colors = []
    for y in range(height):
        row = []
        for x in range(width):
            # Calculate the byte offset for the pixel
            byte_offset = ((height - y - 1) * width + x) * 2

            # Extract RGB565 color code (2 bytes) for each pixel
            color_bytes = pixel_data[byte_offset : byte_offset + 2]
            color = struct.unpack('<H', color_bytes)[0]
            row.append(color)
        colors.append(row)
    return colors

def generate_c_array(colors, width, height, output_filename):
    with open(output_filename, 'w') as f:
        f.write('#include <stdint.h>\n\n')
        f.write('const uint16_t image[%d][%d] = {\n' % (height, width))
        for row in colors:
            f.write('    {')
            for color in row:
                f.write('0x%04X, ' % color)
            f.write('},\n')
        f.write('};\n')

def main():
    input_filename = 'input.bmp'
    output_filename = 'output.c'

    # Read the bitmap image
    width, height, pixel_data = read_bitmap(input_filename)

    # Extract RGB565 color codes
    colors = extract_rgb565(pixel_data, width, height)

    # Generate C array
    generate_c_array(colors, width, height, output_filename)
    print("C array has been generated in", output_filename)

if __name__ == "__main__":
    main()
