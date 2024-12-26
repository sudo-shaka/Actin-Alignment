#include <filesystem>
#include <opencv2/opencv.hpp>

std::vector<std::filesystem::path> imgPaths(std::vector<std::string> arg);
int** im2arr(cv::Mat image);
