#include "Playlist.h"

Playlist::Playlist(QObject *Widget, QString name)
{
    playlist = new QMediaPlaylist(Widget);
    playlistName = name;
}

//Allows user to name created playlist in newPlaylist()
void Playlist::setName(QString aName)
{
    playlistName = aName;
}

//Retrieves the name of the playlist so that it can be displayed and have its name used to access corresponding contents file
QString Playlist::getName()
{
    return playlistName;
}

//Allows you to set the playlist being used by the player by incorporating getPlaylist()
void Playlist::setPlaylist(QMediaPlaylist *aPlaylist)
{
    playlist = aPlaylist;
}

//Gets the playlist
QMediaPlaylist *Playlist::getPlaylist()
{
    return playlist;
}

//Sets the playlist tracks
void Playlist::setTracks(QVector<QString> tracksToSet)
{
    tracks = tracksToSet;
}

//Retrieve playlist tracks to be used in current track line edit
QString Playlist::getTrack(int index)
{
    return tracks[index];
}





