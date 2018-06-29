#include <QDebug>
#include <QFile>
#include <QDir>
#include "thememanager.h"

ThemeInfo::ThemeInfo(const QString& name, const QString& displayName, QObject *parent) : QObject(parent) {
    mName = name;
    mDisplayName = displayName;
}

ThemeManager::ThemeManager(QObject* parent) : QObject(parent),
    mTheme(""),
    mThemeData(""),
    mProperties(new ThemeProperties(parent))
{

}

QVariantList ThemeManager::getThemes() {
    QVariantList ret;

    for(QString dir: QDir(":/themes").entryList()) {
        QString name = dir.mid(0, dir.lastIndexOf('.'));

        QFile dataFile(":/themes/" + name + ".css");
        if(dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString themeData = dataFile.readAll();

            ThemeProperties* props = new ThemeProperties();
            props->load(themeData);

            QString displayName = props->get("Main", "name");
            if(displayName.length() > 0) {
                QVariant variant;
                variant.setValue(new ThemeInfo(name, displayName, this));
                ret.append(variant);
            }

            delete props;
        }
    }

    return ret;
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

    mProperties->load(mThemeData);
    emit propertiesChanged();
}
