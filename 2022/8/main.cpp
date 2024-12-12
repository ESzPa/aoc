#include <iostream>
#include <string>
#include <vector>
#include <fstream>

bool visible(std::vector<std::string>& matrix, int row, int col){
    int height = matrix[row][col] - '0';
    bool left = true,
         right = true,
         up = true,
         down = true;

    for(int p = 0; p < col; p++){
        if(matrix[row][p] - '0' >= height){
            left = false;
            break;
        }
    }
    for(int p = col+1; p < matrix[0].size(); p++){
        if(matrix[row][p] - '0' >= height){
            right = false;
            break;
        }
    }
    for(int p = 0; p < row; p++){
        if(matrix[p][col] - '0' >= height){
            up = false;
            break;
        }
    }
    for(int p = row+1; p < matrix.size(); p++){
        if(matrix[p][col] - '0' >= height){
            down = false;
            break;
        }
    }
    return left || right || up || down;
}

int main(){
    std::vector<std::string> matrix;
    std::ifstream file("input.txt");

    std::string str;
    while(getline(file, str)){
        matrix.push_back(str);
    }

    size_t count = 0;
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            if(i == 0 || j == 0 || i == matrix.size()-1 || j == matrix[0].size()-1) count++;
            else if(visible(matrix, i, j)) count++;
        }
    }
    std::cout << count << '\n';
    return 0;
}