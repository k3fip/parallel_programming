#include "gaussianblur.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
    if (argc < 4 || argc > 5) {
        std::cerr << "Usage: blur <input.bmp> <output.bmp> <threads> [radius]\n";
        return 1;
    }

    std::string inputPath = argv[1];
    std::string outputPath = argv[2];
    int threads = 0;
    int radius = 5;

    try {
        threads = std::stoi(argv[3]);
        if (threads <= 0) {
            std::cerr << "Error: threads must be positive integer.\n";
            return 1;
        }
        if (argc == 5) {
            radius = std::stoi(argv[4]);
            if (radius < 0) {
                std::cerr << "Error: radius must be non-negative integer.\n";
                return 1;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: invalid argument: " << e.what() << "\n";
        return 1;
    }

    // Чтение изображения
    cv::Mat input = cv::imread(inputPath, cv::IMREAD_COLOR);
    if (input.empty()) {
        std::cerr << "Error: cannot read input file: " << inputPath << "\n";
        return 1;
    }

    auto start = std::chrono::steady_clock::now();

    cv::Mat output = ParallelBlur::apply(input, radius, threads);

    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Сохранение результата
    if (!cv::imwrite(outputPath, output)) {
        std::cerr << "Error: cannot write output file: " << outputPath << "\n";
        return 1;
    }

    // Вывод CSV-строки в stdout
    unsigned int hw = std::thread::hardware_concurrency();
    std::cout << threads << "," << radius << "," << hw << "," << ms << "\n";

    return 0;
}