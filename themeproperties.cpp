#include "themeproperties.h"

ThemeProperties::ThemeProperties(QObject *parent) : QObject(parent)
{

}

QString ThemeProperties::get(const QString& element, const QString& prop, QString def, QString state) const {
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

void ThemeProperties::load(QString &themeData) {
    QTextStream stream(&themeData, QIODevice::ReadOnly);
    while(!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if(line.startsWith("qml")) {
            QString element = line.mid(3).trimmed();
            element = element.mid(0, element.indexOf('{')).trimmed();

            QMap<QString, QString> props;

            while(!stream.atEnd()) {
                QString propsLine = stream.readLine().trimmed();
                if(propsLine == "}") {
                    break;
                }

                QStringList arr = propsLine.split(":", QString::SkipEmptyParts);
                if(arr.size() == 2) {
                    QString key = arr.at(0).trimmed();
                    QString value = arr.at(1).mid(0, arr.at(1).indexOf(';')).trimmed();
                    if(value.length() > 0 && value.at(0) == '"') {
                        value = value.mid(1, value.indexOf('"', 1) - 1);
                    }

                    props.insert(key, value);
                }
            }

            mProperties.insert(element, props);
        }
    }
}
