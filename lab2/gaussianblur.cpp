#include "gaussianblur.h"
#include <algorithm>
#include <cmath>
#include <vector>

inline int clamp(int val, int minVal, int maxVal) 
{
    return std::max(minVal, std::min(val, maxVal));
}


static std::vector<float> makeGaussianKernel1D(int radius, float sigma) 
{
    int size = 2 * radius + 1;
    std::vector<float> kernel(size);
    float sum = 0.0f;
    for (int i = -radius; i <= radius; ++i) 
    {
        float val = std::exp(-(i * i) / (2.0f * sigma * sigma));
        kernel[i + radius] = val;
        sum += val;
    }
    for (float& v : kernel) v /= sum;
    return kernel;
}

void ParallelBlur::processRegion(const cv::Mat& input, cv::Mat& output,
                                 int startX, int startY, int endX, int endY,
                                 int radius)
{
    float sigma = radius / 3.0f;
    if (sigma < 0.1f) sigma = 0.1f;
    auto kernel = makeGaussianKernel1D(radius, sigma);

    int w = input.cols;
    int h = input.rows;

    // Проходим по всем пикселям региона
    for (int y = startY; y < endY; ++y) 
    {
        for (int x = startX; x < endX; ++x) 
        {
            float r = 0, g = 0, b = 0;

            for (int dy = -radius; dy <= radius; ++dy) 
            {
                int iy = clamp(y + dy, 0, h - 1);
                for (int dx = -radius; dx <= radius; ++dx) 
                {
                    int ix = clamp(x + dx, 0, w - 1);
                    float weight = kernel[dy + radius] * kernel[dx + radius];
                    cv::Vec3b pixel = input.at<cv::Vec3b>(iy, ix);
                    b += pixel[0] * weight;
                    g += pixel[1] * weight;
                    r += pixel[2] * weight;
                }
            }

            cv::Vec3b& out = output.at<cv::Vec3b>(y, x);
            out[0] = static_cast<uchar>(std::clamp(b, 0.0f, 255.0f));
            out[1] = static_cast<uchar>(std::clamp(g, 0.0f, 255.0f));
            out[2] = static_cast<uchar>(std::clamp(r, 0.0f, 255.0f));
        }
    }
}

cv::Mat ParallelBlur::apply(const cv::Mat& input, int radius, int numThreads) 
{
    cv::Mat output = cv::Mat::zeros(input.size(), input.type());

    int w = input.cols;
    int h = input.rows;

    if (numThreads > w * h) numThreads = std::max(1, w * h);


    int N = numThreads;

    int blockW = (w + N - 1) / N;
    int blockH = (h + N - 1) / N;

    // Вектор потоков
    std::vector<std::jthread> threads;
    threads.reserve(N);

    for (int t = 0; t < N; ++t) 
    {
        threads.emplace_back([&, t]() 
        {
            // Поток t обрабатывает квадраты с индексами t + k*N для k = 0..N-1
            for (int k = 0; k < N; ++k) {
                int squareIndex = t + k * N;
                if (squareIndex >= N * N) break; // на всякий случай

                int sqY = squareIndex / N;
                int sqX = squareIndex % N;

                int startX = sqX * blockW;
                int startY = sqY * blockH;
                int endX = std::min(startX + blockW, w);
                int endY = std::min(startY + blockH, h);

                if (startX >= endX || startY >= endY) continue; // пустой квадрат

                processRegion(input, output, startX, startY, endX, endY, radius);
            }
        });
    }

    // jthread автоматически присоединяется в деструкторе
    return output;
}