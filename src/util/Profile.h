#ifndef PROFILE_H
#define PROFILE_H

#include <QString>

#include <vector>

struct Profile {
    std::vector<QString> weatherTypes;
    std::vector<QString> scrollingTypes;
    std::vector<QString> iconNames;

    void setDefaults();
    bool load(const QString& path);

private:
    void setDefaultWeather();
    void setDefaultScrolling();
    void setDefaultIconNames();
};

#endif // PROFILE_H
