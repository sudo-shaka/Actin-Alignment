#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <regex>
#include <vector>
#include <filesystem>

//return all files found with a image effile extension
std::vector<std::filesystem::path> imgPaths(std::vector<std::string> arg){
  std::vector<std::filesystem::path> paths;
  std::error_code err;
  std::regex ext_check("[^\\s]+(.*?)\\.(jpg|jpeg|png|tiff|tif|JPG|JPEG|PNG|TIFF|TIF)$");

  for(int i=0; i<arg.size(); i++){
    if(std::filesystem::is_directory(arg[i], err)){
      for( auto & entry : std::filesystem::directory_iterator(arg[i])){
        if(std::regex_match(entry.path().string(),ext_check)){
          paths.push_back(entry);
        }
      }
    }
    if(std::filesystem::is_regular_file(arg[i], err) && std::regex_match(arg[i], ext_check)){
      paths.push_back(std::filesystem::path(arg[i]));
    }
    if(err){
      std::cout << "Could not find file: " << arg[i] << " | "<< err << std::endl;
    }
  }

  return paths;
}

//convert the imported RGB image to a 2D grayscale vector matrix
std::vector<std::vector<int>> im2arr(cv::Mat image){
  std::vector<std::vector<int>> imgArr;
  imgArr.resize(image.rows);
  for(int x=0; x<image.rows;x++){
    for(int y=0; y<image.cols; y++){
      cv::Vec3b bgr = image.at<cv::Vec3b>(x,y);
      int grayValue = static_cast<int>(0.114 * bgr[0] + 0.587 * bgr[1] + 0.299 * bgr[2]);
      imgArr[x].push_back(grayValue);
    }
  }
  return imgArr;
}
