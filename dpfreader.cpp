#include "dpfreader.h"

DpfReader::DpfReader()
{

}

int DpfReader::ReadDpf()
{
    return ParseDpf();
}

int DpfReader::ParseDpf()
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

        //Gds file group
        rx.setPattern(gdsSourceFileRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseGdsSourceFileGroup(line);
            continue;
        }

        //Group
        rx.setPattern(groupRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseGroup(line);
            continue;
        }

        //Trace group
        rx.setPattern(traceRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseTraceGroup(line);
            continue;
        }
    }

    f.close();

    return 0;
}

QString DpfReader::rmdq(const QString& str)
{
    std::string stdstr = str.toStdString();
    stdstr.erase(remove( stdstr.begin(), stdstr.end(), '\"'),stdstr.end());

    QString res = QString::fromStdString(stdstr);
    return res;
}

std::vector<int> DpfReader::parsePointList(const QString& line, const QString& regex)
{
    rx.setPattern(regex + integerListRx);
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

void DpfReader::parseTraceGroup(const QString& line)
{
    int trace;

    rx.setPattern(traceRx + "(" + INTEGER + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        trace = match.captured(1).toInt();
    }

    rx.setPattern(TRACE_POINT + "|" + TRACE_WIDTH);
    matchIter = rx.globalMatch(line);
    while(matchIter.hasNext())
    {
        QString str = matchIter.next().captured();
        //Point
        rx.setPattern(TRACE_POINT);
        match = rx.match(str);
        if (match.hasMatch())
        {
            int layer = match.captured(1).toInt();
            int x = match.captured(2).toInt();
            int y = match.captured(3).toInt();
        }

        //Width
        rx.setPattern(TRACE_WIDTH);
        match = rx.match(str);
        if (match.hasMatch())
        {
            int width = match.captured(1).toInt();
        }

    }


    qDebug() << trace;
}

void DpfReader::parseGroup(const QString& line)
{
    QString name;
    rx.setPattern(groupRx + "(" + STRING + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        name = rmdq(match.captured(1));
    }

    std::vector<int> nums = parsePointList(line, "");
}

void DpfReader::parseTextGroup(const QString& line)
{
    QString type = parseType(line);
    Fcp::Rotation rotate = parseRotationValues(line);
    std::vector<int> place = parsePlaceValues(line);
    QString text = parseText(line);
}

void DpfReader::parseGdsStructureFileGroup(const QString& line)
{
    QString fileName;
    rx.setPattern(gdsStructureNameRx + "(" + STRING + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        fileName = rmdq(match.captured(1));
    }

    //qDebug() << fileName;
}

void DpfReader::parseErpFileGroup(const QString& line)
{
    QString fileName;
    rx.setPattern(erpFileRx + "(" + STRING + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        fileName = rmdq(match.captured(1));
    }

    //qDebug() << fileName;
}

void DpfReader::parseTechnologyFileGroup(const QString& line)
{
    QString fileName;
    rx.setPattern(technologyFileRx + "(" + STRING + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        fileName = rmdq(match.captured(1));
    }

    //qDebug() << fileName;
}

void DpfReader::parseGdsSourceFileGroup(const QString& line)
{
    QString fileName;
    rx.setPattern(gdsSourceFileRx + "(" + STRING + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        fileName = rmdq(match.captured(1));
    }

    //qDebug() << fileName;
}

void DpfReader::parsePatterncellGroup(const QString& line)
{
    QString patterncellName;

    rx.setPattern(patterncellRx + "(" + STRING + ")");
    match = rx.match(line);
    if (match.hasMatch())
    {
        patterncellName = rmdq(match.captured(1));
        //qDebug() << patterncellName;
    }

    std::vector<int> pc = parsePcListValues(line);
    std::vector<int> place = parsePlaceValues(line);
    Fcp::Rotation rotate = parseRotationValues(line);

}

void DpfReader::parsePadcellGroup(const QString& line)
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

void DpfReader::parseBumpGroup(const QString& line)
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

void DpfReader::parseDieGroup(const QString& line)
{
    rx.setPattern(STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString name = rmdq(match.captured());
    }

    std::vector<int> place = parsePlaceValues(line);
    qDebug() << place;
    Fcp::Rotation rotate = parseRotationValues(line);
    qDebug() << rotate;
}

void DpfReader::parseVersionGroup(const QString& line)
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

void DpfReader::parseBumpShapeGroup(const QString& line)
{
    std::vector<int> list = parsePlaceValues(line);
}

void DpfReader::parseTechnologyGroup(const QString& line)
{
    rx.setPattern(STRING);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString technologyName = rmdq(match.captured());
    }
}

void DpfReader::parseCoreAreaGroup(const QString& line)
{
    std::vector<int> points = parsePointList(line, coreAreaRx);
}

QString DpfReader::parseText(const QString& line)
{
    QString textName;

    rx.setPattern(textSubRx + WS + STRING);
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

QString DpfReader::parseType(const QString& line)
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

QString DpfReader::parsePinName(const QString& line)
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

QString DpfReader::parseBumpName(const QString& line)
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
        //qDebug() << bumpName;
    }

    return bumpName;
}

QString DpfReader::parseSignalName(const QString& line)
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

QString DpfReader::parseInstanceName(const QString& line)
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

QString DpfReader::parseMasterName(const QString& line)
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

std::vector<int> DpfReader::parseBpListValues(const QString& line)
{
    return parsePointList(line, bpListRx);
}

std::vector<int> DpfReader::parsePcListValues(const QString& line)
{
    return parsePointList(line, pcListRx);
}

Fcp::Rotation DpfReader::parseRotationValues(const QString& line)
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

int DpfReader::parseRowValues(const QString& line)
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

std::vector<int> DpfReader::parsePlaceValues(const QString& line)
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

        return values;
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

        return values;
    }
}



