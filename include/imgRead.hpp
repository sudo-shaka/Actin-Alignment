//imgRead.hpp

#ifndef __IMGREAD_INCLUDED__
#define __IMGREAD_INCLUDED__

#include <filesystem>
#include <opencv2/opencv.hpp>

std::vector<std::filesystem::path> imgPaths(std::vector<std::string> arg);
int** im2arr(cv::Mat image);

#endif