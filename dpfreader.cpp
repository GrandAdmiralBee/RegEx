#include "dpfreader.h"

static QRegularExpression rx;
static QRegularExpressionMatch match;
static QRegularExpressionMatchIterator matchIter;

static QString WS = QString("[ \t\v\f\r]");
static QString INTEGER = QString("[+-]?[0-9]+");
static QString STRING = QString("[\"][^\"\f\r\n]*[\"]");
static QString WORD = QString("[a-zA-Z][a-zA-Z0-9_\-.;:\(\)\{\}\[\]'\\\/]*");
static QString TRACE_POINT = QString(INTEGER + WS + "*\":\"" + WS + "*" + INTEGER + WS + "*\",\""  + WS + "*" + INTEGER);
static QString TRACE_WIDTH = QString("[wW]" + WS + "*\"=\"" + WS + "*" + INTEGER);
static QString pointRx = QString(INTEGER + "[:]" + INTEGER);
static QString integerList = QString("[^{]*");

static QString versionRx = QString("^" + WS + "*VERSION" + WS);
static QString dieRx = QString("^" + WS + "*DIE" + WS);
static QString coreAreaRx = QString("^" + WS + "*CORE_AREA" + WS);
static QString technologyRx = QString("^" + WS + "*TECHNOLOGY" + WS);
static QString technologyFileRx = QString("^" + WS + "*TECHNOLOGY_FILE" + WS);
static QString gdsSourceFileRx = QString("^" + WS + "*GDS_SOURCE_FILE" + WS);
static QString gdsStructureNameRx = QString("^" + WS + "*GDS_SOURCE_NAME" + WS);
static QString erpFileRx = QString("^" + WS + "*ERP_FILE" + WS);
static QString bumpShapeRx = QString("^" + WS + "*BUMP_SHAPE" + WS);
static QString bumpRx = QString("^" + WS + "*BUMP" + WS);
static QString padcellRx = QString("^" + WS + "*PADCELL" + WS);
static QString groupRx = QString("^" + WS + "*GROUP" + WS);
static QString traceRx = QString("^" + WS + "*TRACE" + WS);
static QString patterncellRx = QString("^" + WS + "*PATTERNCELL" + WS);
static QString textRx = QString("^" + WS + "*TEXT" + WS);

static QString placeRx = QString("\{PLACE\}");
static QString bumpNameRx = QString("\{BUMP_NAME\}");
static QString pinNameRx = QString("\{PIN_NAME\}");
static QString rotationRx = QString("\{ROTATION\}");
static QString rowRx = QString("\{ROW\}");
static QString bpListRx = QString("\{BP\#\}");
static QString pcListRx = QString("\{PC\#\}");
static QString masterNameRx = QString("{MASTER_NAME}");
static QString instanceNameRx = QString("\{INSTANCE_NAME\}");
static QString signalNameRx = QString("\{SIGNAL_NAME\}");
static QString typeRx = QString("\{TYPE\}");
static QString textSubRx = QString("\{TEXT\}");
static QString justifyRx = QString("\{JUSTIFY\}");
static QString staggeredRx = QString("STAGGERED");

static DpfReader::KeyValues kvs;
static std::vector<DpfReader::KeyValues> keyVal;

static int ParseDpf();
static QString rmdq(const QString& str);
static void parseVersionGroup(const QString& line);
static void parseDieGroup(const QString& line);

//Dpf values
static void parseDpfValues(const QString& line);
static void parsePlaceValues(const QString& line);
static void parseRowValues(const QString& line);
static void parseRotationValues(const QString& line);
static void parseBpListValues(const QString& line);
static void parsePcListValues(const QString& line);
static void parseMasterName(const QString& line);
static void parseInstanceName(const QString& line);
static void parseSignalName(const QString& line);
static void parseBumpName(const QString& line);
static void parsePinName(const QString& line);
static void typeName(const QString& line);
static void textName(const QString& line);
static void justifyName(const QString& line);
static void staggeredName(const QString& line);

DpfReader::DpfReader()
{

}

int DpfReader::ReadDpf()
{
    return ParseDpf();
}

int ParseDpf()
{
    QFile f("D:/Users/abdulkin_k/Documents/amf_reader/e3s.dpf");
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        qCritical() << "Could not open file!";
        return -1;
    }

    QTextStream in(&f);
    while (!in.atEnd())
    {
        QString line = in.readLine();

        //Version group
        rx.setPattern(versionRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseVersionGroup(line);
            continue;
        }

        //Die group
        rx.setPattern(dieRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseDieGroup(line);
            continue;
        }

    }

    return 0;
}

QString rmdq(const QString& str)
{
    std::string stdstr = str.toStdString();
    stdstr.erase(remove( stdstr.begin(), stdstr.end(), '\"'),stdstr.end());

    QString res = QString::fromStdString(stdstr);
    return res;
}

static void parseTextName(const QString& line)
{
    rx.setPattern(textRx + WS + STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(STRING);
        match = rx.match(str);
        QString text = rmdq(match.captured());
    }
}

static void parseTypeName(const QString& line)
{
    rx.setPattern(typeRx + WS + STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(STRING);
        match = rx.match(str);
        QString type = rmdq(match.captured());
    }
}

static void parsePinName(const QString& line)
{
    rx.setPattern(pinNameRx + WS + STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(STRING);
        match = rx.match(str);
        QString pinName = rmdq(match.captured());
    }
}

static void parseBumpName(const QString& line)
{
    rx.setPattern(bumpNameRx + WS + STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(STRING);
        match = rx.match(str);
        QString bumpName = rmdq(match.captured());
    }
}

static void parseSignalName(const QString& line)
{
    rx.setPattern(signalNameRx + WS + STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(STRING);
        match = rx.match(str);
        QString signalName = rmdq(match.captured());
    }
}

static void parseInstanceName(const QString& line)
{
    rx.setPattern(instanceNameRx + WS + STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(STRING);
        match = rx.match(str);
        QString instanceName = rmdq(match.captured());
    }
}

static void parseMasterName(const QString& line)
{
    rx.setPattern(masterNameRx + WS + STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(STRING);
        match = rx.match(str);
        QString masterName = rmdq(match.captured());
    }
}

void parseDieGroup(const QString& line)
{
    rx.setPattern(STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString name = rmdq(match.captured());
    }

    parseDpfValues(line);
}

void parseVersionGroup(const QString& line)
{
    rx.setPattern(INTEGER);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        int version = str.toInt();
        //qDebug() << version;
    }

    int fformat = 1;
    if(line.contains("v.[1]"))
    {
        fformat = 0;
    }
    return;
}

void parseBpListValues(const QString& line)
{
    rx.setPattern(bpListRx + integerList);

    std::vector<int> bumpList;
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(INTEGER);
        matchIter = rx.globalMatch(str);
        while(matchIter.hasNext())
        {
            bumpList.push_back(matchIter.next().captured().toInt());
        }
    }
}

void parsePcListValues(const QString& line)
{
    rx.setPattern(pcListRx + integerList);

    std::vector<int> pcList;
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(INTEGER);
        matchIter = rx.globalMatch(str);
        while(matchIter.hasNext())
        {
            pcList.push_back(matchIter.next().captured().toInt());
        }
    }
}

void parseRotationValues(const QString& line)
{
    Fcp::Rotation rotate;

    //R*
    rx.setPattern("R" + INTEGER);
    match = rx.match(line);
    if (match.hasMatch())
    {
        int rotation = match.captured().toInt();\
        switch(rotation)
        {
            case 0: rotation = Fcp::Rotation::ROT_0;
            case 90: rotation = Fcp::Rotation::ROT_90;
            case 180: rotation = Fcp::Rotation::ROT_180;
            case 270: rotation = Fcp::Rotation::ROT_270;
            default: rotation = Fcp::Rotation::ROT_0;
        }
    }
    //RX*
    rx.setPattern("RX" + INTEGER);
    match = rx.match(line);
    if (match.hasMatch())
    {
        int rotation = match.captured().toInt();\
        switch(rotation)
        {
            case 0: rotation = Fcp::Rotation::ROT_MX;
            case 90: rotation = Fcp::Rotation::ROT_MX90;
            case 180: rotation = Fcp::Rotation::ROT_MY;
            case 270: rotation = Fcp::Rotation::ROT_MY90;
            default: rotation = Fcp::Rotation::ROT_MX;
        }
    }
    //RY*
    rx.setPattern("RY" + INTEGER);
    match = rx.match(line);
    if (match.hasMatch())
    {
        int rotation = match.captured().toInt();\
        switch(rotation)
        {
            case 0: rotation = Fcp::Rotation::ROT_MY;
            case 90: rotation = Fcp::Rotation::ROT_MY90;
            case 180: rotation = Fcp::Rotation::ROT_MX;
            case 270: rotation = Fcp::Rotation::ROT_MX90;
            default: rotation = Fcp::Rotation::ROT_MY;
        }
    }
}

void parseRowValues(const QString& line)
{
    rx.setPattern(rowRx + WS + INTEGER);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();

        rx.setPattern(INTEGER);
        match = rx.match(str);
        int row = match.captured().toInt();
    }
}

void parsePlaceValues(const QString& line)
{
    //Rect
    rx.setPattern(pointRx + WS + pointRx);
    matchIter = rx.globalMatch(line);
    if(matchIter.hasNext())
    {
        QString str = matchIter.next().captured();

        rx.setPattern(INTEGER);
        QRegularExpressionMatchIterator tempMatchIter = rx.globalMatch(str);

        int a = tempMatchIter.next().captured().toInt();
        int b = tempMatchIter.next().captured().toInt();
        int c = tempMatchIter.next().captured().toInt();
        int d = tempMatchIter.next().captured().toInt();
    }

    //Point
    rx.setPattern(pointRx);
    matchIter = rx.globalMatch(line);
    if(matchIter.hasNext())
    {
        QString str = matchIter.next().captured();

        rx.setPattern(INTEGER);
        QRegularExpressionMatchIterator tempMatchIter = rx.globalMatch(str);

        int x = tempMatchIter.next().captured().toInt();
        int y = tempMatchIter.next().captured().toInt();
    }
}

void parseDpfValues(const QString& line)
{
    //Place
    rx.setPattern(placeRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        parsePlaceValues(line);
    }

    //Row
    rx.setPattern(rowRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        parseRowValues(line);
    }

    //Row
    rx.setPattern(rotationRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        parseRotationValues(line);
    }

    //BP list
    rx.setPattern(bpListRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        parseBpListValues(line);
    }

    //PC list
    rx.setPattern(pcListRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        parsePcListValues(line);
    }

    //Master list
    rx.setPattern(placeRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        parseMasterName(line);
    }

    //Instance list
    rx.setPattern(placeRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        parseInstanceName(line);
    }

    //Signal list
    rx.setPattern(placeRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        parseSignalName(line);
    }

    //Bump list
    rx.setPattern(placeRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        parseBumpName(line);
    }
}


