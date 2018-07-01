#include <QDebug>
#include <QRegularExpression>
#include <QTextStream>
#include "cssparser.h"

CssElement::CssElement(CssElement* other) : CssElement(other->mParser, other->mName) {
    for(QString key: other->mChildren.keys()) {
        CssElement* ch = other->mChildren[key];
        mChildren.insert(key, new CssElement(ch));
    }

    for(QString prop: other->mProperties.keys()) {
        mProperties.insert(prop, other->mProperties[prop]);
    }
}

CssElement::CssElement(const CssParser& parser) : mParser(parser) {

}

CssElement::CssElement(const CssParser& parser, const QString &name) : mParser(parser),
    mName(name) {

}

CssElement::~CssElement() {
    for(CssElement* child: mChildren) {
        delete child;
    }
}

const CssElement& CssElement::child(const QString& name) {
    return *childPtr(name);
}

bool CssElement::has(const QString& prop) const {
    return mProperties.find(prop) != mProperties.end();
}

QString CssElement::prop(const QString& name) const {
    if(mProperties.find(name) != mProperties.end()) {
        return mProperties.find(name).value();
    }

    return "";
}

CssElement* CssElement::childPtr(const QString& name, bool strict) {
    if(mChildren.find(name) != mChildren.end()) {
        return mChildren.find(name).value();
    }

    // Do we have a state?
    if(!strict) {
        if(name.contains(':')) {
            QString nameState = name.mid(0, name.indexOf(':'));
            if(mChildren.find(nameState) != mChildren.end()) {
                return mChildren.find(nameState).value();
            }
        }
    }

    return mParser.mInvalidElement;
}

void CssElement::copyProps(CssElement *other) {
    for(QString propKey: other->mProperties.keys()) {
        if(!mProperties.contains(propKey)) {
            mProperties.insert(propKey, other->mProperties.value(propKey));
        }
    }

    for(QString key: mChildren.keys()) {
        if(!other->mChildren.contains(key)) {
            continue;
        }

        mChildren[key]->copyProps(other->mChildren[key]);
    }
}

void CssElement::updateStates() {
    for(QString key: mChildren.keys()) {
        if(key.contains(':')) {
            QString name = key.mid(0, key.indexOf(':'));
            if(!mChildren.contains(name)) {
                continue;
            }

            mChildren[key]->copyProps(mChildren[name]);
        }

        mChildren[key]->updateStates();
    }
}

void CssElement::parseBlock(const QString &blockData) {
    QString data = blockData.trimmed();
    int pos = 0;

    while(pos < data.size()) {
        int startValuePos = data.indexOf(':', pos);
        int endValuePos = 0;

        // Check if the value is a string
        if(data.midRef(pos).trimmed().at(0) == '\"') {
            endValuePos = data.indexOf('\"', data.indexOf('\"', startValuePos + 1) + 1) + 1;
        } else {
            endValuePos = data.indexOf(';', startValuePos + 1);
        }

        QString key = data.mid(pos, (startValuePos - pos)).trimmed();
        QString value = data.mid(startValuePos + 1, (endValuePos - startValuePos - 1)).trimmed();
        if(value[0] == '\"') {
            value = value.mid(1, value.length() - 2);
        }

        mProperties.insert(key, value);

        pos = endValuePos + 1;
    }
}

CssParser::CssParser() {
    mInvalidElement = new CssElement(*this);
}

CssParser::CssParser(const QString& cssData) : CssParser() {
    load(cssData);
}

CssParser::~CssParser() {
    delete mInvalidElement;

    for(CssElement* el: mElements) {
        delete el;
    }
}

void CssParser::copyFrom(const CssParser& other) {
    for(CssElement* el: mElements) {
        delete el;
    }

    mElements.clear();

    for(QString key: other.mElements.keys()) {
        mElements.insert(key, new CssElement(other.mElements[key]));
    }
}

void CssParser::load(const QString &cssData) {
    for(CssElement* el: mElements) {
        delete el;
    }

    mElements.clear();

    // Remove any comments
    QString data = cssData;
    data.replace(QRegularExpression("/\\*.*?\\*/", QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption), "");
    data.replace("\n", "");

    int pos = 0;
    while(pos < data.length()) {
        int startBlock = data.indexOf('{', pos);
        if(startBlock < 0) {
            break;
        }

        int endBlock = data.indexOf('}', startBlock);
        if(endBlock < 0) {
            endBlock = data.length();
        }

        QString elementsLine = data.mid(pos, (startBlock - pos)).trimmed();
        QString elementData = data.mid(startBlock + 1, (endBlock - startBlock - 1)).trimmed();
        pos = endBlock + 1;

        QStringList elements = elementsLine.split(',');
        for(QString el: elements) {
            parseBlock(el.trimmed(), elementData);
        }
    }

    for(const QString &key: mElements.keys()) {
        if(key.contains(':')) {
            QString name = key.mid(0, key.indexOf(':'));
            if(!mElements.contains(name)) {
                continue;
            }

            mElements[key]->copyProps(mElements[name]);
        }

        mElements[key]->updateStates();
    }
}

const CssElement& CssParser::child(const QString& name) {
    return *childPtr(name);
}

const CssElement& CssParser::find(const QString& line) {
    CssElement* ret = mInvalidElement;

    QStringList list = line.split(' ');
    if(list.size() == 0) {
        return *ret;
    }

    QString name = list[0];
    ret = childPtr(name);
    if(!ret->isValid()) {
        return *ret;
    }

    for(int i = 1; i < list.size(); i++) {
        name = list[i];

        ret = ret->childPtr(name);
        if(!ret->isValid()) {
            return *ret;
        }
    }

    return *ret;
}

CssElement *CssParser::childPtr(const QString& name, bool strict) {
    if(mElements.find(name) != mElements.end()) {
        return mElements.find(name).value();
    }

    // Do we have a state?
    if(!strict) {
        if(name.contains(':')) {
            QString nameState = name.mid(0, name.indexOf(':'));
            if(mElements.find(nameState) != mElements.end()) {
                return mElements.find(nameState).value();
            }
        }
    }

    return mInvalidElement;
}

void CssParser::parseBlock(const QString& elements, const QString& blockData) {
    QStringList elList = elements.split(QRegularExpression("\\s+?"));
    if(elList.length() == 0) {
        elList.append(elements);
    }

    QString elName = elList.at(0);

    CssElement* current = childPtr(elName, true);
    if(!current->isValid()) {
        current = new CssElement(*this, elName);
        mElements.insert(elName, current);
    }

    for(int i = 1; i < elList.size(); i++) {
        elName = elList.at(i);

        CssElement* currentChild = current->childPtr(elName, true);
        if(!currentChild->isValid()) {
            currentChild = new CssElement(*this, elName);
            current->mChildren.insert(elName, currentChild);
        }

        current = currentChild;
    }

    current->parseBlock(blockData);
}
