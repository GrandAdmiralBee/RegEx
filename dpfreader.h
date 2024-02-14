#ifndef DPFREADER_H
#define DPFREADER_H
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <string>

#include <commonstruct.h>

class DpfReader
{
public:
    DpfReader();
    int ReadDpf();

private:
    QRegularExpression rx = QRegularExpression();
    QRegularExpressionMatch match = QRegularExpressionMatch();
    QRegularExpressionMatchIterator matchIter = QRegularExpressionMatchIterator();

    QString WS = QString("[ \t\v\f\r]");
    QString INTEGER = QString("[+-]?[0-9]+");
    QString STRING = QString("[\"][^\"\f\r\n]*[\"]");
    QString WORD = QString("[a-zA-Z0-9]*");
    QString TRACE_POINT = QString("(" + INTEGER  + ")" + WS + "*:" + WS + "*" + "(" + INTEGER  + ")" + WS + "*,"  + WS + "*" + "(" + INTEGER + ")");
    QString TRACE_WIDTH = QString("[wW]" + WS + "*=" + WS + "*" + "(" + INTEGER + ")");
    QString pointRx = QString(INTEGER + "[:]" + INTEGER);
    QString integerListRx = QString("[^{]*");

    QString versionRx = QString("^" + WS + "*VERSION" + WS);
    QString dieRx = QString("^" + WS + "*DIE" + WS);
    QString coreAreaRx = QString("^" + WS + "*CORE_AREA" + WS);
    QString technologyRx = QString("^" + WS + "*TECHNOLOGY" + WS);
    QString technologyFileRx = QString("^" + WS + "*TECHNOLOGY_FILE" + WS);
    QString gdsSourceFileRx = QString("^" + WS + "*GDS_SOURCE_FILE" + WS);
    QString gdsStructureNameRx = QString("^" + WS + "*GDS_SOURCE_NAME" + WS);
    QString erpFileRx = QString("^" + WS + "*ERP_FILE" + WS);
    QString bumpShapeRx = QString("^" + WS + "*BUMP_SHAPE" + WS);
    QString bumpRx = QString("^" + WS + "*BUMP" + WS);
    QString padcellRx = QString("^" + WS + "*PADCELL" + WS);
    QString groupRx = QString("^" + WS + "*GROUP" + WS);
    QString traceRx = QString("^" + WS + "*TRACE" + WS);
    QString patterncellRx = QString("^" + WS + "*PATTERNCELL" + WS);
    QString textRx = QString("^" + WS + "*TEXT" + WS);

    QString placeRx = QString("{PLACE}");
    QString bumpNameRx = QString("{BUMP_NAME}");
    QString pinNameRx = QString("{PIN_NAME}");
    QString rotationRx = QString("{ROTATION}");
    QString rowRx = QString("{ROW}");
    QString bpListRx = QString("{BP#}");
    QString pcListRx = QString("{PC#}");
    QString masterNameRx = QString("{MASTER_NAME}");
    QString instanceNameRx = QString("{INSTANCE_NAME}");
    QString signalNameRx = QString("{SIGNAL_NAME}");
    QString typeRx = QString("{TYPE}");
    QString textSubRx = QString("{TEXT}");
    QString justifyRx = QString("{JUSTIFY}");
    QString staggeredRx = QString("STAGGERED");

    int ParseDpf();
    QString rmdq(const QString& str);
    std::vector<int> parsePointList(const QString& line, const QString& regex);
    void parseVersionGroup(const QString& line);
    void parseDieGroup(const QString& line);
    void parseCoreAreaGroup(const QString& line);
    void parseTechnologyGroup(const QString& line);
    void parseBumpShapeGroup(const QString& line);
    void parseBumpGroup(const QString& line);
    void parsePadcellGroup(const QString& line);
    void parsePatterncellGroup(const QString& line);
    void parseGdsSourceFileGroup(const QString& line);
    void parseTechnologyFileGroup(const QString& line);
    void parseErpFileGroup(const QString& line);
    void parseGdsStructureFileGroup(const QString& line);
    void parseTextGroup(const QString& line);
    void parseGroup(const QString& line);
    void parseTraceGroup(const QString& line);

    //Dpf values
    void parseDpfValues(const QString& line);
    std::vector<int> parsePlaceValues(const QString& line);
    int parseRowValues(const QString& line);
    Fcp::Rotation parseRotationValues(const QString& line);
    std::vector<int> parseBpListValues(const QString& line);
    std::vector<int> parsePcListValues(const QString& line);
    QString parseMasterName(const QString& line);
    QString parseInstanceName(const QString& line);
    QString parseSignalName(const QString& line);
    QString parseBumpName(const QString& line);
    QString parsePinName(const QString& line);
    QString parseType(const QString& line);
    QString parseText(const QString& line);
    QString parseJustify(const QString& line);
    QString parseStaggered(const QString& line);
};

#endif // DPFREADER_H
