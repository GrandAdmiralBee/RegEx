#include "AmfReader.h"

AmfReader::AmfReader()
{

}

int AmfReader::ReadAmf()
{
    // Replacing all double quotes with <">
    QFile f("D:/Users/abdulkin_k/Documents/amf_reader/e3s.amf");
    if (!f.open(QFile::ReadWrite | QFile::Text)) {
        //qCritical() << "Could not open file!";
        return -1;
    }
    QTextStream in(&f);
    QString text = in.readAll();

    text.replace(QString::fromUtf8("\u201C"), QString::fromUtf8("\u0022"));
    text.replace(QString::fromUtf8("\u201D"), QString::fromUtf8("\u0022"));

    f.seek(0);
    f.write(text.toUtf8());
    f.close();
    //

    return parseAmf();
}

int parseAmf()
{
    pAmf = new Fcp::AmfData();

    QFile f("D:/Users/abdulkin_k/Documents/amf_reader/e3s.amf");
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
        }

        //Package group
        rx.setPattern(packageRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parsePackageGroup(line);
        }

        //Package group
        rx.setPattern(componentRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseComponentGroup(line);
        }

        //Itinerary
        rx.setPattern(itineraryRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseItineraryGroup(line);
        }

        //Mode
        rx.setPattern(modeRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseModeGroup(line);
        }

        //Group
        rx.setPattern(groupRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseGroup(line);
        }

        //Signals
        rx.setPattern(signalsRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseSignalsGroup(line);
        }

        //Area
        rx.setPattern(areaRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseAreaGroup(line);
        }

        //Bumps
        rx.setPattern(bumpsRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseBumpsGroup(line);
        }

        //Balls
        rx.setPattern(ballsRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseBallsGroup(line);
        }

        //Assign
        rx.setPattern(assignRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseAssignGroup(line);
        }

        //Trace
        rx.setPattern(traceRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseTraceGroup(line);
        }

        //Layer
        rx.setPattern(layerRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            parseLayerGroup(line);
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

Fcp::PadDestination_t parseDestinationGroup(const QString& line)
{
    //For logic
    rx.setPattern(logicRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        return Fcp::PadDestination_t::DEST_4_LOGIC;
    }

    //For supply
    rx.setPattern(supplyRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        return Fcp::PadDestination_t::DEST_4_SUPPLY;
    }

    //For free
    rx.setPattern(freeRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        return Fcp::PadDestination_t::DEST_4_FREE;
    }

    //Undefined
    return Fcp::PadDestination_t::DEST_UNDEFINED;
}

int parseAutoOrManualGroup(const QString& line)
{
    int manual = -1;

    rx.setPattern(autoRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        manual = 0;
    }
    rx.setPattern(manualRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        manual = 1;
    }
    return manual;
}

std::vector<std::string>* parseListGroup(const QString& line, int offset)
{
    std::vector<std::string>* vec = new std::vector<std::string>();
    rx.setPattern(quotedTextRx);
    matchIter = rx.globalMatch(line);
    if (matchIter.hasNext())
    {
        for (int i = 0; i < offset; i++)
        {
            matchIter.next();
        }
        while(matchIter.hasNext())
        {
            std::string str = rmdq(matchIter.next().captured().trimmed()).toStdString();
            vec->push_back(str);
        }
    }
    return vec;
}

void parseLayerGroup(const QString& line)
{
    QString compId;
    QString bumpId;
    int layerId;
    rx.setPattern(quotedTextRx);
    matchIter = rx.globalMatch(line);
    if (matchIter.hasNext())
    {
        compId = rmdq(matchIter.next().captured().trimmed());
        bumpId = rmdq(matchIter.next().captured().trimmed());
    }

    rx.setPattern(integerRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        layerId = match.captured().toInt();
    }

    pAmf->AddLayer(compId.toLocal8Bit().data(), bumpId.toLocal8Bit().data(), layerId);
}

void parseTraceGroup(const QString& line)
{
    QString compId;
    QString bumpId;
    rx.setPattern(quotedTextRx);
    matchIter = rx.globalMatch(line);
    if (matchIter.hasNext())
    {
        compId = rmdq(matchIter.next().captured().trimmed());
        bumpId = rmdq(matchIter.next().captured().trimmed());
    }


    Fcp::AmfTrace* pTrace = pAmf->AddTrace(compId.toLocal8Bit().data(), bumpId.toLocal8Bit().data());
    int currSegmWidth = 0;

    rx.setPattern(tracePointRx + "|" + widthRx);
    matchIter = rx.globalMatch(line);
    while(matchIter.hasNext())
    {
        QString str = matchIter.next().captured();

        //Trace Point
        rx.setPattern(tracePointRx);
        match = rx.match(str);
        if (match.hasMatch())
        {
            rx.setPattern(INTEGER);
            QRegularExpressionMatchIterator matchIterTemp = rx.globalMatch(str);
            int layer = matchIterTemp.next().captured().toInt();
            int x = matchIterTemp.next().captured().toInt();
            int y = matchIterTemp.next().captured().toInt();
            pTrace->AddPoint(layer, x, y, currSegmWidth);
        }

        //Trace width
        rx.setPattern(widthRx);
        match = rx.match(str);
        if (match.hasMatch())
        {
            rx.setPattern(INTEGER);
            match = rx.match(str);
            if (match.hasMatch())
            {
                currSegmWidth = match.captured().toInt();
                int cPoints = pTrace->CountPoints();
                if(cPoints > 0)
                {
                    pTrace->SetWidthOOfSegmentFromPointAt(cPoints-1, currSegmWidth);
                }
            }
        }
    }
}

void parseAssignGroup(const QString& line){
    QString signalName;
    rx.setPattern(quotedTextRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        signalName = rmdq(match.captured().trimmed());
    }

    bool ball2ball = false;
    bool bump2bump = false;
    bool bump2ball = true;
    int bumpLen = 0;
    int ballLen = 0;
    rx.setPattern(bumpRx);
    matchIter = rx.globalMatch(line);
    if (matchIter.hasNext())
    {
        bumpLen++;
        matchIter.next();
        if (matchIter.hasNext())
        {
           bumpLen++;
           bump2ball = false;
           bump2bump = true;
        }
    }

    rx.setPattern(ballRx);
    matchIter = rx.globalMatch(line);
    if (matchIter.hasNext())
    {
        ballLen++;
        matchIter.next();
        if (matchIter.hasNext())
        {
           ballLen++;
           ball2ball = true;
           bump2ball = false;
        }
    }

    int autoOrManual = parseAutoOrManualGroup(line);

    // For supply
    if (ballLen + bumpLen != 2)
    {
        rx.setPattern(supplyRx);
        match = rx.match(line);
        if (match.hasMatch())
        {

            //AreaName
            QString areaName;

            rx.setPattern(quotedTextRx);
            matchIter = rx.globalMatch(line);
            if (matchIter.hasNext())
            {
                matchIter.next();
                areaName = rmdq(matchIter.next().captured().trimmed());
            }

            //Ball
            rx.setPattern(ballRx);
            match = rx.match(line);
            if (match.hasMatch())
            {
                rx.setPattern(quotedTextRx);
                matchIter = rx.globalMatch(line);
                if (matchIter.hasNext())
                {
                    matchIter.next();
                    matchIter.next();
                    QString name = rmdq(matchIter.next().captured().trimmed());

                    pAmf->AddAssignment4supply(signalName.toLocal8Bit().data(), areaName.toLocal8Bit().data(), name.toLocal8Bit().data(), autoOrManual);
                }
            }

            //Bumps
            rx.setPattern(bumpRx);
            match = rx.match(line);
            if (match.hasMatch())
            {
                rx.setPattern(quotedTextRx);
                matchIter = rx.globalMatch(line);
                if (matchIter.hasNext())
                {
                    matchIter.next();
                    matchIter.next();
                    QString componentId = rmdq(matchIter.next().captured().trimmed());
                    QString name = rmdq(matchIter.next().captured().trimmed());

                    pAmf->AddAssignment4supply(signalName.toLocal8Bit().data(), areaName.toLocal8Bit().data(), componentId.toLocal8Bit().data(), name.toLocal8Bit().data(), autoOrManual);
                }
            }
        }
        return;
    }

    //BUMPS and BALL
    int hidden = 0;
    rx.setPattern(hiddenRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        hidden = 1;
    }

    if (ball2ball)// BALL BALL
    {
        rx.setPattern(quotedTextRx);
        matchIter = rx.globalMatch(line);
        if (matchIter.hasNext())
        {
            matchIter.next();
            QString ballName1 = rmdq(matchIter.next().captured().trimmed());
            QString ballName2 = rmdq(matchIter.next().captured().trimmed());
            pAmf->AddAssignment(signalName.toLocal8Bit().data(), ballName1.toLocal8Bit().data(), ballName2.toLocal8Bit().data(), autoOrManual, hidden);
        }
    }
    else if (bump2bump)// BUMP BUMP
    {
        rx.setPattern(quotedTextRx);
        matchIter = rx.globalMatch(line);
        if (matchIter.hasNext())
        {
            matchIter.next();
            QString componentId1 = rmdq(matchIter.next().captured().trimmed());
            QString bumpName1 = rmdq(matchIter.next().captured().trimmed());
            QString componentId2 = rmdq(matchIter.next().captured().trimmed());
            QString bumpName2 = rmdq(matchIter.next().captured().trimmed());
            pAmf->AddAssignment(signalName.toLocal8Bit().data(), componentId1.toLocal8Bit().data(), bumpName1.toLocal8Bit().data(), componentId2.toLocal8Bit().data(), bumpName2.toLocal8Bit().data(), autoOrManual, hidden);
        }
    }
    else if (bump2ball)//BUMP BALL
    {
        rx.setPattern(quotedTextRx);
        matchIter = rx.globalMatch(line);
        if (matchIter.hasNext())
        {
            matchIter.next();
            QString componentId = matchIter.next().captured();
            QString bumpName = matchIter.next().captured();
            QString ballName = matchIter.next().captured();
            pAmf->AddAssignment(signalName.toLocal8Bit().data(), componentId.toLocal8Bit().data(), bumpName.toLocal8Bit().data(), ballName.toLocal8Bit().data(), autoOrManual, hidden);
        }
    }
}

void parseBallsGroup(const QString& line)
{
    QString areaName;
    rx.setPattern(quotedTextRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        areaName = rmdq(match.captured().trimmed());
    }

    Fcp::PadDestination_t destination = parseDestinationGroup(line);
    int autoOrManual = parseAutoOrManualGroup(line);
    std::vector<std::string>* names = parseListGroup(line, 1);

    pAmf->AddAreaBalls(areaName.toLocal8Bit().data(), autoOrManual, names, destination);
}

void parseBumpsGroup(const QString& line)
{
    QString areaName;
    QString componentId;

    rx.setPattern(quotedTextRx);
    matchIter = rx.globalMatch(line);
    if (matchIter.hasNext())
    {
        areaName = rmdq(matchIter.next().captured().trimmed());
        componentId = rmdq(matchIter.next().captured().trimmed());
    }

    Fcp::PadDestination_t destination = parseDestinationGroup(line);
    int autoOrManual = parseAutoOrManualGroup(line);
    std::vector<std::string>* names = parseListGroup(line, 2);

    pAmf->AddAreaBumps(areaName.toLocal8Bit().data(), autoOrManual, componentId.toLocal8Bit().data(), names, destination);
}

void parseAreaGroup(const QString& line)
{
    QString areaName;
    rx.setPattern(quotedTextRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        areaName = rmdq(match.captured().trimmed());
    }

    //Names
    std::vector<std::string>* names = parseListGroup(line, 1);
    pAmf->AddArea(areaName.toLocal8Bit().data(), names);
}


void parseSignalsGroup(const QString& line)
{
    QString signalName;
    rx.setPattern(quotedTextRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        signalName = rmdq(match.captured().trimmed());
    }

    //Auto
    int autoOrManual = parseAutoOrManualGroup(line);

    //Names
    std::vector<std::string>* names = parseListGroup(line, 1);
    pAmf->AddGroupSignals(signalName.toLocal8Bit().data(), autoOrManual, names);
}

void parseGroup(const QString& line)
{
    rx.setPattern(quotedTextRx);
    matchIter = rx.globalMatch(line);
    if (matchIter.hasNext())
    {
        match = matchIter.next();
        QString group = rmdq(match.captured().trimmed());
        match = matchIter.next();
        QString power = rmdq(match.captured().trimmed());
        match = matchIter.next();
        QString ground = rmdq(match.captured().trimmed());
        pAmf->AddGroup(group.toLocal8Bit().data(), power.toLocal8Bit().data(), ground.toLocal8Bit().data());
    }
}

void parseVersionGroup(const QString& line)
{
    rx.setPattern(INTEGER);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString str = match.captured();
        int version = str.toInt();
        pAmf->SetVersion(version);
    }
    return;
}

void parsePackageGroup(const QString& line)
{
    QString packageName;

    // Package name
    rx.setPattern(quotedTextRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        packageName = rmdq(match.captured()).trimmed();
    }
    else
    {
        return;
    }

    // Descr
    rx.setPattern(descrRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        rx.setPattern(quotedTextRx);
        matchIter = rx.globalMatch(line);
        matchIter.next();
        if (matchIter.hasNext())
        {
            match = matchIter.next();
            QString packageDescr = rmdq(match.captured()).trimmed();
            pAmf->SetPackageDescr(packageName.toLocal8Bit().data(), packageDescr.toLocal8Bit().data());
        }
    }

    // Ref point
    rx.setPattern(refPointRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        rx.setPattern(integerRx);
        matchIter = rx.globalMatch(line);
        if (matchIter.hasNext())
        {
            match = matchIter.next();
            int x = match.captured().toInt();
            match = matchIter.next();
            int y = match.captured().toInt();
            Fcp::Point* point = new Fcp::Point(x, y);
            pAmf->SetPackageRefPoint(packageName.toLocal8Bit().data(), point);
        }
    }

    //Core
    rx.setPattern(coreRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        rx.setPattern(integerRx);
        matchIter = rx.globalMatch(line);
        if (matchIter.hasNext())
        {
            match = matchIter.next();
            int a = match.captured().toInt();
            match = matchIter.next();
            int b= match.captured().toInt();
            match = matchIter.next();
            int c = match.captured().toInt();
            match = matchIter.next();
            int d= match.captured().toInt();
            Fcp::Rect* rect = new Fcp::Rect(a, b, c, d);
            pAmf->SetPackageCore(packageName.toLocal8Bit().data(), rect);
        }
    }
}

void parseComponentGroup(const QString& line)
{
    const char* componentId;
    // Component name
    rx.setPattern(quotedTextRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        componentId = rmdq(match.captured()).trimmed().toLocal8Bit().data();
    }
    else
    {
        return;
    }

    // Type
    rx.setPattern(typeRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        rx.setPattern(quotedTextRx);
        matchIter = rx.globalMatch(line);
        matchIter.next();
        if (matchIter.hasNext())
        {
            match = matchIter.next();
            QString componentType = rmdq(match.captured()).trimmed();
            pAmf->SetComponentType(componentId, componentType.toLocal8Bit().data());
        }
    }

    // Descr
    rx.setPattern(descrRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        rx.setPattern(quotedTextRx);
        matchIter = rx.globalMatch(line);
        matchIter.next();
        if (matchIter.hasNext())
        {
            match = matchIter.next();
            QString componentDescr = rmdq(match.captured()).trimmed();
            pAmf->SetComponentDescr(componentId, componentDescr.toLocal8Bit().data());
        }
    }

    // Format
    rx.setPattern(formatRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        rx.setPattern(quotedTextRx);
        matchIter = rx.globalMatch(line);
        matchIter.next();
        if (matchIter.hasNext())
        {
            match = matchIter.next();
            QString componentFormat = match.captured();
            pAmf->SetComponentFormat(componentId, componentFormat.toLocal8Bit().data());
        }
    }

    // Ref point
    rx.setPattern(refPointRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        rx.setPattern(integerRx);
        matchIter = rx.globalMatch(line);

        if (matchIter.hasNext())
        {
            match = matchIter.next();
            int x = match.captured().toInt();
            match = matchIter.next();
            int y = match.captured().toInt();
            Fcp::Point* point = new Fcp::Point(x, y);
            pAmf->SetComponentRefPoint(componentId, point);
        }
    }

    //Core
    rx.setPattern(coreRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        rx.setPattern(integerRx);
        matchIter = rx.globalMatch(line);
        if (matchIter.hasNext())
        {
            match = matchIter.next();
            int a = match.captured().toInt();
            match = matchIter.next();
            int b= match.captured().toInt();
            match = matchIter.next();
            int c = match.captured().toInt();
            match = matchIter.next();
            int d= match.captured().toInt();
            Fcp::Rect* rect = new Fcp::Rect(a, b, c, d);
            pAmf->SetComponentCore(componentId, rect);
        }
    }

    //Shift
    rx.setPattern(shiftRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        Fcp::Point* point;
        rx.setPattern(integerRx);
        matchIter = rx.globalMatch(line);
        if (matchIter.hasNext())
        {
            match = matchIter.next();
            int x = match.captured().toInt();
            match = matchIter.next();
            int y = match.captured().toInt();
            point = new Fcp::Point(x, y);
        }

        // Rotate
        int rotate = 0;
        rx.setPattern(rotateRx);
        match = rx.match(line);
        if (match.hasMatch())
        {
            match = matchIter.next();
            rotate = match.captured().toInt();
        }

        // OptSide
        int optSide = 0;
        rx.setPattern(bottomRx);
        match = rx.match(line);
        if (match.hasMatch()) optSide = 1;

        pAmf->SetComponentPlacement(componentId, point, rotate, optSide);
    }
}

void parseItineraryGroup(const QString& line)
{
    QString res;
    rx.setPattern(undefinedRouteRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        res = match.captured().trimmed();
    }

    rx.setPattern(fromPackageToDieRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        res = match.captured().trimmed();
    }

    rx.setPattern(fromDieToPackageRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        res = match.captured().trimmed();
    }

    pAmf->SetItinerary(res.toLocal8Bit().data());
}

void parseModeGroup(const QString& line)
{
    rx.setPattern(quotedTextRx);
    match = rx.match(line);
    if (match.hasMatch())
    {
        QString assignmentMode = rmdq(match.captured()).trimmed();
        pAmf->SetAssignmentMode(assignmentMode.toLocal8Bit().data());
    }
}

