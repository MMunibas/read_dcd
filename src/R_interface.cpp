/*
 *  read_dcd : c++ class for reading a CHARMM dcd file
 *  Copyright (C) 2013-2015  Florent Hedin
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

#ifdef R_DCD_LIB

#include "dcd_r.hpp"

#include <R.h>
#include <Rinternals.h>
// #include <Rdefines.h>

static DCD_R* dcdf = nullptr;

extern "C"
{

  SEXP R_open_dcd(SEXP dcdname)
  {

    int n = length(dcdname);
    const char* name = CHAR(STRING_ELT(dcdname, 0));
    dcdf = new DCD_R(s);
   
    return R_NilValue;
   
  }
  
  SEXP R_read_header()
  {
    return R_NilValue;
  }
  
  SEXP R_print_header()
  {
    return R_NilValue;
  }
  
  SEXP R_read_oneFrame()
  {
    return R_NilValue;
  }
  
  SEXP R_close_dcd()
  {
    delete dcdf;
    return R_NilValue;
  }
  
}

#endif

