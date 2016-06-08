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

#endif // VIEWERDESC_H
