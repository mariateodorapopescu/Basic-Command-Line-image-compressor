# Image Compression and Decompression 📸✨

**Note:** Some comments in the source code files are in Romanian.

## Overview
This project, marked as "v0" (first version), provides functions for reading, compressing, decompressing, and writing PPM image files. It introduces a unique approach to image compression using a tree structure, showcasing a novel perspective on handling pixel data.

## Structure
- **read:**
  - Reads a PPM file, extracts metadata, and allocates memory for image storage.
- **compress:**
  - Implements image compression based on color averages and a similarity score threshold. Uses a tree structure to organize color data.
- **for:**
  - Creates a vector of connections for pixel placement during compression.
- **createvector:**
  - Populates the compression vector with color data, marking nodes as leaves if necessary.
- **createtree:**
  - Allocates and initializes the compression tree, filling it with color data.
- **decompress:**
  - Reconstructs the image matrix during decompression, progressively recreating the image based on tree structure.
- **write:**
  - Writes the compressed or decompressed image to an output file.
- **vertically:**
  - Vertically twists the image using a clock-wise order.
- **horizontally:**
  - Horizontally twists the image using a clock-wise order.
- **deallocate:**
  - Recursively frees allocated memory for image blocks.

## Main Function (main)
- **Compression:**
  - Initializes compression tree, reads image and compression factor, compresses image, allocates and populates compression vector, releases allocated memory.
- **Decompression:**
  - Reads color information, initializes tree, reconstructs image using createtree and decompress functions, allocates memory for image, writes the decompressed image.
- **Mirror Image:**
  - Optionally mirrors the compressed image horizontally, vertically, or both, decompresses and writes the mirrored image, and releases allocated memory.

## Important Notes
- **Language:** Some comments and function descriptions are in Romanian.
- **Version Warning:** This is the initial version ("v0") and may have limitations.

Explore the fascinating world of image manipulation with this unique compression and decompression tool! 🌈🖼️ #ImageProcessing #CompressionMagic #CodeInnovation
