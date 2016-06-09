#include "entities.h"
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
            result.age += qrand()%15;
    }
    if (result.age > 60)
        result.age = 60;

    result.alphaLevel = double(qrand()%1000000)/1000000.;
    result.casuality = double(qrand()%1000000)/1000000.;
   // if (qrand()%100 > 70)
     if (qrand()%100 > 50)
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


StreamerDesc::StreamerDesc()
{
    lib = nullptr;
    isMale = 0;
    pictureQuality = webCamQuality = skillLevel = interactiveness = agrLevel = attLevel = charisma = 0.0;
}

StreamerDesc::StreamerDesc(ViewerLibrary *library)
{
    lib = library;
    isMale = 0;
    pictureQuality = webCamQuality = skillLevel = interactiveness = agrLevel = attLevel = charisma = 0.0;
}

StreamerDesc StreamerDesc::generate(ViewerLibrary *library)
{
    StreamerDesc result(library);
    result.name = getRandomString();
    result.isMale = (rand()%2 == 0);
    result.pictureQuality = double(qrand()%1000000)/1000000.;
    result.webCamQuality = double(qrand()%1000000)/1000000.;
    result.skillLevel = double(qrand()%1000000)/1000000.;
    result.interactiveness = double(qrand()%1000000)/1000000.;
    result.agrLevel = double(qrand()%1000000)/1000000.;
    result.attLevel = double(qrand()%1000000)/1000000.;
    result.charisma = double(qrand()%1000000)/1000000.;
    return result;
}

double StreamerDesc::test(ViewerDesc v) const
{
    double result = 0.0;
    result += (1.0 - fabs(agrLevel-v.aggressionInterest))*lib->genderInterestLibrary.getAgressionInterest(agrLevel,v.isMale)*1.25;
    result += pictureQuality*v.qualityImportance;
    result += webCamQuality*v.webcamInterest*attLevel*lib->genderInterestLibrary.getAttractivenessInterest(attLevel,v.isMale)*1.6;
    result += interactiveness*v.interactiveInterest;
    result += skillLevel*v.skillInterest;
    result += charisma*v.casuality*1.25;
    return result;
}

QString StreamerDesc::getRandomString()
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   const int randomStringLength = 12;

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}

QString StreamerDesc::getDesc()
{
    QString result;
    result += isMale? " M " : " F ";
    result += "\tPIC: " + QString::number(pictureQuality) + " ";
    result += "\tWEB: " + QString::number(webCamQuality) + " ";
    result += "\tSKL: " + QString::number(skillLevel) + " ";
    result += "\tINT: " + QString::number(interactiveness) + " ";
    result += "\tAGR: " + QString::number(agrLevel) + " ";
    result += "\tATT: " + QString::number(attLevel) + " ";
    result += "\tCHA: " + QString::number(charisma) + " ";
    return result;
}
