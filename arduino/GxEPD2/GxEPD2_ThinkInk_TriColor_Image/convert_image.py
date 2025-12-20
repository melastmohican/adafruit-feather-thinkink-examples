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

def convert_to_progmem_plane(quantized, width, height, target_color_idx, active_level=0):
    """
    Convert quantized image to PROGMEM byte array format for a specific color plane.
    If active_level is 0: Target color pixels will be 0 (active), others 1 (inactive/white).
    If active_level is 1: Target color pixels will be 1 (active), others 0 (inactive/white).
    1 bit per pixel, 8 pixels per byte, MSB first.
    """
    bytes_list = []
    
    for y in range(height):
        for x in range(0, width, 8):
            byte_val = 0
            for i in range(8):
                if x + i < width:
                    pixel = quantized[y, x + i]
                    # Check if pixel matches target
                    is_target = (pixel == target_color_idx)
                    
                    if active_level == 0:
                        # 0=Active/Target, 1=Inactive
                        bit = 0 if is_target else 1
                    else:
                        # 1=Active/Target, 0=Inactive
                        bit = 1 if is_target else 0
                else:
                    # Padding.
                    # If active_level=0 (active low), inactive is 1.
                    # If active_level=1 (active high), inactive is 0.
                    bit = 1 if active_level == 0 else 0
                
                byte_val = (byte_val << 1) | (bit & 0x01)
            bytes_list.append(byte_val)
    
    return bytes_list

def generate_header(black_bytes, red_bytes, width, height, var_name, filename):
    """Generate C header file content with two arrays."""
    lines = []
    lines.append(f"// Generated from {filename}")
    lines.append(f"// Image size: {width}x{height} pixels")
    lines.append(f"// Format: 3-color (1 bit per pixel), 8 pixels per byte")
    lines.append(f"// Black bytes: {len(black_bytes)}")
    lines.append(f"// Red bytes: {len(red_bytes)}")
    lines.append("")
    lines.append(f"#ifndef _{var_name.upper()}_H_")
    lines.append(f"#define _{var_name.upper()}_H_")
    lines.append("")
    lines.append("#include <Arduino.h>")
    lines.append("")
    
    # Generate Black Array
    lines.append(f"const uint8_t {var_name}_black[{len(black_bytes)}] PROGMEM = {{")
    for i in range(0, len(black_bytes), 16):
        chunk = black_bytes[i:i+16]
        hex_values = ", ".join(f"0x{b:02X}" for b in chunk)
        if i + 16 < len(black_bytes):
            hex_values += ","
        lines.append(f"  {hex_values}")
    lines.append("};")
    lines.append("")

    # Generate Red Array
    lines.append(f"const uint8_t {var_name}_red[{len(red_bytes)}] PROGMEM = {{")
    for i in range(0, len(red_bytes), 16):
        chunk = red_bytes[i:i+16]
        hex_values = ", ".join(f"0x{b:02X}" for b in chunk)
        if i + 16 < len(red_bytes):
            hex_values += ","
        lines.append(f"  {hex_values}")
    lines.append("};")
    lines.append("")

    lines.append("#endif")
    lines.append("")
    
    return "\n".join(lines)

def save_bit_preview(bytes_list, width, height, filename, invert_bits=False):
    """
    Save a preview image from the generated byte data.
    invert_bits: If True, treats bit 0 as White and 1 as Black (Active High source).
                 If False, treats bit 0 as Black and 1 as White (Active Low source).
    """
    img = Image.new('1', (width, height))
    pixels = img.load()
    
    byte_idx = 0
    bit_idx = 0
    
    for y in range(height):
        for x in range(width):
            if byte_idx < len(bytes_list):
                byte = bytes_list[byte_idx]
                # MSB first
                bit = (byte >> (7 - bit_idx)) & 0x01
                
                # For preview:
                # If Active Low (invert_bits=False): 0=Active(Draw), 1=Inactive(White)
                # We want Active -> Black (0), Inactive -> White (1). So just use bit value.
                
                # If Active High (invert_bits=True): 1=Active(Draw), 0=Inactive(White)
                # We want Active -> Black (0), Inactive -> White (1). So we need to invert the bit.
                
                if invert_bits:
                    pixel_val = 1 - bit  # 1->0 (Black), 0->1 (White)
                else:
                    pixel_val = bit      # 0->0 (Black), 1->1 (White)
                
                pixels[x, y] = pixel_val
                
                bit_idx += 1
                if bit_idx == 8:
                    bit_idx = 0
                    byte_idx += 1
    
    img.save(filename)
    print(f"Saved preview: {filename}")


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

    # Count pixels
    unique, counts = np.unique(quantized, return_counts=True)
    pixel_counts = dict(zip(unique, counts))
    print("Pixel counts:")
    print(f"  Black (0): {pixel_counts.get(0, 0)}")
    print(f"  White (1): {pixel_counts.get(1, 0)}")
    print(f"  Red   (2): {pixel_counts.get(2, 0)}")

    
    # Save preview
    preview = Image.fromarray(EINK_PALETTE[quantized])
    preview.save("preview_3color.png")
    print("Saved preview to preview_3color.png")
    
    print("Converting to PROGMEM format (Split Planes)...")
    # 0=Black, 1=White, 2=Red
    # Black Plane: Active Low (0=Black)
    black_bytes = convert_to_progmem_plane(quantized, target_width, target_height, 0, active_level=0)
    # Save preview for Black (Active=0)
    save_bit_preview(black_bytes, target_width, target_height, "preview_black.png", invert_bits=False)
    
    # Red Plane: Active Low (0=Red) - Library inverts this to 1 at controller
    red_bytes = convert_to_progmem_plane(quantized, target_width, target_height, 2, active_level=0)
    # Save preview for Red (Active=0)
    save_bit_preview(red_bytes, target_width, target_height, "preview_red.png", invert_bits=False)
    
    print(f"Generating {output_file}...")
    header_content = generate_header(black_bytes, red_bytes, target_width, target_height, var_name, input_file)
    
    with open(output_file, 'w') as f:
        f.write(header_content)
    
    print(f"Done! Generated {output_file}")
    print(f"Arrays: {var_name}_black, {var_name}_red")

if __name__ == "__main__":
    main()
