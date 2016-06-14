#ifndef LIBRARY_H
#define LIBRARY_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>


struct AgeConfigLibrary
{
    double getAggressionInterest(int age);
    double getQualityImportance(int age);
    double getSearchingRates(int age);
    double getPaymentAbility(int age);
    double getTimeLenCoef(int age);

    struct AgeConfig
    {
        int value;
        double  aggressionValue, qualityImportance, searchingRates, paymentAbility, timeLenCoef;
    };
    QList<AgeConfig> data;
};

struct GenderConfigLibrary
{
    double getWebcamImportance(bool isMale);
    double getPaymentAbility(bool isMale);
    struct GenderConfig
    {
        double webcamImportance, paymentAbility;
    };
    GenderConfig male, female;
};

struct GenderInterestLibrary
{
    double getAgressionInterest(double agressionLevel, bool isMale);
    double getAttractivenessInterest(double attractivenessLevel, bool isMale);
    struct GenderInterestConfig
    {
        double value;
        double aggressionInterestMale, agressionInterestFemale, attractivenessInterestMale, attractivenessInterestFemale;
    };
    QList<GenderInterestConfig> data;
};
struct AlphaPaymentAbilityLibrary
{
    double getAlphaPaymentAbility(double alphaLevel, bool viewerGender, bool streamerGender);
    struct AlphaPaymentAbilityConfig
    {
        double value;
        double paymentAbilitySameGender, paymentAbilityOppositeGender;
    };
    QList<AlphaPaymentAbilityConfig> data;
};

struct AggressionAlphaInterestingLibrary
{
    double getAggressionAlpha(double aggressionLevel, double alphaLevel);
    struct AggressionAlphaInterestConfig
    {
        double value;
        double omega, alpha;
    };
    QList<AggressionAlphaInterestConfig> data;
};

struct CasualityLibrary
{
    double getSearchingRate(double casualityLevel);
    double getInteractiveInterest(double casualityLevel);
    double getSkillInterest(double casualityLevel);
    struct CasualityConfig
    {
        double value;
        double searchingRates, interactiveInterest, skillInterest;
    };
    QList<CasualityConfig> data;
};

struct ViewerNameLibrary
{
    QString generateName();
    QStringList namePrefix, nameRoot, nameSuffix, familyPrefix, familyRoot, familySuffix;
};

struct StreamerNameLibrary
{
    QString generateName();
    QStringList namePrefix, nameRoot, nameRoot2, nameSuffix;
};

class CSVFile
{
public:
    CSVFile(){;}
    CSVFile(QString filename);
    ~CSVFile(){;}
    void load(QString filename);

    QList<QHash<QString, QString> > rows;
};

class ViewerLibrary
{
public:
    ViewerLibrary(QString path);
    ~ViewerLibrary(){;}

    AgeConfigLibrary ageConfigLibrary;
    GenderConfigLibrary genderConfigLibrary;
    GenderInterestLibrary genderInterestLibrary;
    AlphaPaymentAbilityLibrary alphaPaymentAbilityLibrary;
    AggressionAlphaInterestingLibrary aggressionAlphaInterestingLibrary;
    CasualityLibrary casualityLibrary;
    ViewerNameLibrary viewerNameLibrary;
    StreamerNameLibrary streamerNameLibrary;
};


#endif // LIBRARY_H
