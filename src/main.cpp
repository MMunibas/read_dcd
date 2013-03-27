/* 
 * File:   main.cpp
 * Author: hedin
 *
 * Created on March 27, 2013, 9:55 AM
 */

#include <cstdlib>
#include <iostream>

#include "dcd.hpp"

using namespace std;

/*
 * 
 */
int main() {

    // instance of a new object DCD attached to a dcd file
    DCD dcdFile("dyna.dcd");
    
    // read the header and print it
    dcdFile.read_header();
    dcdFile.printHeader();
    
    // in this loop the coordinates are read frame by frame
    for(int i=0;i<dcdFile.getNFILE();i++)
    {
        dcdFile.read_oneFrame();
        /* your code goes here */
    }
    
    return EXIT_SUCCESS;
}
