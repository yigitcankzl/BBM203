#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>


// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    // 3. Update each pixel with the computed mean.



    int height = image.get_height();
    int width = image.get_width();
    int** originalData = image.get_data();
    
    GrayscaleImage filteredImage(width, height);
    int offset = kernelSize / 2;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int sum = 0;

            for (int ky = -offset; ky <= offset; ++ky) {
                for (int kx = -offset; kx <= offset; ++kx) {
                    int neighborY = y + ky;
                    int neighborX = x + kx;

                    if (neighborY >= 0 && neighborY < height && neighborX >= 0 && neighborX < width) {
                        sum += originalData[neighborY][neighborX];
                    }
                }
            }

            int mean = sum / (kernelSize*kernelSize); 
            filteredImage.set_pixel(y,x, mean);
        }
    }

    image = filteredImage;
}





// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.




    int half_size = kernelSize / 2;
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    double sum = 0.0; 

    for (int x = -half_size; x <= half_size; ++x) {
        for (int y = -half_size; y <= half_size; ++y) {
            double value = (1.0 / (2.0 * M_PI * sigma * sigma)) * 
                           exp(-(x * x + y * y) / (2.0 * sigma * sigma));
            kernel[x + half_size][y + half_size] = value;
            sum += value; 
        }
    }

    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] /= sum; 
        }
    }

    GrayscaleImage smoothed_img = image; 
    for (int i = 0; i < image.get_height(); ++i) {
        for (int j = 0; j < image.get_width(); ++j) {
            double new_pixel_value = 0.0;

            for (int ki = -half_size; ki <= half_size; ++ki) {
                for (int kj = -half_size; kj <= half_size; ++kj) {
                    int ni = i + ki;
                    int nj = j + kj;

                    double pixel_value = (ni < 0 || ni >= image.get_height() || nj < 0 || nj >= image.get_width()) 
                                         ? 0.0 
                                         : image.get_pixel(ni, nj); 

                    new_pixel_value += pixel_value * kernel[ki + half_size][kj + half_size];
                }
            }

            smoothed_img.set_pixel(i, j, static_cast<int>(std::min(std::max(new_pixel_value, 0.0), 255.0))); 
        }
    }

    image = smoothed_img;
}



// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // TODO: Your code goes here.
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    // 3. Clip values to ensure they are within a valid range [0-255].


 
    GrayscaleImage blurredImage = image; 
    apply_gaussian_smoothing(blurredImage, kernelSize, 1.0); 

    
    int height = image.get_height();
    int width = image.get_width();
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int original = image.get_pixel(y, x);
            int blurred = blurredImage.get_pixel(y, x);
            int sharpened = static_cast<int>(original + amount * (original - blurred));
            sharpened = std::min(255, std::max(0, sharpened));
            image.set_pixel(y, x, sharpened);
        }
    }
}


