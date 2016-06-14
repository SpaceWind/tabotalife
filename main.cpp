#include "mainwindow.h"
#include <QApplication>
#include "streamerenv.h"
#include "singleiterationemulate.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SingleIterationEmulate emu;
    emu.show();
    StreamerEnv env;
    env.show();
    return a.exec();
}
