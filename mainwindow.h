#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QQmlApplicationEngine>
#include <QObject>

class MainWindow : public QObject
{
    Q_OBJECT
public:
    explicit MainWindow(QObject *parent = nullptr);

signals:

public slots:

private:
    QObject *mParent;
    QObject *mWebView;
};

#endif // MAINWINDOW_H
