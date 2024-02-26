#include "pkgparser.h"
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include "FcpPkgReader.h"
#include "FcpPkgReaderStructs.h"


PkgParser::PkgParser()
{

}

int PkgParser::readPkg()
{
    //callback_ifc = new PKGReaderCallbackI();
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
            int version = (int)match.captured(1).toFloat();
            //callback_ifc->PkgTypeCallback(version);
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
            continue;
        }

        //Extents
        rx.setPattern(extentsRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parseExtents(line);
            continue;
        }

        //Pinlist
        rx.setPattern(pinlistRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parsePinList(textStream);
            continue;
        }

        //PinFunctions
        rx.setPattern(functionsRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parsePinFunctions(textStream);
            continue;
        }

        //Padstack Desc
        rx.setPattern(padstackRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            parsePadstackDescList(textStream);
            continue;
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
            int version = (int)match.captured(1).toFloat();
            //callback_ifc->PkgTypeCallback(version);
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
            int version = (int)match.captured(1).toFloat();
            //callback_ifc->PkgTypeCallback(version);
        }
    }
}

void PkgParser::parsePkgName(const QString& line)
{
    rx.setPattern(nameRx + WSEATER + STRING);
    match = rx.match(line);
    if(match.hasMatch())
    {
        QString name = match.captured(1);
        //callback_ifc->PkgNameCallback(name.toStdString());
    }
}

void PkgParser::parsePitch(const QString& line)
{
    // Pitch X
    rx.setPattern(pitchxRx + WSEATER + "(" + NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {
        float x = match.captured(1).toFloat();
        //callback_ifc->HorizontalPitchCallback(x);
    }

    // Pitch Y
    rx.setPattern(pitchyRx + WSEATER + "(" + NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {
        float y = match.captured(1).toFloat();
        //callback_ifc->VerticalPitchCallback(y);
    }
}

void PkgParser::parseS(const QString& line)
{
    // S X
    rx.setPattern(sxRx + WSEATER + "(" + NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {
        float x = match.captured(1).toFloat();
        //callback_ifc->SxCallback(x);
    }

    // S Y
    rx.setPattern(syRx + WSEATER + "(" + NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {
        float y = match.captured(1).toFloat();
        //callback_ifc->SyCallback(y);
    }
}

void PkgParser::parseUnitsDesc(const QString& line)
{
    rx.setPattern("(" + uMRx + "|" + meterRx + "|"  + dMRx + "|"  + cMRx + "|"  + mMRx + "|"  + nMRx + "|"  + thRx + "|"  + milRx + "),(" + FLOAT_NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {
        int type = Fcp::MEASURE_UNIT_UD;
        if(line.contains(uMRx)){
            type = Fcp::MEASURE_UNIT_UM;
        }
        if(line.contains(dMRx)){
            type = Fcp::MEASURE_UNIT_DM;
        }
        if(line.contains(cMRx)){
            type = Fcp::MEASURE_UNIT_CM;
        }
        if(line.contains(mMRx)){
            type = Fcp::MEASURE_UNIT_MM;
        }
        if(line.contains(nMRx)){
            type = Fcp::MEASURE_UNIT_NM;
        }
        if(line.contains(thRx)){
            type = Fcp::MEASURE_UNIT_TH;
        }
        if(line.contains(milRx)){
            type = Fcp::MEASURE_UNIT_MIL;
        }
        if(line.contains(meterRx)){
            type = Fcp::MEASURE_UNIT_METER;
        }

        double units = GetUmInUnit(type);
        //callback_ifc->UnitsCallback(type, units);
        float precision = match.captured(2).toFloat();
        //callback_ifc->PrecisionCallback((int)precision);
    }

    rx.setPattern("(" + FLOAT_NUM + "),(" + FLOAT_NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {
        int unitType = (int)match.captured(1).toFloat();
        double units = GetUmInUnit(unitType);

        //callback_ifc->UnitsCallback(type, units);
        float precision = match.captured(4).toFloat();
        //callback_ifc->PrecisionCallback((int)precision);
    }

    rx.setPattern(udRx + WSEATER + assignRx + WSEATER +"(" + FLOAT_NUM + "),(" + FLOAT_NUM + ")");
    match = rx.match(line);
    if(match.hasMatch())
    {
        int unitType = Fcp::MEASURE_UNIT_UD;
        double units = match.captured(1).toFloat();
        //callback_ifc->UnitsCallback(type, units);
        float precision = match.captured(4).toFloat();
        //callback_ifc->PrecisionCallback((int)precision);
    }
}

void PkgParser::parseExtents(const QString& line)
{
    rx.setPattern("(" + FLOAT_NUM + "),(" + FLOAT_NUM + ")");
    matchIter = rx.globalMatch(line);
    if(matchIter.hasNext())
    {
        ME_point p1, p2;
        match = matchIter.next();
        p1.x = match.captured(1).toInt();
        p1.y = match.captured(4).toInt();

        match = matchIter.next();
        p2.x = match.captured(1).toInt();
        p2.y = match.captured(4).toInt();
        //callback_ifc->ExtentsCallback(p1, p2);
    }
}

void PkgParser::parsePartNumber(const QString& line)
{
    rx.setPattern(numberRx + WSEATER + STRING);
    match = rx.match(line);
    if(match.hasMatch())
    {
        QString num = match.captured(1);
        callback_ifc->PartNumberCallback(num.toStdString());
    }
}

void PkgParser::parsePadstackDescList(QTextStream& in)
{
    QString prevLine;
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
            QString figure = match.captured();
            qDebug() << figure;
            //callback_ifc->PadstackElementLayer(figure.toStdString());
            parseCircle(in);
        }

        rx.setPattern(rectRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            QString figure = match.captured();
            qDebug() << figure;
            //callback_ifc->PadstackElementLayer(figure.toStdString());
            parseRect(in);
        }

        rx.setPattern(regPolyRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            QString figure = match.captured();
            qDebug() << figure;
            //callback_ifc->PadstackElementLayer(figure.toStdString());
            parseRegPoly(in);
        }

        rx.setPattern(polyRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            QString figure = match.captured();
            qDebug() << figure;
            //callback_ifc->PadstackElementLayer(figure.toStdString());
            parsePoly(in);
        }

        rx.setPattern(polyArcRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            QString figure = match.captured();
            qDebug() << figure;
            //callback_ifc->PadstackElementLayer(figure.toStdString());
            parsePolyArc(in);
        }

        prevLine = line;
    }
}

void PkgParser::parsePolyArc(QTextStream& in)
{
    PolyArcInfo poly;
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
            poly.cut = cut;
        }

        rx.setPattern(lineColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color color = parseColor(line);
            poly.line_color = color;
        }

        rx.setPattern(lineWidthRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float width = parseCoord(line);
            poly.line_width = width;
        }

        rx.setPattern(fillingColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color color = parseColor(line);
            poly.filling_color = color;
        }

        rx.setPattern(fillRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool fill = parseBool(line);
            poly.fill = fill;
        }

        rx.setPattern(locationRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_point location = parsePoint(line);
            poly.position = location;
        }

        rx.setPattern(pointsRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            Segments segments = parseSegments(line);
            if(segments.points.size() == 4)
            {
                segments.points.pop_back();
            }
            std::vector<ME_polyarc_segment> *pp = new std::vector<ME_polyarc_segment>();
        ME_polyarc
            poly.points = points;
        }
    }

    //callback_ifc->PadstackElemBumpPolyCallback(poly.cut, poly.line_color, poly.line_width, poly.filling_color, poly.fill, poly.position, poly.points);
    //callback_ifc->PadstackElementPolyCallback(poly.cut, poly.line_color, poly.line_width, poly.filling_color, poly.fill, poly.position, poly.points);
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
            qDebug() << cut;
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
    RegPolyInfo poly;
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
            poly.cut = cut;
        }

        rx.setPattern(lineColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color color = parseColor(line);
            poly.line_color = color;
        }

        rx.setPattern(lineWidthRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float width = parseCoord(line);
            poly.line_width = width;
        }

        rx.setPattern(fillingColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color color = parseColor(line);
            poly.filling_color = color;
        }

        rx.setPattern(fillRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool fill = parseBool(line);
            poly.fill = fill;
        }

        rx.setPattern(locationRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_point location = parsePoint(line);
            poly.position = location;
        }

        rx.setPattern(vertexCountRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float vertexCount = parseCoord(line);
            poly.vertex_count = vertexCount;
        }

        rx.setPattern(radiusRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float radius = parseCoord(line);
            poly.radius = radius;
        }

        rx.setPattern(angleRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float angle = parseCoord(line);
            poly.rotation = angle;
        }

        rx.setPattern(typeRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool type = parseBool(line);
            poly.InsertedCircle = type;
        }

        rx.setPattern(angleTypeRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool angleType = parseBool(line);
            poly.RotationToEdgeNormalFlag = angleType;
        }
    }

    //callback_ifc->PadstackElemBumpRegPolyCallback(poly.cut, poly.line_color, poly.line_width, poly.filling_color, poly.fill, poly.position, poly.radius, poly.vertex_count, poly.rotation, poly.InsertedCircle, poly.RotationToEdgeNormalFlag);;
    //callback_ifc->PadstackElementRegPolyCallback(poly.cut, poly.line_color, poly.line_width, poly.filling_color, poly.fill, poly.position, poly.radius, poly.vertex_count, poly.rotation, poly.InsertedCircle, poly.RotationToEdgeNormalFlag);;
}

void PkgParser::parseRect(QTextStream& in)
{
    bool point = false;
    RectangleInfo rect;
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
            rect.cut = cut;
        }

        rx.setPattern(lineColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color color = parseColor(line);
            rect.line_color = color;
        }

        rx.setPattern(lineWidthRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float width = parseCoord(line);
            rect.line_width = width;
        }

        rx.setPattern(fillingColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color color = parseColor(line);
            rect.filling_color = color;
        }

        rx.setPattern(fillRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool fill = parseBool(line);
            rect.fill = fill;
        }

        rx.setPattern(locationRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_point location = parsePoint(line);
            rect.position = location;
        }

        rx.setPattern(point2Rx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_point point2 = parsePoint(line);
            if(point){
                rect.p1.x = rect.p2.x;
                rect.p1.y = rect.p2.y;
                rect.p2.x = rect.p1.x + point2.x;
                rect.p2.y = rect.p1.y + point2.y;
            } else
            {
                rect.p1.x = 0.0;
                rect.p1.y = 0.0;
                rect.p2 = point2;
            }
            point = true;
        }
    }

    //callback_ifc->PadstackElemBumpRectangleCallback(rect.cut,rect.line_color, rect.line_width, rect.filling_color, rect.fill, rect.position, rect.p1, rect.p2);
    //callback_ifc->PadstackElementRectangleCallback(rect.cut,rect.line_color, rect.line_width, rect.filling_color, rect.fill, rect.position, rect.p1, rect.p2);
}

void PkgParser::parseCircle(QTextStream& in)
{
    CircleInfo circle;
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
            circle.cut = cut;
        }

        rx.setPattern(lineColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color color = parseColor(line);
            circle.line_color = color;
        }

        rx.setPattern(lineWidthRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float width = parseCoord(line);
            circle.line_width = width;
        }

        rx.setPattern(fillingColorRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_color color = parseColor(line);
            circle.filling_color = color;
        }

        rx.setPattern(fillRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            bool fill = parseBool(line);
            circle.fill = fill;
        }

        rx.setPattern(locationRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            ME_point location = parsePoint(line);
            circle.position = location;
        }

        rx.setPattern(radiusRx);
        match = rx.match(line);
        if(match.hasMatch())
        {
            float radius = parseCoord(line);
            circle.radius = radius;
        }
    }

    //callback_ifc->PadstackElemBumpCircleCallback(circle.cut, circle.line_color, circle.line_width, circle.filling_color, circle.fill, circle.position, circle.radius);
    //callback_ifc->PadstackElementCircleCallback(circle.cut, circle.line_color, circle.line_width, circle.filling_color, circle.fill, circle.position, circle.radius);
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

ME_color PkgParser::parseColor(const QString& line)
{
    ME_color color;
    rx.setPattern(leftP + "(" + FLOAT_NUM + ")," + WSEATER + "(" + FLOAT_NUM + "),"  + WSEATER +  "(" + FLOAT_NUM + ")" + rightP);
    match = rx.match(line);
    if(match.hasMatch())
    {
        float r = match.captured(1).toFloat();
        float g = match.captured(4).toFloat();
        float b = match.captured(7).toFloat();

        color.R = (char)r;
        color.G = (char)g;
        color.B = (char)b;
    }

    return color;
}

std::vector<ME_point> PkgParser::parsePoints(const QString& line)
{
    std::vector<ME_point> points;
    rx.setPattern(leftP + "(" + FLOAT_NUM + ")," + WSEATER + "(" + FLOAT_NUM + ")" + rightP);
    matchIter = rx.globalMatch(line);
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

    return points;
}

ME_point PkgParser::parsePoint(const QString& line)
{
    ME_point point;
    rx.setPattern(leftP + "(" + FLOAT_NUM + ")," + WSEATER + "(" + FLOAT_NUM + ")" + rightP);
    match = rx.match(line);
    if(match.hasMatch())
    {
        float x = match.captured(1).toFloat();
        float y = match.captured(4).toFloat();

        point.x = x;
        point.y = y;
    }

    return point;
}

ME_polyarc_segment PkgParser::parseSegments(const QString& line)
{
    ME_polyarc_segment segments;
    bool arc = false;
    //Determine segment type
    rx.setPattern(R"(.+\),)");
    match = rx.match(line);
    if(match.hasMatch())
    {
        QString str = match.captured();
        rx.setPattern(FLOAT_NUM);
        matchIter = rx.globalMatch(str);
        int count = 0;
        while(matchIter.hasNext())
        {
            matchIter.next();
            count++;
        }
        if (count % 3 == 0){
            arc = true;
        }else{
            arc = false;
        }
    }

    if (arc)
    {
    //Arc segments
        rx.setPattern(leftP + "(" + FLOAT_NUM + ")," + WSEATER + "(" + FLOAT_NUM + ")" + rightP);
    std::vector<ME_point> points;
        matchIter = rx.globalMatch(line);
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

        segments.set()
    } else {

        //Linera segments
        rx.setPattern(leftP + "(" + FLOAT_NUM + ")," + WSEATER + "(" + FLOAT_NUM + ")" + rightP);

        matchIter = rx.globalMatch(line);
        while(matchIter.hasNext())
        {
            match = matchIter.next();
            float x = match.captured(1).toFloat();
            float y = match.captured(4).toFloat();

            ME_point point;
            point.x = x;
            point.y = y;
            segments.points.push_back(point);
        }
    }

    return segments;
}

double PkgParser::GetUmInUnit(int units) {
  switch (units) {
  case 0: // meter
    return 1.0E6;
  case 1: // dM
    return 1.0E5;
  case 2: // cM
    return 1.0E4;
  case 3: // mM
    return 1.0E3;
  case 4: // uM
    return 1.0;
  case 5: // nM
    return 1.0E-3;
  case 6: // th
    return 0.254E4;
  case 7: // Mil
    return 0.254E2;
  case 8: // user defined
    return 1.0E2;
  default:
    return 0;
  }
}
