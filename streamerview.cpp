#include "streamerview.h"
#include "ui_streamerview.h"

StreamerView::StreamerView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StreamerView)
{
    ui->setupUi(this);
}

void StreamerView::load(StreamerDesc *s)
{
    ui->label_nick->setText(s->name);
    ui->label_online->setText(QString::number(s->currentViewers));
    ui->label_follow->setText(QString::number(s->followers.count()));
    ui->label_views->setText(QString::number(s->channelViews));
    ui->pb_quality->setValue(s->pictureQuality*100);
    ui->pb_webcam->setValue(s->webCamQuality*100);
    ui->pb_skill->setValue(s->skillLevel*100);
    ui->pb_interactiveness->setValue(s->interactiveness*100);
    ui->pb_aggression->setValue(s->agrLevel*100);
    ui->pb_att->setValue(s->attLevel*100);
    ui->pb_cha->setValue(s->charisma*100);
    ui->label_wk->setText(hourToString(s->streamTime[0].timeStart));
    ui->label_slp->setText(hourToString(s->streamTime[0].timeEnd));
}

QString StreamerView::hourToString(int h)
{
    QString result = QString::number(h);
    if (h<10)
        result = "0" + result;
    return result;
}

StreamerView::~StreamerView()
{
    delete ui;
}
