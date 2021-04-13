#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QtMultimedia>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QModelIndex>
#include <QDir>
#include <QDirIterator>
#include <QSettings>
#include <QVariant>
#include <QString>
#include <QVector>

#include <QUrl>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QMediaPlayer *player;
    QMediaPlaylist *defaultPlaylist;

    void init();
    //QMediaPlaylist newPlaylist();
};
#endif // WIDGET_H