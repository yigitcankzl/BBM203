#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>







// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {

    // Image loading code using stbi
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);
    }

    // Dynamically allocate memory for a 2D matrix based on the given dimensions
    data = new int*[height]; // Allocate array of row pointers
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width]; // Allocate each row

    }

    // Fill the matrix with pixel values from the image
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            data[y][x] = static_cast<int>(image[y * width + x]); 
            
            }
    }

    
    // Free the dynamically allocated memory of stbi image
    stbi_image_free(image);
}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w): width(w), height(h)  {
    // TODO: Your code goes here.
    // Initialize the image with a pre-existing data matrix by copying the values.
    // Don't forget to dynamically allocate memory for the matrix.

    
    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];
        std::memcpy(data[i], inputData[i], width * sizeof(int)); // Copy inputData directly
    }
}



// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {
    // TODO: Your code goes here.
    // Just dynamically allocate the memory for the new matrix.

    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width](); // Initialize all pixels to 0
    }


}

// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) : width(other.width), height(other.height) {
    // TODO: Your code goes here.
    // Copy constructor: dynamically allocate memory and 
    // copy pixel values from another image.

    
    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];
        std::memcpy(data[i], other.data[i], width * sizeof(int)); // Use memcpy for efficiency
    }


}

// Destructor
GrayscaleImage::~GrayscaleImage() {
    // TODO: Your code goes here.
    // Destructor: deallocate memory for the matrix.

    if (data) {
        for (int i = 0; i < height; ++i) {
            delete[] data[i];  
        }
        delete[] data;
    }

}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {
    // TODO: Your code goes here.
    // Check if two images have the same dimensions and pixel values.
    // If they do, return true.

    if (width != other.width || height != other.height) {
        return false; 
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (data[y][x] != other.data[y][x]) {
                return false; 
            }
        }
    }
    return true;
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
    // TODO: Your code goes here.
    // Add two images' pixel values and return a new image, clamping the results.

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int sum = data[y][x] + other.data[y][x];

            result.data[y][x] = static_cast<unsigned char>(std::min(255, sum));
        }
    }

    return result;
}

// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
    // TODO: Your code goes here.
    // Subtract pixel values of two images and return a new image, clamping the results.

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int diff = data[y][x] - other.data[y][x];

            result.data[y][x] = static_cast<unsigned char>(std::max(0, diff));
        }
    }

    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    // Create a buffer to hold the image data in the format stb_image_write expects
    unsigned char* imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}


// Assignment operator
GrayscaleImage& GrayscaleImage::operator=(const GrayscaleImage& other) {
    if (this != &other) { // Self-assignment check
        // Free existing data
        for (int i = 0; i < height; ++i) {
            delete[] data[i];
        }
        delete[] data;

        // Copy dimensions
        width = other.width;
        height = other.height;

        // Allocate new memory
        data = new int*[height];
        for (int i = 0; i < height; ++i) {
            data[i] = new int[width];
            std::memcpy(data[i], other.data[i], width * sizeof(int));
        }
    }
    return *this;
}
