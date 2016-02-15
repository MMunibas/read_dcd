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
#include <cstdio>
#include <iostream>
#include <vector>

#include "array_tools.hpp"
#include "dcd_r.hpp"
#include "vectorclass.h"

using namespace std;

Vec8f rint(const Vec8f& x)
{
  Vec8f res;
  res =  select(x >= 0.f,x + 0.5f,x - 0.5f);
  return truncate(res);
}

void applyPBC(Vec8f& dx, Vec8f& dy, Vec8f& dz,
              const Vec8f& pb)
{
  dx -= pb * rint(dx/pb);
  dy -= pb * rint(dy/pb);
  dz -= pb * rint(dz/pb);
}

int main(int argc, char* argv[])
{                
    // instance of a new object DCD_R attached to a dcd file 
    DCD_R dcdf(argv[1]);
    
    // read the header and print it
    dcdf.read_header();
    dcdf.printHeader();
    
    int size = dcdf.getNFILE();
    
    const float *x,*y,*z;
    
    float *x1,*y1,*z1,*x2,*y2,*z2;
    
    x1 = new float[size];
    y1 = new float[size];
    z1 = new float[size];
    
    x2 = new float[size];
    y2 = new float[size];
    z2 = new float[size];
    
    //deoxy indexes
    const uint id1 = 4368;
    const uint id2 = 8751;
    
    //oxy indexes
//     const uint id1 = 2146;
//     const uint id2 = 6529;
    
    // pbc[0] pbc[2] pbc[5] contain the x y z box size
    const double *pbc;
    float *pbx;
    pbx = new float[size];
    
    float progress = 0.0;
    
    try{
      // in this loop the coordinates are read frame by frame
      for(int i=0;i<size;i++)
      {
          dcdf.read_oneFrame();

          x=dcdf.getX();
          y=dcdf.getY();
          z=dcdf.getZ();
          
          pbc = dcdf.getPbc();
          
          x1[i] = x[id1];
          y1[i] = y[id1];
          z1[i] = z[id1];
          
          x2[i] = x[id2];
          y2[i] = y[id2];
          z2[i] = z[id2];

          pbx[i] = (float) pbc[0];
          
          if(i%10==0)
          {
            progress = 100.f * (float)i / (float)size;
            fprintf(stdout,"Progress (dcd read): %6.2lf %%\r",progress);
            fflush(stdout);
          }
      }
    }
    catch(std::ios_base::failure f)
    {
      cerr << "Error when reading from dcd ; most probably the dcd ended prematurely" << endl;
      cerr << f.what() << endl;
      cerr << "continuing..." << endl;
    }

    uint steps   = dcdf.getNFILE()/8;
    
    float *dist = new float[size];
    
    for(uint i=0; i<steps; i++)
    {
      Vec8f xa,ya,za,xb,yb,zb;
      Vec8f dx,dy,dz;
      Vec8f pb;
      Vec8f d2,d;
      
      xa.load(x1+i*8);
      ya.load(y1+i*8);
      za.load(z1+i*8);
      
      xb.load(x2+i*8);
      yb.load(y2+i*8);
      zb.load(z2+i*8);
      
      pb.load(pbx+i*8);
      
      dx = xb-xa;
      dy = yb-ya;
      dz = zb-za;
      
      applyPBC(dx,dy,dz,pb);
      
      d2 = square(dx)+square(dy)+square(dz);
      d = sqrt(d2);
      
      d.store(dist+i*8);
    }

    FILE *out = fopen("dist_cpp.dat","at");
    for(int i=0; i<size; i++)
      fprintf(out,"%f\n",dist[i]);
    
    return EXIT_SUCCESS;
}
