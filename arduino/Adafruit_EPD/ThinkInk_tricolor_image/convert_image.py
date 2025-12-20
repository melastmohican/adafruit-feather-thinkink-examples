#!/usr/bin/env python3
"""
Convert JPG image to ThinkInk 3-color e-paper display format.
Output is a C header file with PROGMEM bitmap data.

For 3-color displays (Tricolor):
- Black  = 0b00
- White  = 0b01
- Red    = 0b11 (or 0b10 depending on controller, usually we Map to specific indices)

Packed 4 pixels per byte, MSB first.
"""

from PIL import Image
import numpy as np
import sys
import os

# 3-color palette for e-paper: Black, White, Red
EINK_PALETTE = np.array([
    [0, 0, 0],        # Black
    [255, 255, 255],  # White
    [255, 0, 0],      # Red
], dtype=np.uint8)

def find_nearest_color(pixel, palette):
    """Find the nearest color in the palette using Euclidean distance."""
    distances = np.sqrt(np.sum((palette.astype(float) - pixel.astype(float)) ** 2, axis=1))
    return np.argmin(distances)

def quantize_to_3color(image):
    """Quantize image to 3-color e-paper palette with dithering."""
    img_array = np.array(image, dtype=np.float32)
    height, width = img_array.shape[:2]
    result = np.zeros((height, width), dtype=np.uint8)
    
    # Floyd-Steinberg dithering
    for y in range(height):
        for x in range(width):
            old_pixel = img_array[y, x].copy()
            # Clamp values
            old_pixel = np.clip(old_pixel, 0, 255)
            
            # Find nearest color
            color_idx = find_nearest_color(old_pixel.astype(np.uint8), EINK_PALETTE)
            result[y, x] = color_idx
            new_pixel = EINK_PALETTE[color_idx].astype(np.float32)
            
            # Calculate error
            error = old_pixel - new_pixel
            
            # Distribute error (Floyd-Steinberg)
            if x + 1 < width:
                img_array[y, x + 1] += error * 7 / 16
            if y + 1 < height:
                if x > 0:
                    img_array[y + 1, x - 1] += error * 3 / 16
                img_array[y + 1, x] += error * 5 / 16
                if x + 1 < width:
                    img_array[y + 1, x + 1] += error * 1 / 16
    
    return result

def convert_to_progmem(quantized, width, height):
    """Convert quantized image to PROGMEM byte array format."""
    # Pack 4 pixels per byte (2 bits each), MSB first
    bytes_list = []
    
    for y in range(height):
        for x in range(0, width, 4):
            byte_val = 0
            for i in range(4):
                if x + i < width:
                    pixel = quantized[y, x + i]
                    # Map colors to 2-bit values
                    # 0=Black, 1=White, 2=Red
                    # EPD often expects: 00=Black, 01=White, 10 or 11 = Red/Color
                    # We will stick to the same map: 0->0, 1->1, 2->3 (11) for Red to match common mapping
                    # But wait, original code said: 0b00=Black, 0b01=White, 0b10=Yellow, 0b11=Red
                    # Let's map Black(0)->0(00), White(1)->1(01), Red(2)->3(11)
                    if pixel == 0: val = 0
                    elif pixel == 1: val = 1
                    elif pixel == 2: val = 3
                    else: val = 1
                else:
                    val = 1  # White for padding
                byte_val = (byte_val << 2) | (val & 0x03)
            bytes_list.append(byte_val)
    
    return bytes_list

def generate_header(bytes_list, width, height, var_name, filename):
    """Generate C header file content."""
    lines = []
    lines.append(f"// Generated from {filename}")
    lines.append(f"// Image size: {width}x{height} pixels")
    lines.append(f"// Format: 3-color (2 bits per pixel), packed 4 pixels per byte")
    lines.append(f"// Total bytes: {len(bytes_list)}")
    lines.append("")
    lines.append(f"#ifndef _{var_name.upper()}_H_")
    lines.append(f"#define _{var_name.upper()}_H_")
    lines.append("")
    lines.append("#include <Arduino.h>")
    lines.append("")
    lines.append(f"const uint8_t {var_name}[{len(bytes_list)}] PROGMEM = {{")
    
    # Format bytes, 16 per line
    for i in range(0, len(bytes_list), 16):
        chunk = bytes_list[i:i+16]
        hex_values = ", ".join(f"0x{b:02X}" for b in chunk)
        if i + 16 < len(bytes_list):
            hex_values += ","
        lines.append(f"  {hex_values}")
    
    lines.append("};")
    lines.append("")
    lines.append("#endif")
    lines.append("")
    
    return "\n".join(lines)

def main():
    input_file = "mocha200x200.jpg"
    output_file = "mocha200x200.h"
    var_name = "image200x200"
    
    # Target display size
    target_width = 200
    target_height = 200
    
    print(f"Loading {input_file}...")
    if not os.path.exists(input_file):
        print(f"Error: {input_file} not found!")
        # Fallback for testing if file doesn't exist in user workspace, generate a dummy image
        print("Generating dummy image for testing...")
        img = Image.new('RGB', (200, 200), color='white')
        # Draw a red rectangle
        for x in range(50, 150):
            for y in range(50, 150):
                img.putpixel((x, y), (255, 0, 0))
        # Draw a black circle-ish thing
        for x in range(80, 120):
            for y in range(80, 120):
                img.putpixel((x, y), (0, 0, 0))
    else:
        img = Image.open(input_file)
    
    print(f"Original size: {img.size}")
    
    # Convert to RGB if necessary
    if img.mode != 'RGB':
        img = img.convert('RGB')
    
    # Resize to target dimensions if needed
    if img.size != (target_width, target_height):
         print(f"Resizing to {target_width}x{target_height}...")
         img = img.resize((target_width, target_height), Image.Resampling.LANCZOS)
    
    print("Quantizing to 3-color palette with dithering...")
    quantized = quantize_to_3color(img)
    
    # Save preview
    preview = Image.fromarray(EINK_PALETTE[quantized])
    preview.save("preview_3color.png")
    print("Saved preview to preview_3color.png")
    
    print("Converting to PROGMEM format...")
    bytes_list = convert_to_progmem(quantized, target_width, target_height)
    
    print(f"Generating {output_file}...")
    header_content = generate_header(bytes_list, target_width, target_height, var_name, input_file)
    
    with open(output_file, 'w') as f:
        f.write(header_content)
    
    print(f"Done! Generated {output_file} with {len(bytes_list)} bytes")
    print(f"Array name: {var_name}")

if __name__ == "__main__":
    main()
