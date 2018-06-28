#include <QDebug>
#include <QWebEngineView>
#include "webengineviewfix.h"

WebEngineViewFix::WebEngineViewFix(QQuickItem* parent) : QQuickItem(parent),
    mButton(WebMouseButton::NoButton)
{
    setAcceptedMouseButtons(Qt::BackButton | Qt::ForwardButton);
}

WebEngineViewFix::WebMouseButton WebEngineViewFix::button() const {
    return mButton;
}

void WebEngineViewFix::mousePressEvent(QMouseEvent *event) {
    switch(event->button()) {
    case Qt::ForwardButton:
        mButton = WebMouseButton::ForwardButton;
        break;
    case Qt::BackButton:
        mButton = WebMouseButton::BackButton;
        break;
    default:
        return;
    }

    emit buttonChanged();
}
