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

class AmfReader
{
public:
    AmfReader();
    int ReadAmf();

private:
    Fcp::AmfData *pAmf;

    QRegularExpression rx = QRegularExpression();
    QRegularExpressionMatch match = QRegularExpressionMatch();
    QRegularExpressionMatchIterator matchIter = QRegularExpressionMatchIterator();

    QString WS = QString("[ \t\v\f\r]");
    QString INTEGER = QString("[+-]?[0-9]+");
    QString versionRx = QString("^" + WS + "*VERSION" + WS);
    QString packageRx = QString("^" + WS + "*PACKAGE" + WS);
    QString componentRx = QString("^" + WS + "*COMPONENT" + WS);
    QString itineraryRx = QString("^" + WS + "*ITINERARY" + WS);
    QString modeRx = QString("^" + WS + "*MODE" + WS);
    QString groupRx = QString("^" + WS + "*GROUP" + WS);
    QString signalsRx = QString("^" + WS + "*SIGNALS" + WS);
    QString areaRx = QString("^" + WS + "*AREA" + WS);
    QString ballsRx = QString("^" + WS + "*BALLS" + WS);
    QString bumpsRx = QString("^" + WS + "*BUMPS" + WS);
    QString assignRx = QString("^" + WS + "*ASSIGN" + WS);
    QString traceRx = QString("^" + WS + "*TRACE" + WS);
    QString layerRx = QString("^" + WS + "*LAYER_ID" + WS);

    QString typeRx = QString(WS + "*TYPE" + WS);
    QString formatRx = QString(WS + "*FORMAT" + WS);
    QString descrRx = QString(WS + "*DESCR" + WS);
    QString refPointRx = QString(WS + "*REF_POINT" + WS);
    QString coreRx = QString(WS + "*CORE" + WS);
    QString shiftRx = QString(WS + "*SHIFT" + WS);
    QString rotateRx = QString(WS + "*ROTATE" + WS);

    QString bottomRx = QString(WS + "*BOTTOM" + WS + "?");
    QString fromDieToPackageRx = QString(WS + "*FROM_DIE_TO_PACKAGE" + WS + "?");
    QString fromPackageToDieRx = QString(WS + "*FROM_PACKAGE_TO_DIE" + WS + "?");
    QString undefinedRouteRx = QString(WS + "*UNDEFINED_ROUTE" + WS + "?");

    QString powerRx = QString(WS + "*POWER" + WS);
    QString groundRx = QString(WS + "*GROUND" + WS);
    QString logicRx = QString(WS + "*LOGIC" + WS);
    QString supplyRx = QString(WS + "*SUPPLY" + WS);
    QString freeRx = QString(WS + "*FREE" + WS);

    QString autoRx = QString(WS + "*AUTO" + WS + "?");
    QString manualRx = QString(WS + "*MANUAL" + WS + "?");

    QString groupsRx = QString(WS + "*GROUPS" + WS);
    QString ballRx = QString(WS + "*BALL" + WS);
    QString bumpRx = QString(WS + "*BUMP" + WS);
    QString hiddenRx = QString(WS + "*HIDDEN" + WS + "?");

    QString widthRx = QString(WS + "*W" + WS + "*[=]" + WS + "*" + INTEGER);
    QString tracePointRx = QString(INTEGER + WS + "*[:]" + WS + "*" + INTEGER + WS + "*[,]" + WS + "*" + INTEGER);

    QString q1 = QString::fromUtf8("\\u201C"); // “
    QString q2 = QString::fromUtf8("\\u201D"); // ”
    QString quotedTextRx = QString::fromUtf8("[\"][^\"\f\r\n]*[\"]");
    QString integerRx = QString(WS + INTEGER);

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
};

#endif // AMFREADER_H
