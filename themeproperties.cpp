#include "themeproperties.h"

ThemeProperties::ThemeProperties(QObject *parent) : QObject(parent),
    mParser()
{

}

QString ThemeProperties::get(const QString& element, const QString& prop, QString def, QString state) {
    QString findStr = "qml " + element;
    if(state.length() > 0) {
        findStr += ":" + state;
    }

    const CssElement& el = mParser.find(findStr);
    if(!el.isValid() || !el.has(prop)) {
        return def;
    }

    return el.prop(prop);
}

void ThemeProperties::load(QString &themeData) {
    mParser.load(themeData);
}
