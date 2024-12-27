#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <cmath>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include "../include/analyize.hpp"

//generate a vector of angles every delta. Similar to linspace(-90,90,delta)
std::vector<float> anglesEvery(float delta){
    std::vector<float> a;
    for(float ang = 90.0; ang > -90.0; ang -= delta){
        a.push_back(ang);
    }
    return a;
}

//if value is not in slice function
bool notInSlice(std::vector<float> slice, float val){
  for(int i=0; i < slice.size(); i++){
    if(val == slice[i])
      return false;
  }
  return true;
}

//rotate a point in the line around angle (a)
coord rotate(coord Coord,float a){
  coord p;
  float s = sin(a * M_PI / 180.0);
  float c = cos(a * M_PI / 180.0);
  p.x = int(round(c * float(Coord.x) - s * float(Coord.y)));
  p.y = int(round(s * float(Coord.x) + c * float(Coord.y)));

  return p;
}

//rotate a line by a given angle; save lines as a mask for image analysis
void angleMaskGen(std::vector<float> angles, std::vector<coord> line, std::map<coord,std::vector<float>> *anglesOf, std::map<float,int> *angleCounts, std::vector<coord> *nSlice){
  std::map<coord,std::vector<float>> angOf;
  std::map<float, int> angCounts;
  std::map<coord,bool> neighborSet;
  coord rp;
  for(auto & a : angles){
    for(auto & p : line){
      rp = rotate(p, a);
      neighborSet[rp] = true;
      if(angOf.count(rp) > 0){
        angOf[rp] = std::vector<float>{a};
        angCounts[a]++;
      } else if(notInSlice(angOf[rp],a)){
        angOf[rp].push_back(a);
        angCounts[a]++;
      }
    }
  }

  *anglesOf = angOf;
  *angleCounts = angCounts;
  std::vector<coord> neighborSlice;
  for(auto & n : neighborSet){
    neighborSlice.push_back(n.first);
  }

  *nSlice = neighborSlice;
}

//generate a straight line 
std::vector<coord> lineGen(int radius, int fatness){
  std::vector<coord> lineCoords;
  coord c;
  c.x = radius; c.y = 0;
  lineCoords.push_back(c);
  lineCoords.push_back(c);
  for(int i=-radius+1; i<radius; i++){
    c.x = i;
    lineCoords.push_back(c);
    for(int d = 1; d < fatness; d++){
      c.y = d;
      lineCoords.push_back(c);
      c.y = -d;
      lineCoords.push_back(c);
    }
  }
  return lineCoords;
}

//get the thresholding value for each pixel
int getThresh(cv::Mat image){
  cv::Scalar mean, stddev;
  cv::meanStdDev(image,mean,stddev);
  int ans = int(float(mean[0]) + 0.5*float(stddev[0]));
  return ans > 100 ? 100 : ans;
}

//for each line, find the angle which has the most neighboring pixels
float bestAngle(std::map<float,int> angleTotals, std::map<float,int> angleCounts){
  float bestAngle = -180.0;
  float largestTotal = 0.0;
  float metric;
  for(auto& pair : angleTotals){
    metric = float(pair.second)/float(angleCounts[pair.first]);
    if(metric > largestTotal){
      largestTotal = metric;
      bestAngle = pair.first;
    }
  }
  return bestAngle;
}


std::map<float, int> analyize(int** imgArr,cv::Mat* image, std::vector<float> angles, std::vector<coord> line, int r){

  std::map<float,int> counts; //output histogram
  cv::Mat img = *image; // input image

  //angleMaskGenfunction
  std::map<coord,std::vector<float>> anglesOf;
  std::map<float, int> angleCounts;
  std::vector<coord> neighborSlice;
  angleMaskGen(angles, line, &anglesOf, &angleCounts, &neighborSlice);

  //getThresh
  int thresh = getThresh(img);
  
  //toggle boolean to enable/disable pop-up
  bool display = true;

  //loop through image and do analysis (find best angle for each pixel)
  //after best angle is found, add one to that best angle for a count
  int pixel;
  float best;
  std::map<float,int> angleTotals;
  if(display)
    cv::namedWindow("Progress",cv::WINDOW_AUTOSIZE);
  for(int x = r; x < img.rows-r; x++){
    for(int y = r; y < img.cols-r; y++){
      if(imgArr[x][y] < thresh){
        img.at<cv::Vec3b>(x,y) = cv::Vec3b(0,0,0);
        continue;
      }

      for(auto & a : angles){
        angleTotals[a] = 0;
      }
      for(auto & p : neighborSlice){
        for(auto & a : anglesOf[p]){
          angleTotals[a] += imgArr[x+p.x][y+p.y];
        }
      }
      best = bestAngle(angleTotals,angleCounts); //find best angle
      counts[best]++; //add one for the best angle in histogram
      int scale = int(255.8*(abs(best)/90.0));
      img.at<cv::Vec3b>(x,y) = cv::Vec3b(scale,0,(255-scale)); //set the color (more red = verticle, more blue = horizontal
    }
    if(display && x % int(img.rows/r) == 0){
      cv::imshow("Progress",img);
      cv::waitKey(50);
    }
  }
  //return image and counts
  *image = img;
  return counts;
}
