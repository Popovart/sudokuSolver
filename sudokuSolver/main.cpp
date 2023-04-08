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
    {0, 0, 4, 0, 0, 8, 0, 0, 6},
    {0, 0, 5, 2, 0, 0, 1, 8, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 7},
    {0, 0, 2, 0, 9, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 5, 0, 0, 0, 7, 3, 4, 0},
    {4, 0, 0, 7, 0, 0, 0, 0, 0},
    {0, 3, 0, 0, 0, 4, 8, 5, 0},
    {0, 0, 0, 0, 0, 0, 0, 6, 0},
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

set<int> findPossibleNumbers(vector<vector<int>> sudokuMap, pair<int,int> pos){
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
    
    set<int> result = searchWithoutUnique(sudokuMap, pos, usefullCenterOfsquare);
    
    for (int number : result) {
        // является ли число уникальным для этого квадрата
        int uniqueInSquare = true;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int x = usefullCenterOfsquare.first - 1 + i;
                int y = usefullCenterOfsquare.second - 1 + j;
                //если позиция итерируемого элемента не равна позиции элемента для которого проверка проводится и в этой позиции 0
                if (!(x == pos.first && y == pos.second) && sudokuMap[x][y] == 0) {
                    set<int> otherPossibleNumbers = searchWithoutUnique(sudokuMap, {x, y}, usefullCenterOfsquare);
                    // Если это множество других возможных чисел содержит текущее число number, это означает, что не уникально для данного квадрата
                    if (otherPossibleNumbers.count(number) > 0) {
                        uniqueInSquare = false;
                        break;
                    }
                }
            }
            // если uniqueInSquare все еще true, тогда оно уникально для текущего квадрата
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



void updateSukokuMap(vector<vector<int>> &sudokuMap, list<pair<pair<int,int>, set<int>>> possibleNumbers){
    for (pair<pair<int,int>, set<int> > val : possibleNumbers) {
        if(val.second.size() == 1){
            sudokuMap[val.first.first][val.first.second] = *val.second.begin();
        }
    }
}

bool nothingChanged(list<pair<pair<int,int>, set<int> >> list1, list<pair<pair<int,int>, set<int> >> list2){
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


bool solveSudoku(vector<vector<int>> &sudokuMap, list<pair<pair<int,int>, set<int> >> possibleNumbers){
    list<pair<pair<int,int>, set<int> >> tempPossibleNumbers;
    
    
    for (int i = 0; i < sudokuMap.size(); i++) {
       for (int j = 0; j < sudokuMap[0].size(); j++) {
           if(sudokuMap[i][j] == 0){
               tempPossibleNumbers.push_front( { {i,j}, findPossibleNumbers(sudokuMap, {i,j}) } );
           }
       }
    }
    // судоку уже решена?
    if(tempPossibleNumbers.empty()){
        return true;
    }
    

    if (nothingChanged(tempPossibleNumbers, possibleNumbers)) {
        //cout << "угадывание" << endl;
        for (pair<pair<int,int>, set<int> > val : tempPossibleNumbers) {
            for(auto it = val.second.begin(); it != val.second.end(); it++){
                auto number = *it;
                // Сохрание состояние судоку перед изменениями
                vector<vector<int>> previousState = sudokuMap;
                sudokuMap[val.first.first][val.first.second] = number;
                bool solved = solveSudoku(sudokuMap, tempPossibleNumbers);
                if (solved) {
                    return true;
                } else {
                    sudokuMap = previousState;
                }
            }
        }
        return false;
    }
    
//    for (pair<pair<int,int>, set<int> > val : possibleNumbers) {
//        cout << "[" <<val.first.first << "," << val.first.second << "] = ";
//        for(int i : val.second){
//            cout << i << " ";
//        }
//        cout << endl;
//    }
    
    
    
    updateSukokuMap(sudokuMap, tempPossibleNumbers);
    cout << "----------" << endl;
    printMatrix(sudokuMap);
    cout << "__________________________" << endl;
    return solveSudoku(sudokuMap, tempPossibleNumbers);
    
}


int main(int argc, const char * argv[]) {
    list<pair<pair<int,int>, set<int> >> possibleNumbers;
    printMatrix(sudokuMap);
    solveSudoku(sudokuMap, possibleNumbers);
    //cout << "----------" << endl;
    //printMatrix(sudokuMap);
    return 0;
}
