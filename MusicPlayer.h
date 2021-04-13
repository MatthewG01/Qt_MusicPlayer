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

#include "Playlist.h"

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



    void on_pause_clicked();

    void on_skipBackButton_clicked();

    void on_skipForwardButton_clicked();

    void on_progressSlider_sliderMoved(int position);

    void on_volumeSlider_sliderMoved(int position);

    void on_play_clicked();

private:
    Ui::Widget *ui;
    QMediaPlayer *player;
    Playlist qPlaylist;

    void init();
    Playlist newPlaylist();

};
#endif // WIDGET_H
