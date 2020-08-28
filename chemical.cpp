#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <utility>
#include <set>
#include <iostream>

using namespace std;
typedef unordered_map<string, int> CELL_MAP;
typedef unordered_map<int, int> G_MAP;

int N;

class Equation{
private:
    CELL_MAP l_cells_map;
    CELL_MAP r_cells_map;
    vector<string> left_exp;
    vector<string> right_exp;

    int _Coefficient(const string &item, int &pos, int n){
        int num = 0;
        while(pos < n && item[pos] >= '0' && item[pos] <= '9'){
            num *= 10;
            num += item[pos++] - '0';
        }

        return num ? num : 1;
    }

    void _Cell(CELL_MAP &cell_map, const string &item, int &pos, int n, int times){
        string name;
        //大小写组合而成的原子
        if(pos + 1 < n && item[pos+1] >= 'a' && item[pos+1] <= 'z'){
            name = string{item[pos], item[pos+1]};
            pos += 2;
        }
        //单字母原子
        else{
            name.push_back(item[pos++]);
        }
        //检查原子后的系数
        int cnt = times * _Coefficient(item, pos, n);
        
        if(cell_map.find(name) == cell_map.end()){
            cell_map.insert(make_pair(name, cnt));
        }
        else cell_map[name] += cnt;
    }

    void _Group(G_MAP &g_map, const string &item, int n){
        vector<int> v;
        for(int i = 0; i < n; i++){
            if(item[i] == '('){
                v.push_back(i);
            }
            else if(item[i] == ')'){
                g_map[v.back()] = i;
                v.pop_back();
            }
        }
    }


    void _Item(CELL_MAP &cell_map, G_MAP &g_map, const string &item, int &i, int n, int times){
        while(i < n){
            if(item[i] >= 'A' && item[i] <= 'Z'){
                _Cell(cell_map, item, i, n, times);
            }
            else if(item[i] == '('){
                int r_bracket = g_map[i];
                int next_pos = r_bracket+1;
                int num = _Coefficient(item, next_pos, n);
                _Item(cell_map, g_map, item, ++i, r_bracket, times*num);
                i = next_pos;
            }
        }
    }

public:
    Equation(const string &equa){
        int i = 0, n = equa.size();
        //等式左边
        while(equa[i] != '='){  
            left_exp.push_back("");  
            string &l_last = left_exp.back();      
            while(equa[i] != '=' && equa[i] != '+'){
                l_last.push_back(equa[i++]);
            }
            if(equa[i] == '+')++i;
        };

        ++i; //跳过等号

        //等式右边
        while(i < n){
            right_exp.push_back("");
            string &r_last = right_exp.back();
            while(i < n && equa[i] != '+'){
                r_last.push_back(equa[i++]);
            }
            ++i;
        }
    }

    bool equal(){
        //检查等式左边
        for(string &item : left_exp){
            G_MAP g_map;
            int i = 0, n = item.size();
            int coef = 1;
            //检查开头的系数
            coef = _Coefficient(item, i, n);
            //生成括号位置表
            _Group(g_map, item, n);
            _Item(l_cells_map, g_map, item, i, n, coef);
        }
        //检查等式右边
        for(string &item : right_exp){
            G_MAP g_map;
            int i = 0, n = item.size();
            int coef = 1;
            //检查开头的系数
            coef = _Coefficient(item, i, n);
            //生成括号位置表
            _Group(g_map, item, n);
            _Item(r_cells_map, g_map, item, i, n, coef);
        }
        
        //终极对决！！！
        if(l_cells_map.size() != r_cells_map.size())return false; //太坑爹了，不加这个90分
        for(auto it = l_cells_map.begin(); it != l_cells_map.end(); it++){
            auto it1 = r_cells_map.find(it->first);
            if(it1 == r_cells_map.end() || it1->second != it->second){
                return false;
            }
        }
        
        return true;
    }
};

int main(){
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    cin >> N;
    for(int i = 0; i < N; i++){
        string s;
        cin >> s;
        Equation equation(s);
        
        if(equation.equal()){
            cout << "Y" << endl;
        }
        else cout << "N" << endl;
    }
    return 0;
}