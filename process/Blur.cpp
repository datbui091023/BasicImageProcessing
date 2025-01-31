#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm> 
#include <cmath>
#include <vector>
#define M_PI  3.14159
using namespace std;
using namespace cv;
vector<vector<float>> createGaussianKernel(int size, float sigma) {
    vector<vector<float>> kernel(size, vector<float>(size));
    float sum = 0.0f;

    int halfSize = size / 2;
    for (int x = -halfSize; x <= halfSize; ++x) {
        for (int y = -halfSize; y <= halfSize; ++y) {
            kernel[x + halfSize][y + halfSize] = (1/(2*M_PI*sigma*sigma))*exp(-(x * x + y * y) / (2 * sigma * sigma));
            sum += kernel[x + halfSize][y + halfSize];
        }
    }

    // Chuẩn hóa kernel
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            kernel[i][j] /= sum;
        }
    }
    
    return kernel;
}
Mat BlurImage(Mat &input, float blur){
    vector<vector<float>> kernel = createGaussianKernel(5, blur);
    int halfSize = 5 / 2;
    Mat result = Mat::zeros(input.rows,input.cols,CV_8UC3);
    // Duyệt qua từng pixel trong hình ảnh
    for (int i = 0; i < input.rows; ++i) {
        for (int j = 0; j < input.cols; ++j) {
            Vec3f sum = Vec3f(0.0f, 0.0f, 0.0f);

            // Áp dụng kernel lên pixel cho từng kênh màu
            for (int kx = -halfSize; kx <= halfSize; ++kx) {
                for (int ky = -halfSize; ky <= halfSize; ++ky) {
                    int x = min(max(i + kx, 0), input.rows - 1);
                    int y = min(max(j + ky, 0), input.cols - 1);

                    Vec3b pixel = input.at<Vec3b>(x, y);
                    float weight = kernel[kx + halfSize][ky + halfSize];

                    sum[0] += pixel[0] * weight; // Kênh Blue
                    sum[1] += pixel[1] * weight; // Kênh Green
                    sum[2] += pixel[2] * weight; // Kênh Red
                }
            }

            // Gán giá trị đã làm mờ cho ảnh kết quả
            result.at<Vec3b>(i, j) = Vec3b(static_cast<uchar>(sum[0]), static_cast<uchar>(sum[1]), static_cast<uchar>(sum[2]));
        }
    }
    return result;
}