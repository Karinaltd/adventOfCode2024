#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>
#include <numeric> 
#include <set>
#include <tuple>

void processInput(const std::string& filename, std::vector<std::tuple<std::map<int, int>, std::vector<int>>>& pages) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Wrong file name" << std::endl;
    }
    std::vector<std::vector<std::string>> halves; 
    std::vector<std::string> currentHalf;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            if (!currentHalf.empty()) {
                halves.push_back(currentHalf); 
                currentHalf.clear();         
            }
        } else {
            currentHalf.push_back(line); 
        }
    }
    if (!currentHalf.empty()) {
        halves.push_back(currentHalf);
    }
    file.close();

    std::vector<std::string> pageOrderingsInput = halves[0];
    std::vector<std::string> updatesInput = halves[1];

    for (std::string line : updatesInput){
        std::set<int> validPages;
        std::vector<int> updateNums;
        std::map<int, int> pageOrdering;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ',')) {
            updateNums.push_back(std::stoi(token));
            validPages.insert(std::stoi(token));
        }
        for (std::string line : pageOrderingsInput){
            size_t delimiterPos = line.find('|');   
            if (delimiterPos != std::string::npos) {
                std::string firstPart = line.substr(0, delimiterPos);
                std::string secondPart = line.substr(delimiterPos + 1);
            
                int num1 = std::stoi(firstPart);
                int num2 = std::stoi(secondPart);

                if (validPages.count(num1) && validPages.count(num2)){
                    if (pageOrdering.find(num2) == pageOrdering.end()) {
                        pageOrdering[num2] = 1;
                    }else{
                        pageOrdering[num2]++;
                    }
                }
            }
        }
        std::tuple<std::map<int, int>, std::vector<int>> newPageInfo(pageOrdering, updateNums);
        pages.push_back(newPageInfo);
    }
    file.close();
}
int part1(std::vector<std::tuple<std::map<int, int>, std::vector<int>>> input){
    int middlePages = 0;
    for (std::tuple<std::map<int, int>, std::vector<int>> pages : input){
        int index = -1;
        bool valid = true;
        for(int pageNum : std::get<1>(pages)){
            if (std::get<0>(pages)[pageNum] > index){
                index = std::get<0>(pages)[pageNum];
            }else{
                valid = false;
                break;
            }
        }
        if (valid){
            middlePages += std::get<1>(pages)[(std::ceil(std::get<0>(pages).size())/2)];
        }
    }
    return middlePages;
}

int part2(std::vector<std::tuple<std::map<int, int>, std::vector<int>>> input){
    int middlePages = 0;
    for (std::tuple<std::map<int, int>, std::vector<int>> pages : input){
        std::vector<int> newOrderedList(std::get<1>(pages).size());
        for(int pageNum : std::get<1>(pages)){
            newOrderedList[std::get<0>(pages)[pageNum]] = pageNum;
        }
        middlePages += newOrderedList[(newOrderedList.size())/2];
    }
    return middlePages;
}

int main() {
    std::vector<std::tuple<std::map<int, int>, std::vector<int>>> pagesInput;
    std::vector<std::tuple<std::map<int, int>, std::vector<int>>> pagesExample;

    processInput("Day5Input.txt", pagesInput);
    processInput("Day5Ex.txt", pagesExample);

    int part1ResultEx = part1(pagesExample); 
    int part1ResultInput = part1(pagesInput);
    std::cout << "Day2 P1 Example " << part1ResultEx << std::endl;
    std::cout << "Day2 P1 Input " << part1ResultInput << std::endl;
    
    std::cout << "Day2 P2 Example " << part2(pagesExample) - part1ResultEx << std::endl;
    std::cout << "Day2 P2 Input " << part2(pagesInput) - part1ResultInput << std::endl;

    return 0;
}
