#ifndef WIDGET_H
#define WIDGET_H

#include "Playlist.h"

#include <QtMultimedia>
#include <QListWidgetItem>
#include <QMessageBox>

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
