/* Copyright (c) 2007, INEUM                                     */
/* This software is subject to copyright protection under        */
/* the laws of the Russian Federation and other countries.       */
/* All rights reserved.                                          */

#ifndef STRUCT_H
#define STRUCT_H

#include "defs.h"

namespace Fcp
{

struct Point
{
  int x;
  int y;

  Point() : x(0), y(0) {}
  Point(int _x, int _y) : x(_x), y(_y) {}
  ~Point(){}

  void Shift(int dx, int dy) { x += dx; y += dy;}
  void FlipX(int refX) { x = 2*refX-x;}
  void FlipY(int refY) { y = 2*refY-y;}
  int getX(void) {return x;}
  int getY(void) {return y;}
};

struct Rect
{
  Point ll; // Lower left corner
  Point ur; // Upper right corner

  Rect() : ll(0,0), ur(0,0) {}
  Rect(Point _ll, Point _ur) : ll(_ll), ur(_ur) { Legalize();}
  Rect(int _x1, int _y1, int _x2, int _y2) : ll(_x1,_y1), ur(_x2,_y2) { Legalize();}
  ~Rect(){}

  void Legalize() { if(ll.x>ur.x) { int xmin=ur.x; ur.x=ll.x; ll.x=xmin;}
                    if(ll.y>ur.y) { int ymin=ur.y; ur.y=ll.y; ll.y=ymin;} }
  void Shift(int dx, int dy) { ll.Shift(dx,dy); ur.Shift(dx,dy);}
  void FlipX(int refX) { ll.FlipX(refX); ur.FlipX(refX); Legalize();}
  void FlipY(int refY) { ll.FlipY(refY); ur.FlipY(refY); Legalize();}
  bool Enclose(int x, int y) { return (ll.x <= x && ll.y <= y && ur.x >= x && ur.y >= y);}
  bool Enclose(Point pt) { return Enclose(pt.x,pt.y);}
};

struct Circle
{
  Point center;
  int radius;

  Circle(Point _center, int _radius) : center(_center), radius(_radius) {}
  ~Circle(){}
};

struct Coords
{
  double x;
  double y;

  Coords() : x(0), y(0) {}
  Coords(double _x, double _y) : x(_x), y(_y) {}
  ~Coords(){}
  double getX(void) {return x;}
  double getY(void) {return y;}
};

struct Size
{
  int width; //Left upper corner
  int height; //Right down corner

  Size() : width(0), height(0) {}
  Size(int _width, int _height) : width(_width), height(_height) {}
  ~Size(){}
};

struct RGBColour
{
  unsigned char R, G, B;

  RGBColour(unsigned char red, unsigned char green, unsigned char blue)
   : R(red), G(green), B(blue) {}
  ~RGBColour() {}
};

} // end of namespace Fcp


// ME types (for use in FcpPkgReader)
//typedef Fcp::RGBColour ME_color;
struct ME_color
{
  unsigned char R;
  unsigned char G;
  unsigned char B;
};

struct  ME_point
{
  double x;
  double y;
};

struct ME_polyarc_segment {
  ME_point p1;
  ME_point p2;
  ME_point center; // center
  bool     is_arc; // false - line segment
                   // true  - arc

  void mul(double d) {
      p1.x  = p1.x * d;
      p1.y  = p1.y * d;
      p2.x  = p2.x * d;
      p2.y  = p2.y * d;
      center.x  = center.x * d;
      center.y  = center.y * d;
  }
  void div(double d) {
      p1.x  = p1.x / d;
      p1.y  = p1.y / d;
      p2.x  = p2.x / d;
      p2.y  = p2.y / d;
      center.x  = center.x / d;
      center.y  = center.y / d;
  }
  void set(ME_point the_p1, ME_point the_p2, ME_point the_center, bool the_is_arc) {
    p1 = the_p1; p2 = the_p2; center = the_center; is_arc = the_is_arc;
  }

  void set(double x1, double y1, double x2, double y2, double the_center_x, double the_center_y, bool the_is_arc) {

    p1.x = x1; p1.y = y1; p2.x = x2; p2.y = y2; center.x = the_center_x; center.y = the_center_y; is_arc = the_is_arc;
  }

};

// end of ME types 

namespace Fcp
{

enum ComponentType {
  UNKNOWN_COMPONENT = -1,
  PKG_COMPONENT = 0,
  FPT_COMPONENT,
  PST_COMPONENT
  // DIE_COMPONENT, // obsolete
  // SMD_COMPONENT  // obsolete
};

enum SignalDirectionType {
  SIGNAL_NO_DIR,
  SIGNAL_IN,
  SIGNAL_OUT,
  SIGNAL_INOUT
};

enum SignalLevelType {
  SIGNAL_UNDEF_LEVEL,
  SIGNAL_LOW_LEVEL,
  SIGNAL_HIGH_LEVEL,
  SIGNAL_LOW_HIGH_LEVEL
};

enum UnitsType {
  MEASURE_UNIT_METER,
  MEASURE_UNIT_DM,
  MEASURE_UNIT_CM,
  MEASURE_UNIT_MM,
  MEASURE_UNIT_UM,
  MEASURE_UNIT_NM,
  MEASURE_UNIT_TH,
  MEASURE_UNIT_MIL,
  MEASURE_UNIT_UD
};

enum ComponentSource {
  SOURCE_INTERNAL,
  SOURCE_EXTERNAL,
  SOURCE_NEW,
  SOURCE_CHANGED
};

enum Rotation {
  ROT_0,
  ROT_90,
  ROT_180,
  ROT_270,
  ROT_MX,
  ROT_MX90,
  ROT_MY,
  ROT_MY90
};

enum Justification {
  JUST_NO,
  JUST_LL,
  JUST_LC,
  JUST_LR,
  JUST_CL,
  JUST_CC,
  JUST_CR,
  JUST_UL,
  JUST_UC,
  JUST_UR
};

enum Side_t {
  SIDE_ANY,
  SIDE_LEFT,
  SIDE_BOTTOM,
  SIDE_RIGHT,
  SIDE_TOP
};

} // end of namespace Fcp


extern int DLLEXPORT calcManhattanDistance(Fcp::Point p1, Fcp::Point p2);


#endif
