#include "Playlist.h"

Playlist::Playlist(QObject *Widget, QString name)
{
    playlist = new QMediaPlaylist(Widget);
    playlistName = name;
}
