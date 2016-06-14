#include "entities.h"
#include "math.h"


ViewerDesc::ViewerDesc()
{

}

ViewerDesc ViewerDesc::generate(ViewerLibrary *library)
{
    ViewerDesc result;
    result.age = qrand()%12 +10;    //10..22
    for (int i = 1; i<5; i++)
    {
        if ((rand()%int(pow(2,i))) == 0)
            result.age += qrand()%16;
    }
    if (result.age > 60)
        result.age = 60;

    result.alphaLevel = FRAND;
    result.casuality = FRAND;
   // if (qrand()%100 > 70)
     if (qrand()%100 > 50)
        result.isMale = false;
    else
        result.isMale = true;

    int fiveDaysStartWatchHour = 19 + rand()%6 - 3;
    if (FRAND > 0.8) //owl
        fiveDaysStartWatchHour+=rand()%12;
    fiveDaysStartWatchHour %= 24;
    double timeLenMod = library->ageConfigLibrary.getTimeLenCoef(result.age);
    for (int i = 128; i>1; i/=2)
    {
        if (rand()%256 < i)
            fiveDaysStartWatchHour+= rand()%6 - 3;
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

ViewerDesc *ViewerDesc::generateDynamic(ViewerLibrary *library)
{
    ViewerDesc * newViewer = new ViewerDesc();
    (*newViewer) = ViewerDesc::generate(library);
    return newViewer;
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
    currentViewers = 0;
    channelViews = 0;
}

StreamerDesc StreamerDesc::generate(ViewerLibrary *library)
{
    StreamerDesc result(library);
    result.name = getRandomString();
    result.isMale = (rand()%2 == 0);
    result.pictureQuality = FRAND;
    result.webCamQuality = FRAND;
    result.skillLevel = FRAND;
    result.interactiveness = FRAND;
    result.agrLevel = FRAND;
    result.attLevel = FRAND;
    result.charisma = FRAND;
    return result;
}

StreamerDesc *StreamerDesc::generateDynamic(ViewerLibrary *library)
{
    StreamerDesc * result = new StreamerDesc(library);
    (*result) = StreamerDesc::generate(library);
    return result;
}

double StreamerDesc::test(ViewerDesc v) const
{
    double result = 0.0;
    result += (1.0 - fabs(agrLevel-v.aggressionInterest))*lib->genderInterestLibrary.getAgressionInterest(agrLevel,v.isMale)*1.8;
    result += (pictureQuality-0.5)*v.qualityImportance*1.3;
    result += (webCamQuality-0.5)*v.webcamInterest*lib->genderInterestLibrary.getAttractivenessInterest(attLevel,v.isMale)*
              (1.0 - v.alphaLevel*0.33)*1.25;
    result += interactiveness*v.interactiveInterest*0.7;
    result += skillLevel*v.skillInterest*0.7;
    result += charisma*pow(v.casuality,0.5)*((isMale == v.isMale) ? 1.0 : 1.1)*(v.alphaLevel*0.34 + 1.0)*0.9;
    return result;
}

QString StreamerDesc::getRandomString()
{
   const QString possibleCharacters("abcdefghijklnopqrstuvxyz0123456789");
   const int randomStringLength = 12;

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString + "\t";
}

QString StreamerDesc::getDesc()
{
    QString result;
    result += isMale? " M " : " F ";
    result += "\tPIC: " + QString::number(pictureQuality, 'f',3) + " ";
    result += "\tWEB: " + QString::number(webCamQuality, 'f',3) + " ";
    result += "\tSKL: " + QString::number(skillLevel, 'f',3) + " ";
    result += "\tINT: " + QString::number(interactiveness, 'f',3) + " ";
    result += "\tAGR: " + QString::number(agrLevel, 'f',3) + " ";
    result += "\tATT: " + QString::number(attLevel, 'f',3) + " ";
    result += "\tCHA: " + QString::number(charisma, 'f',3) + " ";
    return result;
}

void StreamerDesc::follow(ViewerDesc *v)
{
    if (!followers.contains(v))
        followers.append(v);
}

void StreamerDesc::unfollow(ViewerDesc *v)
{
    followers.removeOne(v);
}

void StreamerDesc::subscribe(ViewerDesc *v)
{
    if (!subscribers.contains(v))
        subscribers.append(v);
}

void StreamerDesc::unsubcribe(ViewerDesc *v)
{
    subscribers.removeOne(v);
}
