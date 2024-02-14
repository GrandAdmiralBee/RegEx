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

    parseColor("");

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

void PkgParser::parsePinList(QTextStream& in)
{
    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(line.startsWith(']')){
            break;
        }

        QString pinNameToSignalName(STRING + WSEATER + "(" + "|" + undefRx + "|" + inoutRx + "|" + inRx + "|" + outRx + ")" +  WSEATER + STRING + WSEATER + "(" + FLOAT_NUM + ")" + WSEATER + "(" + FLOAT_NUM + ")");
        QString signalName("(" + STRING + "|" + undefRx + ")");
        QString pinColorDescr(leftP + FLOAT_NUM + "," + FLOAT_NUM + "," + FLOAT_NUM + rightP);
        QString groupName("(" + STRING + "|" + undefRx + ")");
        QString pinMasterCellInstName("(" + STRING + "|" + undefRx + ")");
        QString pinMasterCellName("(" + STRING + "|" + undefRx + ")");
        QString RX(pinNameToSignalName + WSEATER + signalName + WSEATER + pinColorDescr + WSEATER + groupName  + WSEATER + pinMasterCellInstName + WSEATER + pinMasterCellName);
        rx.setPattern(RX);
        match = rx.match(line);
        if(match.hasMatch())
        {
            qDebug() << match.captured();
            qDebug() << match.captured(1) << match.captured(2) << match.captured(3) << match.captured(4) << match.captured(7) << match.captured(10);
        }
    }
}

void PkgParser::parseColor(const QString& line)
{
    rx.setPattern(leftP + "(" + FLOAT_NUM + ")," + WSEATER + "(" + FLOAT_NUM + "),"  + WSEATER +  "(" + FLOAT_NUM + ")" + rightP);
    match = rx.match(line);
    if(match.hasMatch())
    {
        float r = match.captured(1).toFloat();
        float g = match.captured(4).toFloat();
        float b = match.captured(7).toFloat();
    }
}
