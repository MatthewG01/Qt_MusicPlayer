#ifndef WIDGET_H
#define WIDGET_H

#include <Qt>
#include <QWidget>
#include <QDebug>
#include <QtMultimedia>
#include <QMediaPlayer>     //May be able to just include QtMulitmedia and take out playlist and player
#include <QMediaPlaylist>
#include <QModelIndex>
#include <QDir>
#include <QDirIterator>
#include <QSettings> //May be able to get rid of this
#include <QVariant> //May be able to get rid of this
#include <QString>
#include <QVector>
#include <QDialogButtonBox>
#include <QListWidgetItem>
#include <QComboBox>
#include <QStringList>
#include <QMessageBox>
#include <QValidator>
#include <QRegularExpressionValidator>
#include <QMediaObject>
#include <QMediaMetaData>  //May be able to delete
#include <QMediaService>  //May be able to delete
#include <QFile>
#include <QTextStream>

#include <QUrl>
#include <QFileSystemModel> //May be able to get rid of this

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

    QVector<QString> allMP3Files {};
    QVector<QString> allTrackNames {};
    QVector<QString> selectedTrackNames {};
    QVector<QString> selectedTrackPaths {};
    QVector<QString> songs {};

    QList<QListWidgetItem *> selectedTracks {};

    QStringList playlistNames {};

    QString filename {};
    QString item {};

private slots:
    void on_play_clicked();

    void on_pause_clicked();

    void on_skipBackButton_clicked();

    void on_skipForwardButton_clicked();

    void on_progressSlider_sliderMoved(int position);

    void on_volumeSlider_sliderMoved(int position);

    void on_durationChanged(qint64 position);

    void on_positionChanged(qint64 position);

    void on_playlistSave_accepted();

    void on_addToPlaylist_clicked();

    void on_playlistSave_rejected();

    void on_confirmSelected_clicked();

private:
    Ui::Widget *ui;
    QMediaPlayer *player;
    Playlist qPlaylist;
    QLineEdit *enteredName;

    void init();
    Playlist newPlaylist(QString newPlaylistName);
    void loadPlaylistNames();
};
#endif // WIDGET_H
