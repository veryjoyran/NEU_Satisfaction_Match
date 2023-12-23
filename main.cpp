#include <iostream>
#include <vector>
#include <algorithm>
#define INT_MAX 2147483647
using namespace std;

void createMatrix(vector<vector<int>> &matrix, int rows, int cols) {
    matrix.resize(rows);
    for (int i = 0; i < rows; i++) {
        matrix[i].resize(cols);
    }
}

void initMatrix(vector<vector<int>> &matrix, int rows, int cols) {
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            cin>>matrix[i][j];
        }
    }
}

void printMatrix(vector<vector<int>> &matrix, int rows, int cols) {
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            cout<<matrix[i][j]<<" ";
        }
        cout<<endl;
    }
}

vector<vector<int>> createCombinedSatisfactionMatrix(vector<vector<int>>& mf, vector<vector<int>>& fm) {
    int rows = mf.size();
    int cols = fm.size();
    vector<vector<int>> combinedSatisfactionMatrix;
    createMatrix(combinedSatisfactionMatrix, rows, cols);
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            combinedSatisfactionMatrix[i][j] = mf[i][j] + fm[j][i];
        }
    }
    return combinedSatisfactionMatrix;
}


//增加男人
void addMale(vector<vector<int>>& combinedSatisfactionMatrix, vector<vector<int>>& mf, vector<vector<int>>& fm) {
    int rows = combinedSatisfactionMatrix.size();
    int cols = combinedSatisfactionMatrix[0].size();
    // 增加男人矩阵中该男人对女人的满意度
    vector<int> newRow(cols, 0);
    cout << "请输入增加的男运动员对女运动员的满意度矩阵:" <<"(当前女运动员人数为）" <<cols<<endl;
    for(int i = 0; i < cols; ++i) {
        cin>>newRow[i];
    }
    mf.push_back(newRow);

    // 增加女人矩阵中女人对该男人的的满意度
    cout << "请输入女运动员对新增的男运动员的满意度矩阵:" <<"(当前女运动员人数为)"<<cols<< endl;
    for(int i = 0; i < cols; ++i) {
        int satisfaction;
        cin>>satisfaction;
        fm[i].push_back(satisfaction);
    }

    //更新满意度矩阵
    combinedSatisfactionMatrix = createCombinedSatisfactionMatrix(mf, fm);
}

//增加女人
void addFemale(vector<vector<int>>& combinedSatisfactionMatrix, vector<vector<int>>& mf, vector<vector<int>>& fm) {
    int rows = combinedSatisfactionMatrix.size();
    int cols = combinedSatisfactionMatrix[0].size();
    // 增加女人矩阵中该女人对男人的满意度
    vector<int> newRow(rows, 0);
    cout << "请输入增加的女运动员对男运动员的满意度矩阵:" <<"(当前男运动员人数为）" <<rows<<endl;
    for(int i = 0; i < rows; ++i) {
        cin>>newRow[i];
    }
    fm.push_back(newRow);

    // 增加男人矩阵中男人对该女人的的满意度
    cout << "请输入男运动员对新增的女运动员的满意度矩阵:" <<"(当前男运动员人数为)"<<rows<< endl;
    for(int i = 0; i < rows; ++i) {
        int satisfaction;
        cin>>satisfaction;
        mf[i].push_back(satisfaction);
    }

    //更新满意度矩阵
    combinedSatisfactionMatrix = createCombinedSatisfactionMatrix(mf, fm);
}



//寻找增广路
bool findPath(int u, vector<int>& match, vector<int>& lx, vector<int>& ly, vector<bool>& sx, vector<bool>& sy, vector<int>& slack, vector<int>& pre,vector<vector<int>>& combinedSatisfactionMatrix) {
    sx[u] = true;
    for(int v = 0; v < ly.size(); ++v) {
        if(sy[v])
            continue;
        int gap = lx[u] + ly[v] - combinedSatisfactionMatrix[u][v];
        if(gap == 0) {
            sy[v] = true;
            if(match[v] == -1 || findPath(match[v], match, lx, ly, sx, sy, slack, pre, combinedSatisfactionMatrix)) {
                match[v] = u;
                return true;
            }
        } else {
            slack[v] = min(slack[v], gap);
        }
    }
    return false;
}

//基于KM算法的满意度匹配
void KM(vector<vector<int>>& combinedSatisfactionMatrix) {
    int rows = combinedSatisfactionMatrix.size();
    int cols = combinedSatisfactionMatrix[0].size();
    vector<int> match(rows, -1);   //match[i]表示女运动员i匹配的男运动员
    vector<int> lx(rows, 0);     //lx[i]表示男运动员i的标号
    vector<int> ly(cols, 0);    //ly[i]表示女运动员i的标号
    vector<bool> sx(rows, false);    //sx[i]表示男运动员i是否在增广路中
    vector<bool> sy(cols, false);   //sy[i]表示女运动员i是否在增广路中
    vector<int> slack(cols, 0);    //slack[j]表示女运动员j的松弛量
    vector<int> pre(cols, -1);   //pre[j]表示女运动员j所匹配的男运动员
    //初始化lx
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j)
            lx[i] = max(lx[i], combinedSatisfactionMatrix[i][j]);
    }

    //主循环（匈牙利算法）
    for(int i = 0; i < rows; ++i) {
        while(true) {
            sx.assign(rows, false);
            sy.assign(cols, false);
            fill(slack.begin(), slack.end(), INT_MAX);
            if(findPath(i, match, lx, ly, sx, sy, slack, pre, combinedSatisfactionMatrix))
                break;
            int d = INT_MAX;
            for(int j = 0; j < cols; ++j) {
                if(!sy[j])
                    d = min(d, slack[j]);
            }
            for(int j = 0; j < rows; ++j) {
                if(sx[j])
                    lx[j] -= d;
            }
            for(int j = 0; j < cols; ++j) {
                if(sy[j])
                    ly[j] += d;
                else
                    slack[j] -= d;
            }
        }
    }
}


int main() {
    int rows, cols;
    cout<<"请输入男生人数"<<endl;
    cin >> rows;
    cout<<"请输入女生人数"<<endl;
    cin >> cols;

    vector<vector<int>> mf;
    vector<vector<int>> fm;

    cout<<"请输入男生对女生的满意度矩阵"<<endl;
    createMatrix(mf, rows, cols);
    initMatrix(mf, rows, cols);
    cout<<"男生对女生的满意度矩阵为："<<endl;
    printMatrix(mf, rows, cols);

    cout<<"请输入女生对男生的满意度矩阵"<<endl;
    createMatrix(fm, cols, rows);
    initMatrix(fm, cols, rows);
    cout<<"女生对男生的满意度矩阵为："<<endl;
    printMatrix(fm, cols, rows);


    vector<vector<int>> combinedSatisfactionMatrix = createCombinedSatisfactionMatrix(mf, fm);
    cout<<"男生和女生的满意度矩阵为："<<endl;
    printMatrix(combinedSatisfactionMatrix, rows, cols);


    

    


    return 0;
}
