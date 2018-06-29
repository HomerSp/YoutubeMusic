#ifndef THEMEPROPERTIES_H
#define THEMEPROPERTIES_H

#include <QQuickItem>
#include <QMap>

class ThemeProperties : public QObject
{
    Q_OBJECT
public:
    ThemeProperties(const ThemeProperties &other) : QObject(nullptr) { mProperties = other.mProperties; }
    ~ThemeProperties() { }

    explicit ThemeProperties(QObject *parent = nullptr);

    Q_INVOKABLE QString get(const QString& element, const QString& prop, QString def = "", QString state = "") const;

    void load(QString& themeData);

signals:

public slots:

private:
    QMap<QString, QMap<QString, QString> > mProperties;
};

Q_DECLARE_METATYPE(ThemeProperties*)

#endif // THEMEPROPERTIES_H
