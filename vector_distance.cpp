#include<vector>
#include<cmath>

double get_euclidean_distance(const std::vector<double> &a, const std::vector<double> &b){
    if(a.size() != b.size()){
        return -1;
    }
    double sum = 0.0;
    for(int i = 0; i < a.size(); i++){
        sum += pow(a[i] - b[i],2);
    }
    return sqrt(sum);
}

double get_cosine_similarity(const std::vector<double> &a, const std::vector<double> &b){
    if(a.size() != b.size()){
        return -1;
    }
    double cosine = 0.0;
    double sum_a = 0.0;
    double sum_b = 0.0;
    for(int i = 0; i < a.size(); i++){
        cosine += a[i] * b[i];
        sum_a += pow(a[i], 2);
        sum_b += pow(b[i], 2);
    }
    sum_a = sqrt(sum_a);
    sum_b = sqrt(sum_b);
    return cosine / (sum_a * sum_b);
}