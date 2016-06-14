#include "streamerenv.h"
#include "ui_streamerenv.h"
#include <QDebug>

StreamerEnv::StreamerEnv(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StreamerEnv)
{
    ui->setupUi(this);
}

StreamerEnv::~StreamerEnv()
{
    delete ui;
}

StreamerEnviroment::StreamerEnviroment()
{
    library = new ViewerLibrary("");
}

StreamerEnviroment::~StreamerEnviroment()
{

}

void StreamerEnviroment::generateEnviroment(int viewerPool, int streamerPool)
{
    for (int i = 0; i< viewerPool; ++i)
    {
        ViewerDesc * v = ViewerDesc::generateDynamic(library);
        viewersWakeTime[v->viewTime[0].timeStart].append(v);
        viewersSleepTime[v->viewTime[0].timeEnd].append(v);
        viewers.append(v);
    }
    for (int i = 0; i < streamerPool; i++)
    {
        StreamerDesc * s = StreamerDesc::generateDynamic(library);
        topStreamers.append(s);
        streamers.append(s);
    }
}

bool compareStreamerByViewers(const StreamerDesc &s1, const StreamerDesc &s2)
{
    return s1.currentViewers > s2.currentViewers;
}

void StreamerEnviroment::update(int time, bool newHour)
{
    if (newHour)
    {

        QList<ViewerDesc*> currentViewersWakeTime;
        QList<ViewerDesc*> currentViewersSleepTime;
        if (viewersWakeTime.contains(time)) currentViewersWakeTime = viewersWakeTime[time];
        if (viewersSleepTime.contains(time)) currentViewersSleepTime = viewersSleepTime[time];

        for (int i = 0; i<currentViewersWakeTime.count(); ++i)
        {
            ViewerDesc * v = currentViewersWakeTime[i];
            StreamerDesc * s = findStreamer(v);
            s->currentViewers++;
            if (watchers.contains(v))
                watchers.remove(v);
            watchers[v] = s;
            qDebug() << "NH: " << v;
            emit onDecideWatch(s,v);
        }
        for (int i = 0; i<currentViewersSleepTime.count(); ++i)
        {
             ViewerDesc * v = currentViewersSleepTime[i];
             if (watchers.contains(v))
             {
                 watchers[v]->currentViewers--;
                 watchers.remove(v);
                 emit onSleep(v);
             }
        }
    }
    QList<ViewerDesc*> keys = watchers.keys();
    for (QList<ViewerDesc*>::iterator it = keys.begin(); it != keys.end(); ++it)
    {
        ViewerDesc * v = (*it);
        auto str = watchers[v];
        qDebug() << str << "V=" << v;
        v->watchTime[watchers[v]] += 0.5;
        if (v->watchTime[watchers[v]] > 8.0)
        {
            watchers[v]->follow(v);
            v->followed.append(watchers[v]);
            emit onFollowed(watchers[v], v);
        }
        if (rand()%10 == 0) //should depend on qiality of stream!
        {
            watchers[v]->currentViewers--;
            watchers.remove(v);
            StreamerDesc * s = findStreamer(v);
            s->currentViewers++;
            watchers[v] = s;
        }
    }
    sortTopStreamers();
}

StreamerDesc *StreamerEnviroment::findStreamer(const ViewerDesc *v)
{
  //  A.1 find one in followed list (weighted random) and return him. Or if proc - instead go to next step;
  //  B.1 find one in all list
  //  B.2 Took top SR% items from topStreamers and find best one. OR if proc SR*2% - go to C
  //  C.1 Took RANDOM pool*0.2
  //  3. Ищем по top SR%, но при этом есть шанс SR*2%, что юзер пойдет искать, выбирая рандомно pool*0.2 стримов, вместо поиска по лучшим.

    if (FRAND > v->searchingRate/75.)
    {
        //QHash<StreamerDesc*, double> followingResult;
        std::map<double, StreamerDesc*> followingResult;
        double minValue = 100000., maxValue = -10000.0;
        for (auto it = v->followed.begin(); it != v->followed.end(); ++it)
        {
            StreamerDesc * s = (*it);
            double value = s->test(*v);
            if (value < minValue) minValue = value;
            if (value > maxValue) maxValue = value;
            followingResult[s->test(*v)] = s;
        }
        if (!followingResult.empty())
            return followingResult.upper_bound(FRAND*(maxValue - minValue) - minValue)->second;
    }
    if (FRAND > v->searchingRate/50.)
    {
        int maxCount = v->searchingRate/100.*topStreamers.count() + 10;
        StreamerDesc * foundStreamer = 0;
        double value = -1000000.0;
        for(int i = 0; i < maxCount; ++i)
        {
            double currentValue = topStreamers[i]->test(*v);
            if (currentValue > value)
            {
                value = currentValue;
                foundStreamer = topStreamers[i];
            }
        }
        if (foundStreamer)
            return foundStreamer;
    }


    StreamerDesc * foundStreamer = 0;
    double value = -10000.0;
    int pickCount = streamers.count()/5;

    bool getFound = false;
    for (int i = 0; i<pickCount; ++i)
    {
        int itemIndex = rand()%streamers.count();
        double currentValue = streamers[itemIndex]->test(*v);
        if (currentValue > value)
        {
            value = currentValue;
            foundStreamer = streamers[itemIndex];
            getFound = true;
        }
        if (!getFound)
        {
            std::abort();
        }
    }
    return foundStreamer;
}

void StreamerEnviroment::sortTopStreamers()
{
    quickSortPrivate(0,topStreamers.count() - 1);
}

void StreamerEnviroment::quickSortPrivate(int l, int r)
{
    int x = topStreamers[l + (r - l) / 2]->currentViewers;
    int i = l;
    int j = r;
    while (i <= j)
    {
        while (topStreamers[i]->currentViewers > x) i++;
        while (topStreamers[j]->currentViewers < x) j--;
        if (i <= j)
        {
            topStreamers.swap(i,j);
            i++;
            j--;
        }
    }
    if (i < r)
        quickSortPrivate(i, r);
    if (l < r)
        quickSortPrivate(l,j);
}

void StreamerEnv::on_pushButton_clicked()
{
    env.generateEnviroment(100000, 500);
    connect(&timer, SIGNAL(timeout()), this,SLOT(onTimer()));
    connect(&env,SIGNAL(onFollowed(StreamerDesc*,ViewerDesc*)),this,SLOT(onFollowed(StreamerDesc*,ViewerDesc*)));
    connect(&env,SIGNAL(onDecideWatch(StreamerDesc*,ViewerDesc*)),this,SLOT(onDecideWatch(StreamerDesc*,ViewerDesc*)));
    connect(&env,SIGNAL(onSleep(ViewerDesc*)),this,SLOT(onSleep(ViewerDesc*)));
    timer.start(150);
    currentTime = 7;
}

void StreamerEnv::onTimer()
{
    static int v = 1;
    static int minutes = 0;
    minutes++; minutes %= 60;

    if (minutes%30 == 0)
    {
        v = 1 - v;
        if (v%2){
            currentTime++; currentTime%=24;}

        env.update(currentTime, v%2);
        ui->listWidget->clear();
        for (int i = 0; i < 50; i++)
        {
            StreamerDesc * currentStreamer = env.topStreamers[i];
            ui->listWidget->addItem(currentStreamer->name + ": " + QString::number(currentStreamer->currentViewers) + " F[" +
                                    QString::number(currentStreamer->followers.count()) + "] {" + currentStreamer->getDesc() + "}");
            ui->label_2->setText("Users online: " + QString::number(env.watchers.count()));
        }
    }
    ui->label->setText(QString::number(currentTime) + ":" + QString::number(minutes));
}

void StreamerEnv::onFollowed(StreamerDesc *s, ViewerDesc *v)
{
    ui->plainTextEdit->appendPlainText("Viewer followed " + s->name);
}

void StreamerEnv::onDecideWatch(StreamerDesc *s, ViewerDesc *v)
{

}

void StreamerEnv::onSleep(ViewerDesc *v)
{

}
