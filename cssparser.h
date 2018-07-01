#ifndef CSSPARSER_H
#define CSSPARSER_H

#include <QObject>

class CssParser;

class CssElement {
public:
    CssElement(CssElement* other);
    CssElement(const CssParser& parser);
    CssElement(const CssParser& parser, const QString &name);
    ~CssElement();

    bool isValid() const {
        return mName.length() > 0;
    }

    const CssElement& child(const QString& name);

    bool has(const QString& prop) const;
    QString prop(const QString& name) const;

protected:
    CssElement* childPtr(const QString& name, bool strict = false);

    void copyProps(CssElement* other);
    void updateStates();

    void parseBlock(const QString& blockData);

private:
    const CssParser& mParser;
    QString mName;
    QHash<QString, CssElement*> mChildren;
    QHash<QString, QString> mProperties;

    friend class CssParser;
};

class CssParser {
public:
    CssParser();
    CssParser(const QString& cssData);
    ~CssParser();

    void copyFrom(const CssParser& other);
    void load(const QString& cssData);

    const CssElement& child(const QString& name);
    const CssElement& find(const QString& line);

protected:
    CssElement *childPtr(const QString& name, bool strict = false);
    void parseBlock(const QString& elementData, const QString& blockData);

private:
    CssElement* mInvalidElement;
    QHash<QString, CssElement*> mElements;

    friend class CssElement;
};

#endif // CSSPARSER_H
