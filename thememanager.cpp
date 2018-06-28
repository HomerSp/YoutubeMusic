#include <QDebug>
#include <QFile>
#include "thememanager.h"

ThemeManager::ThemeManager(QObject* parent) : QObject(parent),
    mTheme(""),
    mThemeData(""),
    mProperties(new ThemeProperties(parent))
{

}

QString ThemeManager::themeData() const {
    return mThemeData;
}

QString ThemeManager::theme() const {
    return mTheme;
}

void ThemeManager::setTheme(const QString &theme) {
    mTheme = theme;
    loadTheme();

    emit themeChanged();
}

void ThemeManager::loadTheme() {
    mThemeData = "";

    QFile dataFile(":/themes/" + mTheme + ".css");
    if(!dataFile.exists()) {
        dataFile.setFileName(":/themes/default.css");
    }

    if(dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        mThemeData = dataFile.readAll();
    }

    mProperties->load(mTheme);
    emit propertiesChanged();
}
