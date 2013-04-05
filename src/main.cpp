/*
 *  read_dcd : c++ class + main file example for reading a CHARMM dcd file
 *  Copyright (C) 2013  Florent Hedin
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <iostream>

#include "dcd_r.hpp"

using namespace std;

int main() {

    // instance of a new object DCD_R attached to a dcd file 
    DCD_R dcdFile("dyna.dcd");
    
    // read the header and print it
    dcdFile.read_header();
    dcdFile.printHeader();
    
    const float *x,*y,*z;
    
    // in this loop the coordinates are read frame by frame
    for(int i=0;i<dcdFile.getNFILE();i++)
    {
        dcdFile.read_oneFrame();
        
        /* your code goes here */
        
        x=dcdFile.getX();
        y=dcdFile.getX();
        z=dcdFile.getX();
        /* ... */
        
    }
    
    return EXIT_SUCCESS;
}
