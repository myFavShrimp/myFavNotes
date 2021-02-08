#include <Settings.h>

QString settings::settingName(settings::Setting num)
{
    QStringList settings = {
            "AlwaysOnTop",
            "DBPath",
            "TimestampFormat"
    };
    return settings[num];
}

void settings::setupSettings()
{
    QSettings settings = getAppSettings();
    settings.beginGroup("Settings");

    QHash<settings::Setting, QVariant> defaultSettings = getDefaultSettings();
    QHashIterator<settings::Setting, QVariant> settingsIterator(defaultSettings);

    while (settingsIterator.hasNext()) {
        settingsIterator.next();
        QVariant value = settings.value(settingName(settingsIterator.key()), settingsIterator.value());
        saveSetting(settingsIterator.key(), value);
    }
}

void settings::saveSetting(const Setting &key, const QVariant &value)
{
    QSettings settings = getAppSettings();
    settings.beginGroup("Settings");
    settings.setValue(settingName(key), value);
}

QVariant settings::loadSetting(const Setting &key)
{
    QSettings settings = getAppSettings();
    settings.beginGroup("Settings");
    return settings.value(settingName(key));
}

QSettings settings::getAppSettings()
{
    return QSettings(QSettings::IniFormat, QSettings::UserScope, "myFavNotes", "myFavNotes");
}

QHash<settings::Setting, QVariant> settings::getDefaultSettings()
{
    QHash<settings::Setting, QVariant> defaultSettings;

    defaultSettings.insert(Setting::AlwaysOnTop, false);
    defaultSettings.insert(Setting::DBPath, "");
    defaultSettings.insert(Setting::TimestampFormat, "dd.MM.yyyy hh:mm:ss");

    return defaultSettings;
}
