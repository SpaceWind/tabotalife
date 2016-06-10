#include "streamerenv.h"
#include "ui_streamerenv.h"

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

void StreamerEnviroment::update(int time, bool newHour)
{
    if (newHour)
    {
        auto currentViewersWakeTime = viewersWakeTime[time];
        auto currentViewersSleepTime = viewersSleepTime[time];
        for (int i = 0; i<currentViewersWakeTime.count(); ++i)
        {
            ViewerDesc * v = currentViewersWakeTime[i];
            StreamerDesc * s = findStreamer(v);
            s->currentViewers++;
            if (watchers.contains(v))
                watchers.remove(v);
            watchers[v] = s;
        }
        for (int i = 0; i<currentViewersSleepTime.count(); ++i)
        {
             ViewerDesc * v = currentViewersSleepTime[i];
             if (watchers.contains(v))
             {
                 watchers[v]->currentViewers--;
                 watchers.remove(v);
             }
        }
    }
  /*  QList<ViewerDesc*> watchersList = watchers.keys();
    for (int i = 0; i<watchersList.count(); i++)
    {
        watchersList[i]->watchTime[watchers[watchers[i]]]
    //    watchers[i]->watchTime[wa]
    }*/
    for (auto it = watchers.keys().begin(); it != watchers.keys().end(); ++it)
    {
        ViewerDesc * v = (*it);
        v->watchTime[watchers[v]] += 0.5;
        if (v->watchTime[watchers[v]] > 4.0)
        {
            watchers[v]->follow(v);
            v->followed.append(watchers[v]);
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
    //sort top streamer!

    std::sort(topStreamers.first(),topStreamers.last(), [](StreamerDesc &a, StreamerDesc &b) { return a.currentViewers > b.currentViewers; } );
}

StreamerDesc *StreamerEnviroment::findStreamer(const ViewerDesc *v)
{
  //  A.1 find one in followed list (weighted random) and return him. Or if proc - instead go to next step;
  //  B.1 find one in all list
  //  B.2 Took top SR% items from topStreamers and find best one. OR if proc SR*2% - go to C
  //  C.1 Took RANDOM pool*0.2
  //  3. Ищем по top SR%, но при этом есть шанс SR*2%, что юзер пойдет искать, выбирая рандомно pool*0.2 стримов, вместо поиска по лучшим.

    if (FRAND > v->searchingRate/100.)
    {
        //QHash<StreamerDesc*, double> followingResult;
        std::map<double, StreamerDesc*> followingResult;
        for (auto it = v->followed.begin(); it != v->followed.end(); ++it)
        {
            StreamerDesc * s = (*it);
            followingResult[s->test(*v)] = s;
        }
        return followingResult.upper_bound(FRAND)->second;
    }
    else if (FRAND > v->searchingRate/50.)
    {
        int maxCount = v->searchingRate/100.*topStreamers.count();
        StreamerDesc * foundStreamer = 0;
        double value = -1000.0;
        for(int i = 0; i < maxCount; ++i)
        {
            double currentValue = topStreamers[i]->test(*v);
            if (currentValue > value)
            {
                value = currentValue;
                foundStreamer = topStreamers[i];
            }
        }
        return foundStreamer;
    }
    else
    {
        StreamerDesc * foundStreamer = 0;
        double value = -10000.0;
        int pickCount = streamers.count()/5;
        for (int i = 0; i<pickCount; ++i)
        {
            int itemIndex = rand()%streamers.count();
            double currentValue = streamers[itemIndex]->test(*v);
            if (currentValue > value)
            {
                value = currentValue;
                foundStreamer = streamers[itemIndex];
            }
        }
        return foundStreamer;
    }
}
