#include "process_vector.h"
#include <map>

void sort_vector_accending(vector<double>& v) {
    if(v.size() <= 1)
        return;
    size_t mid = v.size() / 2;
    vector<double> left(v.begin(), v.begin() + mid);
    vector<double> right(v.begin() + mid, v.end());
    sort_vector_accending(left);
    sort_vector_accending(right);
    v = merge(left, right);
}

vector<double> merge(vector<double> v1, vector<double> v2) {
    vector<double> result;
    size_t i = 0, j = 0;
    while(i < v1.size() && j < v2.size()){
        if(v1[i] <= v2[j]){
            result.push_back(v1[i]);
            i++;
        } else {
            result.push_back(v2[j]);
            j++;
        }
    }
    while(i < v1.size()){
        result.push_back(v1[i]);
        i++;
    }
    while(j < v2.size()){
        result.push_back(v2[j]);
        j++;
    }
    return result;
}

double find_mean(vector<double> v) {
    if(v.empty())
        return 0.0;
    double sum = 0.0;
    for(double num : v) {
        sum += num;
    }
    return sum / v.size();
}

double find_median(vector<double> v) {
    if(v.empty())
        return 0.0;

    sort_vector_accending(v);
    size_t n = v.size();
    if(n % 2 == 1)
        return v[n / 2];
    else
        return (v[n / 2 - 1] + v[n / 2]) / 2.0;
}

double find_mode(vector<double> v) {
    if(v.empty())
        return 0.0;

    map<double, int> frequency;
    for(double num : v) {
        frequency[num]++;
    }
    int maxFreq = 0;
    for(auto pair : frequency) {
        if(pair.second > maxFreq)
            maxFreq = pair.second;
    }

    double sumModes = 0.0;
    int countModes = 0;
    for(auto pair : frequency) {
        if(pair.second == maxFreq) {
            sumModes += pair.first;
            countModes++;
        }
    }
    return sumModes / countModes;
}