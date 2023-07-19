#include <iostream>
#include <vector>
#include <string>
#include <fstream>

struct Pixel {
    int red;
    int green;
    int blue;
};

class Image {
private:
    int width;
    int height;
    std::vector<Pixel> pixels;

public:
    Image(int w, int h) : width(w), height(h) {
        pixels.resize(width * height);
    }

    void setPixel(int x, int y, int r, int g, int b) {
        int index = y * width + x;
        pixels[index].red = r;
        pixels[index].green = g;
        pixels[index].blue = b;
    }

    void readPPM(const std::string& filename) {
        std::ifstream file(filename);
        std::string format;
        file >> format >> width >> height;
        int maxValue;
        file >> maxValue;

        pixels.resize(width * height);

        for (int i = 0; i < width * height; ++i) {
            int r, g, b;
            file >> r >> g >> b;
            setPixel(i % width, i / width, r, g, b);
        }

        file.close();
    }

    void writePPM(const std::string& filename) {
        std::ofstream file(filename);
        file << "P3\n";
        file << width << " " << height << "\n";
        file << "255\n";

        for (int i = 0; i < width * height; ++i) {
            file << pixels[i].red << " " << pixels[i].green << " " << pixels[i].blue << "\n";
        }

        file.close();
    }

    void grayscale() {
        for (int i = 0; i < width * height; ++i) {
            int gray = (pixels[i].red + pixels[i].green + pixels[i].blue) / 3;
            pixels[i].red = gray;
            pixels[i].green = gray;
            pixels[i].blue = gray;
        }
    }

    void enlarge() {
        int newWidth = width * 2;
        int newHeight = height * 2;
        std::vector<Pixel> enlargedPixels(newWidth * newHeight);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = y * width + x;
                int enlargedIndex = (y * 2) * newWidth + (x * 2);
                enlargedPixels[enlargedIndex] = pixels[index];
                enlargedPixels[enlargedIndex + 1] = pixels[index];
                enlargedPixels[enlargedIndex + newWidth] = pixels[index];
                enlargedPixels[enlargedIndex + newWidth + 1] = pixels[index];
            }
        }

        width = newWidth;
        height = newHeight;
        pixels = enlargedPixels;
    }

    void reduce() {
        int newWidth = width / 2;
        int newHeight = height / 2;
        std::vector<Pixel> reducedPixels(newWidth * newHeight);

        for (int y = 0; y < newHeight; ++y) {
            for (int x = 0; x < newWidth; ++x) {
                int index = y * newWidth + x;
                int originalX = x * 2;
                int originalY = y * 2;
                int r = (pixels[originalY * width + originalX].red +
                         pixels[originalY * width + originalX + 1].red +
                         pixels[(originalY + 1) * width + originalX].red +
                         pixels[(originalY + 1) * width + originalX + 1].red) / 4;
                int g = (pixels[originalY * width + originalX].green +
                         pixels[originalY * width + originalX + 1].green +
                         pixels[(originalY + 1) * width + originalX].green +
                         pixels[(originalY + 1) * width + originalX + 1].green) / 4;
                int b = (pixels[originalY * width + originalX].blue +
                         pixels[originalY * width + originalX + 1].blue +
                         pixels[(originalY + 1) * width + originalX].blue +
                         pixels[(originalY + 1) * width + originalX + 1].blue) / 4;
                reducedPixels[index].red = r;
                reducedPixels[index].green = g;
                reducedPixels[index].blue = b;
            }
        }

        width = newWidth;
        height = newHeight;
        pixels = reducedPixels;
    }

    void rotate() {
        int newWidth = height;
        int newHeight = width;
        std::vector<Pixel> rotatedPixels(newWidth * newHeight);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = y * width + x;
                int rotatedIndex = x * newWidth + (newWidth - 1 - y);
                rotatedPixels[rotatedIndex] = pixels[index];
            }
        }

        width = newWidth;
        height = newHeight;
        pixels = rotatedPixels;
    }

    void applySharpeningFilter() {
        std::vector<std::vector<int>> filter = {
            {0, -1, 0},
            {-1, 5, -1},
            {0, -1, 0}
        };
        applyFilter(filter);
    }

    void applyBlurringFilter() {
        std::vector<std::vector<int>> filter = {
            {1, 1, 1},
            {1, 1, 1},
            {1, 1, 1}
        };
        applyFilter(filter);
    }

private:
    void applyFilter(const std::vector<std::vector<int>>& filter) {
        int filterSize = filter.size();
        int filterRadius = filterSize / 2;

        std::vector<Pixel> filteredPixels(width * height);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = y * width + x;
                int sumR = 0, sumG = 0, sumB = 0;

                for (int fy = 0; fy < filterSize; ++fy) {
                    for (int fx = 0; fx < filterSize; ++fx) {
                        int nx = x + fx - filterRadius;
                        int ny = y + fy - filterRadius;

                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            int pixelIndex = ny * width + nx;
                            int filterValue = filter[fy][fx];
                            sumR += pixels[pixelIndex].red * filterValue;
                            sumG += pixels[pixelIndex].green * filterValue;
                            sumB += pixels[pixelIndex].blue * filterValue;
                        }
                    }
                }

                filteredPixels[index].red = std::max(0, std::min(sumR, 255));
                filteredPixels[index].green = std::max(0, std::min(sumG, 255));
                filteredPixels[index].blue = std::max(0, std::min(sumB, 255));
            }
        }

        pixels = filteredPixels;
    }
};

void printMenu() {
    std::cout << "========== Image Processing Program ==========" << std::endl;
    std::cout << "1. Transform to Grayscale" << std::endl;
    std::cout << "2. Enlarge Image" << std::endl;
    std::cout << "3. Reduce Image" << std::endl;
    std::cout << "4. Rotate Image" << std::endl;
    std::cout << "5. Apply Sharpening Filter" << std::endl;
    std::cout << "6. Apply Blurring Filter" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << "Enter your choice: ";
}

int main(int argc, char* argv[]) {

    // código que eu fiz para funcionar ./program_name function_name < imgs/original_img_name.ppm > imgs_output/processed_img_name.ppm
    // exemplo que era pra funcionar: ./procimg rotate < imgs/galinhos.ppm > imgs_output/rotate.ppm
    // exemplo que tá funcionando (função fora da classe): ./procimg printMenu < imgs/galinhos.ppm > imgs_output/printMenu.ppm
    //Image image();
    // Check if the correct number of arguments is provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <function_name>\n";
        return 1;
    }

    // Extract command line argument for function name
    std::string function_name = argv[1];

    // Open input image file from standard input
    std::ifstream inputFile;
    inputFile.open("/dev/stdin");
    if (!inputFile) {
        std::cerr << "Error: Failed to open input image file.\n";
        return 0;
    }

    // Open output image file from standard output
    std::ofstream outputFile;
    outputFile.open("/dev/stdout");
    if (!outputFile) {
        std::cerr << "Error: Failed to open output image file.\n";
        return 0;
    }


    if (function_name == "printMenu") {
        printMenu();
    }
    /*
    // Determine which image processing function to call based on function_name
    if (function_name == "grayscale") {
        image.grayscale();//inputFile, outputFile);
    } else if (function_name == "enlarge") {
        image.enlarge();//inputFile, outputFile);
    } else if (function_name == "reduce") {
        image.reduce();//inputFile, outputFile);
    } else if (function_name == "rotate") {
        image.rotate();//inputFile, outputFile);
    } else if (function_name == "applyBlurringFilter") {
        image.applyBlurringFilter();//inputFile, outputFile);
    } else if (function_name == "applySharpeningFilter") {
        image.applySharpeningFilter();//inputFile, outputFile);
    } else {
        std::cerr << "Error: Invalid function name.\n";
        return 1;
    }
    */

    // Close input and output files (not necessary for standard input and output)
    inputFile.close();
    outputFile.close();

    /* // o código que o Kaio fez inicialmente:
    std::string filename;
    std::cout << "Enter the filename of the image (PPM format): ";
    std::cin >> filename;

    Image image(0, 0);
    image.readPPM(filename);

    int choice = -1;

    while (choice != 0) {
        printMenu();
        std::cin >> choice;

        switch (choice) {
            case 0:
                std::cout << "Exiting program..." << std::endl;
                break;
            case 1:
                image.grayscale();
                std::cout << "Image transformed to grayscale." << std::endl;
                break;
            case 2:
                image.enlarge();
                std::cout << "Image enlarged." << std::endl;
                break;
            case 3:
                image.reduce();
                std::cout << "Image reduced." << std::endl;
                break;
            case 4:
                image.rotate();
                std::cout << "Image rotated." << std::endl;
                break;
            case 5:
                image.applySharpeningFilter();
                std::cout << "Sharpening filter applied to the image." << std::endl;
                break;
            case 6:
                image.applyBlurringFilter();
                std::cout << "Blurring filter applied to the image." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }

    std::string outputFilename;
    std::cout << "Enter the output filename (PPM format): ";
    std::cin >> outputFilename;
    image.writePPM(outputFilename);

    std::cout << "Image saved to " << outputFilename << std::endl;

    */

    return 0;
}