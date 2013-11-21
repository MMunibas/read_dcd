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
#include <fstream>

#include <cmath>

#include "array_tools.hpp"

#include "dcd_r.hpp"

using namespace std;

int main(int argc, char* argv[])
{                
    if (argc != 5 )
    {
        std::cout << "Need 5 arguments : 'dcd_name a b c' where a b c are the PBC parameters." << std::endl;
        return EXIT_FAILURE;
    }
    
    // instance of a new object DCD_R attached to a dcd file 
    DCD* dcdf = new DCD_R(argv[1]);
    
    // read the header and print it
    dcdf->read_header();
    dcdf->printHeader();
    
    unsigned int dim = 32;
    double a = strtod(argv[2],nullptr);
    double b = strtod(argv[3],nullptr);
    double c = strtod(argv[4],nullptr);
    
    double da,db,dc;
    da = a/(double)dim;
    db = b/(double)dim;
    dc = c/(double)dim;
    
    double damin,damax,dbmin,dbmax,dcmin,dcmax;
    damin = -a/2;
    damax = a/2;
    dbmin = -b/2;
    dbmax = b/2;
    dcmin = -c/2;
    dcmax = c/2;

    ARRAY_3D<double> dens(dim,dim,dim); //allocate the array
    
    const float *x=nullptr, *y=nullptr, *z=nullptr;
    // in this loop the coordinates are read frame by frame
    for(int i=0; i<dcdf->getNFILE(); i++)
    {
        dcdf->read_oneFrame();
        
        /* your code goes here */
        
        x = dcdf->getX();
        y = dcdf->getY();
        z = dcdf->getZ();
        
        /* ... */

        unsigned int lx, ly, lz;
        lx = (unsigned int) ceil( (x[0]-damin)/da );
        ly = (unsigned int) ceil( (y[0]-dbmin)/db );
        lz = (unsigned int) ceil( (z[0]-dcmin)/dc );
        
//         std::cout << "lx ly lz : \t" << lx << '\t' << ly << '\t'<< lz << '\t' << std::endl;
        
        dens(lx,ly,lz) += 1.0;
    }
    
//     dens.dump();

    std::fstream ascii("dens.dat",ios::out);
    dens.toFile_ascii(ascii);
    ascii.close();
    
    delete dcdf;
    
    return EXIT_SUCCESS;
}
