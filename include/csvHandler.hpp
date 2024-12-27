#include <vector>
#include <map>
#include <string>

void csvInit(std::string file, std::vector<float> angles);
void csvAppendHist(std::string file, std::string conditionName,std::map<float,int> hist);