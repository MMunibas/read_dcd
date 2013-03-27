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
#include <cstring>

#include <fstream>
#include <iostream>

#include "dcd.hpp"

using namespace std;

DCD::DCD(const char filename[])
{
    dcdf.open(filename,ios::in|ios::binary);
    dcd_first_read=true;
}

void DCD::read_header()
{
    unsigned int fortcheck1,fortcheck2;
    
    dcdf.read((char*)&fortcheck1,sizeof(unsigned int));
    dcdf.read((char*)HDR,sizeof(char)*4);
    dcdf.read((char*)ICNTRL,sizeof(int)*20);
    dcdf.read((char*)&fortcheck2,sizeof(unsigned int));
    checkFortranIOerror(__FILE__,__LINE__,fortcheck1,fortcheck2);

    HDR[4]='\0';
    NFILE = ICNTRL[0];
    NPRIV = ICNTRL[1];
    NSAVC = ICNTRL[2];
    NSTEP = ICNTRL[3];
    NDEGF = ICNTRL[7];
    FROZAT= ICNTRL[8];
    DELTA4= ICNTRL[9];
    QCRYS = ICNTRL[10];
    CHARMV= ICNTRL[19];
    
    dcdf.read((char*)&fortcheck1,sizeof(unsigned int));
    dcdf.read((char*)&NTITLE,sizeof(int));
    if(NTITLE==0)
    {
        TITLE=new char[80+1];
        TITLE[0]='\0';
    }
    else
    {
        TITLE=new char[NTITLE*80+1];
        for(int it=0;it<NTITLE;it++)
        {
            dcdf.read((char*)&TITLE[it*80],sizeof(char)*80);
        }
        TITLE[NTITLE*80]='\0';
    }
    dcdf.read((char*)&fortcheck2,sizeof(unsigned int));
    checkFortranIOerror(__FILE__,__LINE__,fortcheck1,fortcheck2);
    
    dcdf.read((char*)&fortcheck1,sizeof(unsigned int));
    dcdf.read((char*)&NATOM,sizeof(int));
    dcdf.read((char*)&fortcheck2,sizeof(unsigned int));
    checkFortranIOerror(__FILE__,__LINE__,fortcheck1,fortcheck2);
    
    LNFREAT = NATOM - FROZAT;
    if (LNFREAT != NATOM)
    {
        FREEAT=new int[LNFREAT];
        dcdf.read((char*)&fortcheck1,sizeof(unsigned int));
        dcdf.read((char*)FREEAT,sizeof(int)*LNFREAT);
        dcdf.read((char*)&fortcheck2,sizeof(unsigned int));
        checkFortranIOerror(__FILE__,__LINE__,fortcheck1,fortcheck2);
    }
    
    alloc_crd();
}

void DCD::alloc_crd()
{
    X=new float[NATOM];
    Y=new float[NATOM];
    Z=new float[NATOM];
}

void DCD::read_oneFrame()
{
    unsigned int fortcheck1,fortcheck2;
    
    int siz=(dcd_first_read) ? NATOM : LNFREAT ;
    
    float *tmpX=new float[siz];
    float *tmpY=new float[siz];
    float *tmpZ=new float[siz];
    
    if (QCRYS)
    {
        dcdf.read((char*)&fortcheck1,sizeof(unsigned int));
        dcdf.read((char*)pbc,sizeof(double)*6);
        dcdf.read((char*)&fortcheck2,sizeof(unsigned int));
        checkFortranIOerror(__FILE__,__LINE__,fortcheck1,fortcheck2);
    }
    
    // X
    dcdf.read((char*)&fortcheck1,sizeof(unsigned int));
    dcdf.read((char*)tmpX,sizeof(float)*siz);
    dcdf.read((char*)&fortcheck2,sizeof(unsigned int));
    checkFortranIOerror(__FILE__,__LINE__,fortcheck1,fortcheck2);
    
    // Y
    dcdf.read((char*)&fortcheck1,sizeof(unsigned int));
    dcdf.read((char*)tmpY,sizeof(float)*siz);
    dcdf.read((char*)&fortcheck2,sizeof(unsigned int));
    checkFortranIOerror(__FILE__,__LINE__,fortcheck1,fortcheck2);
    
    // Z
    dcdf.read((char*)&fortcheck1,sizeof(unsigned int));
    dcdf.read((char*)tmpZ,sizeof(float)*siz);
    dcdf.read((char*)&fortcheck2,sizeof(unsigned int));
    checkFortranIOerror(__FILE__,__LINE__,fortcheck1,fortcheck2);
    
    if(dcd_first_read)
    {
        memcpy(X,tmpX,NATOM*sizeof(float));
        memcpy(Y,tmpY,NATOM*sizeof(float));
        memcpy(Z,tmpZ,NATOM*sizeof(float));
    }
    else
    {
        if (LNFREAT != NATOM)
        {
            for(int it=0;it<siz;it++)
            {
                X[ FREEAT[it]-1 ] = tmpX[it];
                Y[ FREEAT[it]-1 ] = tmpY[it];
                Z[ FREEAT[it]-1 ] = tmpZ[it];
            }
        }
        else
        {
            memcpy(X,tmpX,NATOM*sizeof(float));
            memcpy(Y,tmpY,NATOM*sizeof(float));
            memcpy(Z,tmpZ,NATOM*sizeof(float));
        }
    }
    
    if(dcd_first_read)
        dcd_first_read=false;
    
    delete[] tmpX;
    delete[] tmpY;
    delete[] tmpZ;
}

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

void DCD::printHeader()
{
    int i;
    
    cout << "HDR :\t" << HDR << endl;
    
    cout << "ICNTRL :\t";
    for(i=0;i<20;i++)
        cout << ICNTRL[i] << "\t" ;
    cout << endl;
    
    cout << "NTITLE :\t" << NTITLE << endl;
    cout << "TITLE :\t" << TITLE << endl;
    
    cout << "NATOM :\t" << NATOM << endl;
    cout << "LNFREAT :\t" << LNFREAT << endl;
    
}

int DCD::getNFILE() const {
    return NFILE;
}

const float* DCD::getZ() const {
    return Z;
}

const float* DCD::getY() const {
    return Y;
}

const float* DCD::getX() const {
    return X;
}

const int* DCD::getFREEAT() const {
    return FREEAT;
}

int DCD::getLNFREAT() const {
    return LNFREAT;
}

int DCD::getNATOM() const {
    return NATOM;
}

int DCD::getCHARMV() const {
    return CHARMV;
}

int DCD::getQCRYS() const {
    return QCRYS;
}

int DCD::getDELTA4() const {
    return DELTA4;
}

int DCD::getFROZAT() const {
    return FROZAT;
}

int DCD::getNDEGF() const {
    return NDEGF;
}

int DCD::getNSTEP() const {
    return NSTEP;
}

int DCD::getNSAVC() const {
    return NSAVC;
}

int DCD::getNPRIV() const {
    return NPRIV;
}

const double* DCD::getPbc() const {
    return pbc;
}

const char* DCD::getTITLE() const {
    return TITLE;
}

int DCD::getNTITLE() const {
    return NTITLE;
}

const int* DCD::getICNTRL() const {
    return ICNTRL;
}

const char* DCD::getHDR() const {
    return HDR;
}

DCD::~DCD()
{
    dcdf.close();
    
    delete[] TITLE;
    
    if (LNFREAT != NATOM)
        delete[] FREEAT;
    
    delete[] X;
    delete[] Y;
    delete[] Z;
}
