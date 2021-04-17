#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QWidget>
#include <QString>
#include <QMediaPlaylist>
#include <QMediaContent>

class Playlist
{
public:
    Playlist(QObject *Widget = nullptr, QString name = "New Playlist");

    //Make variables private member variables?
    QString playlistName;
    QMediaPlaylist *playlist;

    void setName(QString name);
    QString getName();
    QMediaPlaylist newPlaylist(QString newPlaylistName);
};

#endif // PLAYLIST_H
