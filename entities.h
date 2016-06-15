#ifndef VIEWERDESC_H
#define VIEWERDESC_H

#include "library.h"
#define FRAND (double(rand()%RAND_MAX)/double(RAND_MAX))

class StreamerDesc;

struct TimeRange
{
    int timeStart;
    int timeEnd;
};

class ViewerDesc
{
public:
    ViewerDesc();

    static ViewerDesc generate(ViewerLibrary * library);
    static ViewerDesc *generateDynamic(ViewerLibrary * library);

    double age;
    double alphaLevel;
    double casuality;
    bool isMale;
    QString name;

    QHash<int, TimeRange> viewTime;
    QList<StreamerDesc*> followed;
    QHash<StreamerDesc*, double> watchTime;

    double aggressionInterest;
    double qualityImportance;
    double searchingRate;
    double paymentAbilityMale;
    double paymentAbilityFemale;
    double webcamInterest;
    double interactiveInterest;
    double skillInterest;
};

class StreamerDesc
{
public:
    StreamerDesc();
    StreamerDesc(ViewerLibrary * library);
    static StreamerDesc generate(ViewerLibrary * library);
    static StreamerDesc *generateDynamic(ViewerLibrary * library);
    double test(ViewerDesc v) const;
    static QString getRandomString();
    QString getDesc();

    bool operator<(const StreamerDesc* other) const
    {
        if (currentViewers == other->currentViewers)
        {
            if (followers.count() == other->followers.count())
                return channelViews > other->channelViews;
            else
                return followers.count() > other->followers.count();
        }
        return currentViewers > other->currentViewers;
    }

    void follow(ViewerDesc* v);
    void unfollow(ViewerDesc * v);
    void subscribe(ViewerDesc * v);
    void unsubcribe(ViewerDesc * v);

    QString name;
    bool isMale;
    double pictureQuality;
    double webCamQuality;
    double skillLevel;
    double interactiveness;
    double agrLevel;
    double attLevel;
    double charisma;

    QList<ViewerDesc*> followers;
    QList<ViewerDesc*> subscribers;
    QHash<int, TimeRange> streamTime;
    int currentViewers;
    int channelViews;
private:
    ViewerLibrary * lib;
};

#endif // VIEWERDESC_H
