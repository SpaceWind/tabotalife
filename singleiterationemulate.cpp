#include "singleiterationemulate.h"
#include "ui_singleiterationemulate.h"
#include <QHash>
#include <QList>
#include "entities.h"
#include "library.h"

SingleIterationEmulate::SingleIterationEmulate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleIterationEmulate)
{
    ui->setupUi(this);
}

SingleIterationEmulate::~SingleIterationEmulate()
{
    delete ui;
}

void SingleIterationEmulate::on_pushButton_clicked()
{
    ViewerLibrary * library = new ViewerLibrary("");
    ui->listWidget->clear();
    QList<StreamerDesc> streamers;
    QList<ViewerDesc> viewers;
    QHash<QString, int> prefers;
    QHash<QString, StreamerDesc> streamerByName;
    QHash<QString, QList<ViewerDesc> > streamerToViewer;
    for (int i = 0; i<ui->spinBox_2->value(); i++)
    {
        streamers.append(StreamerDesc::generate(library));
        prefers[streamers.last().name] = 0;
        streamerByName[streamers.last().name] = streamers.last();
    }
    for (int i = 0; i<ui->spinBox->value(); i++)
        viewers.append(ViewerDesc::generate(library));
    foreach (const ViewerDesc &v, viewers)
    {
        double testValue = -10000.;
        QString name;
        foreach (const StreamerDesc &s,streamers)
        {
            double value = s.test(v);
            if (value > testValue)
            {
                testValue = value;
                name = s.name;
            }
        }
        prefers[name] = prefers[name] + 1;
        streamerToViewer[name].append(v);
    }
    for (int i=0; i<100; i++)
    {
        int maxValue = 0;
        QString streamerName;
        int index = 0, foundIndex = -1;
        foreach (const StreamerDesc &sd, streamers)
        {
            if (prefers[sd.name] > maxValue)
            {
                maxValue = prefers[sd.name];
                streamerName = sd.name;
                foundIndex = index;
            }
            index++;
        }
        if (maxValue == 0)
            return;
        int maleCount = 0, femaleCount = 0;
        double alphaMiddle = 0, ageMiddle = 0,casualityMiddle = 0;

        foreach (const ViewerDesc &cv, streamerToViewer[streamerName])
        {
            if (cv.isMale)
                maleCount++;
            else
                femaleCount++;
            alphaMiddle+=cv.alphaLevel;
            ageMiddle+=cv.age;
            casualityMiddle+=cv.casuality;
        }
        alphaMiddle/=maxValue;
        ageMiddle/=maxValue;
        casualityMiddle/=maxValue;
        double malePercent = maleCount * 100 / (maleCount + femaleCount);

        StreamerDesc currentStreamer = streamerByName[streamerName];
        ui->listWidget->addItem(streamerName + " [" + QString::number(maxValue) + "]" + currentStreamer.getDesc() + " |||\t" +
                                "\tMALES: " + QString::number(malePercent) + "% ALPHA: " + QString::number(alphaMiddle) +
                                " CAS: " + QString::number(casualityMiddle) + " AGE: " + QString::number(ageMiddle));
        streamers.removeAt(foundIndex);
    }
}
