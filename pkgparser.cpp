#include "pkgparser.h"
#include <QTextStream>
#include <QDebug>
#include <QFile>



PkgParser::PkgParser()
{

}

int PkgParser::readPkg()
{
    return parsePkg();
}

int PkgParser::parsePkg()
{
    QFile f("D:/Users/abdulkin_k/Documents/RegEx/bga_test.pkg");
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        qCritical() << "Could not open file!";
        return -1;
    }

    QTextStream in(&f);
    while (!in.atEnd())
    {
        QString line = in.readLine();

        //Pkg file
        rx.setPattern(pkgRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            in.seek(0);
            parsePkgFile(in);
        }

        //Fpt file
        rx.setPattern(fptRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            in.seek(0);
            parseFptFile(in);
        }

        //Pst file
        rx.setPattern(pstRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            in.seek(0);
            parsePstFile(in);
        }
    }

    f.close();
    return 0;
}

void PkgParser::parsePkgFile(QTextStream& textStream)
{
    while(!textStream.atEnd())
    {
        QString line = textStream.readLine();

        //Title
        rx.setPattern(pkgRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            float version = match.captured(1).toFloat();
            continue;
        }

        //Name
        rx.setPattern(nameRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parsePkgName(line);
            continue;
        }

        //Pitch
        parsePitch(line);

        //S
        parseS(line);

        //Units
        rx.setPattern(unitsRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parseUnitsDesc(line);
        }

        //Extents
        rx.setPattern(extentsRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parseExtents(line);
        }

        //Pinlist
        rx.setPattern(pinlistRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parsePinList(textStream);
        }

        //PinFunctions
        rx.setPattern(functionsRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parsePinFunctions(textStream);
        }
    }
}

void PkgParser::parseFptFile(QTextStream& textStream)
{
    while(!textStream.atEnd())
    {
        QString line = textStream.readLine();

        //Title
        rx.setPattern(fptRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            float version = match.captured(1).toFloat();
        }

        //Units
        rx.setPattern(unitsRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parseUnitsDesc(line);
        }

        //Part Number
        rx.setPattern(numberRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parsePartNumber(line);
        }

        //Name
        rx.setPattern(nameRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parsePkgName(line);
            continue;
        }

        //Extents
        rx.setPattern(extentsRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parseExtents(line);
        }
    }
}

void PkgParser::parsePstFile(QTextStream& textStream)
{
    while(!textStream.atEnd())
    {
        QString line = textStream.readLine();

        //Title
        rx.setPattern(pstRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            float version = match.captured(1).toFloat();
        }
    }
}

void PkgParser::parsePkgName(const QString& line)
{
    rx.setPattern(STRING);
    match = rx.match(line);
    if(match.hasMatch())
    {
        qDebug() << match.captured(1);
    }
}

void PkgParser::parsePitch(const QString& line)
{
    // Pitch X
    rx.setPattern(pitchxRx + WSEATER + "(" + NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {
        qDebug() << match.captured(1);
    }

    // Pitch Y
    rx.setPattern(pitchyRx + WSEATER + "(" + NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {
        qDebug() << match.captured(1);
    }
}

void PkgParser::parseS(const QString& line)
{
    // S X
    rx.setPattern(sxRx + WSEATER + "(" + NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {
        qDebug() << match.captured(1);
    }

    // S Y
    rx.setPattern(syRx + WSEATER + "(" + NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {
        qDebug() << match.captured(1);
    }
}

void PkgParser::parseUnitsDesc(const QString& line)
{
    rx.setPattern("(" + uMRx + "|"  + dMRx + "|"  + cMRx + "|"  + mMRx + "|"  + nMRx + "|"  + thRx + "|"  + milRx + "),(" + FLOAT_NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {
        qDebug() << match.captured(1) << match.captured(2);
    }

    rx.setPattern("(" + FLOAT_NUM + "),(" + FLOAT_NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {

    }

    rx.setPattern(udRx + WSEATER + assignRx + WSEATER +"(" + FLOAT_NUM + "),(" + FLOAT_NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {

    }
}

void PkgParser::parseExtents(const QString& line)
{
    rx.setPattern("(" + NUM + "),(" + NUM + ")");
    matchIter = rx.globalMatch(line);
    if(matchIter.hasNext())
    {
        match = matchIter.next();
        int x = match.captured(1).toInt();
        int y = match.captured(2).toInt();

        match = matchIter.next();
        int a = match.captured(1).toInt();
        int b = match.captured(2).toInt();
        qDebug() << x << y << a << b;
    }
}

void PkgParser::parsePartNumber(const QString& line)
{
    rx.setPattern(numberRx + WSEATER + STRING);
    match = rx.match(line);
    if(match.hasMatch())
    {
        qDebug() << match.captured(1);
    }
}

void PkgParser::parsePadstackDescList(QTextStream& in)
{
    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(line.startsWith(']')){
            break;
        }

        rx.setPattern(circleRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parseCircle(in);
        }

        rx.setPattern(rectRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parseRect(in);
        }

        rx.setPattern(regPolyRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parseRegPoly(in);
        }

        rx.setPattern(polyRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parsePoly(in);
        }

        rx.setPattern(polyArcRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parsePolyArc(in);
        }
    }
}

void PkgParser::parsePoly(QTextStream& in)
{
    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(line.contains('}')){
            break;
        }

        rx.setPattern(cutRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool cut = parseBool(line);
        }

        rx.setPattern(lineColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color* color = parseColor(line);
        }

        rx.setPattern(lineWidthRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float width = parseCoord(line);
        }

        rx.setPattern(fillingColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color* color = parseColor(line);
        }

        rx.setPattern(fillRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool fill = parseBool(line);
        }

        rx.setPattern(locationRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_point* location = parsePoint(line);
        }

        rx.setPattern(pointsRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            std::vector<ME_point> points = parsePoints(line);
        }

    }
}

void PkgParser::parseRegPoly(QTextStream& in)
{
    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(line.contains('}')){
            break;
        }

        rx.setPattern(cutRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool cut = parseBool(line);
        }

        rx.setPattern(lineColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color* color = parseColor(line);
        }

        rx.setPattern(lineWidthRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float width = parseCoord(line);
        }

        rx.setPattern(fillingColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color* color = parseColor(line);
        }

        rx.setPattern(fillRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool fill = parseBool(line);
        }

        rx.setPattern(locationRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_point* location = parsePoint(line);
        }

        rx.setPattern(vertexCountRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float vertexCount = parseCoord(line);
        }

        rx.setPattern(radiusRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float radius = parseCoord(line);
        }

        rx.setPattern(angleRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float angle = parseCoord(line);
        }

        rx.setPattern(typeRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool type = parseBool(line);
        }

        rx.setPattern(angleTypeRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool angleType = parseBool(line);
        }
    }
}

void PkgParser::parseRect(QTextStream& in)
{
    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(line.contains('}')){
            break;
        }

        rx.setPattern(cutRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool cut = parseBool(line);
        }

        rx.setPattern(lineColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color* color = parseColor(line);
        }

        rx.setPattern(lineWidthRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float width = parseCoord(line);
        }

        rx.setPattern(fillingColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color* color = parseColor(line);
        }

        rx.setPattern(fillRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool fill = parseBool(line);
        }

        rx.setPattern(locationRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_point* location = parsePoint(line);
        }

        rx.setPattern(point2Rx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_point* point2 = parsePoint(line);
        }

    }
}

void PkgParser::parseCircle(QTextStream& in)
{
    while(!in.atEnd())
    {
        QString line = in.readLine();
        if  (line.contains('}')){
            break;
        }

        rx.setPattern(cutRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool cut = parseBool(line);
        }

        rx.setPattern(lineColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color* color = parseColor(line);
        }

        rx.setPattern(lineWidthRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float width = parseCoord(line);
        }

        rx.setPattern(fillingColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color* color = parseColor(line);
        }

        rx.setPattern(fillRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool fill = parseBool(line);
        }

        rx.setPattern(locationRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_point* location = parsePoint(line);
        }

        rx.setPattern(radiusRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float radius = parseCoord(line);
        }
    }
}

bool PkgParser::parseBool(const QString& line)
{
    bool res;
    rx.setPattern("true|false");
    match = rx.match(line);
    if(match.hasMatch())
    {
        QString str = match.captured();
        if(str=="true"){
            res = true;
        }
    }
    return res;
}

void PkgParser::parsePinFunctions(QTextStream& in)
{
    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(line.startsWith(']')){
            break;
        }

        QString notWS(R"((\S+))");
        QString pattern(notWS + WS + notWS + WS + notWS + WS + notWS);
        rx.setPattern(pattern);
        match = rx.match(line);
        if(match.hasMatch())
        {
            QString signalName = match.captured(1);
            QString signalActiveLevel = match.captured(2);
            QString pinDirection = match.captured(3);
            QString signalFunction = match.captured(4);
        }
    }
}

void PkgParser::parsePinList(QTextStream& in)
{
    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(line.startsWith(']')){
            break;
        }

        QString notWS(R"((\S+))");
        QString pattern(notWS + WS + notWS + WS + notWS + WS + notWS + WS + notWS + WS + notWS + WS + notWS + WS + notWS + WS + notWS + WS + notWS);
        rx.setPattern(pattern);
        match = rx.match(line);
        if(match.hasMatch())
        {
            // Записываем в строки, иначе все ломается
            QString pinName = match.captured(1);
            QString pinDir = match.captured(2);
            QString padstackName = match.captured(3);
            QString xCoord = match.captured(4);
            QString yCoord = match.captured(5);
            QString signalName = match.captured(6);
            QString pinColor = match.captured(7);
            QString groupName = match.captured(8);
            QString instName = match.captured(9);
            QString name = match.captured(10);
        }
    }
}

float PkgParser::parseCoord(const QString& line)
{
    float res;
    rx.setPattern(FLOAT_NUM);
    match = rx.match(line);
    if(match.hasMatch())
    {
        res = match.captured().toFloat();
    }

    return res;
}

PkgParser::SignalDir PkgParser::parsePinDir(const QString& line)
{
    SignalDir dir = SignalDir::UNDEF;

    rx.setPattern(inRx);
    match = rx.match(line);
    if(match.hasMatch())
    {
        dir = SignalDir::IN;
    }

    rx.setPattern(outRx);
    match = rx.match(line);
    if(match.hasMatch())
    {
        dir = SignalDir::OUT;
    }

    rx.setPattern(inoutRx);
    match = rx.match(line);
    if(match.hasMatch())
    {
        dir = SignalDir::INOUT;
    }

    return dir;
}

QString PkgParser::parseName(const QString& line)
{
    QString str;
    rx.setPattern(STRING);
    match = rx.match(line);
    if(match.hasMatch())
    {
        str = match.captured(1);
    }

    return str;
}

ME_color* PkgParser::parseColor(const QString& line)
{
    ME_color* color = nullptr;
    rx.setPattern(leftP + "(" + FLOAT_NUM + ")," + WSEATER + "(" + FLOAT_NUM + "),"  + WSEATER +  "(" + FLOAT_NUM + ")" + rightP);
    match = rx.match(line);
    if(match.hasMatch())
    {
        float r = match.captured(1).toFloat();
        float g = match.captured(4).toFloat();
        float b = match.captured(7).toFloat();

        color = new ME_color();
        color->R = (char)r;
        color->G = (char)g;
        color->B = (char)b;
    }

    return color;
}

std::vector<ME_point> PkgParser::parsePoints(const QString& line)
{
    std::vector<ME_point> points;
    QString test("((1, 2) (3, 4), (5, 6), (7, 8))");
    rx.setPattern(leftP + "(" + FLOAT_NUM + ")," + WSEATER + "(" + FLOAT_NUM + ")" + rightP);
    matchIter = rx.globalMatch(test);
    while(matchIter.hasNext())
    {
        match = matchIter.next();
        float x = match.captured(1).toFloat();
        float y = match.captured(4).toFloat();
        ME_point point;
        point.x = x;
        point.y = y;
        points.push_back(point);
    }

    qDebug() << points;
}

ME_point* PkgParser::parsePoint(const QString& line)
{
    ME_point* point = nullptr;
    rx.setPattern(leftP + "(" + FLOAT_NUM + ")," + WSEATER + "(" + FLOAT_NUM + ")" + rightP);
    match = rx.match(line);
    if(match.hasMatch())
    {
        float x = match.captured(1).toFloat();
        float y = match.captured(4).toFloat();

        point = new ME_point();
        point->x = x;
        point->y = y;
    }

    return point;
}
