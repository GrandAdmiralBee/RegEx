#include <QCoreApplication>
#include <AmfReader.h>
#include <dpfreader.h>

int main(int argc, char* argv[])
{
    //QCoreApplication a(argc, argv);

    DpfReader* reader = new DpfReader();
    reader->ReadDpf();

//    AmfReader* reader = new AmfReader();
//    reader-> ReadAmf();

    return 0;
}
