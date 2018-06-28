#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QMap>
#include "themeproperties.h"

class ThemeManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_PROPERTY(ThemeProperties* properties MEMBER mProperties NOTIFY propertiesChanged)

public:
    explicit ThemeManager(QObject *parent = nullptr);

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
