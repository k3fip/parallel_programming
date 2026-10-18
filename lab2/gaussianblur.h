#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <thread>

class ParallelBlur {
public:
    // Выполняет размытие изображения с заданным радиусом и количеством потоков.
    // Возвращает новое изображение, не изменяя исходное.
    static cv::Mat apply(const cv::Mat& input, int radius, int numThreads);

private:
    // Однопоточная обработка одного квадрата (региона)
    static void processRegion(const cv::Mat& input, cv::Mat& output,
                              int startX, int startY, int endX, int endY,
                              int radius);
};