#include "Profile.h"

#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#define READ_JSON_ARRAY(key, value) if (jsonDoc[key].isArray()) value = jsonDoc[key].toArray();

bool Profile::load(const QString& path) {
    QFile file(path);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data));

    weatherTypes.clear();

    QJsonArray weatherArray;
    READ_JSON_ARRAY("weather_types", weatherArray)
    if (!weatherArray.empty()) {
        for (int i = 0; i < weatherArray.size(); ++i) {
            weatherTypes.push_back(weatherArray[i].toString());
        }
    } else {
        setDefaultWeather();
    }

    scrollingTypes.clear();
    QJsonArray scrollingArray;
    READ_JSON_ARRAY("scrolling_types", scrollingArray);

    if (!scrollingArray.empty()) {
        for (int i = 0; i < scrollingArray.size(); ++i) {
            scrollingTypes.push_back(scrollingArray[i].toString());
        }
    } else {
        setDefaultScrolling();
    }

    iconNames.clear();
    QJsonArray iconArray;
    READ_JSON_ARRAY("icon_names", iconArray);

    if (!iconArray.empty()) {
        for (int i = 0; i < iconArray.size(); ++i) {
            iconNames.push_back(iconArray[i].toString());
        }
    } else {
        setDefaultIconNames();
    }

    return true;
}

void Profile::setDefaults() {
    setDefaultWeather();
    setDefaultScrolling();
    setDefaultIconNames();
}

void Profile::setDefaultWeather() {
    weatherTypes.push_back("Normal");
    weatherTypes.push_back("Rain");
    weatherTypes.push_back("Leaves");
    weatherTypes.push_back("Rain & Leaves");
    weatherTypes.push_back("Snow");
}

void Profile::setDefaultScrolling() {
    scrollingTypes.push_back("Static");
    scrollingTypes.push_back("Vertical");
    scrollingTypes.push_back("Horizontal");
    scrollingTypes.push_back("Vertical & Horizontal");
}

void Profile::setDefaultIconNames() {
    iconNames.push_back("Question mark");
    iconNames.push_back("Forest Hill");
    iconNames.push_back("Forest Hill at night");
    iconNames.push_back("Deep Forest");
    iconNames.push_back("Deep Forest at Night");
    iconNames.push_back("Field");
    iconNames.push_back("Field at night");
    iconNames.push_back("Mountains");
    iconNames.push_back("Castle");
    iconNames.push_back("Red Castle");
    iconNames.push_back("Cave");
    iconNames.push_back("Boss Battle");
    iconNames.push_back("Factory");
    iconNames.push_back("Custom icon #14");
    iconNames.push_back("Custom icon #15");
    iconNames.push_back("Custom icon #16");
    iconNames.push_back("Custom icon #17");
    iconNames.push_back("Custom icon #18");
    iconNames.push_back("Custom icon #19");
    iconNames.push_back("Custom icon #20");
    iconNames.push_back("Custom icon #21");
    iconNames.push_back("Custom icon #22");
}
