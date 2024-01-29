#include "dpfreader.h"

static QRegularExpression rx;
static QRegularExpressionMatch match;
static QRegularExpressionMatchIterator matchIter;

static QString WS = QString("[ \t\v\f\r]");
static QString INTEGER = QString("[+-]?[0-9]+");
static QString STRING = QString("[\"][^\"\f\r\n]*[\"]");
static QString WORD = QString("[a-zA-Z0-9]*");
static QString TRACE_POINT = QString(INTEGER + WS + "*\":\"" + WS + "*" + INTEGER + WS + "*\",\""  + WS + "*" + INTEGER);
static QString TRACE_WIDTH = QString("[wW]" + WS + "*\"=\"" + WS + "*" + INTEGER);
static QString pointRx = QString(INTEGER + "[:]" + INTEGER);
static QString pointListRx = QString("[^{]*");

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
static std::vector<int> parsePointList(const QString& line, const QString& regex);
static void parseVersionGroup(const QString& line);
static void parseDieGroup(const QString& line);
static void parseCoreAreaGroup(const QString& line);
static void parseTechnologyGroup(const QString& line);
static void parseBumpShapeGroup(const QString& line);
static void parseBumpGroup(const QString& line);
static void parsePadcellGroup(const QString& line);
static void parsePatterncellGroup(const QString& line);

//Dpf values
static void parseDpfValues(const QString& line);
static std::vector<int> parsePlaceValues(const QString& line);
static int parseRowValues(const QString& line);
static Fcp::Rotation parseRotationValues(const QString& line);
static std::vector<int> parseBpListValues(const QString& line);
static std::vector<int> parsePcListValues(const QString& line);
static QString parseMasterName(const QString& line);
static QString parseInstanceName(const QString& line);
static QString parseSignalName(const QString& line);
static QString parseBumpName(const QString& line);
static QString parsePinName(const QString& line);
static QString typeName(const QString& line);
static QString textName(const QString& line);
static QString justifyName(const QString& line);
static QString staggeredName(const QString& line);

DpfReader::DpfReader()
{

}

int DpfReader::ReadDpf()
{
    return ParseDpf();
}

int ParseDpf()
{
    QFile f("C:/Users/user/Documents/RegEx/e3s.dpf");
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

        //Core group
        rx.setPattern(coreAreaRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseCoreAreaGroup(line);
            continue;
        }

        //Technology group
        rx.setPattern(technologyRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseTechnologyGroup(line);
            continue;
        }

        //Bump shape group
        rx.setPattern(bumpShapeRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseBumpShapeGroup(line);
            continue;
        }

        //Bump group
        rx.setPattern(bumpRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseBumpGroup(line);
            continue;
        }

        //Padcell group
        rx.setPattern(padcellRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parsePadcellGroup(line);
            continue;
        }

        //Patterncell group
        rx.setPattern(patterncellRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parsePatterncellGroup(line);
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

std::vector<int> parsePointList(const QString& line, const QString& regex)
{
    rx.setPattern(regex + pointListRx);
    std::vector<int> pointList;
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(INTEGER);
        matchIter = rx.globalMatch(str);
        while(matchIter.hasNext())
        {
            pointList.push_back(matchIter.next().captured().toInt());
        }
    }

    return pointList;
}

void parsePatterncellGroup(const QString& line)
{
    QString patterncellName;

    rx.setPattern(patterncellRx + "(" + STRING + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        patterncellName = rmdq(match.captured(1));
        qDebug() << patterncellName;
    }

    std::vector<int> pc = parsePcListValues(line);
    std::vector<int> place = parsePlaceValues(line);
    Fcp::Rotation rotate = parseRotationValues(line);

}

void parsePadcellGroup(const QString& line)
{
    int padcellId;

    rx.setPattern(padcellRx + "(" + INTEGER + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        padcellId = match.captured(1).toInt();
    }

    std::vector<int> bp = parseBpListValues(line);
    QString masterName = parseMasterName(line);
    QString instanceName = parseInstanceName(line);
    QString signalName = parseSignalName(line);
    std::vector<int> place = parsePlaceValues(line);
    Fcp::Rotation rotate = parseRotationValues(line);
    int row = parseRowValues(line);
    //Justify
}

void parseBumpGroup(const QString& line)
{
    int bumpId;

    rx.setPattern(bumpRx + "(" + INTEGER + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        bumpId = match.captured(1).toInt();
    }

    QString bumpName = parseBumpName(line);
    QString pinName = parsePinName(line);

    std::vector<int> place = parsePlaceValues(line);

}

void parseDieGroup(const QString& line)
{
    rx.setPattern(STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString name = rmdq(match.captured());
    }

    std::vector<int> place = parsePlaceValues(line);
    Fcp::Rotation rotate = parseRotationValues(line);
}

void parseVersionGroup(const QString& line)
{
    rx.setPattern(INTEGER);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        int version = str.toInt();
    }

    int fformat = 1;
    if(line.contains("v.[1]"))
    {
        fformat = 0;
    }
    return;
}

void parseBumpShapeGroup(const QString& line)
{
    std::vector<int> list = parsePointList(line, bumpShapeRx);
}

void parseTechnologyGroup(const QString& line)
{
    rx.setPattern(STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString technologyName = rmdq(match.captured());
    }
}

void parseCoreAreaGroup(const QString& line)
{
    //TODO!
}

QString parseTextName(const QString& line)
{
    QString textName;

    rx.setPattern(textRx + WS + STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(STRING);
        match = rx.match(str);
        textName = rmdq(match.captured());
    }

    return textName;
}

QString parseTypeName(const QString& line)
{
    QString typeName;

    rx.setPattern(typeRx + WS + STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(STRING);
        match = rx.match(str);
        typeName = rmdq(match.captured());
    }

    return typeName;
}

QString parsePinName(const QString& line)
{
    QString pinName;

    rx.setPattern(pinNameRx + WS + "(" + STRING + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        pinName = match.captured(1);
    }

    rx.setPattern(pinNameRx + WS + "(" + WORD + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        pinName = match.captured(1);
    }

    return pinName;
}

QString parseBumpName(const QString& line)
{
    QString bumpName;

    rx.setPattern(bumpNameRx + WS + "(" + STRING + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        bumpName = match.captured(1);
    }

    rx.setPattern(bumpNameRx + WS + "(" + WORD + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        bumpName = match.captured(1);
        qDebug() << bumpName;
    }

    return bumpName;
}

QString parseSignalName(const QString& line)
{
    QString signalName;

    rx.setPattern(signalNameRx + WS + STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(STRING);
        match = rx.match(str);
        signalName = rmdq(match.captured());
    }

    return signalName;
}

QString parseInstanceName(const QString& line)
{
    QString instanceName;

    rx.setPattern(instanceNameRx + WS + STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(STRING);
        match = rx.match(str);
        instanceName = rmdq(match.captured());
    }

    return instanceName;
}

QString parseMasterName(const QString& line)
{
    QString masterName;

    rx.setPattern(masterNameRx + WS + STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(STRING);
        match = rx.match(str);
        masterName = rmdq(match.captured());
    }

    return masterName;
}

std::vector<int> parseBpListValues(const QString& line)
{
    return parsePointList(line, bpListRx);
}

std::vector<int> parsePcListValues(const QString& line)
{
    return parsePointList(line, pcListRx);
}

Fcp::Rotation parseRotationValues(const QString& line)
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
            case 0: rotate = Fcp::Rotation::ROT_0;
            case 90: rotate = Fcp::Rotation::ROT_90;
            case 180: rotate = Fcp::Rotation::ROT_180;
            case 270: rotate = Fcp::Rotation::ROT_270;
            default: rotate = Fcp::Rotation::ROT_0;
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
            case 0: rotate = Fcp::Rotation::ROT_MX;
            case 90: rotate = Fcp::Rotation::ROT_MX90;
            case 180: rotate = Fcp::Rotation::ROT_MY;
            case 270: rotate = Fcp::Rotation::ROT_MY90;
            default: rotate = Fcp::Rotation::ROT_MX;
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
            case 0: rotate = Fcp::Rotation::ROT_MY;
            case 90: rotate = Fcp::Rotation::ROT_MY90;
            case 180: rotate = Fcp::Rotation::ROT_MX;
            case 270: rotate = Fcp::Rotation::ROT_MX90;
            default: rotate = Fcp::Rotation::ROT_MY;
        }
    }

    return rotate;
}

int parseRowValues(const QString& line)
{
    int row = 0;
    rx.setPattern(rowRx + WS + INTEGER);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();

        rx.setPattern(INTEGER);
        match = rx.match(str);
        row = match.captured().toInt();
    }

    return row;
}

std::vector<int> parsePlaceValues(const QString& line)
{
    std::vector<int> values;
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

        values.push_back(a);
        values.push_back(b);
        values.push_back(c);
        values.push_back(d);
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

        values.push_back(x);
        values.push_back(y);
    }

    return values;
}



