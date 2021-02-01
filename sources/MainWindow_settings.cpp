#include "headers/MainWindow.h"

#include <QSettings>

void MainWindow::setupSettings()
{
    QSettings settings("myFavShrimp", "myFavNotes");
    settings.beginGroup("Settings");
    for (Setting item : defaultSettings) {
        item.value = settings.value(item.name, item.value);
    }
}

void MainWindow::saveSetting(const QString &key, const QVariant &value)
{
    QSettings settings("myFavShrimp", "myFavNotes");
    settings.beginGroup("Settings");
    settings.setValue(key, value);
}

QVariant MainWindow::loadSetting(const QString &key)
{
    QSettings settings("myFavShrimp", "myFavNotes");
    settings.beginGroup("Settings");
    return settings.value(key);
}
