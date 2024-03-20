def bmp_to_byte_array(image_path, output_file):
    with open(image_path, 'rb') as f:
        # Skip the BMP file header (first 54 bytes)
        f.seek(54)

        # Read the width and height of the image from the BMP header
        width_bytes = f.read(4)
        height_bytes = f.read(4)
        width = int.from_bytes(width_bytes, byteorder='little')
        height = int.from_bytes(height_bytes, byteorder='little')

        # Initialize byte array to store pixel data
        byte_array = bytearray()

        # Each pixel in BMP file is represented by 3 bytes (BGR format)
        # Read pixel data row by row, starting from bottom-left corner
        for y in range(height - 1, -1, -1):
            for x in range(width):
                # Read pixel data (3 bytes: blue, green, red)
                pixel_data = f.read(3)
                # Convert pixel data to RGB565 format
                # For simplicity, just use red component for RGB565
                # Adjust the conversion method according to your requirements
                r, g, b = pixel_data
                pixel = ((r & 0xF8) << 8)  # Only red component used in RGB565
                byte_array.extend(pixel.to_bytes(2, byteorder='little'))

    # Export byte array to a text file
    with open(output_file, "w") as output:
        for byte in byte_array:
            output.write(format(byte, '02X') + '\n')  # Write each byte as hexadecimal value to text file

    return byte_array

# Example usage:
image_path = "Conversion Code Tools\Images\background 1.bmp"
output_file = "byte_array.txt"
byte_array = bmp_to_byte_array(image_path, output_file)