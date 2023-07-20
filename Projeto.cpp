#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

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

    void readPPM(){
        std::string format;
        int maxValue;
        std::cin >> format >> width >> height >> maxValue;

        pixels.resize(width * height);

        for (int i = 0; i < width * height; ++i) {
            int r, g, b;
            std::cin >> r >> g >> b;
            setPixel(i % width, i / width, r, g, b);
        }
    }

    void printPPM() {//const std::string& filename) {
        std::cout << "P3\n";
        std::cout << width << " " << height << "\n";
        std::cout << "255\n";

/*         for (int i = 0; i < width * height; ++i) {
            std::cout << pixels[i].red << " " << pixels[i].green << " " << pixels[i].blue << std::endl;
        } */

        for (int i = 0; i < height; ++i) {
            for (int j=0; j < width; ++j) {
                std::cout << pixels[j].red << " " << pixels[j].green << " " << pixels[j].blue << " ";
                if(j== width-1){
                    std::cout << std::endl;
                }
            }
        }

    }

    void grayscale() {
        for (int i = 0; i < width * height; ++i) {
            int gray = (pixels[i].red + pixels[i].green + pixels[i].blue) / 3;
            pixels[i].red = gray;
            pixels[i].green = gray;
            pixels[i].blue = gray;
        }
        printPPM();
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
        printPPM();
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
        printPPM();
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
        printPPM();
    }

    void applySharpeningFilter() {
        std::vector<std::vector<int>> filter = {
            {0, -1, 0},
            {-1, 5, -1},
            {0, -1, 0}
        };
        applyFilter(filter);
        printPPM();
    }

    void applyBlurringFilter() {
        std::vector<std::vector<int>> filter = {
            {1, 1, 1},
            {1, 1, 1},
            {1, 1, 1}
        };
        applyFilter(filter);
        printPPM();
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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <the_desired_function_name> <my_image_name> > <my_processed_image_name>" << std::endl;
        return 1;
    }

    std::string function_name = argv[1];
    //std::string my_image;
    //std::string my_processed_image_name;

    //std::cin >> my_image;

    Image image(0,0);
    image.readPPM();

    // switch case não funciona pra string por padrão
    /*  switch (function_name) {
        case 'grayscale':
            image.grayscale();
            break;
        case 'enlarge':
            image.enlarge();
            break;
        case 'reduce':
            image.reduce();
            break;
        case 'rotate':
            image.rotate();
            break;
        case 'applySharpeningFilter':
            image.applySharpeningFilter();
            break;
        case 'applyBlurringFilter':
            image.applyBlurringFilter();
            break;
        default:
            std::cerr << "Error: Invalid function name.\n";
            return 0;
    } */

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

    return 0;
}