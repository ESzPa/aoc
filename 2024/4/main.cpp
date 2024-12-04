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
            if(matrix[i][j] != 'X') continue;
            // Check diagonally
            if(i >= 3 && j >= 3 && matrix[i-1][j-1] == 'M' && matrix[i-2][j-2] == 'A' && matrix[i-3][j-3] == 'S') count++;
            if(i >= 3 && j+3 < matrix[0].size() && matrix[i-1][j+1] == 'M' && matrix[i-2][j+2] == 'A' && matrix[i-3][j+3] == 'S') count++;
            if(i+3 < matrix.size() && j+3 < matrix[0].size() && matrix[i+1][j+1] == 'M' && matrix[i+2][j+2] == 'A' && matrix[i+3][j+3] == 'S') count++;
            if(i+3 < matrix.size() && j >= 3 && matrix[i+1][j-1] == 'M' && matrix[i+2][j-2] == 'A' && matrix[i+3][j-3] == 'S') count++;
            // Check horizontally
            if(j >= 3 && matrix[i][j-1] == 'M' && matrix[i][j-2] == 'A' && matrix[i][j-3] == 'S') count++;
            if(j+3 < matrix[0].size() && matrix[i][j+1] == 'M' && matrix[i][j+2] == 'A' && matrix[i][j+3] == 'S') count++;
            // Check vertically
            if(i >= 3 && matrix[i-1][j] == 'M' && matrix[i-2][j] == 'A' && matrix[i-3][j] == 'S') count++;
            if(i+3 < matrix.size() && matrix[i+1][j] == 'M' && matrix[i+2][j] == 'A' && matrix[i+3][j] == 'S') count++;
        }
    }

    std::cout << count << "\n";
}