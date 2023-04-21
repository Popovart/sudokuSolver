//
//  main.cpp
//  sudokuSolver
//
//  Created by Dmitry Popov on 07.04.2023.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include<chrono>
using namespace std;

vector<vector<int>> sudokuMap = {
    {7, 2, 0, 0, 0, 0, 0, 0, 6},
    {9, 5, 0, 0, 1, 4, 8, 3, 7},
    {8, 4, 0, 7, 9, 6, 1, 2, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 3},
    {0, 0, 4, 5, 7, 0, 0, 8, 0},
    {6, 0, 8, 0, 2, 1, 0, 0, 0},
    {0, 0, 0, 1, 6, 0, 3, 0, 4},
    {4, 0, 0, 0, 0, 0, 5, 1, 0},
    {0, 0, 7, 0, 0, 5, 0, 0, 0},
};

const bool debug = false;

//{0, 0, 0, 0, 0, 0, 0, 0, 0},
//{0, 0, 0, 0, 0, 0, 0, 0, 0},
//{0, 0, 0, 0, 0, 0, 0, 0, 0},
//{0, 0, 0, 0, 0, 0, 0, 0, 0},
//{0, 0, 0, 0, 0, 0, 0, 0, 0},
//{0, 0, 0, 0, 0, 0, 0, 0, 0},
//{0, 0, 0, 0, 0, 0, 0, 0, 0},
//{0, 0, 0, 0, 0, 0, 0, 0, 0},
//{0, 0, 0, 0, 0, 0, 0, 0, 0},

//{8, 0, 9, 4, 0, 7, 0, 0, 1},
//{0, 0, 0, 0, 3, 0, 0, 7, 0},
//{4, 0, 0, 0, 8, 0, 0, 0, 0},
//{5, 0, 0, 1, 0, 4, 0, 2, 0},
//{0, 0, 3, 0, 0, 0, 5, 0, 0},
//{0, 0, 0, 8, 0, 0, 0, 0, 0},
//{3, 0, 0, 5, 0, 2, 0, 1, 0},
//{0, 0, 0, 0, 7, 0, 0, 0, 0},
//{0, 6, 0, 0, 0, 0, 0, 0, 9},

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

set<int> searchWithoutUnique(vector<vector<int>> sudokuMap, pair<int,int> pos, pair<int,int> usefullCenterOfsquare){
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
    set<int> squareNumbers = {1,2,3,4,5,6,7,8,9};
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            squareNumbers.erase(sudokuMap[usefullCenterOfsquare.first-1+i][usefullCenterOfsquare.second-1+j]);
        }
    }
    set<int> result;
    set_intersection(squareNumbers.begin(), squareNumbers.end(), possibleNumbers.begin(), possibleNumbers.end(), inserter(result, result.begin()));
    
    
    return result;
};

pair<int,int> findUsefullCenterOfsquare(pair<int,int> pos){
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
    return usefullCenterOfsquare;
}


set<int> findPossibleNumbers(vector<vector<int>> sudokuMap, pair<int,int> pos){
    //searching in squares
    pair<int,int> usefullCenterOfsquare = findUsefullCenterOfsquare(pos);
    
    set<int> result = searchWithoutUnique(sudokuMap, pos, usefullCenterOfsquare);
    
    for (int number : result) {
        // Initially, we assume that the number is unique for the current square.
        int uniqueInSquare = true;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int x = usefullCenterOfsquare.first - 1 + i;
                int y = usefullCenterOfsquare.second - 1 + j;
                //If the position of the iterated element isn't equal to the position of the element for which the check is being performed, and there is a 0 at this position.
                if (!(x == pos.first && y == pos.second) && sudokuMap[x][y] == 0) {
                    set<int> otherPossibleNumbers = searchWithoutUnique(sudokuMap, {x, y}, usefullCenterOfsquare);
                    //if this subset of otherPossibleNumbers contain current value of variable number, it means that number is not unique for current square
                    if (otherPossibleNumbers.count(number) > 0) {
                        uniqueInSquare = false;
                        break;
                    }
                }
            }
            // if uniqueInSquare is still true, it means that variable number is unique for current square
            if (!uniqueInSquare) {
                break;
            }
        }
        if (uniqueInSquare) {
            result.clear();
            result.insert(number);
            break;
        }
    }
    
    return result;
};



//adding number to the sudoku map
void updateSukokuMap(vector<vector<int>> &sudokuMap, vector<pair<pair<int,int>, set<int>>> possibleNumbers){
    for (pair<pair<int,int>, set<int> > val : possibleNumbers) {
        if(val.second.size() == 1){
            sudokuMap[val.first.first][val.first.second] = *val.second.begin();
        }
    }
};

//checking if sukoku is valid
bool isSudokuValid(const vector<vector<int>> &sudokuMap) {
    for (int i = 0; i < 9; ++i) {
        set<int> row, col, square;
        for (int j = 0; j < 9; ++j) {
            if (sudokuMap[i][j] != 0) {
                if (row.count(sudokuMap[i][j])) {
                    return false;
                }
                row.insert(sudokuMap[i][j]);
            }
            if (sudokuMap[j][i] != 0) {
                if (col.count(sudokuMap[j][i])) {
                    return false;
                }
                col.insert(sudokuMap[j][i]);
            }

            int squareRow = (i / 3) * 3 + j / 3;
            int squareCol = (i % 3) * 3 + j % 3;
            if (sudokuMap[squareRow][squareCol] != 0) {
                if (square.count(sudokuMap[squareRow][squareCol])) {
                    return false;
                }
                square.insert(sudokuMap[squareRow][squareCol]);
            }
        }
    }
    return true;
}

bool nothingChanged(vector<pair<pair<int,int>, set<int> >> list1, vector<pair<pair<int,int>, set<int> >> list2){
    if(list1.size() != list2.size()){
        return false;
    }
    // здесь я не проверяю позиции, а только их возможные числа, потому что на этот момент они будут всегда одинаковыми
    auto it1 = list1.begin();
    auto it2 = list2.begin();

    while (it1 != list1.end() && it2 != list2.end()) {
        if (it1->second != it2->second) {
            return false;
        }
        ++it1;
        ++it2;
    }
    return true;
}

bool zeros(vector<vector<int>> sudokuMap){
    for (int i = 0; i < sudokuMap.size(); i ++) {
        for (int j = 0; j < sudokuMap[0].size(); j ++) {
            if(sudokuMap[i][j] == 0){
                return true;
            }
        }
    }
    return false;
}



bool solveSudoku(vector<vector<int>> &sudokuMap, vector<pair<pair<int,int>, set<int> >> possibleNumbers){
    vector<pair<pair<int,int>, set<int> >> tempPossibleNumbers;
    
    
    for (int i = 0; i < sudokuMap.size(); i++) {
       for (int j = 0; j < sudokuMap[0].size(); j++) {
           if(sudokuMap[i][j] == 0){
               set<int> posNumList = findPossibleNumbers(sudokuMap, {i,j});
               
               //all tempPossibleNumbers' sets must be sorted by size
               bool itsBig = true;
               for (auto it = tempPossibleNumbers.begin(); it != tempPossibleNumbers.end(); ++it) {
                   if (posNumList.size() <=  it->second.size()) {
                       tempPossibleNumbers.insert(it, {{i, j}, posNumList});
                       itsBig = false;
                       break;
                   }
                   
               }
               if(itsBig){
                   tempPossibleNumbers.push_back( { {i,j}, posNumList } );
               }
           }
       }
    }
    
    if(debug){
        cout << endl << "-------Possible numbers--------" << endl;
        for (const auto &item : tempPossibleNumbers) {
            const auto &coordinates = item.first;
            const auto &numbers = item.second;

            std::cout << "[" << coordinates.first << "," << coordinates.second << "]: ";

            for (auto it = numbers.begin(); it != numbers.end(); ++it) {
                if (it != numbers.begin()) {
                    std::cout << ",";
                }
                std::cout << *it;
            }
            std::cout << std::endl;
        }
    }
    
    // судоку уже решена?
    if(tempPossibleNumbers.empty()){
        return true;
    }
    
//    if (nothingChanged(tempPossibleNumbers, possibleNumbers)) {
//        bool foundSolution = false;
//        for (pair<pair<int,int>, set<int> > val : tempPossibleNumbers) {
//
//            for(auto it = val.second.begin(); it != val.second.end(); it++){
//                auto number = *it;
//                vector<vector<int>> previousState = sudokuMap;
//                sudokuMap[val.first.first][val.first.second] = number;
//                bool solved = solveSudoku(sudokuMap, tempPossibleNumbers);
//                if (solved) {
//                    foundSolution = true;
//                    break;
//                } else {
//                    sudokuMap = previousState;
//                }
//            }
//            if (foundSolution) {
//                break;
//            }
//        }
//        return foundSolution;
//    }
    
    
    updateSukokuMap(sudokuMap, tempPossibleNumbers);
    if(!isSudokuValid(sudokuMap)){
        return false;
    }

    
    return solveSudoku(sudokuMap, tempPossibleNumbers);
    
}


int main(int argc, const char * argv[]) {
    vector<pair<pair<int,int>, set<int> >> possibleNumbers;
    
    printMatrix(sudokuMap);
    auto start = std::chrono::high_resolution_clock::now();
    bool answer = solveSudoku(sudokuMap, possibleNumbers);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    
    if(answer){
        cout << "Sudoku has been solved! \n\n";
    } else {
        cout << "Sudoku hasn't been solved! \n\n";
    }
    
    cout << "TIME ELAPSED (nanoseconds) "<< duration.count() << endl;

    printMatrix(sudokuMap);
    return 0;
    //11278792
    //11081250
}


