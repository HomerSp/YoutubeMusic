#ifndef YOUTUBEMUSICVIEW_H
#define YOUTUBEMUSICVIEW_H

#include <QQuickItem>
#include <QMouseEvent>

class WebEngineViewFix : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(WebMouseButton button READ button NOTIFY buttonChanged)
public:
    explicit WebEngineViewFix(QQuickItem* parent = 0);

    enum WebMouseButton { NoButton, ForwardButton, BackButton };
    Q_ENUM(WebMouseButton)

    WebMouseButton button() const;

signals:
    void buttonChanged();

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    WebMouseButton mButton;
};

#endif // YOUTUBEMUSICVIEW_H
