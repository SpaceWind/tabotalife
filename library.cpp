#include "library.h"

#include <QFile>


double AgeConfigLibrary::getAggressionInterest(int age)
{
    foreach (const AgeConfig &cfg,data){
        if (cfg.value == age)
            return cfg.aggressionValue;
    }
    return 1.0;
}

double AgeConfigLibrary::getQualityImportance(int age)
{
    foreach (const AgeConfig &cfg,data){
        if (cfg.value == age)
            return cfg.qualityImportance;
    }
    return 1.0;
}

double AgeConfigLibrary::getSearchingRates(int age)
{
    foreach (const AgeConfig &cfg,data){
        if (cfg.value == age)
            return cfg.searchingRates;
    }
    return 1.0;
}

double AgeConfigLibrary::getPaymentAbility(int age)
{
    foreach (const AgeConfig &cfg,data){
        if (cfg.value == age)
            return cfg.paymentAbility;
    }
    return 1.0;
}

double AgeConfigLibrary::getTimeLenCoef(int age)
{
    foreach (const AgeConfig &cfg,data){
        if (cfg.value == age)
            return cfg.timeLenCoef;
    }
    return 1.0;
}

double GenderConfigLibrary::getWebcamImportance(bool isMale)
{
    return isMale ? male.webcamImportance : female.webcamImportance;
}

double GenderConfigLibrary::getPaymentAbility(bool isMale)
{
    return isMale ? male.paymentAbility : female.paymentAbility;
}

double GenderInterestLibrary::getAgressionInterest(double agressionLevel, bool isMale)
{
    double foundValue, nextValue;
    double foundKey, nextKey;
    int index = 0;
    foreach (const GenderInterestConfig &cfg, data)
    {
        if (agressionLevel <= cfg.value)
        {
            if (index == 0)
            {
                foundValue = (isMale? cfg.aggressionInterestMale : cfg.agressionInterestFemale);
                nextValue = foundValue;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                nextValue = (isMale? cfg.aggressionInterestMale : cfg.agressionInterestFemale);
                nextKey = cfg.value;
                break;
            }
        }
        else
        {
            if (index == data.count() - 1)
            {
                foundValue = (isMale? cfg.aggressionInterestMale : cfg.agressionInterestFemale);
                nextValue = foundValue;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                foundValue = (isMale? cfg.aggressionInterestMale : cfg.agressionInterestFemale);
                foundKey = cfg.value;
            }
        }
    }
    if (agressionLevel >= nextKey || agressionLevel < foundKey)
        return nextValue;
    if (agressionLevel == foundKey)
        return foundValue;

    return (nextValue-foundValue)*(agressionLevel-foundKey)/(nextKey-foundKey) + foundValue;
}

double GenderInterestLibrary::getAttractivenessInterest(double attractivenessLevel, bool isMale)
{
    double foundValue, nextValue;
    double foundKey, nextKey;
    int index = 0;
    foreach (const GenderInterestConfig &cfg, data)
    {
        if (attractivenessLevel <= cfg.value)
        {
            if (index == 0)
            {
                foundValue = (isMale? cfg.attractivenessInterestMale: cfg.attractivenessInterestFemale);
                nextValue = foundValue;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                nextValue = (isMale? cfg.attractivenessInterestMale: cfg.attractivenessInterestFemale);
                nextKey = cfg.value;
                break;
            }
        }
        else
        {
            if (index == data.count() - 1)
            {
                foundValue = (isMale? cfg.attractivenessInterestMale: cfg.attractivenessInterestFemale);
                nextValue = foundValue;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                foundValue = (isMale? cfg.attractivenessInterestMale: cfg.attractivenessInterestFemale);
                foundKey = cfg.value;
            }
        }
    }
    if (attractivenessLevel >= nextKey || attractivenessLevel < foundKey)
        return nextValue;
    if (attractivenessLevel == foundKey)
        return foundValue;

    return (nextValue-foundValue)*(attractivenessLevel-foundKey)/(nextKey-foundKey) + foundValue;
}

double AlphaPaymentAbilityLibrary::getAlphaPaymentAbility(double alphaLevel, bool viewerGender, bool streamerGender)
{
    bool sameGender = (viewerGender==streamerGender);

    double foundValue, nextValue;
    double foundKey, nextKey;
    int index = 0;
    foreach (const AlphaPaymentAbilityConfig &cfg, data)
    {
        if (alphaLevel <= cfg.value)
        {
            if (index == 0)
            {
                foundValue = (sameGender? cfg.paymentAbilitySameGender: cfg.paymentAbilityOppositeGender);
                nextValue = foundValue;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                nextValue = (sameGender? cfg.paymentAbilitySameGender: cfg.paymentAbilityOppositeGender);
                nextKey = cfg.value;
                break;
            }
        }
        else
        {
            if (index == data.count() - 1)
            {
                foundValue = (sameGender? cfg.paymentAbilitySameGender: cfg.paymentAbilityOppositeGender);
                nextValue = foundValue;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                foundValue = (sameGender? cfg.paymentAbilitySameGender: cfg.paymentAbilityOppositeGender);
                foundKey = cfg.value;
            }
        }
    }
    if (alphaLevel >= nextKey || alphaLevel < foundKey)
        return nextValue;
    if (alphaLevel == foundKey)
        return foundValue;

    return (nextValue-foundValue)*(alphaLevel-foundKey)/(nextKey-foundKey) + foundValue;
}

double AggressionAlphaInterestingLibrary::getAggressionAlpha(double aggressionLevel, double alphaLevel)
{
    double foundValueOmega, nextValueOmega;
    double foundValueAlpha, nextValueAlpha;
    double foundKey, nextKey;
    int index = 0;
    foreach (const AggressionAlphaInterestConfig &cfg, data)
    {
        if (aggressionLevel <= cfg.value)
        {
            if (index == 0)
            {
                foundValueOmega = cfg.omega;
                foundValueAlpha = cfg.alpha;
                nextValueOmega = foundValueOmega;
                nextValueAlpha = foundValueAlpha;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                nextValueOmega = cfg.omega;
                nextValueAlpha = cfg.alpha;
                nextKey = cfg.value;
                break;
            }
        }
        else
        {
            if (index == data.count() - 1)
            {
                foundValueOmega = cfg.omega;
                foundValueAlpha = cfg.alpha;
                nextValueOmega = foundValueOmega;
                nextValueAlpha = foundValueAlpha;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                foundValueOmega = cfg.omega;
                foundValueAlpha = cfg.alpha;
                foundKey = cfg.value;
            }
        }
    }
    if (aggressionLevel >= nextKey || aggressionLevel < foundKey)
        return (nextValueAlpha-nextValueOmega)*alphaLevel + nextValueOmega;
    if (aggressionLevel == foundKey)
        return (foundValueAlpha-foundValueOmega)*alphaLevel + foundValueOmega;

    double resultValueOmega = (nextValueOmega-foundValueOmega)*(aggressionLevel-foundKey)/(nextKey-foundKey) + foundValueOmega;
    double resultValueAlpha = (nextValueAlpha-foundValueAlpha)*(aggressionLevel-foundKey)/(nextKey-foundKey) + foundValueAlpha;
    return (resultValueAlpha-resultValueOmega)*alphaLevel + resultValueOmega;
}

double CasualityLibrary::getSearchingRate(double casualityLevel)
{
    double foundValue, nextValue;
    double foundKey, nextKey;
    int index = 0;
    foreach (const CasualityConfig &cfg, data)
    {
        if (casualityLevel <= cfg.value)
        {
            if (index == 0)
            {
                foundValue = cfg.searchingRates;
                nextValue = foundValue;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                nextValue = cfg.searchingRates;
                nextKey = cfg.value;
                break;
            }
        }
        else
        {
            if (index == data.count() - 1)
            {
                foundValue = cfg.searchingRates;
                nextValue = foundValue;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                foundValue = cfg.searchingRates;
                foundKey = cfg.value;
            }
        }
    }
    if (casualityLevel >= nextKey || casualityLevel < foundKey)
        return nextValue;
    if (casualityLevel == foundKey)
        return foundValue;

    return (nextValue-foundValue)*(casualityLevel-foundKey)/(nextKey-foundKey) + foundValue;
}

double CasualityLibrary::getInteractiveInterest(double casualityLevel)
{
    double foundValue, nextValue;
    double foundKey, nextKey;
    int index = 0;
    foreach (const CasualityConfig &cfg, data)
    {
        if (casualityLevel <= cfg.value)
        {
            if (index == 0)
            {
                foundValue = cfg.interactiveInterest;
                nextValue = foundValue;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                nextValue = cfg.interactiveInterest;
                nextKey = cfg.value;
                break;
            }
        }
        else
        {
            if (index == data.count() - 1)
            {
                foundValue = cfg.interactiveInterest;
                nextValue = foundValue;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                foundValue = cfg.interactiveInterest;
                foundKey = cfg.value;
            }
        }
    }
    if (casualityLevel >= nextKey || casualityLevel < foundKey)
        return nextValue;
    if (casualityLevel == foundKey)
        return foundValue;

    return (nextValue-foundValue)*(casualityLevel-foundKey)/(nextKey-foundKey) + foundValue;
}

double CasualityLibrary::getSkillInterest(double casualityLevel)
{
    double foundValue, nextValue;
    double foundKey, nextKey;
    int index = 0;
    foreach (const CasualityConfig &cfg, data)
    {
        if (casualityLevel <= cfg.value)
        {
            if (index == 0)
            {
                foundValue = cfg.skillInterest;
                nextValue = foundValue;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                nextValue = cfg.skillInterest;
                nextKey = cfg.value;
                break;
            }
        }
        else
        {
            if (index == data.count() - 1)
            {
                foundValue = cfg.skillInterest;
                nextValue = foundValue;
                foundKey = nextKey = cfg.value;
                break;
            }
            else
            {
                foundValue = cfg.skillInterest;
                foundKey = cfg.value;
            }
        }
    }
    if (casualityLevel >= nextKey || casualityLevel < foundKey)
        return nextValue;
    if (casualityLevel == foundKey)
        return foundValue;

    return (nextValue-foundValue)*(casualityLevel-foundKey)/(nextKey-foundKey) + foundValue;
}

CSVFile::CSVFile(QString filename)
{
    load(filename);
}

void CSVFile::load(QString filename)
{
    rows.clear();
    QFile file(filename);
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();
    QStringList items = data.split("\n");
    if (items.count() == 0)
        return;
    QString headerLine = items.at(0);
    QStringList header = headerLine.split(",");
    items.removeAt(0);

    foreach (const QString &line, items)
    {
        if (line.isEmpty())
            continue;
        QHash<QString,QString> newrow;
        QStringList currentRowItems = line.split(",");
        for (int i = 0; i< header.count(); ++i)
            newrow[header.at(i)] = currentRowItems[i];
        rows.append(newrow);
    }
}

ViewerLibrary::ViewerLibrary(QString path)
{
    CSVFile csv;

    csv.load(path + "age.csv");
    foreach (const auto &row, csv.rows)
    {
        AgeConfigLibrary::AgeConfig ageConfig;
        ageConfig.value = row["Age"].toInt();
        ageConfig.aggressionValue = row["AggressionValue"].toDouble();
        ageConfig.qualityImportance = row["QualityImportance"].toDouble();
        ageConfig.searchingRates = row["SearchingRates"].toDouble();
        ageConfig.paymentAbility = row["PaymentAbility"].toDouble();
        ageConfig.timeLenCoef = row["TimeLenCoef"].toDouble();
        ageConfigLibrary.data.append(ageConfig);
    }

    csv.load(path + "agressionAlpha.csv");
    foreach (const auto &row, csv.rows)
    {
        AggressionAlphaInterestingLibrary::AggressionAlphaInterestConfig agrConfig;
        agrConfig.value = row["AgressionValue"].toDouble();
        agrConfig.alpha = row["Alpha"].toDouble();
        agrConfig.omega = row["Omega"].toDouble();
        aggressionAlphaInterestingLibrary.data.append(agrConfig);
    }

    csv.load(path + "genderInterest.csv");
    foreach (const auto &row, csv.rows)
    {
        GenderInterestLibrary::GenderInterestConfig genInterestConfig;
        genInterestConfig.value = row["value"].toDouble();
        genInterestConfig.aggressionInterestMale = row["agrInterestMale"].toDouble();
        genInterestConfig.agressionInterestFemale = row["agrInterestFemale"].toDouble();
        genInterestConfig.attractivenessInterestMale = row["attInterestMale"].toDouble();
        genInterestConfig.attractivenessInterestFemale = row["attInterestFemale"].toDouble();
        genderInterestLibrary.data.append(genInterestConfig);
    }

    csv.load(path + "gender.csv");
    foreach (const auto &row, csv.rows)
    {
        GenderConfigLibrary::GenderConfig genderConfig;
        genderConfig.paymentAbility = row["PaymentAbility"].toDouble();
        genderConfig.webcamImportance = row["WebCamQuality"].toDouble();
        if (row["Gender"] == "0")
            genderConfigLibrary.male = genderConfig;
        else
            genderConfigLibrary.female = genderConfig;
    }

    csv.load(path + "alphaPaymentAbility.csv");
    foreach (const auto &row, csv.rows)
    {
        AlphaPaymentAbilityLibrary::AlphaPaymentAbilityConfig paymentConfig;
        paymentConfig.value = row["AlphaLevel"].toDouble();
        paymentConfig.paymentAbilitySameGender = row["PaymentAbilitySameGender"].toDouble();
        paymentConfig.paymentAbilityOppositeGender = row["PaymentAbilityOppositeGender"].toDouble();
        alphaPaymentAbilityLibrary.data.append(paymentConfig);
    }

    csv.load(path + "casuality.csv");
    foreach (const auto &row, csv.rows)
    {
        CasualityLibrary::CasualityConfig casualityConfig;
        casualityConfig.value = row["Casuality"].toDouble();
        casualityConfig.searchingRates = row["SearchingRates"].toDouble();
        casualityConfig.interactiveInterest = row["InteractiveInterest"].toDouble();
        casualityConfig.skillInterest = row["SkillInterest"].toDouble();
        casualityLibrary.data.append(casualityConfig);
    }


}
