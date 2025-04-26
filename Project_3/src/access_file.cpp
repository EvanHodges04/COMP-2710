#include "access_file.h"
#include <sstream>
#include <limits>
#include <cctype>
#include <iomanip>

bool check_file(string FILEPATH) {
    ifstream infile(FILEPATH);
    bool good = infile.good();
    infile.close();
    return good;
}

bool read_file(string FILEPATH, vector<double>& v) {
    ifstream infile(FILEPATH);
    if (!infile.is_open()){
        return false;
    }
    string line;
    bool hasValidData = false;
    while(getline(infile, line)) {
        if (line.empty())
            continue;
        try {
            size_t pos;
            double num = std::stod(line, &pos);
            // Skip trailing whitespace
            while (pos < line.size() && isspace(line[pos])) {
                pos++;
            }
            // If extra characters remain, the line is malformed.
            if (pos != line.size()){
                v.clear();
                infile.close();
                return false;
            }
            v.push_back(num);
            hasValidData = true;
        } catch(exception &e) {
            v.clear();
            infile.close();
            return false;
        }
    }
    infile.close();
    return hasValidData;
}

bool write_file(string FILEPATH, vector<double> v, double mean, double median, double mode) {
    ofstream outfile(FILEPATH);
    if(!outfile.is_open()){
        return false;
    }

    outfile << "The orderly sorted list of " << v.size() << " values is:" << endl;
    for (size_t i = 0; i < v.size(); i++){
        outfile << v[i];
        if (i != v.size() - 1)
            outfile << ",";
    }
    outfile << endl << endl;

    outfile << "mean," << fixed << setprecision(4) << mean << endl;
    outfile << "median," << fixed << setprecision(4) << median << endl;
    outfile << "mode," << fixed << setprecision(4) << mode << endl;
    outfile.close();
    return true;
}