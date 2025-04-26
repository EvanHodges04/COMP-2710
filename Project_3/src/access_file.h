#ifndef _ACCESS_FILE_H_
#define _ACCESS_FILE_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

/*
function name: check_file
arguements:
    FILEPATH: a string of file path
return value: a bool that indicates if file in FILEPATH is accessable or not
*/
bool check_file(string FILEPATH);

/*
function name: read_file
arguements:
    FILEPATH: a string of file path
    v: a reference to a vector of double
purpose: read file in FILEPATH then parse the file and save its content into vector v
return value: a bool that indicates if the contents in FILEPATH is legal or not
*/
bool read_file(string FILEPATH, vector<double>& v);

/*
function name: write_file
arguements:
    FILEPATH: a string of file path
    v: a sorted vector of double
    mean: a double that is the calculated mean of v
    median: a double that is the calculated median of v
    mode: a double that is the calculated mode of v
purpose: write v, mean, median and mode to file in FILEPATH
return value: a bool that indicates the write process is successful or not
*/
bool write_file(string FILEPATH, vector<double> v, double mean, double median, double mode);

#endif