#include <QCoreApplication>
#include <AmfReader.h>
#include <dpfreader.h>
#include "pkgparser.h"

int main(int argc, char* argv[])
{
    //QCoreApplication a(argc, argv);

    PkgParser* reader = new PkgParser();
    reader->readPkg();

//    AmfReader* reader = new AmfReader();
//    reader-> ReadAmf();

    return 0;
}
