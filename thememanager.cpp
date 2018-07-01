#include <QDebug>
#include <QFile>
#include <QDir>
#include "thememanager.h"
#include "cssparser.h"

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
    QMap<QString, QString> sortedList;

    for(QString dir: QDir(":/themes").entryList()) {
        QString name = dir.mid(0, dir.lastIndexOf('.'));

        QFile dataFile(":/themes/" + name + ".css");
        if(dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            CssParser parser(dataFile.readAll());

            QString displayName = parser.find("qml Main").prop("name");
            if(displayName.length() > 0) {
                sortedList[displayName] = name;
            }
        }
    }

    QVariantList ret;
    for(QString displayName: sortedList.keys()) {
        QVariant variant;
        variant.setValue(new ThemeInfo(sortedList.value(displayName), displayName, this));
        ret.append(variant);
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
