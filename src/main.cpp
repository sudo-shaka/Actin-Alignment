#include <vector>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <map>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "../include/analyize.hpp"
#include "../include/imgRead.hpp"

int main(int argc, char *argv[]){
    //getting filepaths from user arguments
    std::vector<std::string> filePaths;
    if(argc <= 1){
      filePaths.push_back(".");
    } else {
      for(int i = 1; i < argc; i++){
        filePaths.push_back(argv[i]);
      }
    }

    //Get the image files from the entered filePath list/directory
    std::vector<std::filesystem::path> imgFiles = imgPaths(filePaths);

    //if no images found, exit program
    if(imgFiles.size() == 0){
            std::cout << "No image files found. Exiting." << std::endl;
            return 0;
    }
    //making CSV to export data
    std::ofstream outFile("output.csv");

    //setting up analysis;
    int radius = 20;
    std::vector<float> angles = anglesEvery(15.0);
    std::vector<coord> line = lineGen(radius, 1);

    //for each image, open and do analysis
    for(int i=0; i<imgFiles.size(); i++){
      cv::Mat image = cv::imread(imgFiles[i].string(), cv::IMREAD_COLOR);
      if(image.empty()){
        std::cerr << "Unable to load image: " << imgFiles[i].string() << std::endl;
        continue;
      }

      //making array
      std::vector<std::vector<int>> imgArr = im2arr(image);
      //analyzing
      std::cout << "Analyzing image: " << imgFiles[i].string() << " ... " << std::flush;
      std::map<float,int> counts = analyize(imgArr, &image, angles, line, radius);
      std::cout << " Done." <<std::endl;
      cv::imwrite(imgFiles[i].filename().string() + "-out.png", image);

      //writing csv file
      if(outFile.is_open()){
        if(i == 0){
          outFile << "Angle:";
          for(auto& a : angles){
            outFile << "," << a;
          }
          outFile << "\n";
        }
        outFile << imgFiles[i].string() << ":";
        for(auto& pair : counts){
          outFile << "," << pair.second;
        }
        outFile << "\n";
      } else {
        std::cerr << "Error: Could not open output csv for writing." << std::endl;
      }
    }

    if(outFile.is_open()){
      std::cout << "Check 'output.csv' for results." << std::endl;
    }

    return 0;
}
