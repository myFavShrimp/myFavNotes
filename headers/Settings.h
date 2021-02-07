#ifndef MYFAVNOTES_SETTINGS_H
#define MYFAVNOTES_SETTINGS_H

#include <QSettings>

namespace settings {
    QHash<QString, QVariant> getDefaultSettings();

    QSettings getAppSettings();

    void setupSettings();
    void saveSetting(const QString &key, const QVariant &value);
    QVariant loadSetting(const QString &key);

};

#endif //MYFAVNOTES_SETTINGS_H
