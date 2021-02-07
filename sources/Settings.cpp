#include <Settings.h>

void settings::setupSettings() {
    QSettings settings = getAppSettings();
    settings.beginGroup("Settings");

    QHash<QString, QVariant> defaultSettings = getDefaultSettings();
    QHashIterator<QString, QVariant> settingsIterator(defaultSettings);

    while (settingsIterator.hasNext()) {
        settingsIterator.next();
        QVariant value = settings.value(settingsIterator.key(), settingsIterator.value());
        saveSetting(settingsIterator.key(), value);
    }
}

void settings::saveSetting(const QString &key, const QVariant &value) {
    QSettings settings = getAppSettings();
    settings.beginGroup("Settings");
    settings.setValue(key, value);
}

QVariant settings::loadSetting(const QString &key) {
    QSettings settings = getAppSettings();
    settings.beginGroup("Settings");
    return settings.value(key);
}

QSettings settings::getAppSettings() {
    return QSettings(QSettings::IniFormat, QSettings::UserScope, "myFavNotes", "myFavNotes");
}

QHash<QString, QVariant> settings::getDefaultSettings() {
    QHash<QString, QVariant> defaultSettings;

    defaultSettings.insert("AlwaysOnTop", false);
    defaultSettings.insert("DBPath", "");

    return defaultSettings;
}
