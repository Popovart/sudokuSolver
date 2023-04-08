//
//  main.cpp
//  sudokuSolver
//
//  Created by Dmitry Popov on 07.04.2023.
//

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <set>
using namespace std;

vector<vector<int>> sudokuMap = {
    {0, 0, 0, 6, 2, 0, 1, 5, 0},
    {0, 3, 0, 0, 0, 0, 0, 0, 7},
    {6, 0, 0, 0, 7, 1, 0, 0, 0},
    {5, 4, 0, 0, 0, 0, 0, 9, 1},
    {1, 0, 0, 2, 0, 0, 0, 0, 0},
    {0, 6, 0, 0, 0, 4, 7, 3, 0},
    {8, 0, 7, 3, 4, 2, 0, 1, 6},
    {4, 0, 6, 0, 1, 9, 3, 8, 5},
    {3, 0, 9, 5, 0, 0, 0, 0, 0},
};

const vector<pair<int,int>> centersOfsquares = {{1,1},{1,4},{1,7},{4,1},{4,4},{4,7},{7,1},{7,4},{7,7}};

void printMatrix(vector<vector<int>> matrix){
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            if((i == 3 && j == 0) || (i == 6 && j == 0)){
                cout << endl;
            }
            if(j == 2 || j == 5){
                cout << matrix[i][j] << "  ";
                continue;
            }
            cout << matrix[i][j] << " ";
            
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int distanceToPoint(pair<int,int> pos1, pair<int,int> pos2){
    return abs(pos2.first-pos1.first)+abs(pos2.second-pos1.second);
}

void updateSukokuMap(){
    
}

set<int> findPossibleNumbers(vector<vector<int>> sudokuMap, pair<int,int> pos){
    set<int> possibleNumbers;
    set<int> colNumbers = {1,2,3,4,5,6,7,8,9};
    set<int> rowNumbers = {1,2,3,4,5,6,7,8,9};
    
    //searchig in column
    for(int i = 0; i < sudokuMap.size(); i++){
        colNumbers.erase(sudokuMap[i][pos.second]);
    }
    //searchig in row
    for(int i = 0; i < sudokuMap[0].size(); i++){
        rowNumbers.erase(sudokuMap[pos.first][i]);
    }
    set_intersection(colNumbers.begin(), colNumbers.end(), rowNumbers.begin(), rowNumbers.end(), inserter(possibleNumbers, possibleNumbers.begin()));
    
    
    //searching in squares
    pair<int,int> usefullCenterOfsquare;
    for(int i = 0; i < centersOfsquares.size(); i++){
        int distanse = distanceToPoint(centersOfsquares[i], pos);
        if(distanse == 2){
            usefullCenterOfsquare = centersOfsquares[i];
        }
        if(distanse == 1){
            usefullCenterOfsquare = centersOfsquares[i];
            break;
        }
        if(distanse == 0){
            usefullCenterOfsquare = centersOfsquares[i];
            break;
        }
        
        
    };
    //cout << usefullCenterOfsquare.first << "," << usefullCenterOfsquare.second << endl;
    set<int> squareNumbers = {1,2,3,4,5,6,7,8,9};
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            //cout << sudokuMap[usefullCenterOfsquare.first-1+i][usefullCenterOfsquare.second-1+j] << " ";
            squareNumbers.erase(sudokuMap[usefullCenterOfsquare.first-1+i][usefullCenterOfsquare.second-1+j]);
        }
        //cout << endl;
    }
    set<int> result;
    set_intersection(squareNumbers.begin(), squareNumbers.end(), possibleNumbers.begin(), possibleNumbers.end(), inserter(result, result.begin()));



//    for (int val : result) {
//        cout << val << " ";
//    }
//    cout << endl;
    
    
    return result;
};


void solveSudoku(vector<vector<int>> &sudokuMap){
    
    list<pair<pair<int,int>, set<int> >> possibleNumbers;
    
    
    for (int i = 0; i < sudokuMap.size(); i++) {
       for (int j = 0; j < sudokuMap[0].size(); j++) {
           if(sudokuMap[i][j] == 0){
               possibleNumbers.push_front( { {i,j}, findPossibleNumbers(sudokuMap, {i,j}) } );
           }
       }
    }
    
    
    
    
    
//    for (pair<pair<int,int>, set<int> > val : possibleNumbers) {
//        cout << "[" <<val.first.first << "," << val.first.second << "] = ";
//        for(int i : val.second){
//            cout << i << " ";
//        }
//        cout << endl;
//    }
    
}
    


int main(int argc, const char * argv[]) {
    printMatrix(sudokuMap);
    solveSudoku(sudokuMap);
    return 0;
}
