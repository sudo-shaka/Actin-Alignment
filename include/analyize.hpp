#include <vector>
#include <opencv2/opencv.hpp>
struct coord {
    int x, y;

  //operator for use in std::map
  bool operator==(const coord &p) const {
    return x == p.x && y == p.y;
  }
  bool operator<(const coord &p) const {
    return x < p.x || (x == p.x && y < p.y);
  }
};


std::vector<float> anglesEvery(float delta);
bool notInSlice(std::vector<float> slice, float val);
coord rotate(coord Coord,float a);
void angleMaskGen(std::vector<float> angles, std::vector<coord> line, std::map<coord,std::vector<float>> *anglesOf, std::map<float,int> *angleCounts, std::vector<coord> *nSlice);
cv::Vec3b colorScale(float angle);
std::vector<coord> lineGen(int radius, int fatness);
int getThresh(cv::Mat image);
float bestAngle(std::map<float,int> angleTotals, std::map<float,int> angleCounts);
std::map<float, int> analyize(std::vector<std::vector<int>> imgArr,cv::Mat* image, std::vector<float> angles, std::vector<coord> line, int r);
