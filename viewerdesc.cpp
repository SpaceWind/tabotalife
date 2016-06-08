#include "viewerdesc.h"
#include "math.h"

ViewerDesc::ViewerDesc()
{

}

ViewerDesc ViewerDesc::generate(ViewerLibrary *library)
{
    ViewerDesc result;
    result.age = qrand()%12 +10;    //10..19
    for (int i = 1; i<5; i++)
    {
        if ((rand()%int(pow(2,i))) == 0)
            result.age += qrand()%11;
    }
    if (result.age > 60)
        result.age = 60;

    result.alphaLevel = double(qrand()%1000000)/1000000.;
    result.casuality = double(qrand()%1000000)/1000000.;
    if (qrand()%100 > 70)
        result.isMale = false;
    else
        result.isMale = true;

    int fiveDaysStartWatchHour = 16 + rand()%6 - 3;
    double timeLenMod = library->ageConfigLibrary.getTimeLenCoef(result.age);
    for (int i = 128; i>1; i/=2)
    {
        if (rand()%256 < i)
            fiveDaysStartWatchHour+= rand()%4 - 2;
    }
    for (int i = 0; i< 7; i++)
    {
        double standartViewTime = 3.0;
        if (i>5)
        {
            standartViewTime = 5.0;
            fiveDaysStartWatchHour+=1;
        }
        TimeRange tr;
        tr.timeStart = fiveDaysStartWatchHour;
        tr.timeEnd = int(fiveDaysStartWatchHour + standartViewTime*timeLenMod)%24;
        result.viewTime[i] = tr;
    }

    result.aggressionInterest = library->ageConfigLibrary.getAggressionInterest(result.age);
    result.qualityImportance = library->ageConfigLibrary.getQualityImportance(result.age);
    result.searchingRate = library->ageConfigLibrary.getSearchingRates(result.age) * library->casualityLibrary.getSearchingRate(result.casuality);
    result.paymentAbilityMale = library->ageConfigLibrary.getPaymentAbility(result.age) *
                         library->genderConfigLibrary.getPaymentAbility(result.isMale) *
                         library->alphaPaymentAbilityLibrary.getAlphaPaymentAbility(result.alphaLevel,result.isMale,true);
    result.paymentAbilityFemale=library->ageConfigLibrary.getPaymentAbility(result.age) *
                         library->genderConfigLibrary.getPaymentAbility(result.isMale) *
                         library->alphaPaymentAbilityLibrary.getAlphaPaymentAbility(result.alphaLevel,result.isMale,false);
    result.webcamInterest=library->genderConfigLibrary.getWebcamImportance(result.isMale);
    result.interactiveInterest=library->casualityLibrary.getInteractiveInterest(result.casuality);
    result.skillInterest = library->casualityLibrary.getSkillInterest(result.casuality);
    return result;
}

