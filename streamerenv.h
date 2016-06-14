#ifndef STREAMERENV_H
#define STREAMERENV_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include "entities.h"

namespace Ui {
class StreamerEnv;
}


class StreamerEnviroment : public QObject
{
    Q_OBJECT
public:
    StreamerEnviroment();
    ~StreamerEnviroment();
    void generateEnviroment(int viewerPool, int streamerPool);
    void update(int time, bool newHour);
    StreamerDesc* findStreamer(const ViewerDesc* v);
//private:
    ViewerLibrary * library;
    QList<StreamerDesc*> topStreamers;
    QList<StreamerDesc*> streamers;
    QHash<int, QList<ViewerDesc*> > viewersWakeTime;
    QHash<int, QList<ViewerDesc*> > viewersSleepTime;
    QHash<ViewerDesc*, StreamerDesc*> watchers;
    QList<ViewerDesc*> viewers;

    void sortTopStreamers();
    void quickSortPrivate(int l, int r);

signals:
    void onFollowed(StreamerDesc*, ViewerDesc*);
    void onDecideWatch(StreamerDesc*, ViewerDesc*);
    void onSleep(ViewerDesc*);
};


class StreamerEnv : public QWidget
{
    Q_OBJECT

public:
    explicit StreamerEnv(QWidget *parent = 0);
    ~StreamerEnv();

private slots:
    void on_pushButton_clicked();
    void onTimer();

    void onFollowed(StreamerDesc*s, ViewerDesc* v);
    void onDecideWatch(StreamerDesc* s, ViewerDesc* v);
    void onSleep(ViewerDesc* v);


    void on_pushButton_2_clicked();

    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::StreamerEnv *ui;
    StreamerEnviroment env;
    QTimer timer;
    int currentTime;
};

#endif // STREAMERENV_H
