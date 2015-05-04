//============================================================================
//                                  I B E X
// File        : ibex_PixelMap.h
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2014
//============================================================================

#include "ibex_CtcPixelMap.h"

namespace ibex {

CtcPixelMap::CtcPixelMap(PixelMap &data): Ctc(data.ndim), I(data) {
    pixel_coords = new int[2*I.ndim];
}

//-------------------------------------------------------------------------------------------------------------
CtcPixelMap::~CtcPixelMap() {
    delete[] pixel_coords;
}


//-------------------------------------------------------------------------------------------------------------
void CtcPixelMap::world_to_grid(IntervalVector box) {

    for(int i = 0; i < box.size(); i++) {
        box[i] = (box[i] - I.origin_[i]) / I.leaf_size_[i];
        // Limit range to image size on pixel_coord
        box[i] &= Interval(0,I.grid_size_[i]);
    }
    
    for (unsigned int i = 0; i < I.ndim; i++) {
        pixel_coords[2*i]   = floor(box[i].lb());
        pixel_coords[2*i+1] = ceil(box[i].ub()-1);
    }
}

//-------------------------------------------------------------------------------------------------------------
void CtcPixelMap::grid_to_world(IntervalVector& box) {
    for(unsigned int i = 0; i < I.ndim; i++) {
        box[i] &= Interval(pixel_coords[2*i], pixel_coords[2*i+1]+1) * I.leaf_size_[i] + I.origin_[i];
        if(box[i].is_empty()){
            box.set_empty();
            return;
        }   
    }
}

//----------------------------------------------------------------------------------------------------------------
void CtcPixelMap::contract(IntervalVector& box) {

    assert(box.size() == (int)I.ndim);
    if(box.is_empty()) return;

    // Convert world coordinates into pixel coordinates
    world_to_grid(box);

    // Contractor the box
    if( I.ndim == 2) {
        contract(pixel_coords[0],pixel_coords[1],pixel_coords[2],pixel_coords[3]);
    } else if ( I.ndim == 3){
        contract(pixel_coords[0],pixel_coords[1],pixel_coords[2],pixel_coords[3],pixel_coords[4],pixel_coords[5]);        
    }
    // Check the result
    if(pixel_coords[0] == -1) {
        box.set_empty();
        return;
    }

    // Convert pixel coordinates into world coordinates
    grid_to_world(box);
}

//------------------------------------------------------------------------------
//psi contraction
void CtcPixelMap::contract(int& cxmin, int& cxmax, int& cymin, int& cymax) {

    //compute enclosed pixels on consecutive lines from all dimensions of the box

    cxmax = std::max(0,std::min(I.grid_size_[0]-1,cxmax));
    cxmin = std::min(I.grid_size_[0]-1,std::max(0,cxmin));
    cymax = std::max(0,std::min(I.grid_size_[1]-1,cymax));
    cymin = std::min(I.grid_size_[1]-1,std::max(0,cymin));

    if (enclosed_pixels(cxmin,cxmax,cymin,cymax) == 0){
        cxmin = -1;
        return;
    }

    //right
    for (int i = cxmin; i <= cxmax; i++)
    {
        if(enclosed_pixels(cxmin,i,cymin,cymax)>0) {
            cxmin = i;
            break;
        }
    }
    //left
    for (int i = cxmax; i >= cxmin ; i--)
    {
        if(enclosed_pixels(i,cxmax,cymin,cymax)>0) {
            cxmax = i;
            break;
        }
    }
    //down
    for (int j = cymin; j <=cymax; j++)
    {
        if(enclosed_pixels(cxmin,cxmax,cymin,j)>0) {
            cymin = j;
            break;
        }
    }
    //up
    for (int j = cymax; j >= cymin ; j--)
    {
        if(enclosed_pixels(cxmin,cxmax,j,cymax)>0) {
            cymax = j;
            break;
        }
    }
}
//////////////////////////////////////////////////////////////////////

void CtcPixelMap::contract(int& cxmin, int& cxmax, int& cymin,int& cymax, int& czmin, int& czmax) {

    cxmax = std::max(0,std::min(I.grid_size_[0]-1,cxmax));
    cxmin = std::min(I.grid_size_[0]-1,std::max(0,cxmin));
    cymax = std::max(0,std::min(I.grid_size_[1]-1,cymax));
    cymin = std::min(I.grid_size_[1]-1,std::max(0,cymin));
    czmax = std::max(0, std::min(I.grid_size_[2]-1,czmax));
    czmin = std::min(I.grid_size_[2]-1, std::max(0,czmin));
    
    if (enclosed_pixels(cxmin,cxmax,cymin,cymax,czmin,czmax) == 0) {
        cxmin = -1;
        return;
    }

    //right
    for (int i = cxmin; i <= cxmax; i++)
    {
        if(enclosed_pixels(cxmin,i,cymin,cymax,czmin,czmax)>0) {
            cxmin = i;
            break;
        }
    }
    //left
    for (int i = cxmax; i >= cxmin ; i--)
    {
        if(enclosed_pixels(i,cxmax,cymin,cymax,czmin,czmax)>0) {
            cxmax = i;
            break;
        }
    }
    //down
    for (int j = cymin; j <=cymax; j++)
    {
        if(enclosed_pixels(cxmin,cxmax,cymin,j,czmin,czmax)>0) {
            cymin = j;
            break;
        }
    }
    //up
    for (int j = cymax; j >= cymin ; j--)
    {
        int res = enclosed_pixels(cxmin,cxmax,j,cymax,czmin,czmax);
        if(res>0) {
            cymax = j;
            break;
        }
    }
    //bottom
    for (int k = czmin; k <=czmax; k++)
    {
        if(enclosed_pixels(cxmin,cxmax,cymin,cymax,czmin,k)>0) {
            czmin = k;
            break;
        }
    }
    //top
    for (int k = czmax; k >= czmin ; k--)
    {
        int res = enclosed_pixels(cxmin,cxmax,cymin,cymax,k,czmax);
        if(res>0) {
            czmax = k;
            break;
        }
    }
}

unsigned int CtcPixelMap::enclosed_pixels(int xmin,int xmax,int ymin,int ymax) {

    // The raster picture is cast into a 2D PixelMap to access its elements
    PixelMap2D& I_tmp = (PixelMap2D&) I;
    int b1 = I_tmp(xmax,ymax);
    int b2 = I_tmp(xmax,ymin-1);
    int b3 = I_tmp(xmin-1,ymax);
    int b4 = I_tmp(xmin-1,ymin-1);
    return b1 - b2 - b3 + b4;
    
}

unsigned int CtcPixelMap::enclosed_pixels(int xmin, int xmax, int ymin, int ymax, int zmin, int zmax) {

    // The raster picture is cast into a 3D PixelMap to access its elements
    PixelMap3D &I_tmp = (PixelMap3D&) I;
    unsigned int L8 = I_tmp(xmax, ymax,zmax);
    unsigned int L5 = I_tmp(xmin-1, ymin-1, zmax);
    unsigned int L6 = I_tmp(xmin-1, ymax,zmax) ;
    unsigned int L7 = I_tmp(xmax, ymin-1,zmax);
    unsigned int L4 = I_tmp(xmax, ymax,zmin-1);
    unsigned int L1 = I_tmp(xmin-1, ymin-1, zmin-1);
    unsigned int L2 = I_tmp(xmin-1, ymax,zmin-1);
    unsigned int L3 = I_tmp(xmax, ymin-1,zmin-1);
    unsigned int L = (L8 + L5 - L6  - L7) -(L4 + L1 - L2 - L3);
    return L;
}

} // end namespace ibex
