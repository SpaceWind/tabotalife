#ifndef VIEWERDESC_H
#define VIEWERDESC_H

#include "library.h"


class ViewerDesc
{
public:
    ViewerDesc();

    static ViewerDesc generate(ViewerLibrary * library);

    double age;
    double alphaLevel;
    double casuality;
    bool isMale;

    struct TimeRange
    {
        int timeStart;
        int timeEnd;
    };
    QHash<int, TimeRange> viewTime;

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
    double test(ViewerDesc v) const;
    static QString getRandomString();
    QString getDesc();

    QString name;
    bool isMale;
    double pictureQuality;
    double webCamQuality;
    double skillLevel;
    double interactiveness;
    double agrLevel;
    double attLevel;
    double charisma;
private:
    ViewerLibrary * lib;
};

#endif // VIEWERDESC_H
