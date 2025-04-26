//================================================================================================
// Name     : Evan Hodges
// UserID   : ejh0058
// Filename : data_analyzer.cpp
// Compile  : make (in the root directory with makefile)
// Execute  : ./bin/main
// Sources  : Internet resources, Dr. Li slides, ChatGPT for 'understanding assistance' (no code)
//================================================================================================

#include "access_file.h"
#include "process_vector.h"
#include <iostream>
#include <limits>
#include <iomanip>

using namespace std;

int main(){
    cout << "*** Welcome to Evan's Data Analyzer ***" << endl;
    cout << "Enter the number of files to read: ";
    int numFiles;
    while(!(cin >> numFiles) || numFiles <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a positive integer: ";
    }
    
    vector<double> combined;
    for(int i = 0; i < numFiles; i++){
        string filename;
        vector<double> fileData;
        bool validFile = false;
        do {
            cout << "\nEnter the filename for file " << (i+1) << ": ";
            cin >> filename;
            if(!check_file(filename)) {
                cout << "File not found. Please try again." << endl;
            } else if(!read_file(filename, fileData)) {
                cout << "Not an input file. Illegal content/structure detected. Please try again." << endl;
            } else {
                validFile = true;
            }
        } while(!validFile);
        
        cout << "The list of " << fileData.size() << " values in file " << filename << " is:" << endl;
        for(double num : fileData) {
            cout << num << endl;
        }
        
        combined.insert(combined.end(), fileData.begin(), fileData.end());
    }
    
    sort_vector_accending(combined);
    
    double mean = find_mean(combined);
    double median = find_median(combined);
    double mode = find_mode(combined);
    
    cout << "\n*** Summarized Statistics ***\n" << endl;
    cout << "The orderly sorted list of " << combined.size() << " values is:" << endl;
    for(double num : combined) {
        cout << num << " ";
    }
    cout << "\n" << endl;
    cout << "The mean is " << fixed << setprecision(4) << mean << endl;
    cout << "The median is " << fixed << setprecision(4) << median << endl;
    cout << "The mode is " << fixed << setprecision(4) << mode << endl;
    
    string outFilename;
    bool fileWritten = false;
    do {
        cout << "\nEnter the output filename to save: ";
        cin >> outFilename;
        if(write_file(outFilename, combined, mean, median, mode)){
            fileWritten = true;
            cout << "\n*** File " << outFilename << " has been written to disk ***" << endl;
        } else {
            cout << "Could not write to file. Please try again." << endl;
        }
    } while(!fileWritten);
    
    cout << "\n*** Goodbye. ***" << endl;
    return 0;
}