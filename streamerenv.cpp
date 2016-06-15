#include "streamerenv.h"
#include "ui_streamerenv.h"
#include <QDebug>

StreamerEnv::StreamerEnv(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StreamerEnv)
{
    ui->setupUi(this);
    currentTime = 0;
    currentDayOfWeek = 0;
    connect(&timer, SIGNAL(timeout()), this,SLOT(onTimer()));
    connect(&env,SIGNAL(onFollowed(StreamerDesc*,ViewerDesc*)),this,SLOT(onFollowed(StreamerDesc*,ViewerDesc*)));
    connect(&env,SIGNAL(onDecideWatch(StreamerDesc*,ViewerDesc*)),this,SLOT(onDecideWatch(StreamerDesc*,ViewerDesc*)));
    connect(&env,SIGNAL(onSleep(ViewerDesc*)),this,SLOT(onSleep(ViewerDesc*)));
    env.generateEnviroment(250000, 1000);
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
        for (int i = 0; i < 7; i++)
        {
            if (v->viewTime.contains(i))
            {
                WeekDayHour wdh;
                wdh.dayOfWeek = i;
                wdh.hour = v->viewTime[i].timeStart;
                viewersWakeTime[wdh.value()].append(v);
                wdh.hour = v->viewTime[i].timeEnd;
                viewersSleepTime[wdh.value()].append(v);
            }
        }
        viewers.append(v);
    }
    for (int i = 0; i < streamerPool; i++)
    {
        StreamerDesc * s = StreamerDesc::generateDynamic(library);

        for (int i = 0; i < 7; i++)
        {
            if (s->streamTime.contains(i))
            {
                WeekDayHour wdh;
                wdh.dayOfWeek = i;
                wdh.hour = s->streamTime[i].timeStart;
                streamersWakeTime[wdh.value()].append(s);
                wdh.hour = s->streamTime[i].timeEnd;
                streamersSleepTime[wdh.value()].append(s);
            }
        }
     //   topStreamers.append(s);
     //   streamers.append(s);
    }
}

bool compareStreamerByViewers(const StreamerDesc &s1, const StreamerDesc &s2)
{
    return s1.currentViewers > s2.currentViewers;
}

void StreamerEnviroment::update(WeekDayHour wdh, bool newHour)
{
    if (newHour)
    {

        QList<ViewerDesc*> currentViewersWakeTime;
        QList<ViewerDesc*> currentViewersSleepTime;
        QList<StreamerDesc*> currentStreamersWakeTime;
        QList<StreamerDesc*> currentStreamersSleepTime;

        int value = wdh.value();
        if (viewersWakeTime.contains(value)) currentViewersWakeTime = viewersWakeTime[value];
        if (viewersSleepTime.contains(value)) currentViewersSleepTime = viewersSleepTime[value];
        if (streamersWakeTime.contains(value)) currentStreamersWakeTime = streamersWakeTime[value];
        if (streamersSleepTime.contains(value)) currentStreamersSleepTime = streamersSleepTime[value];

        for (int i = 0; i < currentStreamersWakeTime.count(); ++i)
        {
            StreamerDesc * s = currentStreamersWakeTime[i];
            streamers.append(s);
            topStreamers.append(s);
        }
        for (int i = 0; i < currentStreamersSleepTime.count(); ++i)
        {
            StreamerDesc * s = currentStreamersSleepTime[i];
            streamers.removeOne(s);
            topStreamers.removeOne(s);
        }

        for (int i = 0; i<currentViewersWakeTime.count(); ++i)
        {
            ViewerDesc * v = currentViewersWakeTime[i];
            StreamerDesc * s = findStreamer(v);
            s->currentViewers++;
            if (watchers.contains(v))
                watchers.remove(v);
            watchers[v] = s;
            s->channelViews++;
          //  emit onDecideWatch(s,v);
        }
        for (int i = 0; i<currentViewersSleepTime.count(); ++i)
        {
             ViewerDesc * v = currentViewersSleepTime[i];
             if (watchers.contains(v))
             {
                 watchers[v]->currentViewers--;
                 watchers.remove(v);
             //    emit onSleep(v);
             }
        }
    }
    QList<ViewerDesc*> keys = watchers.keys();
    for (QList<ViewerDesc*>::iterator it = keys.begin(); it != keys.end(); ++it)
    {
        ViewerDesc * v = (*it);
        v->watchTime[watchers[v]] += 0.5;
        if (watchers[v])
        {
            if (v->watchTime[watchers[v]] > 4.0)
            {
                if (!v->followed.contains(watchers[v]))
                {
                    watchers[v]->follow(v);
                    v->followed.append(watchers[v]);
                 //   emit onFollowed(watchers[v], v);
                }
            }
            if (rand()%10 == 0) //should depend on quality of the stream!
            {
                watchers[v]->currentViewers--;
                watchers.remove(v);
                StreamerDesc * s = findStreamer(v);
                s->currentViewers++;
                s->channelViews++;
                watchers[v] = s;
            }
        }
    }
    if (topStreamers.count() > 0)
        sortTopStreamers();
}

StreamerDesc *StreamerEnviroment::findStreamer(const ViewerDesc *v)
{
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
            followingResult[value] = s;
        }
        if (!followingResult.empty())
            return followingResult.upper_bound(FRAND*(maxValue - minValue) - minValue)->second;
    }
    if (FRAND > v->searchingRate/50.)
    {
        int maxCount = v->searchingRate/100.*topStreamers.count() + 5;
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
    if (foundStreamer == NULL)
    {
        qDebug() <<"ERROR: cant find the streamer!";
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
    timer.start(100);
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
            currentTime++;
            if (currentTime>=24)
            {
                currentDayOfWeek++;
                currentDayOfWeek %= 7;
            }
            currentTime%=24;
        }

        WeekDayHour wdh;
        wdh.dayOfWeek = currentDayOfWeek;
        wdh.hour = currentTime;
        env.update(wdh, v%2);
        ui->listWidget->clear();
        for (int i = 0; i < 50 && i<env.topStreamers.count(); i++)
        {
            StreamerDesc * currentStreamer = env.topStreamers[i];
            ui->listWidget->addItem(currentStreamer->name + ": " + QString::number(currentStreamer->currentViewers) + " INFO:[" +
                                    QString::number(currentStreamer->followers.count()) + " / " +
                                    QString::number(currentStreamer->channelViews) + "] |" +
                                    getStreamTimeDesc(currentStreamer, currentDayOfWeek) +
                                    "\t{" + currentStreamer->getDesc() + "}");
            ui->label_2->setText("Users online: " + QString::number(env.watchers.count()));
        }
    }
    ui->label->setText(QString::number(currentTime) + ":" + QString::number(minutes));
}

void StreamerEnv::onFollowed(StreamerDesc *s, ViewerDesc *v)
{
    if (currentTime == 0)
        ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(v->name + " followed " + s->name);
}

void StreamerEnv::onDecideWatch(StreamerDesc *s, ViewerDesc *v)
{

}

void StreamerEnv::onSleep(ViewerDesc *v)
{

}


void StreamerEnv::on_pushButton_2_clicked()
{
    timer.stop();
}

void StreamerEnv::on_horizontalSlider_sliderMoved(int position)
{
    switch (position)
    {
    case 0:
        timer.setInterval(50);
        break;
    case 1:
        timer.setInterval(100);
        break;
    case 2:
        timer.setInterval(200);
        break;
    case 3:
        timer.setInterval(300);
        break;
    case 4:
        timer.setInterval(500);
        break;
    case 5:
        timer.setInterval(750);
        break;
    case 6:
        timer.setInterval(1000);
        break;
    case 7:
        timer.setInterval(1500);
        break;
    case 8:
    default:
        timer.setInterval(2000);
        break;
    }
}

QString StreamerEnv::getStreamTimeDesc(StreamerDesc *s, int dayOfWeek)
{
    if (s->streamTime.contains(dayOfWeek))
        return QString::number(s->streamTime[dayOfWeek].timeStart) + " - " + QString::number(s->streamTime[dayOfWeek].timeEnd);
    return "";
}

bool WeekDayHour::operator==(const WeekDayHour &other) const
{
    return (dayOfWeek == other.dayOfWeek) && (hour == other.hour);
}
