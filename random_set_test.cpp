#include<unordered_map>
#include "random_set.cpp"
#include<vector>
#include<iostream>
#include<cstdio>

int N = 10;
int MIN_BOOTSTRAP = 100;
int MAX_BOOTSTRAP = 10000;
int SPEED = 10;
int STEP = 0;

union cnt_or_probability{
    int cnt;
    double probability;
} ;

bool init_param(){
    freopen("in.text", "r", stdin);
    std::cin >> N;
    std::cin >> MIN_BOOTSTRAP;
    std::cin >> MAX_BOOTSTRAP;
    std::cin >> SPEED;
    std::cin >> STEP;
}

// 计算样本大小为n， 测试用例为bootstrap的方差大小，方差越小，代表差异性越小, 概率越趋于平均
double get_unrepeatable_random_set_variance(int n, int bootstrap){
    std::unique_ptr<RandomSet> random_set_ptr(new RandomSet(false, time(nullptr)));
    for(int i = 0; i < n; i ++){
        random_set_ptr->insert(i);
    }
    std::unordered_map<int, cnt_or_probability> random_cnt_hashtable;
    for(size_t i = 0; i < bootstrap; i++) {
        int val = random_set_ptr->getRandom();
        random_cnt_hashtable[val].cnt++;
    }
    double variance = 0.0;
    for(auto entry : random_cnt_hashtable){
        entry.second.probability = entry.second.cnt * 1.0/ bootstrap;
        variance += (entry.second.probability - 1.0 / n) * (entry.second.probability - 1.0/ n);
    }
    return variance / (n - 1);
}

// 计算样本大小为n， 测试用例为bootstrap的方差大小，方差越小，代表差异性越小,概率越趋于平均
double get_repeatable_random_set_variance(int n, int bootstrap){
    std::unique_ptr<RandomSet> random_set_ptr(new RandomSet(true, time(nullptr)));
    for(int i = 1; i <= n; i ++){
        for(int j = 1; j <= i; j++){
            random_set_ptr ->insert(i);
        }
    }
    std::unordered_map<int, cnt_or_probability> random_cnt_hashtable;
    for(size_t i = 0; i < bootstrap; i++) {
        int val = random_set_ptr->getRandom();
        random_cnt_hashtable[val].cnt++;
    }
    double variance = 0.0;
    int sample_size = (1 + n) * n / 2;
    double average = 0.0;
    for(auto entry : random_cnt_hashtable){
        average += entry.second.cnt * 1.0/ bootstrap;
    }
    average = average / n;
    for(auto entry : random_cnt_hashtable){
        entry.second.probability = entry.second.cnt * 1.0/ bootstrap;
        variance += (entry.second.probability - average) * (entry.second.probability - average);
    }
    return variance / (n - 1);

}

// 不断增大数据量，得到方差变化列表
std::vector<double> get_unrepeatable_random_variance_list(int n, int min_bootstrap, int max_bootstrap, int speed, int step){
    std::vector<double> variance_vec;
    for(int bootstrap = min_bootstrap; bootstrap <= max_bootstrap; bootstrap = speed * bootstrap + step){
        double variance = get_unrepeatable_random_set_variance(n, bootstrap);
        variance_vec.push_back(variance);
    }
    return variance_vec;
}

std::vector<double> get_repeatable_random_variance_list(int n, int min_bootstrap, int max_bootstrap, int speed, int step){
    std::vector<double> variance_vec;
    for(int bootstrap = min_bootstrap; bootstrap <= max_bootstrap; bootstrap = speed * bootstrap + step){
        double variance = get_repeatable_random_set_variance(n, bootstrap);
        variance_vec.push_back(variance);
    }
    return variance_vec;
}
// 验证当数据量增大的时候，如果方差越来越小，可以证明是等概率的。
bool verify(std::vector<double> & variance_list){
    for(int i = 1; i < variance_list.size(); i++){
        if(variance_list[i] > variance_list[i-1]){
            return false;
        }
    }
    return true;
}

// 初始化参数，进行随机测试，
bool test_unrepeatable_random_set_probability(){
    init_param();
    auto variance_list = get_unrepeatable_random_variance_list(N, MIN_BOOTSTRAP, MAX_BOOTSTRAP, SPEED, STEP);
    // debug
    int bootstrap = MIN_BOOTSTRAP;
    std::cout << "样本空间大小为" << N << std::endl;
    for(double variance : variance_list){
        std::cout << "当产生的随机数大小是" << bootstrap << "时, 方差为" << variance << std::endl;
        bootstrap = SPEED * bootstrap + STEP;
    }
    // debug
    return verify(variance_list);
}

// 初始化参数，进行随机测试，
bool test_repeatable_random_set_probability(){
    init_param();
    auto variance_list = get_repeatable_random_variance_list(N, MIN_BOOTSTRAP, MAX_BOOTSTRAP, SPEED, STEP);
    // debug
    int bootstrap = MIN_BOOTSTRAP;
    std::cout << "样本空间大小为" << N << std::endl;
    for(double variance : variance_list){
        std::cout << "当产生的随机数大小是" << bootstrap << "时, 方差为" << variance << std::endl;
        bootstrap = SPEED * bootstrap + STEP;
    }
    // debug
    return verify(variance_list);
}

int main(){
    bool ans = test_unrepeatable_random_set_probability();
    if(ans){
        std::cout << "unrepeatable_random_set验证通过!" << std::endl;
    }else{
        std::cout << "unrepeatable_random_set验证未通过!" << std::endl;
    }

    ans = test_repeatable_random_set_probability();
    if(ans){
        std::cout << "repeatable_random_set验证通过!" << std::endl;
    }else{
        std::cout << "repeatable_random_set验证未通过!" << std::endl;
    }
    return 0;
}
