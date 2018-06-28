#include "themeproperties.h"

ThemeProperties::ThemeProperties(QObject *parent) : QObject(parent)
{

}

QString ThemeProperties::getColor(const QString& element, const QString& prop, QString def, QString state) const {
    if(state.length() > 0 && mProperties.find(element + ":" + state) != mProperties.end()) {
        QMap<QString, QString> props = mProperties.find(element + ":" + state).value();
        if(props.find(prop) != props.end()) {
            return props.find(prop).value();
        }
    }

    if(mProperties.find(element) == mProperties.end()) {
        return def;
    }

    QMap<QString, QString> props = mProperties.find(element).value();
    if(props.find(prop) != props.end()) {
        return props.find(prop).value();
    }

    return def;
}

void ThemeProperties::load(const QString& theme) {
    QFile dataFile(":/themes/" + theme + ".css");
    if(!dataFile.exists()) {
        dataFile.setFileName(":/themes/default.css");
    }

    if(dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while(!dataFile.atEnd()) {
            QString line = dataFile.readLine().trimmed();
            if(line.startsWith("qml")) {
                QString element = line.mid(3).trimmed();
                element = element.mid(0, element.indexOf('{')).trimmed();

                QMap<QString, QString> props;

                while(!dataFile.atEnd()) {
                    QString propsLine = dataFile.readLine().trimmed();
                    if(propsLine == "}") {
                        break;
                    }

                    QStringList arr = propsLine.split(":", QString::SkipEmptyParts);
                    if(arr.size() == 2) {
                        props.insert(arr.at(0).trimmed(), arr.at(1).mid(0, arr.at(1).indexOf(';')).trimmed());
                    }
                }

                mProperties.insert(element, props);
            }
        }
    }
}
