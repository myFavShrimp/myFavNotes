#ifndef MYFAVNOTES_SETTINGS_H
#define MYFAVNOTES_SETTINGS_H

#include <QSettings>

namespace settings {
    enum Setting {
        AlwaysOnTop, DBPath
    };

    QString settingName(Setting num);

    QHash<settings::Setting, QVariant> getDefaultSettings();

    QSettings getAppSettings();

    void setupSettings();
    void saveSetting(const Setting &key, const QVariant &value);
    QVariant loadSetting(const Setting &key);

};

#endif //MYFAVNOTES_SETTINGS_H
