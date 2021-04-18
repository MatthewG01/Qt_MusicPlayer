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

    void setName(QString aName);
    QString getName();

    void setPlaylist(QMediaPlaylist *aPlaylist);
    QMediaPlaylist * getPlaylist();

    QMediaPlaylist newPlaylist(QString newPlaylistName);

private:
    QString playlistName;
    QMediaPlaylist *playlist;
};

#endif // PLAYLIST_H
