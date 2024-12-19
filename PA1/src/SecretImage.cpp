#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    // 2. Fill both matrices with the pixels from the GrayscaleImage.

    width = image.get_width();
    height = image.get_height();
    
    upper_triangular = new int[width * (width + 1) / 2];
    lower_triangular = new int[width * (width - 1) / 2];

    int upper_index = 0, lower_index = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel_value = image.get_pixel(i, j);
            if (i <= j) {
                upper_triangular[upper_index++] = pixel_value;
            } else {
                lower_triangular[lower_index++] = pixel_value;
            }
        }
    }
}



// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // TODO: Your code goes here.
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.

    width = w;
    height = h;

    upper_triangular = new int[width * (width + 1) / 2];
    lower_triangular = new int[width * (width - 1) / 2];

    for (int i = 0; i < width * (width + 1) / 2; i++) {
        upper_triangular[i] = upper[i];
    }

    for (int i = 0; i < width * (width - 1) / 2; i++) {
        lower_triangular[i] = lower[i];
    }
}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    // TODO: Your code goes here.
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.

    delete[] upper_triangular;
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    // TODO: Your code goes here.

    int upper_index = 0, lower_index = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i <= j) {
                image.set_pixel(i, j, upper_triangular[upper_index++]);
            } else {
                image.set_pixel(i, j, lower_triangular[lower_index++]);
            }
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    // Update the lower and upper triangular matrices 
    // based on the GrayscaleImage given as the parameter.


    int upper_index = 0, lower_index = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel_value = image.get_pixel(i, j);
            if (i <= j) {
                upper_triangular[upper_index++] = pixel_value;
            } else {
                lower_triangular[lower_index++] = pixel_value;
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    // TODO: Your code goes here.
    // 1. Write width and height on the first line, separated by a SINGLE SPACE.
    // 2. Write the upper_triangular array to the SECOND line.
    // Ensure that the elements are space-separated. 
    // If there are 15 elements, write them as: "element1 element2 ... element15"
    // 3. Write the lower_triangular array to the THIRD line in a similar manner
    // as the second line.

    std::ofstream outfile(filename);


    outfile << width << " " << height << "\n";

    for (int i = 0; i < width * (width + 1) / 2; i++) {
        outfile << upper_triangular[i];
        if (i != width * (width + 1) / 2 - 1) outfile << " ";
    }
    outfile << "\n";

    for (int i = 0; i < width * (width - 1) / 2; i++) {
        outfile << lower_triangular[i];
        if (i != width * (width - 1) / 2 - 1) outfile << " ";
    }
    outfile << "\n";

    outfile.close();
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    // TODO: Your code goes here.
    // 1. Open the file and read width and height from the first line, separated by a space.
    // 2. Calculate the sizes of the upper and lower triangular arrays.
    // 3. Allocate memory for both arrays.
    // 4. Read the upper_triangular array from the second line, space-separated.
    // 5. Read the lower_triangular array from the third line, space-separated.
    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.

    std::ifstream infile(filename);


    int w, h;
    infile >> w >> h;

    int upper_size = w * (w + 1) / 2;
    int lower_size = w * (w - 1) / 2;

    int *upper = new int[upper_size];
    int *lower = new int[lower_size];

    for (int i = 0; i < upper_size; i++) {
        infile >> upper[i];
    }

    for (int i = 0; i < lower_size; i++) {
        infile >> lower[i];
    }

    infile.close();
    
    return SecretImage(w, h, upper, lower);
;
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
