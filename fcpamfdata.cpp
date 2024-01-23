/*
 * Copyright (c) 2007, INEUM
 * This software is subject to copyright protection under
 * the laws of the Russian Federation and other countries.
 * All rights reserved.
 */
// Program: Flip Chip Packaging
// Module: Assignment Solution Manager
// Description: AmfData class implementation
// Author: egorov_s@mcst.ru

#include <assert.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "fcpamfdata.h"

namespace Fcp
{

void AmfTrace::AddPoint(long layer, long x, long y, long width)
{
    int iLast =(int)mPoints.size()-1;
    if (iLast >= 0 &&
        mLayers[iLast] == (int)layer &&
        mPoints[iLast].x == (int)x && mPoints[iLast].y == (int)y) {
            return;
    }
    mLayers.push_back((int)layer);
    mPoints.push_back(Fcp::Point((int)x,(int)y));
    mWidth.push_back((int)width);
}

AmfData::AmfData()
{
    mVersion = 1;

    mPackageRefPoint.x = 0;
    mPackageRefPoint.y = 0;

    mPackageCore.ll.x = 0;
    mPackageCore.ll.y = 0;
    mPackageCore.ur.x = 0;
    mPackageCore.ur.y = 0;

    mItinerary = "UNDEFINED_ROUTE";
    mMode = "Free";

    m_pBalls4supply = 0;
    m_pBumps4supply = 0;
    m_pTraces = 0;
    m_pLayers = 0;
}

AmfData::AmfData( const AmfData& amfObj )
{
    Copy( &amfObj );
}

AmfData::~AmfData()
{
    Clear();
}

void AmfData::Clear()
{
    mPackageName.clear();
    mComponents.clear();
    mGroups.clear();
    mAreas.clear();
    mCoreGround.clear();
    mCorePower.clear();
    mPowerSignals.clear();
    mGroundSignals.clear();
    mAssignments.clear();
    mAssignments4supply.clear();
    mTraces.clear();
    mLayers.clear();
    if (m_pBalls4supply != 0) {
        m_pBalls4supply->clear();
        DELETE(m_pBalls4supply);
    }
    if (m_pBumps4supply != 0) {
        m_pBumps4supply->clear();
        DELETE(m_pBumps4supply);
    }
    if (m_pTraces != 0) {
        m_pTraces->clear();
        DELETE(m_pTraces);
    }
    if (m_pLayers != 0) {
        m_pLayers->clear();
        DELETE(m_pLayers);
    }
}

void AmfData::Copy( const AmfData* pAMF )
{
    Clear();
    if (pAMF == 0) {
        return;
    }
    mAmfFileName = pAMF->mAmfFileName;
    mVersion = pAMF->mVersion;

    mComponents = pAMF->mComponents;

    mPackageDescr = pAMF->mPackageDescr;
    mPackageName = pAMF->mPackageName;
    mPackageRefPoint = pAMF->mPackageRefPoint;
    mPackageCore = pAMF->mPackageCore;

    mItinerary = pAMF->mItinerary;
    mMode = pAMF->mMode;

    mCoreGround = pAMF->mCoreGround;
    mCorePower = pAMF->mCorePower;
    mPowerSignals = pAMF->mPowerSignals;
    mGroundSignals = pAMF->mGroundSignals;

    mGroups = pAMF->mGroups;
    mAreas = pAMF->mAreas;
    mAssignments = pAMF->mAssignments;
    mAssignments4supply = pAMF->mAssignments4supply;
    mTraces = pAMF->mTraces;
    mLayers = pAMF->mLayers;
}

void AmfComponent::SetRefPoint( Point* pXY )
{
    mRefPoint.x = (pXY)?(pXY->x):0;
    mRefPoint.y = (pXY)?(pXY->y):0;
}

void AmfComponent::SetCore( Rect* pBB )
{
    mCore.ll.x = (pBB)?((pBB->ll).x):0;
    mCore.ll.y = (pBB)?((pBB->ll).y):0;
    mCore.ur.x = (pBB)?((pBB->ur).x):0;
    mCore.ur.y = (pBB)?((pBB->ur).y):0;
}

void AmfComponent::SetShift( Point* pXY )
{
    mShift.x = (pXY)?(pXY->x):0;
    mShift.y = (pXY)?(pXY->y):0;
}

void AmfData::SetComponentType( const char* componentId, const char* compType )
{
    int ic = IndexOfComponent(componentId,true);
    if (ic < 0)
        return;
    mComponents[ic].SetType(compType);
}

void AmfData::SetComponentDescr( const char* componentId, const char* compDescr )
{
    int ic = IndexOfComponent(componentId,true);
    if (ic < 0)
        return;
    mComponents[ic].SetDescription(compDescr);
}

void AmfData::SetComponentFormat( const char* componentId, const char* compFormat )
{
    int ic = IndexOfComponent(componentId,true);
    if (ic < 0)
        return;
    mComponents[ic].SetFormat(compFormat);
}

void AmfData::SetComponentRefPoint( const char* componentId, Point* pXY )
{
    int ic = IndexOfComponent(componentId,true);
    if (ic < 0)
        return;
    mComponents[ic].SetRefPoint(pXY);
}

void AmfData::SetComponentCore( const char* componentId, Rect* pBB )
{
    int ic = IndexOfComponent(componentId,true);
    if (ic < 0)
        return;
    mComponents[ic].SetCore(pBB);
}

void AmfData::SetComponentPlacement( const char* componentId, Point* pXY, long degrees, long side )
{
    int ic = IndexOfComponent(componentId,true);
    mComponents[ic].SetShift(pXY);
    mComponents[ic].SetRotation(degrees);
    mComponents[ic].SetSide(side);
}

int AmfData::IndexOfComponent( const char* componentId, bool add )
{
    uint id = 0;
    for (id = 0 ; id < mComponents.size(); id++)
    {
        if (mComponents[id].Name() == std::string(componentId))
            return int(id);
    }
    if (!add)
        return -1;
    AmfComponent newDie(componentId);
    mComponents.push_back(newDie);
    return int(id);
}

void AmfData::SetPackageDescr( const char* packageName, const char* packDescr )
{
    if (mPackageName.empty())
        mPackageName = packageName;
    else
        assert(mPackageName == std::string(packageName));
    mPackageDescr = packDescr;
}

void AmfData::SetPackageRefPoint( const char* packageName, Point* pXY )
{
    if (mPackageName.empty())
        mPackageName = packageName;
    else
        assert(mPackageName == std::string(packageName));
    mPackageRefPoint.x = (pXY)?(pXY->x):0;
    mPackageRefPoint.y = (pXY)?(pXY->y):0;
}

void AmfData::SetPackageCore( const char* packageName, Rect* pBB )
{
    if (mPackageName.empty())
        mPackageName = packageName;
    else
        assert(mPackageName == std::string(packageName));
    mPackageCore.ll.x = (pBB)?((pBB->ll).x):0;
    mPackageCore.ll.y = (pBB)?((pBB->ll).y):0;
    mPackageCore.ur.x = (pBB)?((pBB->ur).x):0;
    mPackageCore.ur.y = (pBB)?((pBB->ur).y):0;
}

void AmfData::AddGroup(	const char* groupName,
                        const char* pwrName,
                        const char* gndName )
{
    uint igrp = 0;
    for (igrp = 0; igrp < mGroups.size(); igrp++)
        if (mGroups[igrp].Name() == std::string(groupName))
            break;
    if (igrp == mGroups.size())
        mGroups.push_back(groupName);
    mGroups[igrp].mPower.assign(pwrName);
    mGroups[igrp].mGround.assign(gndName);
    uint ipwr = 0;
    for (ipwr = 0; ipwr < mPowerSignals.size(); ipwr++)
        if (mPowerSignals[ipwr] == mGroups[igrp].mPower)
            break;
    if (ipwr == mPowerSignals.size())
        mPowerSignals.push_back(pwrName);
    uint ignd = 0;
    for (ignd = 0; ignd < mGroundSignals.size(); ignd++)
        if (mGroundSignals[ignd] == mGroups[igrp].mGround)
            break;
    if (ignd == mGroundSignals.size())
        mGroundSignals.push_back(gndName);
}

AmfComponent* AmfData::GetComponent( const char* componentId )
{
    int ic = IndexOfComponent(componentId,false);
    if (ic < 0)
        return 0;
    return &(mComponents[ic]);
}

void AmfData::RemoveComponent( const char* componentId )
{
    int ic = IndexOfComponent(componentId,false);
    if (ic < 0)
        return;
    mComponents.erase(mComponents.begin()+ic);
    std::vector<int> vIdx4ass, vIdx4sup;
    FindAssignments4pin( componentId, 0, &vIdx4ass, &vIdx4sup );
    RemoveAssignments( &vIdx4ass, &vIdx4sup );
}

void AmfData::FindAssignments4pin(const char* componentId,
                                  const char* pinId,
                                  std::vector<int>* pIdx4ass,
                                  std::vector<int>* pIdx4sup)
{
    if (pIdx4ass != 0) {
        pIdx4ass->clear();
    }
    if (pIdx4sup != 0) {
        pIdx4sup->clear();
    }
    std::string sCompId, sBumpId, sBallId;
    bool remAssOnBall = false, remAssOnBump = false;
    if (componentId == 0) {
        if (pinId == 0) {
            return;
        }
        sBallId.assign(pinId);
        remAssOnBall = true;
    } else {
        sCompId.assign(componentId);
        if (pinId != 0) {
            sBumpId.assign(pinId);
            remAssOnBump = true;
        }
    }
    if (pIdx4ass != 0) {
        int cAssgns = CountAssignments();
        for (int ia = 0; ia < cAssgns; ia++)
        {
            if (remAssOnBall) {
                if (mAssignments[ia].Bump2Ball() &&
                    mAssignments[ia].BallName() == sBallId) {
                    pIdx4ass->push_back(ia);
                    break; // there should be just one connection to ball
                }
            } else if (remAssOnBump) {
                if (mAssignments[ia].BumpHandle() == sBumpId &&
                    mAssignments[ia].ComponentId() == sCompId) {
                    pIdx4ass->push_back(ia);
                } else
                if (!mAssignments[ia].Bump2Ball() &&
                    mAssignments[ia].BumpHandle2() == sBumpId &&
                    mAssignments[ia].ComponentId2() == sCompId) {
                    pIdx4ass->push_back(ia);
                }
            } else {
                if (mAssignments[ia].ComponentId() == sCompId) {
                    pIdx4ass->push_back(ia);
                } else
                if (!mAssignments[ia].Bump2Ball() &&
                    mAssignments[ia].ComponentId2() == sCompId) {
                    pIdx4ass->push_back(ia);
                }
            }
        }
    }
    if (pIdx4sup != 0) {
        int cAs4sup = CountAssignments4supply();
        for (int ia = cAs4sup-1; ia >= 0; ia--)
        {
            if (remAssOnBall) {
                if (mAssignments4supply[ia].SupplyOnBall() &&
                    mAssignments4supply[ia].BallName() == sBallId) {
                    pIdx4sup->push_back(ia);
                    break; // there should be just one supply assignment on ball
                }
            } else if (remAssOnBump) {
                if (!mAssignments4supply[ia].SupplyOnBall() &&
                    mAssignments4supply[ia].BumpHandle() == sBumpId &&
                    mAssignments4supply[ia].ComponentId() == sCompId) {
                    pIdx4sup->push_back(ia);
                    break; // there should be just one supply assignment to bump
                }
            } else {
                if (!mAssignments4supply[ia].SupplyOnBall() &&
                    mAssignments4supply[ia].ComponentId() == sCompId) {
                    pIdx4sup->push_back(ia);
                }
            }
        }
    }
}

void AmfData::FindAssignments4net(const char* signalName,
                                  std::vector<int>* pIdx4ass,
                                  std::vector<int>* pIdx4sup)
{
    if (pIdx4ass != 0) {
        pIdx4ass->clear();
    }
    if (pIdx4sup != 0) {
        pIdx4sup->clear();
    }
    if (signalName == 0) {
            return;
    }
    std::string sSignal(signalName);
    if (pIdx4ass != 0) {
        int cAssgns = CountAssignments();
        for (int ia = 0; ia < cAssgns; ia++)
        {
            if (mAssignments[ia].SignalName() == sSignal) {
                pIdx4ass->push_back(ia);
            }
        }
    }
    if (pIdx4sup != 0) {
        int cAs4sup = CountAssignments4supply();
        for (int ia = cAs4sup-1; ia >= 0; ia--)
        {
            if (mAssignments4supply[ia].SignalName() == sSignal) {
                pIdx4sup->push_back(ia);
            }
        }
    }
}

void AmfData::RemoveAssignments( std::vector<int>* pIdx4ass, std::vector<int>* pIdx4sup )
{
    if (pIdx4ass != 0 ) {
        int cIdx =(int)pIdx4ass->size();
        for (int ii = cIdx-1; ii >= 0; ii--) {
            mAssignments.erase(mAssignments.begin()+pIdx4ass->at(ii));
        }
    }
    if (pIdx4sup != 0 ) {
        int cIdx =(int)pIdx4sup->size();
        for (int ii = cIdx-1; ii >= 0; ii--) {
            mAssignments4supply.erase(mAssignments4supply.begin()+pIdx4sup->at(ii));
        }
    }
}

std::string AmfData::GetCoreGroundSignal()
{
    uint ia = 0;
    for (ia = 0; ia < mAreas.size(); ia++)
        if (mAreas[ia].Name() == std::string("Core"))
            break;
    if (ia == mAreas.size())
        return "";
    if (mAreas[ia].mGroups.size() <= 0)
        return "";
    uint igrp = 0;
    for (igrp = 0; igrp < mGroups.size(); igrp++)
        if (mGroups[igrp].Name() == mAreas[ia].mGroups[0])
            break;
    if (igrp == mGroups.size())
        return "";
    return mGroups[igrp].mGround;
}

std::string AmfData::GetCorePowerSignal()
{
    uint ia = 0;
    for (ia = 0; ia < mAreas.size(); ia++)
        if (mAreas[ia].Name() == std::string("Core"))
            break;
    if (ia == mAreas.size())
        return "";
    if (mAreas[ia].mGroups.size() <= 0)
        return "";
    uint igrp = 0;
    for (igrp = 0; igrp < mGroups.size(); igrp++)
        if (mGroups[igrp].Name() == mAreas[ia].mGroups[0])
            break;
    if (igrp == mGroups.size())
        return "";
    return mGroups[igrp].mPower;
}

bool AmfData::IsPower( std::string signal )
{
    int cSig =(int)mPowerSignals.size();
    for (int is = 0; is < cSig; is++) {
        if (mPowerSignals[is] == signal) {
            return true;
        }
    }
    //return (signal == GetCorePowerSignal());
    return false;
}

bool AmfData::IsGround( std::string signal )
{
    int cSig =(int)mGroundSignals.size();
    for (int is = 0; is < cSig; is++) {
        if (mGroundSignals[is] == signal) {
            return true;
        }
    }
    //return (signal == GetCoreGroundSignal());
    return false;
}

void AmfData::AddGroupSignals( const char* groupName,
                               long manual,
                               std::vector<std::string>* pSigNames )
{
    uint ig = 0;
    for (ig = 0; ig < mGroups.size(); ig++)
        if ( mGroups[ig].Name() == std::string(groupName) )
            break;
    if (ig == mGroups.size())
        mGroups.push_back(groupName);
    for(uint is = 0; is < pSigNames->size(); is++)
    {
        mGroups[ig].mSignals.push_back( pSigNames->at(is) );
        mGroups[ig].mManual.push_back( int(manual) );
    }
}

void AmfData::AddArea( const char* areaName,
                       std::vector<std::string>* pGroupNames )
{
    if (pGroupNames == 0)
        return;
    uint ia = 0;
    for (ia = 0; ia < mAreas.size(); ia++)
        if ( mAreas[ia].Name() == std::string(areaName) )
            break;
    if (ia == mAreas.size())
        mAreas.push_back(areaName);
    for(uint ig = 0; ig < pGroupNames->size(); ig++)
    {
        mAreas[ia].mGroups.push_back( pGroupNames->at(ig) );
    }
}

void AmfData::AddAreaBumps( const char* areaName,
                            long manual,
                            const char* componentId,
                            std::vector<std::string>* pBumpHandles,
                            PadDestination_t dest )
{
    if (pBumpHandles == 0)
        return;
    uint ia = 0;
    for (ia = 0; ia < mAreas.size(); ia++)
        if ( mAreas[ia].Name() == std::string(areaName) )
            break;
    if (ia == mAreas.size())
        mAreas.push_back(areaName);
    for(uint ib = 0; ib < pBumpHandles->size(); ib++)
    {
        AmfBumpPad tmpPad( componentId, pBumpHandles->at(ib).c_str(), (int)manual, dest );
        mAreas[ia].mBumps.push_back( tmpPad );
    }
}

void AmfData::AddAreaBalls(	const char* areaName,
                            long manual,
                            std::vector<std::string>* pBallNames,
                            PadDestination_t dest )
{
    if (pBallNames == 0)
        return;
    uint ia = 0;
    for (ia = 0; ia < mAreas.size(); ia++)
        if ( mAreas[ia].Name() == std::string(areaName) )
            break;
    if (ia == mAreas.size())
        mAreas.push_back(areaName);
    for(uint ib = 0; ib < pBallNames->size(); ib++)
    {
        AmfBallPad tmpPad( pBallNames->at(ib).c_str(), manual, dest );
        mAreas[ia].mBalls.push_back( tmpPad );
    }
}

void AmfData::AddAssignment( const char* sigName,
                             const char* componentId,
                             const char* bumpHandle,
                             const char* ballName,
                             long manual,
                             long hidden )
{
    AmfAssign assignment( sigName, componentId, bumpHandle, ballName, manual, hidden );
    mAssignments.push_back(assignment);
}

void AmfData::AddAssignment( const char* sigName,
                             const char* compId1,
                             const char* bumpHandle1,
                             const char* compId2,
                             const char* bumpHandle2,
                             long manual,
                             long hidden )
{
    AmfAssign assignment( sigName, compId1, bumpHandle1, compId2, bumpHandle2, manual, hidden );
    mAssignments.push_back(assignment);
}

void AmfData::AddAssignment( const char* sigName,
                             const char* ballName1,
                             const char* ballName2,
                             long manual,
                             long hidden )
{
    AmfAssign assignment( sigName, ballName1, ballName2, manual, hidden );
    mAssignments.push_back(assignment);
}

void AmfData::AddAssignment4supply( const char* sigName,
                                    const char* areaName,
                                    const char* componentId,
                                    const char* bumpHandle,
                                    long manual )
{
    AmfAssign4supply assignment( sigName, componentId, bumpHandle, areaName, manual );
    mAssignments4supply.push_back(assignment);
}

void AmfData::AddAssignment4supply( const char* sigName,
                                    const char* areaName,
                                    const char* ballName,
                                    long manual )
{
    AmfAssign4supply assignment( sigName, ballName, areaName, manual );
    mAssignments4supply.push_back(assignment);
}

std::vector<AmfBallPad>* AmfData::GetBalls4supply( const char* sigName,
                                                   const char* areaName )
{
    DELETE(m_pBalls4supply);
    m_pBalls4supply = new std::vector<AmfBallPad>;
    if (mAssignments4supply.empty())
        return m_pBalls4supply;
    std::string supplyName =(sigName  == 0)?"": sigName;
    std::string supplyArea =(areaName == 0)?"":areaName;
    for (uint ia = 0; ia < mAssignments4supply.size(); ia++)
    {
        if (!mAssignments4supply[ia].SupplyOnBall())
            continue;
        if (sigName != 0)
            if (mAssignments4supply[ia].SignalName() != supplyName)
                continue;
        if (areaName != 0)
            if (mAssignments4supply[ia].AreaName() != supplyArea)
                continue;
        AmfBallPad pad(
            mAssignments4supply[ia].BallName().c_str(),
            mAssignments4supply[ia].IsManual(),
            DEST_4_SUPPLY );
        m_pBalls4supply->push_back(pad);
    }
    return m_pBalls4supply;
}

std::vector<AmfBumpPad>* AmfData::GetBumps4supply( const char* sigName,
                                                   const char* areaName )
{
    DELETE(m_pBumps4supply);
    m_pBumps4supply = new std::vector<AmfBumpPad>;
    if (mAssignments4supply.empty())
        return m_pBumps4supply;
    std::string supplyName =(sigName  == 0)?"": sigName;
    std::string supplyArea =(areaName == 0)?"":areaName;
    for (uint ia = 0; ia < mAssignments4supply.size(); ia++)
    {
        if (mAssignments4supply[ia].SupplyOnBall()) {
            continue;
        }
        if (sigName != 0) {
            if (mAssignments4supply[ia].SignalName() != supplyName)
                continue;
        }
        if (areaName != 0) {
            if (mAssignments4supply[ia].AreaName() != supplyArea)
                continue;
        }
        AmfBumpPad pad(
            mAssignments4supply[ia].ComponentId().c_str(),
            mAssignments4supply[ia].BumpHandle().c_str(),
            mAssignments4supply[ia].IsManual(),
            DEST_4_SUPPLY );
        m_pBumps4supply->push_back(pad);
    }
    return m_pBumps4supply;
}

AmfTrace* AmfData::AddTrace(const char* compId, const char* bumpHandle)
{
    AmfTrace trc(compId,bumpHandle);
    mTraces.push_back(trc);
    int cTrcs =(int)mTraces.size();
    return &(mTraces[cTrcs-1]);
}

std::vector<AmfTrace>* AmfData::GetTraces(const char* compId,
                                          const char* bumpHandle)
{
    DELETE(m_pTraces);
    m_pTraces = new std::vector<AmfTrace>;
    if (mTraces.empty())
        return m_pTraces;
    std::string compName =(compId  == 0)?"": compId;
    std::string bumpName =(bumpHandle == 0)?"":bumpHandle;
    int cTrcs =(int)mTraces.size(), cPts = 0;
    for (int iTrc = 0; iTrc < cTrcs; iTrc++)
    {
        if (compId != 0) {
            if (mTraces[iTrc].ComponentId() != compName)
                continue;
        }
        if (bumpHandle != 0) {
            if (mTraces[iTrc].BumpHandle() != bumpName)
                continue;
        }
        AmfTrace trc(
            mTraces[iTrc].ComponentId().c_str(),
            mTraces[iTrc].BumpHandle().c_str());
        cPts = mTraces[iTrc].CountPoints();
        for (int iPt = 0; iPt < cPts; iPt++)
        {
            trc.AddPoint(
                mTraces[iTrc].LayerOfPointAt(iPt),
                mTraces[iTrc].CoordsOfPointAt(iPt).x,
                mTraces[iTrc].CoordsOfPointAt(iPt).y,
                mTraces[iTrc].WidthOfSegmentFromPointAt(iPt));
        }
        m_pTraces->push_back(trc);
    }
    return m_pTraces;
}

AmfLayer* AmfData::AddLayer(const char* compId, const char* bumpHandle, int layerId)
{
    AmfLayer lyr(compId,bumpHandle,layerId);
    mLayers.push_back(lyr);
    int cLyrs =(int)mLayers.size();
    return &(mLayers[cLyrs-1]);
}

std::vector<AmfLayer>* AmfData::GetLayers(const char* compId,
                                          const char* bumpHandle)
{
    DELETE(m_pLayers);
    m_pLayers = new std::vector<AmfLayer>;
    if (mLayers.empty()) {
        return m_pLayers;
    }
    std::string compName =(compId  == 0)?"": compId;
    std::string bumpName =(bumpHandle == 0)?"":bumpHandle;
    int cLyrs =(int)mLayers.size();
    for (int iLr = 0; iLr < cLyrs; iLr++)
    {
        if (compId != 0) {
            if (mLayers[iLr].ComponentId() != compName)
                continue;
        }
        if (bumpHandle != 0) {
            if (mLayers[iLr].BumpHandle() != bumpName)
                continue;
        }
        AmfLayer lyr(
            mLayers[iLr].ComponentId().c_str(),
            mLayers[iLr].BumpHandle().c_str(),
            mLayers[iLr].LayerId() );

        m_pLayers->push_back(lyr);
    }
    return m_pLayers;
}

int AmfData::Write(const char* fileName) const
{
    std::string dieNames;
    for (uint id = 0; id < mComponents.size(); id++)
    {
        dieNames = dieNames + mComponents[id].Name() + std::string("+");
    }
    std::string fName = dieNames + mPackageName + std::string(".amf");
    if (fileName != 0 && strlen(fileName) > 0)
        fName = std::string(fileName);
    FILE *pAmfFile = fopen( fName.c_str(),"w");
//    if (pAmfFile == 0)
//    {
//        IMessenger* pMessenger = ILogger::GetMessenger("fcpasm");
//        pMessenger->out(msERROR,"CantOpen",fName.c_str());
//        return 1;
//    }
    time_t time_now = time((time_t *)0);
    fprintf( pAmfFile, "#\"%s\" \"%.24s.\"\n", fName.c_str(), ctime(&time_now) );
    fprintf( pAmfFile, "VERSION \t %d \n", mVersion );

    fprintf( pAmfFile, "# \n# \t %s \n# \n", "Package");
    fprintf( pAmfFile, "PACKAGE \"%s\"  DESCR  \"%s\" \n",
        mPackageName.c_str(),
        mPackageDescr.c_str() );
    fprintf( pAmfFile, "PACKAGE \"%s\"  REF_POINT %d %d \n",
        mPackageName.c_str(),
        mPackageRefPoint.x,
        mPackageRefPoint.y );
    if (mPackageCore.ll.x || mPackageCore.ll.y || mPackageCore.ur.x || mPackageCore.ur.y)
    fprintf( pAmfFile, "PACKAGE \"%s\"  CORE %d %d %d %d \n",
        mPackageName.c_str(),
        mPackageCore.ll.x,
        mPackageCore.ll.y,
        mPackageCore.ur.x,
        mPackageCore.ur.y );

    fprintf( pAmfFile, "# \n# \t %s \n# \n", "Components");
    for (uint id = 0; id < mComponents.size(); id++)
    {
        fprintf( pAmfFile, "COMPONENT \"%s\" \t DESCR  \"%s\" \n",
            mComponents[id].Name().c_str(),
            mComponents[id].Description().c_str() );
        fprintf( pAmfFile, "COMPONENT \"%s\" \t TYPE  \"%s\" \n",
            mComponents[id].Name().c_str(),
            mComponents[id].Type().c_str() );
        fprintf( pAmfFile, "COMPONENT \"%s\" \t FORMAT  \"%s\" \n",
            mComponents[id].Name().c_str(),
            mComponents[id].Format().c_str() );
        fprintf( pAmfFile, "COMPONENT \"%s\" \t REF_POINT %d %d \n",
            mComponents[id].Name().c_str(),
            mComponents[id].RefPoint().x,
            mComponents[id].RefPoint().y );
        Rect dc = mComponents[id].Core();
        if (dc.ll.x || dc.ll.y || dc.ur.x || dc.ur.y)
        fprintf( pAmfFile, "COMPONENT \"%s\" \t CORE %d %d %d %d \n",
            mComponents[id].Name().c_str(),
            dc.ll.x,
            dc.ll.y,
            dc.ur.x,
            dc.ur.y );
        if (id < mComponents.size()-1)
        fprintf( pAmfFile, "# \n");
    }

    fprintf( pAmfFile, "# \n# \t %s \n# \n", "Placement, Route and Mode");
    for (uint id = 0; id < mComponents.size(); id++)
    {
        fprintf( pAmfFile, "COMPONENT \"%s\" SHIFT %d %d  ROTATE %d %s\n",
            mComponents[id].Name().c_str(),
            mComponents[id].Shift().x,
            mComponents[id].Shift().y,
            mComponents[id].Rotation(),
            ((mComponents[id].Side()==1)?"BOTTOM":""));
    }
    fprintf( pAmfFile, "ITINERARY  %s \n", mItinerary.c_str() );
    fprintf( pAmfFile, "MODE  \"%s\" \n", mMode.c_str() );

    WriteGroups( pAmfFile );
    WriteAreas( pAmfFile );
    WriteAssignments( pAmfFile );
    WriteAssignments4supply( pAmfFile );
    WriteTraces( pAmfFile );
    WriteLayers( pAmfFile );

    fclose(pAmfFile);
    return 0;
}

void AmfData::WriteGroups( FILE *pAmfFile ) const
{
    if (pAmfFile == 0)
        return;
    if (mGroups.empty()) {
        fprintf( pAmfFile, "# \n# \t %s \n# \n\n", "Groups are not created");
        return;
    }
    fprintf( pAmfFile, "# \n# \t %s \n# \n", "Groups");
    for (uint ig = 0; ig < mGroups.size(); ig++)
    {
        fprintf(pAmfFile, "# \nGROUP \"%s\"  POWER \"%s\"  GROUND \"%s\" \n",
            mGroups[ig].Name().c_str(),
            mGroups[ig].mPower.c_str(),
            mGroups[ig].mGround.c_str() );
        for (int manual = 0; manual <= 1; manual++)
        {
            int ns = 0;
            for (uint is = 0; is < mGroups[ig].mSignals.size(); is++)
            {
                if (mGroups[ig].mManual[is] != manual)
                    continue;
                if (ns == 0) {
                    fprintf(pAmfFile, "SIGNALS \"%s\" %s",
                        mGroups[ig].Name().c_str(), ((manual)?"MANUAL":"AUTO"));
                }
                fprintf( pAmfFile, " \"%s\"", mGroups[ig].mSignals[is].c_str() );
                ns++;
                if (ns == 3) {
                    fprintf( pAmfFile, " \n");
                    ns = 0;
                }
            }
            if (ns > 0)
                fprintf( pAmfFile, " \n");
        }
    }
}

void AmfData::WriteAreas( FILE *pAmfFile ) const
{
    if (pAmfFile == 0)
        return;
    if (mAreas.empty()) {
        fprintf( pAmfFile, "# \n# \t %s \n# \n\n", "Areas are not created.");
        return;
    }
    fprintf( pAmfFile, "# \n# \t %s \n# \n", "Areas");
    char* padDestination = 0;
    for (uint ir = 0; ir < mAreas.size(); ir++)
    {
        fprintf( pAmfFile, "# \nAREA \"%s\" \t GROUPS ", mAreas[ir].Name().c_str() );
        for (uint iag = 0; iag < mAreas[ir].mGroups.size(); iag++)
        {
            fprintf( pAmfFile, " \"%s\"", mAreas[ir].mGroups[iag].c_str() );
        }
        fprintf( pAmfFile, " \n");

        for (uint id = 0; id < mComponents.size(); id++)
        {
            for (int manual = 0; manual <= 1; manual++)
            {
                for (int dest = DEST_UNDEFINED; dest <= DEST_4_FREE; dest++)
                {
                    padDestination = "";
                    if (dest == DEST_4_LOGIC)
                        padDestination = "LOGIC";
                    else if (dest == DEST_4_SUPPLY)
                        padDestination = "SUPPLY";
                    else // if (dest == DEST_4_FREE)
                        padDestination = "FREE";
                    int nb = 0;
                    for (uint ib = 0; ib < mAreas[ir].mBalls.size(); ib++)
                    {
                        if (id > 0)
                            continue;
                        if (mAreas[ir].mBalls[ib].mManual != manual)
                            continue;
                        if (mAreas[ir].mBalls[ib].mDestiny != dest)
                            continue;
                        if (nb == 0) {
                            fprintf(pAmfFile, "BALLS \"%s\" %s %s",
                                mAreas[ir].Name().c_str(),
                                padDestination,
                                ((manual)?"MANUAL":"AUTO"));
                        }
                        fprintf( pAmfFile, " \"%s\"", mAreas[ir].mBalls[ib].Name() );
                        nb++;
                        if (nb == 5) {
                            fprintf( pAmfFile, " \n");
                            nb = 0;
                        }
                    }
                    if (nb > 0)
                        fprintf( pAmfFile, " \n");
                    nb = 0;
                    for (uint ib = 0; ib < mAreas[ir].mBumps.size(); ib++)
                    {
                        if (mAreas[ir].mBumps[ib].ComponentId() != mComponents[id].Name())
                            continue;
                        if (mAreas[ir].mBumps[ib].mManual != manual)
                            continue;
                        if (mAreas[ir].mBumps[ib].mDestiny != dest)
                            continue;
                        if (nb == 0) {
                            fprintf(pAmfFile, "BUMPS \"%s\" %s \"%s\" %s",
                                mAreas[ir].Name().c_str(),
                                padDestination,
                                mAreas[ir].mBumps[ib].ComponentId().c_str(),
                                ((manual)?"MANUAL":"AUTO"));
                        }
                        fprintf( pAmfFile, " \"%s\"", mAreas[ir].mBumps[ib].Handle().c_str() );
                        nb++;
                        if (nb == 5) {
                            fprintf( pAmfFile, " \n");
                            nb = 0;
                        }
                    }
                    if (nb > 0)
                        fprintf( pAmfFile, " \n");
                }
            }
        }
    }
}

void AmfData::WriteAssignments( FILE *pAmfFile ) const
{
    if (pAmfFile == 0)
        return;
    if (mAssignments.empty()) {
        fprintf( pAmfFile, "# \n# \t %s \n# \n\n", "Assignments are not created.");
        return;
    }
    fprintf( pAmfFile, "# \n# \t %s \n# \n", "Assignments");
    for (uint ia = 0; ia < mAssignments.size(); ia++)
    {
        if (mAssignments[ia].Ball2Ball())
        {
            fprintf( pAmfFile,"ASSIGN \"%s\" \t BALL \"%s\" \t BALL \"%s\" %s %s\n",
                mAssignments[ia].SignalName().c_str(),
                mAssignments[ia].BallName().c_str(),
                mAssignments[ia].BallName2().c_str(),
                ((mAssignments[ia].IsManual())?"MANUAL":"AUTO"),
                ((mAssignments[ia].Hidden())?"HIDDEN":"") );
        }
        else
        if (mAssignments[ia].Bump2Ball())
        {
            fprintf( pAmfFile,"ASSIGN \"%s\" \t BUMP \"%s\" \"%s\" \t BALL \"%s\" %s %s\n",
                mAssignments[ia].SignalName().c_str(),
                mAssignments[ia].ComponentId().c_str(),
                mAssignments[ia].BumpHandle().c_str(),
                mAssignments[ia].BallName().c_str(),
                ((mAssignments[ia].IsManual())?"MANUAL":"AUTO"),
                ((mAssignments[ia].Hidden())?"HIDDEN":"") );
        }
        else
        {
            fprintf( pAmfFile,"ASSIGN \"%s\" \t BUMP \"%s\" \"%s\" \t BUMP \"%s\" \"%s\" %s %s\n",
                mAssignments[ia].SignalName().c_str(),
                mAssignments[ia].ComponentId().c_str(),
                mAssignments[ia].BumpHandle().c_str(),
                mAssignments[ia].ComponentId2().c_str(),
                mAssignments[ia].BumpHandle2().c_str(),
                ((mAssignments[ia].IsManual())?"MANUAL":"AUTO"),
                ((mAssignments[ia].Hidden())?"HIDDEN":"") );
        }
    }
}

void AmfData::WriteAssignments4supply( FILE *pAmfFile ) const
{
    if (pAmfFile == 0)
        return;
    if (mAssignments4supply.empty()) {
        fprintf( pAmfFile, "# \n# \t %s \n# \n\n", "Assignments for supply are not created.");
        return;
    }
    fprintf( pAmfFile, "# \n# \t %s \n# \n", "Assignments for supply signals");
    for (uint ia = 0; ia < mAssignments4supply.size(); ia++)
    {
        if (mAssignments4supply[ia].SupplyOnBall())
        {
            fprintf( pAmfFile,"ASSIGN \"%s\" \t SUPPLY \"%s\" \t BALL \"%s\" %s \n",
                mAssignments4supply[ia].SignalName().c_str(),
                mAssignments4supply[ia].AreaName().c_str(),
                mAssignments4supply[ia].BallName().c_str(),
                ((mAssignments4supply[ia].IsManual())?"MANUAL":"AUTO") );
        }
        else
        {
            fprintf( pAmfFile,"ASSIGN \"%s\" \t SUPPLY \"%s\" \t BUMP \"%s\" \"%s\" %s \n",
                mAssignments4supply[ia].SignalName().c_str(),
                mAssignments4supply[ia].AreaName().c_str(),
                mAssignments4supply[ia].ComponentId().c_str(),
                mAssignments4supply[ia].BumpHandle().c_str(),
                ((mAssignments4supply[ia].IsManual())?"MANUAL":"AUTO") );
        }
    }
}

void AmfData::WriteTraces( FILE *pAmfFile ) const
{
    if (pAmfFile == 0)
        return;
    if (mTraces.empty()) {
        return;
    }
    fprintf( pAmfFile, "# \n# \t %s \n# \n", "Escape traces from bumps");
    int cTrcs =(int)mTraces.size(), cPts = 0;
    for (int iTrc = 0; iTrc < cTrcs; iTrc++)
    {
        fprintf( pAmfFile,"TRACE \"%s\" \"%s\" \t",
            mTraces[iTrc].ComponentId().c_str(),
            mTraces[iTrc].BumpHandle().c_str());
        cPts = mTraces[iTrc].CountPoints();
        int segmWidth = 0;
        for (int iPt = 0; iPt < cPts; iPt++)
        {
            if (segmWidth != mTraces[iTrc].WidthOfSegmentFromPointAt(iPt)) {
                segmWidth  = mTraces[iTrc].WidthOfSegmentFromPointAt(iPt);
                fprintf( pAmfFile,"  w=%d", segmWidth );
            }
            fprintf( pAmfFile,"  %d:%d,%d",
                mTraces[iTrc].LayerOfPointAt(iPt),
                mTraces[iTrc].CoordsOfPointAt(iPt).x,
                mTraces[iTrc].CoordsOfPointAt(iPt).y);
        }
        fprintf( pAmfFile," \n");
    }
}

void AmfData::WriteLayers( FILE *pAmfFile ) const
{
    if (pAmfFile == 0)
        return;
    if (mLayers.empty()) {
        return;
    }
    fprintf( pAmfFile, "# \n# \t %s \n# \n", "Layers for escape traces");
    int cLyrs =(int)mLayers.size();
    for (int iLr = 0; iLr < cLyrs; iLr++)
    {
        fprintf( pAmfFile,"LAYER_ID \"%s\" \"%s\"  %d\n",
            mLayers[iLr].ComponentId().c_str(),
            mLayers[iLr].BumpHandle().c_str(),
            mLayers[iLr].LayerId() );
    }
}

} // namespace Fcp
