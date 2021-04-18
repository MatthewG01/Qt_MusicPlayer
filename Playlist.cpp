#include "Playlist.h"

Playlist::Playlist(QObject *Widget, QString name)
{
    playlist = new QMediaPlaylist(Widget);
    playlistName = name;

}

void Playlist::setName(QString aName)
{
    playlistName = aName;
}

QString Playlist::getName()
{
    return playlistName;
}

void Playlist::setPlaylist(QMediaPlaylist *aPlaylist)
{
    playlist = aPlaylist;
}

QMediaPlaylist *Playlist::getPlaylist()
{
    return playlist;
}

