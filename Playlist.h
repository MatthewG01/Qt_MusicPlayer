#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QWidget>
#include <QString>
#include <QMediaPlaylist>
#include <QMediaContent>

class Playlist
{
public:
    Playlist(QObject *Widget = nullptr, QString name = "default playlist");

    QString playlistName;
    QMediaPlaylist *playlist;

    void setName(QString name);
    QString getName();
};

#endif // PLAYLIST_H
