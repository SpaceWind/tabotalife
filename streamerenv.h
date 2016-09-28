#ifndef STREAMERENV_H
#define STREAMERENV_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QThread>
#include "entities.h"

namespace Ui {
class StreamerEnv;
}

struct WeekDayHour
{
    bool operator==(const WeekDayHour &other) const;
    int dayOfWeek;
    int hour;
    int value(){return dayOfWeek*24 + hour;}
};

class StreamerEnviroment : public QObject
{
    Q_OBJECT
public:
    StreamerEnviroment();
    ~StreamerEnviroment();
    StreamerDesc* findStreamer(const ViewerDesc* v);
//private:
    ViewerLibrary * library;
    QList<StreamerDesc*> topStreamers;
    QList<StreamerDesc*> streamers;


    QHash<int, QList<ViewerDesc*> > viewersWakeTime;
    QHash<int, QList<ViewerDesc*> > viewersSleepTime;

    QHash<int, QList<StreamerDesc*> > streamersWakeTime;
    QHash<int, QList<StreamerDesc*> > streamersSleepTime;

    QHash<ViewerDesc*, StreamerDesc*> watchers;
    QList<ViewerDesc*> viewers;

    void sortTopStreamers();
    void quickSortPrivate(int l, int r);
public slots:

    void generateEnviroment(int viewerPool, int streamerPool);
    void update(WeekDayHour wdh, bool newHour);

signals:
    void onFollowed(StreamerDesc*, ViewerDesc*);
    void onDecideWatch(StreamerDesc*, ViewerDesc*);
    void onSleep(ViewerDesc*);

    void generated();
    void envUpdated();
};

class StreamerEnviromentThread : public QThread
{
    Q_OBJECT
public:
    StreamerEnviromentThread(QObject *parent = 0);
    ~StreamerEnviromentThread(){delete env;}

    void run();
    void generate(int viewerPool, int streamerPool);
    void update(WeekDayHour wdh, bool newHour);

    StreamerEnviroment * env;
signals:
    void envUpdated();
    void envGenerated();

    void onFollowed(StreamerDesc*, ViewerDesc*);
    void onDecideWatch(StreamerDesc*, ViewerDesc*);
    void onSleep(ViewerDesc*);

    void needUpdate(WeekDayHour wdh, bool newHour);
    void needGenerate(int viewerPool, int streamerPool);

private:
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

    void onGenerated();
    void onUpdated();
    void onEnvCreated();

    QString getStreamTimeDesc(StreamerDesc * s, int dayOfWeek);

    void on_pushButton_3_clicked();

private:
    Ui::StreamerEnv *ui;
    StreamerEnviromentThread * env;
    QTimer timer;
    int currentTime;
    int currentDayOfWeek;
};

#endif // STREAMERENV_H
