#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QDebug>
#include <QObject>
#include <QMap>
#include "themeproperties.h"

class ThemeInfo : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER mName)
    Q_PROPERTY(QString displayName MEMBER mDisplayName)

public:
    ThemeInfo(QObject* parent = nullptr) : QObject(parent) {}
    ThemeInfo(const ThemeInfo &other) : QObject(nullptr) {
        mName = other.mName;
        mDisplayName = other.mDisplayName;
    }
    ~ThemeInfo() {}

    explicit ThemeInfo(const QString& name, const QString& displayName, QObject *parent = nullptr);

private:
    QString mName;
    QString mDisplayName;
};

Q_DECLARE_METATYPE(ThemeInfo*)

class ThemeManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_PROPERTY(ThemeProperties* properties MEMBER mProperties NOTIFY propertiesChanged)

public:
    explicit ThemeManager(QObject *parent = nullptr);

    Q_INVOKABLE QVariantList getThemes();

    Q_INVOKABLE QString themeData() const;

    QString theme() const;

    void setTheme(const QString &theme);

signals:
    void themeChanged();
    void propertiesChanged();

protected:
    void loadTheme();

private:
    QString mTheme;
    QString mThemeData;

    ThemeProperties *mProperties;
};

#endif // THEMEMANAGER_H
