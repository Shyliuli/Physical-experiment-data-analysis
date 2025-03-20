#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <iomanip>
#include <sstream>

using namespace std;

int main() {
    // Word兼容格式设置
    cout << fixed << setprecision(4);
    cout << boolalpha;

    // 初始化
    cout << "======== 不确定度计算程序 (Word格式优化版) ========\n"
         << "输出格式特性：\n"
         << "1. 所有数学符号使用线性格式\n"
         << "2. 使用下划线表示下标 (如L_A)\n"
         << "3. 平方表示为^2，根号用sqrt()\n"
         << "4. 输入'quit'退出程序\n"
         << "=================================================\n";

    // 获取物理量符号


    while(true) {
    cout << "\n输入待测物理量符号 (如L、m): ";
    string symbol;
    cin >> symbol;
    cin.ignore();  // 清除输入缓冲
        cout << "\n输入测量数据 [格式: n 数据1 数据2...]\n"
             << "或输入 quit 退出: ";

        string raw_input;
        getline(cin, raw_input);

        // 退出检测
        if(raw_input.find("quit") != string::npos) break;

        // 数据解析
        istringstream iss(raw_input);
        int n;
        if(!(iss >> n) || n <= 1) {
            cout << "! 错误：数据个数必须≥2\n";
            continue;
        }

        vector<double> data;
        bool valid = true;
        for(int i=0; i<n; ++i) {
            double val;
            if(!(iss >> val)) {
                valid = false;
                break;
            }
            data.push_back(val);
        }

        if(!valid || data.size()!=n) {
            cout << "! 错误：数据不完整或格式错误\n";
            continue;
        }

        /* ---------- 计算流程 ---------- */
        // 平均值计算
        double sum = accumulate(data.begin(), data.end(), 0.0);
        double mean = sum / n;
        cout << "\n【平均值】\n"
             << symbol << "_avg = (";
        for(size_t i=0; i<data.size(); ++i) {
            cout << data[i] << (i!=data.size()-1 ? " + " : "");
        }
        cout << ")/" << n << " = " << mean << endl;

        // 偏差计算
        vector<double> diffs(n);
        cout << "\n【偏差计算】\n";
        for(int i=0; i<n; ++i) {
            diffs[i] = data[i] - mean;
            cout << "Δ" << symbol << "_" << i+1 << " = "
                 << data[i] << " - " << mean << " = " << diffs[i] << endl;
        }

        // 标准差计算 (Word兼容格式)
        double sq_sum = 0.0;
        for(double d : diffs) sq_sum += pow(d, 2);
        double std_dev = sqrt(sq_sum/(n-1));
        cout << "\n【标准差】\nσ =" << std_dev << endl;

        // 异常值检测
        cout << "\n【异常检测】3σ范围: ["
             << mean-3*std_dev << ", " << mean+3*std_dev << "]\n";
        int bad_count = 0;
        int i=0;
        for(auto val : data) {
            bool is_bad = (val < mean-3*std_dev) || (val > mean+3*std_dev);
            cout << symbol <<"_"<<i<< " = " << val << " \t"
                 << (is_bad ? "[异常]" : "[正常]") << endl;
                 i++;
            if(is_bad) bad_count++;
        }

        // A类不确定度 (Word下标格式)
        cout << "\n【结果输出】\n"
             << symbol << "_A = σ = " << std_dev << endl
             << "===============================\n";
    }

    cout << "程序已安全退出" << endl;
    return 0;
}
