///////////////////////////////////
//  HEADER NAME:    FcpPkgReader.h
//  ABSTRACT   :    Reader of package, component or padstack description
//                  in PKG, FPT and PST formats respectively
//  CREATED BY :    Sergey Katserov
//  CREATION DATE:  01.09.2007
//  REMARKS    :    This header declares callback API for the reader  
///////////////////////////////////                                            
// Copyright (c) 2007, INEUM                                     
// This software is subject to copyright protection under        
// the laws of the Russian Federation and other countries.       
// All rights reserved.                                          
///////////////////////////////////                                             

#ifndef FCP_PKGREADER_H
#define FCP_PKGREADER_H

//     Include Headers       
///////////////////////////////////
#include "defs.h"
#include "commonstruct.h"
#include <vector>
#include <string>

// ----------------- parser callback interface ------------------- //

using Fcp::ComponentType;

///////////////////////////////////////////////////
//  CLASS_NAME:    PKGReaderCallbackI
//  DESCRIPTION :  Callback API for the reader of PKG, FPT and PST formats
///////////////////////////////////////////////////

class PKGReaderCallbackI {
 public:
  enum PadstackSearchPolicy {
    IN_FILE_FIRST,         // first get padstack from the same file;
                           // if absent then search the padstack in search
                           // path.
    IN_SEARCH_PATH_FIRST,  // first search padstack in search path;
                           // if absent then get padstack from the same file.
    NO_SEARCH,             // act precisely as now: all padstacks that are 
                           // present in the file are read and no search is
                           // done.
    DEFAULT_POLICY         // actual policy is determined internally
                           // (i.e using boo-file)
  };
  
  virtual ~PKGReaderCallbackI() {}
  virtual void PkgTypeCallback(int /*pkg_type*/) {} // pkg_title && die_title   (beginning of package found)
  virtual void PrecisionCallback(int /*precision*/) {} // me_units_tail
  virtual void UnitsCallback(int /*units*/, double /*um_in_unit*/) {} // me_unit_type
  virtual void PkgNameCallback(std::string /*pkg_name*/) {} // pkg_name_stmt
  virtual void PartNumberCallback(std::string /*part_number*/) {} // part_number_stmt;
  virtual void HorizontalPitchCallback(double /*pitch*/) {} // pitch_x_stmt
  virtual void VerticalPitchCallback(double /*pitch*/) {} // pitch_y_stmt
  virtual void SxCallback(double /*sx*/) {} // sx_stmt
  virtual void SyCallback(double /*sy*/) {} // sy_stmt
  virtual void ExtentsCallback(ME_point /*p1*/, ME_point /*p2*/) {} // extents_stmt
  //void PadstackCallback() = 0; // padstack_desc
  
  virtual void PadstackNameCallback(std::string /*name*/, bool /*is_internal*/) {}  // padstack_init   (beginning of padstack found)
  virtual void PadstackElementLayer(std::string /*layer*/) {} // for explicit layer definition
  virtual void PadstackElementEmptyLayer() {} // for implicit layer definition

  virtual void PadstackElementCircleCallback(bool /*cut*/,
					     ME_color /*line_color*/,
					     double /*line_width*/,
					     ME_color /*filling_color*/,
					     bool /*fill*/,
					     ME_point /*position*/,
					     double /*radius*/
					     ) {} // circle_descr_body
  virtual void PadstackElemBumpCircleCallback(bool /*cut*/,
					     ME_color /*line_color*/,
					     double /*line_width*/,
					     ME_color /*filling_color*/,
					     bool /*fill*/,
					     ME_point /*position*/,
					     double /*radius*/
					     ) {} // circle_descr_body for die bump shape
  
  virtual void PadstackElementRectangleCallback(bool /*cut*/,
						ME_color /*line_color*/,
						double /*line_width*/,
						ME_color /*filling_color*/,
						bool /*fill*/,
						ME_point /*position*/,
						ME_point /*p1*/,
						ME_point /*p2*/
						) {} // rectangle_descr_body
  virtual void PadstackElemBumpRectangleCallback(bool /*cut*/,
						ME_color /*line_color*/,
						double /*line_width*/,
						ME_color /*filling_color*/,
						bool /*fill*/,
						ME_point /*position*/,
						ME_point /*p1*/,
						ME_point /*p2*/
						) {} // rectangle_descr_body for die bump shape
  
  virtual void PadstackElementPolyCallback(bool /*cut*/,
					   ME_color /*line_color*/,
					   double /*line_width*/,
					   ME_color /*filling_color*/,
					   bool /*fill*/,
					   ME_point /*position*/,
					   std::vector<ME_point>* /*points*/    // vector is returned to user
					   ) {} // polygon_descr_body 
  virtual void PadstackElemBumpPolyCallback(bool /*cut*/,
					   ME_color /*line_color*/,
					   double /*line_width*/,
					   ME_color /*filling_color*/,
					   bool /*fill*/,
					   ME_point /*position*/,
					   std::vector<ME_point>* /*points*/    // vector is returned to user
					   ) {} // polygon_descr_body for die bump shape
  
  virtual void PadstackElementPolyArcCallback(bool /*cut*/,
					      ME_color /*line_color*/,
					      double /*line_width*/,
					      ME_color /*filling_color*/,
					      bool /*fill*/,
					      ME_point /*position*/,
					      std::vector<ME_polyarc_segment>* /*segments*/    // vector is returned to user
					      ) {} // polyarc_descr_body 
  virtual void PadstackElemBumpPolyArcCallback(bool /*cut*/,
					      ME_color /*line_color*/,
					      double /*line_width*/,
					      ME_color /*filling_color*/,
					      bool /*fill*/,
					      ME_point /*position*/,
					      std::vector<ME_polyarc_segment>* /*segments*/    // vector is returned to user
					      ) {} // polyarc_descr_body for die bump shape
  
  virtual void PadstackElementRegPolyCallback(bool /*cut*/,
					      ME_color /*line_color*/,
					      double /*line_width*/,
					      ME_color /*filling_color*/,
					      bool /*fill*/,
					      ME_point /*position*/,
					      double /*radius*/,
					      int /*vertex_count*/,              // vertexesTotal
					      double /*rotation*/,               // angle
					      bool /*InsertedCircle*/,           // smallRadius
					      bool /*RotationToEdgeNormalFlag*/ // angleToSmallRadius
					      ) {} // regpoly_descr_body
  virtual void PadstackElemBumpRegPolyCallback(bool /*cut*/,
					      ME_color /*line_color*/,
					      double /*line_width*/,
					      ME_color /*filling_color*/,
					      bool /*fill*/,
					      ME_point /*position*/,
					      double /*radius*/,
					      int /*vertex_count*/,              // vertexesTotal
					      double /*rotation*/,               // angle
					      bool /*InsertedCircle*/,           // smallRadius
					      bool /*RotationToEdgeNormalFlag*/ // angleToSmallRadius
					      ) {} // regpoly_descr_body for die bump shape
  
  virtual void PadstackEndCallback() {} // (padstack_desc) end of padstack found
  virtual void PackageOutlineBeginCallback() {} // (outline_desc) beginning of outline found
  virtual void PackageOutlineEndCallback() {} // (outline_desc) end of outline found
  virtual void PackageEndCallback() {}  // (desc) end of package  found
  
  virtual void PinCallback(std::string /*pin_name*/,
					      int /*pin_direction*/,
					      std::string /*padstack_name*/,
					      const char* /*signal_name*/,
					      ME_point /*pos*/,
					      ME_color* /*color*/,
					      const char* /*group_name*/,
					      const char* /*master_cell_inst_name*/,
					      const char* /*master_cell_name*/
					      ) {} // pin_descr
  
  virtual void PinFunctionCallback(const char* /*signal_name*/,
					      const char* /*sig_active_level*/,
					      int /*pin_direction*/,
					      const char* /*sig_function*/
					      ) {} // pin_function_descr
  
  virtual int ReadPKG(const char* /*pkgfile*/,
	      const char* /*pkg_module_name*/,
	      double /*the_um_in_user_unit*/,
	      PadstackSearchPolicy padstack_search_policy = DEFAULT_POLICY);
  virtual const char *getPkgParseProblems();
  static ComponentType fileType(std::string /*file_name*/);
  static std::string GetUnitString(int /*unit*/);

protected:
  virtual PKGReaderCallbackI& operator=(const PKGReaderCallbackI &) { return *this; }

}; // end of PKGReaderCallbackI



#endif
