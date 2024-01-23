/*
 * Copyright (c) 2007, INEUM
 * This software is subject to copyright protection under
 * the laws of the Russian Federation and other countries.
 * All rights reserved.
 */
// Program: Flip Chip Packaging
// Module: Assignment Solution Manager
// Description: AmfData class definition
// Author: egorov_s@mcst.ru

#ifndef FcpAmfClassH
#define FcpAmfClassH

#include <vector>
#include <string>

#include "defs.h"
#include "commonstruct.h"
//#include "FcpCore/FcpMessenger.h"

namespace Fcp
{
//---------------------------------------------------------------------
class AmfComponent
{
public:
    AmfComponent(const char* componentId) :
      mName(componentId), mType("COMPONENT"), mFormat("DPF"), mRotation(0), mSide(0) {}

    std::string Name() const { return mName;}

    void SetType( const char* cType ) { mType.assign(cType);}
    void SetDescription( const char* cDescr ) { mDescription.assign(cDescr);}
    void SetFormat( const char* cFormat ) { mFormat.assign(cFormat);}
    void SetCore( Rect* pBB );
    void SetRefPoint( Point* pXY );
    void SetShift( Point* pXY );
    void SetRotation( int rotation ) { mRotation = rotation;}
    void SetSide( int side ) { mSide = side;}

    std::string Format() const { return mFormat;} // known formats:
        // "DPF" - Die Planner Format (DIE type components only)
        // "FPT" - FootPrinT format (DIE, C, R, ... type componets)
    std::string Type() const { return mType;}
    std::string Description() const { return mDescription;}
    Point RefPoint() const { return mRefPoint;}
    Rect Core() const { return mCore;}
    Point Shift() const { return mShift;}
    int Rotation() const { return mRotation;}
    int Side() const { return mSide;} // 0 - TOP, 1 - BOTTOM

    std::string mPropertyString;
private:
    std::string mName;
    std::string mType;
    std::string mDescription;
    std::string mFormat;
    Rect mCore;
    Point mRefPoint;
    Point mShift;
    int mRotation;
    int mSide;
};
//---------------------------------------------------------------------
class AmfGroup
{
public:
    AmfGroup(const char* groupName) : mName(groupName) {}

    std::string Name() const { return mName;}
    std::string mGround;
    std::string mPower;
    std::vector<std::string> mSignals;
    std::vector<int> mManual;
private:
    std::string mName;
};
//---------------------------------------------------------------------

//enum Itinerary_t
//{
//	UNDEFINED_ROUTE = 0,
//	FROM_DIE_TO_PACKAGE = 1,
//	FROM_PACKAGE_TO_DIE = 2
//};

enum PadDestination_t
{
    DEST_UNDEFINED = 0,
    DEST_4_LOGIC = 1,
    DEST_4_SUPPLY = 2,
    DEST_4_FREE = 3
};
//---------------------------------------------------------------------
class AmfBumpPad
{
public:
    AmfBumpPad( const char* compId, const char* handle, int ma = 0, PadDestination_t dest = DEST_UNDEFINED ):
        mManual(ma),
        mDestiny(dest),
        mComponentId(compId),
        mHandle(handle) {}

    std::string ComponentId() const { return mComponentId;}
    std::string Handle() const { return mHandle;}
    int mManual;
    PadDestination_t mDestiny;
    std::string mPropertyString;
private:
    std::string mComponentId;
    std::string mHandle;
};
//---------------------------------------------------------------------
class AmfBallPad
{
public:
    AmfBallPad(const char* name, int ma = 0, PadDestination_t dest = DEST_UNDEFINED ):
        mManual(ma),
        mDestiny(dest),
        mName(name) {}

    const char* Name() const { return mName.c_str();}
    int mManual;
    PadDestination_t mDestiny;
    std::string mPropertyString;
private:
    std::string mName;
};
//---------------------------------------------------------------------
class AmfArea
{
public:
    AmfArea(const char* areaName) : mName(areaName) {}

    std::string Name() const { return mName;}
    std::vector<std::string> mGroups;
    std::vector<AmfBumpPad> mBumps;
    std::vector<AmfBallPad> mBalls;
private:
    std::string mName;
};
//---------------------------------------------------------------------
class AmfAssign
{
public:
    AmfAssign( const char* sgn, const char* cmp, const char* bmp, const char* bll, long mnl, long hid):
        mSignal(sgn), mComponentId(cmp), mBump(bmp), mBall(bll), mManual(mnl), mHidden(hid) {}
    AmfAssign( const char* sgn, const char* cmp1, const char* bmp1, const char* cmp2, const char* bmp2, long mnl, long hid):
        mSignal(sgn), mComponentId(cmp1), mBump(bmp1), mBall(cmp2), mBmp2(bmp2), mManual(mnl), mHidden(hid)  {}
    AmfAssign( const char* sgn, const char* bll1, const char* bll2, long mnl, long hid):
        mSignal(sgn), mBall(bll1), mBmp2(bll2), mManual(mnl), mHidden(hid) {}

    std::string SignalName() const { return mSignal;}
    int IsManual() const { return mManual;}
    int Hidden() const { return mHidden;}
    bool Ball2Ball() const { return mComponentId.empty(); }
    bool Bump2Ball() const { return mBmp2.empty(); }
    // --- bump 2 ball
    std::string ComponentId() const { return mComponentId;}
    std::string BumpHandle() const { return mBump;}
    std::string BallName() const { return mBall;}
    // --- bump 2 bump
    std::string ComponentId2() const { return mBall;}
    std::string BumpHandle2() const { return mBmp2;}
    // --- ball 2 ball
    std::string BallName2() const { return mBmp2;}
private:
    //friend class AmfData;

    std::string mSignal;
    std::string mComponentId;
    std::string mBump;
    std::string mBall;
    std::string mBmp2;
    int mManual;
    int mHidden;
};
//---------------------------------------------------------------------
class AmfAssign4supply
{
public:
    AmfAssign4supply( const char* sgn, const char* cmp, const char* bmp, const char* arn, long mnl):
        mSignal(sgn), mComponentId(cmp), mBump(bmp), mArea(arn), mManual(mnl) {}
    AmfAssign4supply( const char* sgn, const char* bll, const char* arn, long mnl):
        mSignal(sgn), mBall(bll), mArea(arn), mManual(mnl) {}

    std::string SignalName() const { return mSignal;}
    void SetSignalName(const char* sgn) { if (sgn) mSignal.assign(sgn);}
    std::string AreaName() const { return mArea;}
    int IsManual() const { return mManual;}
    bool SupplyOnBall() const { return mComponentId.empty(); }
    std::string ComponentId() const { return mComponentId;}
    std::string BumpHandle() const { return mBump;}
    std::string BallName() const { return mBall;}
private:
    std::string mSignal;
    std::string mComponentId;
    std::string mBump;
    std::string mBall;
    std::string mArea;
    int mManual;
};
//---------------------------------------------------------------------
class AmfTrace
{
public:
    AmfTrace(const char* cmp, const char* bmp) {
        if (cmp)
            mComponentId.assign(cmp);
        if (bmp)
            mBump.assign(bmp);
    }
    void AddPoint(long layer, long x, long y, long width);
    std::string ComponentId() const { return mComponentId;}
    std::string BumpHandle() const { return mBump;}

    int CountPoints(void) const { return (int)mPoints.size();}
    int LayerOfPointAt(int i) const { return mLayers.at(i);}
    Fcp::Point CoordsOfPointAt(int i) const { return mPoints.at(i);}
    int WidthOfSegmentFromPointAt(int i) const { return mWidth.at(i);}
    void SetWidthOOfSegmentFromPointAt(int i, int wdt) { mWidth.at(i) = wdt;}
private:
    std::string mComponentId;
    std::string mBump;
    std::vector<int> mLayers;
    std::vector<Fcp::Point> mPoints;
    std::vector<int> mWidth; // segment  mPoint.at(i-1) - mPoint.at(i)  has mWitdh.at(i)
};
//---------------------------------------------------------------------
class AmfLayer
{
public:
    AmfLayer(const char* cmp, const char* bmp, int lid) {
        if (cmp)
            mComponentId.assign(cmp);
        if (bmp)
            mBump.assign(bmp);
        mLayer = lid;
    }
    std::string ComponentId() const { return mComponentId;}
    std::string BumpHandle()  const { return mBump;}
    int         LayerId()     const { return mLayer;}
private:
    std::string mComponentId;
    std::string mBump;
    int         mLayer;
};
//---------------------------------------------------------------------
class AmfData
{
public:
    AmfData();
    ~AmfData();
    AmfData( const AmfData& asfObj );

    void Clear();
    void Copy( const AmfData* pAMF );
//-----------------
    void SetAmfFileName( const char* asfFile ) { mAmfFileName = asfFile;}
    void SetVersion( long nVer ) { mVersion = (int) nVer;}

    void SetPackageDescr( const char* packageName, const char* packDescr );
    void SetPackageRefPoint( const char* packageName, Point* pXY );
    void SetPackageCore( const char* packageName, Rect* pBB );

    void SetComponentFormat( const char* componentId, const char* compFormat );
    void SetComponentType( const char* componentId, const char* compType );
    void SetComponentDescr( const char* componentId, const char* compDescr );
    void SetComponentPropertyString( const char* componentId, const char* compProp );
    void SetComponentRefPoint( const char* componentId, Point* pXY );
    void SetComponentCore( const char* componentId, Rect* pBB );
    void SetComponentPlacement( const char* componentId, Point* pXY, long degrees, long side );

    void SetItinerary( const char* route ) { mItinerary = route; }
    void SetAssignmentMode( const char* asmMode ) { mMode = asmMode; }

    void AddGroup(
        const char* groupName,
        const char*pwrName,
        const char*gndName );
    void AddGroupSignals(
        const char* groupName,
        long manual,
        std::vector<std::string>* pSigNames );

    void AddArea(
        const char* areaName,
        std::vector<std::string>* pGroupNames );
    void AddAreaBalls(
        const char* areaName,
        long manual,
        std::vector<std::string>* pBallNames,
        PadDestination_t dest = DEST_UNDEFINED );
    void AddAreaBumps(
        const char* areaName,
        long manual,
        const char* componentId,
        std::vector<std::string>* pBumpHandles,
        PadDestination_t dest = DEST_UNDEFINED );

    void AddAssignment(
        const char* sigName,
        const char* componentId,
        const char* bumpHandle,
        const char* ballName,
        long manual,
        long hidden );
    void AddAssignment(
        const char* sigName,
        const char* compId1,
        const char* bumpHandle1,
        const char* compId2,
        const char* bumpHandle2,
        long manual,
        long hidden );
    void AddAssignment(
        const char* sigName,
        const char* ballName1,
        const char* ballName2,
        long manual,
        long hidden );

    void AddAssignment4supply(
        const char* sigName,
        const char* areaName,
        const char* componentId,
        const char* bumpHandle,
        long manual );
    void AddAssignment4supply(
        const char* sigName,
        const char* areaName,
        const char* ballName,
        long manual );

    AmfTrace* AddTrace(
        const char* componentId,
        const char* bumpHandle );

    AmfLayer* AddLayer(
        const char* componentId,
        const char* bumpHandle,
              int   layerId );

//-----------------
    std::string GetAmfFileName() const { return mAmfFileName;}
    int GetVersion() { return (int)mVersion;}

    std::vector<AmfComponent>* GetComponents() { return &mComponents;}
    AmfComponent* GetComponent( const char* componentId );
    void RemoveComponent( const char* componentId );

    void FindAssignments4pin( const char* componentId, const char* pinId,
        std::vector<int>* pIdx4ass, std::vector<int>* pIdx4sup );
    void FindAssignments4net( const char* signalName,
        std::vector<int>* pIdx4ass, std::vector<int>* pIdx4sup );
    void RemoveAssignments( std::vector<int>* pIdx4ass, std::vector<int>* pIdx4sup );

    int CountAssignments() { return (int)mAssignments.size();}
    AmfAssign* GetAssignment(int ia) {
        return (ia < 0 || ia >= CountAssignments())? 0 : &mAssignments[ia];
    }
    std::vector<AmfAssign>* GetAssignments() { return &mAssignments;}

    int CountAssignments4supply() { return (int)mAssignments4supply.size();}
    AmfAssign4supply* GetAssignment4supply(int ia) {
        return (ia < 0 || ia >= CountAssignments4supply())? 0 : &mAssignments4supply[ia];
    }
    std::vector<AmfAssign4supply>* GetAssignments4supply() { return &mAssignments4supply;}

    std::string GetPackageName() const { return mPackageName;}
    std::string GetPackageDescr() const { return mPackageDescr;}
    std::string GetPackageType() const { return std::string("BGA");}
    std::string GetPackageFormat() const { return std::string("PKG");}
    Point GetPackageRefPoint() const { return mPackageRefPoint;}
    Rect GetPackageCore() const { return mPackageCore;}

    std::string GetItenerary() { return mItinerary; }
    std::string GetAssignmentMode() { return mMode; }

    std::vector<AmfGroup>* GetSignalGroups() { return &mGroups;}
    std::vector<AmfArea>* GetAreas() { return &mAreas;}

    bool IsPower( std::string signal );
    bool IsGround( std::string signal );
    bool IsSupply( std::string signal ) { return (IsPower(signal) || IsGround(signal));}

    std::string GetCoreGroundSignal();
    std::string GetCorePowerSignal();
    std::vector<std::string>* GetPowerSignals() { return &mPowerSignals;}
    std::vector<std::string>* GetGroundSignals() { return &mGroundSignals;}

    std::vector<AmfBallPad>* GetBalls4supply(
        const char* sigName,
        const char* areaName = 0);
    std::vector<AmfBumpPad>* GetBumps4supply(
        const char* sigName,
        const char* areaName = 0);

    std::vector<AmfTrace>* GetTraces(
        const char* compId = 0,
        const char* bumpHandle = 0);

    std::vector<AmfLayer>* GetLayers(
        const char* compId = 0,
        const char* bumpHandle = 0);

//-----------------
    int Write( const char* fileName = 0 ) const;
//-----------------
private:
    std::string mAmfFileName;
    int mVersion;

    std::vector<AmfComponent> mComponents;
    int IndexOfComponent( const char* componentId, bool add = true );

    std::string mPackageDescr;
    std::string mPackageName;
    Point mPackageRefPoint;
    Rect mPackageCore;

    std::string mItinerary;
    std::string mMode;

    std::string mCoreGround;
    std::string mCorePower;
    std::vector<std::string> mPowerSignals;
    std::vector<std::string> mGroundSignals;

    std::vector<AmfGroup> mGroups;
    std::vector<AmfArea> mAreas;
    std::vector<AmfAssign> mAssignments;
    std::vector<AmfAssign4supply> mAssignments4supply;
    std::vector<AmfTrace> mTraces;
    std::vector<AmfLayer> mLayers;

    std::vector<AmfBallPad>* m_pBalls4supply;
    std::vector<AmfBumpPad>* m_pBumps4supply;
    std::vector<AmfTrace>* m_pTraces;
    std::vector<AmfLayer>* m_pLayers;

    void WriteGroups( FILE *pAmfFile ) const;
    void WriteAreas( FILE *pAmfFile ) const;
    void WriteAssignments( FILE *pAmfFile ) const;
    void WriteAssignments4supply( FILE *pAmfFile ) const;
    void WriteTraces( FILE *pAmfFile ) const;
    void WriteLayers( FILE *pAmfFile ) const;
};
//---------------------------------------------------------------------
} // namespace Fcp
#endif
