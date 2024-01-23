/* Copyright (c) 2007, INEUM                                     */
/* This software is subject to copyright protection under        */
/* the laws of the Russian Federation and other countries.       */
/* All rights reserved.                                          */

#include "commonstruct.h"

int calcManhattanDistance(Fcp::Point p1, Fcp::Point p2)
{
  int dx = p1.x - p2.x;
  int dy = p1.y - p2.y;
  if (dx < 0) dx = -dx;
  if (dy < 0) dy = -dy;
  
  return (dx + dy);
}

