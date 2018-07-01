#ifndef THEMEPROPERTIES_H
#define THEMEPROPERTIES_H

#include <QQuickItem>
#include <QMap>
#include "cssparser.h"

class ThemeProperties : public QObject
{
    Q_OBJECT
public:
    ThemeProperties(const ThemeProperties &other) : QObject(nullptr) { mParser.copyFrom(other.mParser); }
    ~ThemeProperties() { }

    explicit ThemeProperties(QObject *parent = nullptr);

    Q_INVOKABLE QString get(const QString& element, const QString& prop, QString def = "", QString state = "");

    void load(QString& themeData);

signals:

public slots:

private:
    CssParser mParser;
};

Q_DECLARE_METATYPE(ThemeProperties*)

#endif // THEMEPROPERTIES_H
