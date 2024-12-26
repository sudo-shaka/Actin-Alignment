#include <vector>
#include <opencv2/opencv.hpp>

std::vector<std::filesystem::path> imgPaths(std::vector<std::string> arg);
std::vector<std::vector<int>> im2arr(cv::Mat image);
