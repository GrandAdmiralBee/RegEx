///////////////////////////////////
//  HEADER NAME:    FcpPkgReaderStructs.h
//  ABSTRACT   :    Definitions of structures, which are mentioned in 
//                  class PKGReaderCallbackI
//  CREATED BY :    Sergey Katserov
//  CREATION DATE:  01.09.2007
//  REMARKS    :    
///////////////////////////////////                                             
// Copyright (c) 2007, INEUM                                     
// This software is subject to copyright protection under        
// the laws of the Russian Federation and other countries.       
// All rights reserved.                                          
///////////////////////////////////

#ifndef FCP_PKGREADER_STRUCTS_H
#define FCP_PKGREADER_STRUCTS_H

//     Include Headers       
///////////////////////////////////

#include <vector>
#include "defs.h"
#include "commonstruct.h"

///////////////////////////////////////////////////
//  CLASS_NAME:    CircleInfo
//  DESCRIPTION :  Place holder for circle attributes
///////////////////////////////////////////////////

struct CircleInfo {
  bool cut;
  ME_color line_color;
  double line_width;
  ME_color filling_color;
  bool fill;
  ME_point position;
  double radius;
};

///////////////////////////////////////////////////
//  CLASS_NAME:    RectangleInfo
//  DESCRIPTION :  Place holder for rectangle attributes
///////////////////////////////////////////////////

struct RectangleInfo {
  bool cut;
  ME_color line_color;
  double line_width;
  ME_color filling_color;
  bool fill;
  ME_point position;
  ME_point p1;
  ME_point p2;
};

///////////////////////////////////////////////////
//  CLASS_NAME:    PolyInfo
//  DESCRIPTION :  Place holder for polygon attributes
///////////////////////////////////////////////////

struct PolyInfo {
  bool cut;
  ME_color line_color;
  double line_width;
  ME_color filling_color;
  bool fill;
  ME_point position;
  std::vector<ME_point> *points;
};

///////////////////////////////////////////////////
//  CLASS_NAME:    PolyArcInfo
//  DESCRIPTION :  Place holder for polyarc attributes
///////////////////////////////////////////////////

struct PolyArcInfo {
  bool cut;
  ME_color line_color;
  double line_width;
  ME_color filling_color;
  bool fill;
  ME_point position;
  std::vector<ME_polyarc_segment> *segments;
};

///////////////////////////////////////////////////
//  CLASS_NAME:    RegPolyInfo
//  DESCRIPTION :  Place holder for regular polygon attributes
///////////////////////////////////////////////////

struct RegPolyInfo {
  bool cut;
  ME_color line_color;
  double line_width;
  ME_color filling_color;
  bool fill;
  ME_point position;
  double radius;
  int vertex_count;
  double rotation;
  bool InsertedCircle;
  bool RotationToEdgeNormalFlag;
};

#endif


