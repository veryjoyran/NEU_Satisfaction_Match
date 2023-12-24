#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <FL/fl_ask.H> 
#include <sstream>
#include <string>
#include <fstream>
#define INT_MAX 2147483647
using namespace std;


// 全局变量，用于访问 FLTK 控件
Fl_Multiline_Input *mfInput;
Fl_Multiline_Input *fmInput;
Fl_Text_Display *resultBox;

// 将字符串转换为整数矩阵
vector<vector<int>> stringToMatrix(const string &str) {
    istringstream iss(str);
    vector<vector<int>> matrix;
    string line;
    while (getline(iss, line)) {
        istringstream lineStream(line);
        matrix.emplace_back(istream_iterator<int>{lineStream}, istream_iterator<int>());
    }
    return matrix;
}




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

void printMatrix(vector<vector<int>> &matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    for(int i=0; i<rows; i++) {
        for(int j=0; j<cols; j++) {
            cout<<matrix[i][j]<<" ";
        }
        cout<<endl;
    }
}

vector<vector<int>> createCombinedSatisfactionMatrix(vector<vector<int>>& mf, vector<vector<int>>& fm) {
    int maleCount = mf.size();  // 男生数量
    int femaleCount = fm.size(); // 女生数量

    // 扩展矩阵以适应女生数量多于男生的情况
    if (maleCount < femaleCount) {
        // 扩展 mf 矩阵，增加虚拟男生
        for (auto& row : mf) {
            row.resize(femaleCount, 0);
        }
        // 同时扩展 fm 矩阵，为每个女生添加对虚拟男生的满意度
        for (auto& row : fm) {
            while (row.size() < maleCount) {
                row.push_back(0);
            }
        }
    }
    //扩展矩阵以适应男生数量多于女生数量的情况
    else{
        // 扩展 fm 矩阵，增加虚拟女生
        for (auto& row : fm) {
            row.resize(maleCount, 0);
        }
        // 同时扩展 mf 矩阵，为每个男生添加对虚拟女生的满意度
        for (auto& row : mf) {
            while (row.size() < femaleCount) {
                row.push_back(0);
            }
        }
    }

    // 创建组合满意度矩阵
    int combinedSize = max(maleCount, femaleCount);
    vector<vector<int>> combinedSatisfactionMatrix(combinedSize, vector<int>(combinedSize, 0));

    // 计算组合满意度
    for (int i = 0; i < maleCount; i++) {
        for (int j = 0; j < femaleCount; j++) {
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

//删除指定下标位置的男人
void deleteMale(vector<vector<int>>& combinedSatisfactionMatrix, vector<vector<int>>& mf, vector<vector<int>>& fm, int index) {
    int rows = combinedSatisfactionMatrix.size();
    int cols = combinedSatisfactionMatrix[0].size();
    // 删除男人矩阵中该男人对女人的满意度
    mf.erase(mf.begin()+index);

    // 删除女人矩阵中女人对该男人的的满意度
    for(int i = 0; i < cols; ++i) {
        fm[i].erase(fm[i].begin()+index);
    }

    //更新满意度矩阵
    combinedSatisfactionMatrix = createCombinedSatisfactionMatrix(mf, fm);
}

//删除指定下标位置的女人
void deleteFemale(vector<vector<int>>& combinedSatisfactionMatrix, vector<vector<int>>& mf, vector<vector<int>>& fm, int index) {
    int rows = combinedSatisfactionMatrix.size();
    int cols = combinedSatisfactionMatrix[0].size();
    // 删除女人矩阵中该女人对男人的满意度
    fm.erase(fm.begin()+index);

    // 删除男人矩阵中男人对该女人的的满意度
    for(int i = 0; i < rows; ++i) {
        mf[i].erase(mf[i].begin()+index);
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
vector<int> KM(vector<vector<int>>& combinedSatisfactionMatrix) {
    int rows = combinedSatisfactionMatrix.size();
    int cols = combinedSatisfactionMatrix[0].size();
    vector<int> match(rows, -1);   //match[i]表示女运动员i所匹配的男运动员
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
    return match;
}

// 匹配按钮回调函数
void match_cb(Fl_Widget*, void*) {
    cout << "匹配逻辑开始执行" << endl;

    // 从输入框获取数据并转换为矩阵
    string mfStr = mfInput->value();
    vector<vector<int>> mfMatrix = stringToMatrix(mfStr);

    string fmStr = fmInput->value();
    vector<vector<int>> fmMatrix = stringToMatrix(fmStr);



    // 进行输入检查
    int mfRows = mfMatrix.size();
    int mfCols = (mfRows > 0) ? mfMatrix[0].size() : 0;
    int fmRows = fmMatrix.size();
    int fmCols = (fmRows > 0) ? fmMatrix[0].size() : 0;
        // 检查 mfMatrix 中每一行是否具有相同数量的元素
    bool mfMatrixIsValid = true;
    for (const auto& row : mfMatrix) {
        if (row.size() != mfCols) {
            mfMatrixIsValid = false;
            break;
        }
    }

    // 检查 fmMatrix 中每一行是否具有相同数量的元素
    bool fmMatrixIsValid = true;
    for (const auto& row : fmMatrix) {
        if (row.size() != fmCols) {
            fmMatrixIsValid = false;
            break;
        }
    }
    if (mfRows == 0 || mfCols == 0 || fmRows == 0 || fmCols == 0 || mfCols != fmRows|| mfRows != fmCols||!mfMatrixIsValid || !fmMatrixIsValid) {
        // 显示输入错误消息框
        fl_message("输入错误", "请检查输入的满意度矩阵是否符合要求。");
    } else {
        // 进行满意度匹配
        vector<vector<int>> combinedSatisfactionMatrix = createCombinedSatisfactionMatrix(mfMatrix, fmMatrix);
        vector<int> match = KM(combinedSatisfactionMatrix);

        // 计算满意度总和
        int totalSatisfaction = 0;
        for (int i = 0; i < match.size(); ++i) {
            int femaleIndex = i;
            int maleIndex = match[i];
            totalSatisfaction += combinedSatisfactionMatrix[maleIndex][femaleIndex];
        }

        // 显示匹配结果
        ostringstream result;
        for (int i = 0; i < match.size(); ++i) {
            result << "女运动员" << i << "与男运动员" << match[i] << "匹配\n";
        }
        result << "满意度总和：" << totalSatisfaction << endl;


        // 将结果添加到resultBox的缓冲区中
        Fl_Text_Buffer *buffer = resultBox->buffer();
        buffer->text(result.str().c_str());

        cout << "控制台输出:\n" << result.str() << endl;
    }

    cout << "匹配逻辑执行完毕" << endl;
}


int visualTesting(){
    Fl_Window *window = new Fl_Window(2000, 2000, "Satisfaction Match");
    Fl_Box* Theme=new Fl_Box(500, 80, 1000, 50, "东北大学数据结构课程设计B题——满意度匹配");
    Theme->labelsize(48); // 设置文本的字体大小
    Theme->color(FL_WHITE); // 设置Fl_Box的背景颜色为白色
    Theme->labelcolor(FL_BLACK); // 设置文本的颜色为黑色

    Fl_Box* MaleInputMartix=new Fl_Box(250, 365, 600, 36, "男运动员对女运动员的满意度矩阵");
    MaleInputMartix->labelsize(36); // 设置文本的字体大小
    // 设置男运动员的输入框的尺寸和位置
    mfInput = new Fl_Multiline_Input(250, 400, 600, 600);
    mfInput->textsize(30);

    Fl_Box* FemaleInputMartix=new Fl_Box(1000, 365, 600, 36, "女运动员对男运动员的满意度矩阵");
    FemaleInputMartix->labelsize(36); // 设置文本的字体大小
    // 设置女运动员的输入框的尺寸和位置
    fmInput = new Fl_Multiline_Input(1000, 400, 600, 600);
    fmInput->textsize(30);

    // 设置结果显示框的尺寸和位置
    resultBox = new Fl_Text_Display(250, 1200, 1350, 700);
    resultBox->textsize(36);
    resultBox->buffer(new Fl_Text_Buffer());
    resultBox->color(FL_WHITE);
    resultBox->labelcolor(FL_BLACK);


    // 调整按钮的位置和尺寸，确保在视觉上突出
    Fl_Button *matchButton = new Fl_Button(790, 1050, 300, 100, "开始匹配");
    matchButton->labelsize(48); // 设置文本的字体大小
    matchButton->callback(match_cb, nullptr);

   

    window->end();
    window->show();

    return Fl::run();
}


void consoleTesting(){
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
    printMatrix(mf);

    cout<<"请输入女生对男生的满意度矩阵"<<endl;
    createMatrix(fm, cols, rows);
    initMatrix(fm, cols, rows);
    cout<<"女生对男生的满意度矩阵为："<<endl;
    printMatrix(fm);


    vector<vector<int>> combinedSatisfactionMatrix = createCombinedSatisfactionMatrix(mf, fm);
    cout<<"男生和女生的满意度矩阵为："<<endl;
    printMatrix(combinedSatisfactionMatrix);

    while(1){
        cout<<"请选择测试方式："<<endl;
    cout<<"1.增加男生"<<endl;
    cout<<"2.增加女生"<<endl;
    cout<<"3.删除男生"<<endl;
    cout<<"4.删除女生"<<endl;
    cout<<"5.进行满意度匹配"<<endl;
    int choice;
    cin>>choice;
    if(choice==1){
        //增加男生
        addMale(combinedSatisfactionMatrix, mf, fm);
        cout<<"男生矩阵为："<<endl;
        printMatrix(mf);
        cout<<"女生矩阵为："<<endl;
        printMatrix(fm);
        cout<<"男生和女生的满意度矩阵为："<<endl;
        printMatrix(combinedSatisfactionMatrix);
    }
    else if(choice==2){
        //增加女生
        addFemale(combinedSatisfactionMatrix, mf, fm);
        cout<<"男生矩阵为："<<endl;
        printMatrix(mf);
        cout<<"女生矩阵为："<<endl;
        printMatrix(fm);
        cout<<"男生和女生的满意度矩阵为："<<endl;
        printMatrix(combinedSatisfactionMatrix);
    }
    else if(choice==3){
        cout<<"请输入要删除的男生的下标"<<endl;
        int index;
        cin>>index;
        deleteMale(combinedSatisfactionMatrix, mf, fm, index);
        cout<<"男生矩阵为："<<endl;
        printMatrix(mf);
        cout<<"女生矩阵为："<<endl;
        printMatrix(fm);
        cout<<"男生和女生的满意度矩阵为："<<endl;
        printMatrix(combinedSatisfactionMatrix);
    }
    else if(choice==4){
        cout<<"请输入要删除的女生的下标"<<endl;
        int index;
        cin>>index;
        deleteFemale(combinedSatisfactionMatrix, mf, fm, index);
        cout<<"男生矩阵为："<<endl;
        printMatrix(mf);
        cout<<"女生矩阵为："<<endl;
        printMatrix(fm);
        cout<<"男生和女生的满意度矩阵为："<<endl;
        printMatrix(combinedSatisfactionMatrix);
    }
    else if(choice==5){
        //进行满意度匹配
        vector<int> match=KM(combinedSatisfactionMatrix);
        // 计算满意度总和
        int totalSatisfaction = 0;
        for (int i = 0; i < match.size(); ++i) {
            int femaleIndex = i;
            int maleIndex = match[i];
            totalSatisfaction += combinedSatisfactionMatrix[maleIndex][femaleIndex];
        }
        cout<<"匹配结果为："<<endl;
        for(int i = 0; i < cols; ++i) {
            cout << "女运动员" << i << "与男运动员" << match[i] << "匹配" << endl;
        }
        cout<<"满意度总和为："<<totalSatisfaction<<endl;
    }
    else{
        cout<<"输入错误"<<endl;
    }
    }
    
}

int main() {
    cout<<"请选择测试方式："<<endl;
    cout<<"1.控制台测试"<<endl;
    cout<<"2.可视化测试"<<endl;
    int choice;
    cin>>choice;
    if(choice==1){
        consoleTesting();
    }
    else if(choice==2){
        visualTesting();
    }
    else{
        cout<<"输入错误"<<endl;
    }


    return 0;
}



/*

1 2 3
1 2 3
1 2 3
1 2 3


1 2 3 4
1 2 3 4
1 2 3 4
*/