#ifndef PKGPARSER_H
#define PKGPARSER_H

#include<QString>
#include <QTextStream>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
#include<QRegularExpressionMatchIterator>

class PkgParser
{
public:
    PkgParser();
    int readPkg();
private:
    int parsePkg();

    void parsePkgFile(QTextStream& textStream);
    void parseFptFile(QTextStream& textStream);
    void parsePstFile(QTextStream& textStream);

    void parseUnitsDesc(const QString& line);
    void parsePkgName(const QString& line);
    void parsePitch(const QString& line);
    void parseS(const QString& line);
    void parseExtents(const QString& line);
    void parsePinList(QTextStream& in);
    //void parsePinFunctions(const QString& line);
    //void parsePadstackDescList(const QString& line);

    void parsePartNumber(const QString& line);
//    void parseOutlineDesc(const QString& line);
//    void parsePadstackDesc(const QString& line);

    void parseColor(const QString& line);

    QRegularExpression rx = QRegularExpression();
    QRegularExpressionMatch match = QRegularExpressionMatch();
    QRegularExpressionMatchIterator matchIter = QRegularExpressionMatchIterator();

    QString WS = QString("[ \t\v\f\r]");
    QString WSEATER = QString("[ \t\v\f\r]*");
    QString DIGIT = QString("[0-9]");
    QString NUM = QString("[+-]?[0-9]+");
    QString FLOAT_NUM = QString(R"([+-]?[0-9]+(\.[0-9]*([Ee][+-]?[0-9]+)?)?)");
    QString STRING = QString("\"([^\"\f\r\n]*)\"");
    QString WORD = QString("[a-zA-Z0-9]*");

    QString leftP = R"(\()";
    QString rightP = R"(\))";

    QString pkgRx = QString("PKG" + WSEATER + "(" + FLOAT_NUM + ")");
    QString fptRx = QString("FPT" + WSEATER + "(" + FLOAT_NUM + ")");
    QString pstRx = QString("PST" + WSEATER + "(" + FLOAT_NUM + ")");
    QString unitsRx = QString("Units:");
    QString nameRx = QString("Name:");
    QString numberRx = QString("Number:");
    QString pitchxRx = QString("Pitch_x:");
    QString pitchyRx = QString("Pitch_y:");
    QString sxRx = QString("Sx:");
    QString syRx = QString("Sy:");
    QString extentsRx = QString("Extents:");
    QString outlineRx = QString("Outline:");
    QString padstackRx = QString("Padstack");
    QString pinlistRx = QString("PINLIST");
    QString functionsRx = QString("FUNCTIONS");
    QString fillRx = QString("Fill:");
    QString lineWidthRx = QString("LineWidth:");
    QString lineColorRx = QString("LineColor:");
    QString fillingColorRx = QString("FillingColor:");
    QString radiusRx = QString("Radius:");
    QString typeRx = QString("Type:");
    QString cutRx = QString("Cut:");
    QString vertexCountRx = QString("VertexCount:");
    QString pointsRx = QString("Points:");
    QString point2Rx = QString("Point2:");
    QString segmentsRx = QString("Segments:");

    QString circleRx = QString("Circle");
    QString rectRx = QString("Rect");
    QString polyRx = QString("Poly");
    QString polyArcRx = QString("PolyArc");
    QString regPolyRx = QString("RegPoly");
    QString locationRx = QString("Location:");
    QString angleTypeRx = QString("AngleType:");
    QString angleRx = QString("Angle:");
    QString trueRx = QString("true");
    QString falseRx = QString("false");
    QString undefRx = QString(R"(\*)");
    QString inoutRx = QString("INOUT");
    QString inRx = QString("IN");
    QString outRx = QString("OUT");
    QString meterRx = QString("METER");
    QString dMRx = QString("dM");
    QString cMRx = QString("cM");
    QString mMRx = QString("mM");
    QString uMRx = QString("uM");
    QString nMRx = QString("nM");
    QString thRx = QString("th");
    QString milRx = QString("Mil");
    QString udRx = QString("ud");
    QString assignRx = QString("=");
};

#endif // PKGPARSER_H
