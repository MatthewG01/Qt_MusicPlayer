#include "Playlist.h"

Playlist::Playlist(QObject *Widget, QString name)
{
    playlist = new QMediaPlaylist(Widget);
    playlistName = name;

}

void Playlist::setName(QString name)
{
    name = playlistName;
}

QString Playlist::getName()
{
    return playlistName;
}

