#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QWidget>
#include <QString>
#include <QMediaPlaylist>   //May be able to get rid of some of these as they are already in MusicPlayer.h
#include <QMediaContent>

class Playlist
{
public:
    Playlist(QObject *Widget = nullptr, QString name = "New Playlist");

    void setName(QString aName);
    QString getName();

    void setPlaylist(QMediaPlaylist *aPlaylist);
    QMediaPlaylist * getPlaylist();

    void setTracks(QVector<QString> tracksToSet);
    QString getTrack(int index);

    QMediaPlaylist newPlaylist(QString newPlaylistName);

private:
    QString playlistName;
    QMediaPlaylist *playlist;
    QVector<QString> tracks;
};

#endif // PLAYLIST_H
