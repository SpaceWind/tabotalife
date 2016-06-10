#ifndef STREAMERENV_H
#define STREAMERENV_H

#include <QWidget>
#include "entities.h"

namespace Ui {
class StreamerEnv;
}


class StreamerEnviroment
{
public:
    StreamerEnviroment();
    ~StreamerEnviroment();
    void generateEnviroment(int viewerPool, int streamerPool);
    void update(int time, bool newHour);
    StreamerDesc* findStreamer(const ViewerDesc* v);
private:
    ViewerLibrary * library;
    QList<StreamerDesc*> topStreamers;
    QList<StreamerDesc*> streamers;
    QHash<int, QList<ViewerDesc*> > viewersWakeTime;
    QHash<int, QList<ViewerDesc*> > viewersSleepTime;
    QHash<ViewerDesc*, StreamerDesc*> watchers;
    QList<ViewerDesc*> viewers;
};


class StreamerEnv : public QWidget
{
    Q_OBJECT

public:
    explicit StreamerEnv(QWidget *parent = 0);
    ~StreamerEnv();

private:
    Ui::StreamerEnv *ui;
};

#endif // STREAMERENV_H
