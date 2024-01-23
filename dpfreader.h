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

    enum KeyValuesType {
        K_PLACE_RECT,
        K_PLACE_POINT,
        K_ROTATION,
        K_MASTERNAME,
        K_INSTANCENAME,
        K_SIGNAL_NAME,
        K_BUMP_NAME,
        K_PIN_NAME,
        K_CELL_NUM,
        K_BP_LIST,
        K_ROW,
        K_PC_LIST,
        K_STAGGERED,
        K_TYPE,
        K_TEXT,
        K_JUSTIFY
    };

    struct KeyValues // Used for SubKeys reading;
    {
        KeyValuesType		KeyName;
        std::vector <int>	Place;
        std::vector <int>	BumpsList;
        std::vector <int>	PadCellList;
        std::string			MasterName;
        std::string			InstanceName;
        std::string			SignalName;
        std::string			BumpName;
        std::string			PinName;
        std::string			FilePath;
        std::string			Text;
        std::string			TextType;
        int					Row;
        //Rotation			Rotate;
        //Justification		Justify;
        bool				Staggered;
        //KeyValues():KeyName(K_TYPE),Row(0),Rotate(ROT_0),Justify(JUST_NO),Staggered(false){}
    };
};

#endif // DPFREADER_H
