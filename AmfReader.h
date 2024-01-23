#ifndef AMFREADER_H
#define AMFREADER_H

#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <stdlib.h>

#include <iostream>
#include <string>

#include <fcpamfdata.h>

static Fcp::AmfData *pAmf;

static QRegularExpression rx;
static QRegularExpressionMatch match;
static QRegularExpressionMatchIterator matchIter;

static QString WS = QString("[ \t\v\f\r]");
static QString INTEGER = QString("[+-]?[0-9]+");
static QString versionRx = QString("^" + WS + "*VERSION" + WS);
static QString packageRx = QString("^" + WS + "*PACKAGE" + WS);
static QString componentRx = QString("^" + WS + "*COMPONENT" + WS);
static QString itineraryRx = QString("^" + WS + "*ITINERARY" + WS);
static QString modeRx = QString("^" + WS + "*MODE" + WS);
static QString groupRx = QString("^" + WS + "*GROUP" + WS);
static QString signalsRx = QString("^" + WS + "*SIGNALS" + WS);
static QString areaRx = QString("^" + WS + "*AREA" + WS);
static QString ballsRx = QString("^" + WS + "*BALLS" + WS);
static QString bumpsRx = QString("^" + WS + "*BUMPS" + WS);
static QString assignRx = QString("^" + WS + "*ASSIGN" + WS);
static QString traceRx = QString("^" + WS + "*TRACE" + WS);
static QString layerRx = QString("^" + WS + "*LAYER_ID" + WS);

static QString typeRx = QString(WS + "*TYPE" + WS);
static QString formatRx = QString(WS + "*FORMAT" + WS);
static QString descrRx = QString(WS + "*DESCR" + WS);
static QString refPointRx = QString(WS + "*REF_POINT" + WS);
static QString coreRx = QString(WS + "*CORE" + WS);
static QString shiftRx = QString(WS + "*SHIFT" + WS);
static QString rotateRx = QString(WS + "*ROTATE" + WS);

static QString bottomRx = QString(WS + "*BOTTOM" + WS + "?");
static QString fromDieToPackageRx = QString(WS + "*FROM_DIE_TO_PACKAGE" + WS + "?");
static QString fromPackageToDieRx = QString(WS + "*FROM_PACKAGE_TO_DIE" + WS + "?");
static QString undefinedRouteRx = QString(WS + "*UNDEFINED_ROUTE" + WS + "?");

static QString powerRx = QString(WS + "*POWER" + WS);
static QString groundRx = QString(WS + "*GROUND" + WS);
static QString logicRx = QString(WS + "*LOGIC" + WS);
static QString supplyRx = QString(WS + "*SUPPLY" + WS);
static QString freeRx = QString(WS + "*FREE" + WS);

static QString autoRx = QString(WS + "*AUTO" + WS + "?");
static QString manualRx = QString(WS + "*MANUAL" + WS + "?");

static QString groupsRx = QString(WS + "*GROUPS" + WS);
static QString ballRx = QString(WS + "*BALL" + WS);
static QString bumpRx = QString(WS + "*BUMP" + WS);
static QString hiddenRx = QString(WS + "*HIDDEN" + WS + "?");

static QString widthRx = QString(WS + "*W" + WS + "*[=]" + WS + "*" + INTEGER);
static QString tracePointRx = QString(INTEGER + WS + "*[:]" + WS + "*" + INTEGER + WS + "*[,]" + WS + "*" + INTEGER);


static QString q1 = QString::fromUtf8("\\u201C"); // “
static QString q2 = QString::fromUtf8("\\u201D"); // ”
static QString quotedTextRx = QString::fromUtf8("[\"][^\"\f\r\n]*[\"]");
static QString integerRx = QString(WS + INTEGER);

int parseAmf();

std::vector<std::string>* parseListGroup(const QString& line, int offset);
int parseAutoOrManualGroup(const QString& line);
Fcp::PadDestination_t parseDestinationGroup(const QString& line);

void parseVersionGroup(const QString& line);
void parsePackageGroup(const QString& line);
void parseComponentGroup(const QString& line);
void parseItineraryGroup(const QString& line);
void parseModeGroup(const QString& line);
void parseGroup(const QString& line);
void parseSignalsGroup(const QString& line);
void parseAreaGroup(const QString& line);
void parseBumpsGroup(const QString& line);
void parseBallsGroup(const QString& line);
void parseAssignGroup(const QString& line);
void parseTraceGroup(const QString& line);
void parseLayerGroup(const QString& line);
QString rmdq(const QString& str);


class AmfReader
{
public:
    AmfReader();
    int ReadAmf();
};

#endif // AMFREADER_H
