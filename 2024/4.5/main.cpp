#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main(void){
    std::vector<std::string> matrix;
    std::string tmp;
    std::ifstream file("input.txt");

    while(file >> tmp){
        matrix.push_back(tmp);
    }

    size_t count = 0;
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            if(matrix[i][j] != 'A') continue;
            if(i < 1 || j < 1 || i > matrix.size()-2 || j > matrix[0].size()-2) continue;

            if(matrix[i-1][j-1] == 'M' && matrix[i+1][j-1] == 'M' && matrix[i-1][j+1] == 'S' && matrix[i+1][j+1] == 'S') count++;
            if(matrix[i-1][j-1] == 'S' && matrix[i+1][j-1] == 'S' && matrix[i-1][j+1] == 'M' && matrix[i+1][j+1] == 'M') count++;
            if(matrix[i-1][j-1] == 'M' && matrix[i+1][j-1] == 'S' && matrix[i-1][j+1] == 'M' && matrix[i+1][j+1] == 'S') count++;
            if(matrix[i-1][j-1] == 'S' && matrix[i+1][j-1] == 'M' && matrix[i-1][j+1] == 'S' && matrix[i+1][j+1] == 'M') count++;
        }
    }

    std::cout << count << "\n";
}