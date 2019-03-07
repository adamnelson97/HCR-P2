#include "functions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

//Don't Change!
bool isTextFile (string const &fileName, string const &ending) {
    if (fileName.length() >= ending.length()) {
        return (0 == fileName.compare(fileName.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}