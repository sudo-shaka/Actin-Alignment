#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include "csvHandler.hpp"

void csvInit(std::string filename, std::vector<float> angles){
    std::ofstream file(filename);
    if(!file.is_open() || file.fail() || file.bad()){
        std::cerr << "Unable to create file to export data." << std::endl;
        return;
    }
    file << "Angle:";
    for(auto & a : angles){
        file << "," << a;
    }
    file << "\n";
}

void csvAppendHist(std::string filename, std::string conditionName,std::map<float,int> hist){
    std::ofstream file(filename, std::ios::app);
    if(!file || !file.is_open() || file.fail() || file.bad()){
        std::cerr << "Unable to append file to export data." << std::endl;
        return;
    }
    file << conditionName << ":";
    for(auto& p : hist){
        file << "," << p.second;
    }
    file << "\n";
    std::cout << " ---> data exported." << std::endl;
}