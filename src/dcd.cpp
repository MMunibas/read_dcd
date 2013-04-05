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

#include "dcd.hpp"

using namespace std;

DCD::DCD()
{
}

/*
 * When writing a block of binary data to a file, Fortran adds 2 unsigned integer before and after the block : each one contains the total number of bytes of the block.
 * 
 * For example for a fortran array of real(8) of size 10, 80 bytes of a data have to be written : so 3 things are written to the binary file : 
 *  1) An unsigned integer (4 bytes) , its value is 80
 *  2) The fortran array (80 bytes)
 *  3) A second unsigned integer (4 bytes), same value of 80.
 * This corresponds to the fortran statement "write(file_descriptor) fortran_array"
 * 
 * Now when writing several things at the same tine, i.e. "write(file_descriptor) fortran_array,an_integer"
 * things are written as following:
 * 1) unsigned int size
 * 2) fortran array
 * 3) an_integer
 * 4) unsigned int size
 * The value of the 2 unsigned ints is 84 : 80 for the array, 4 for the integer(4).
 * 
 * The following method DCD::checkFortranIOerror check that the 2 unsigned ints have the same value: if not there was a probem when reading the binary fortran file.
 */
void DCD::checkFortranIOerror(const char file[], const int line, const unsigned int fortcheck1, const unsigned int fortcheck2)
{
    if( fortcheck1 != fortcheck2 )
    {
        cout << "Error when reading data from dcd : quantities do not match." << endl;
        cout << "fortcheck1 = " << fortcheck1 <<  " and fortcheck2 = " << fortcheck2 << endl;
        cout << "in File " << file << " at Line " << line << endl;
        exit(EXIT_FAILURE);
    }
}

DCD::~DCD()
{
}
