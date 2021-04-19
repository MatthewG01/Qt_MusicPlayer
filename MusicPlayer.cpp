#include "MusicPlayer.h"
#include "ui_MusicPlayer.h"

QVector<QString> mp3Files {};
QVector<QString> trackNames {};
QList<QListWidgetItem *> selectedTracks {};
QVector<QString> selectedTrackNames {};
QVector<QString> selectedTrackPaths {};
QStringList playlistNames {};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
}

Widget::~Widget()
{
    delete ui;
    delete (player);
}

void Widget::init()
{
    //Creates a QMediaPlayer and a playlist containing All Music located in the MP3 Files folder
    player = new QMediaPlayer(this);
    qPlaylist = Playlist(this, "All Music");

    //Adds tracks to the QMediaPlaylist by using files in the MP3 Files folder
    QDirIterator music("../MP3 Files", QDir::Files);
    while (music.hasNext())
    {
        music.next();
        ui->selectMusic->addItem(music.fileName());
        trackNames.append(music.fileName());
        mp3Files.append(music.filePath());
    }

    for (int i = 0; i < mp3Files.size(); i++)
    {
        qPlaylist.getPlaylist()->addMedia(QUrl(mp3Files[i]));
    }

    qPlaylist.getPlaylist()->save(QUrl::fromLocalFile("../Playlists/All Music.m3u"), "m3u");
    qDebug() << qPlaylist.getPlaylist()->save(QUrl::fromLocalFile("../Playlists/All Music.m3u"), "m3u");

    playlistNames.append(qPlaylist.getName());
    ui->selectPlaylist->clear();
    loadPlaylistNames();

    ui->playlistTitle->setReadOnly(true);
    ui->playlistTitle->insert(qPlaylist.getName());
    player->setPlaylist(qPlaylist.getPlaylist());


    ui->track->setReadOnly(true);
    ui->track->insert(trackNames[qPlaylist.getPlaylist()->currentIndex()]);

    /*Connects the QMediaPlayer signals for positionChanged and durationChanged to their corresponding widget
     *slots, allowing the user to use the position slider to determine where a song plays from. Referenced
     *from a Bryan Cairns tutorial on the VoidRealms YouTube channel, see declaration.
    */
    connect(player, &QMediaPlayer::positionChanged, this, &Widget::on_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &Widget::on_durationChanged);
}

Playlist Widget::newPlaylist(QString newPlaylistName)
{
    //Creates a new playlist named by the user
    Playlist newPlaylist(this, newPlaylistName);
    newPlaylist.setName(newPlaylistName);

    //Takes the music selected by the user and populates the new playlist with the paths of the selected music

    for(int i = 0; i < selectedTrackNames.size(); i++)
    {
        selectedTrackPaths.append("../MP3 Files/" + selectedTrackNames[i]);
        newPlaylist.getPlaylist()->addMedia(QUrl(selectedTrackPaths[i]));
        qDebug() << selectedTrackPaths[i];
    }

    //Clears the selected track names and paths so that they don't interfere with the next new playlist created by user
    selectedTrackNames.clear();
    selectedTrackPaths.clear();

    playlistNames.append(newPlaylist.getName());
    ui->selectPlaylist->clear();
    loadPlaylistNames();

    ui->playlistTitle->clear();
    ui->playlistTitle->insert(newPlaylist.getName());

    newPlaylist.getPlaylist()->save(QUrl::fromLocalFile("../Playlists/" + newPlaylistName + ".m3u"), "m3u");
    qDebug() << newPlaylist.getPlaylist()->save((QUrl::fromLocalFile("../Playlists/" + newPlaylistName + ".m3u")), "m3u");

    player->setPlaylist(newPlaylist.getPlaylist());

    return newPlaylist;
}

void Widget::loadPlaylistNames()
{
    //Used to load existing playlist names and prevent duplicate playlists appearing in the dropdown box
    QDirIterator combo("../Playlists", QDir::Files);
    while (combo.hasNext())
    {
        combo.next();
        playlistNames.append(combo.fileName());
        playlistNames.replaceInStrings(".m3u", "");
        playlistNames.removeDuplicates();
        ui->selectPlaylist->clear();
        for (int i = 0; i < playlistNames.size(); i++)
        {
            ui->selectPlaylist->addItem(playlistNames[i]);
        }
    }
}

void Widget::on_pause_clicked()
{
    player->pause();
}

void Widget::on_skipBackButton_clicked()
{
    //Allows you to skip song. Playlist will loop back around if you skip backwards at the beginning of the playlist
    (player->playlist())->previous();
    ui->track->clear();

    qDebug() << qPlaylist.getPlaylist()->currentIndex();

    if (qPlaylist.getPlaylist()->currentIndex() < 0)
        ui->track->insert(trackNames[qPlaylist.getPlaylist()->mediaCount() - 1]);
    else
        ui->track->insert(trackNames[qPlaylist.getPlaylist()->currentIndex()]);
}

void Widget::on_skipForwardButton_clicked()
{
    //Allows you to skip song. Playlist will loop back around if you skip forwards at the beginning of the playlist
    (player->playlist())->next();
    ui->track->clear();

    qDebug() << qPlaylist.getPlaylist()->currentIndex();

    if (qPlaylist.getPlaylist()->currentIndex() < 0)
        ui->track->insert(trackNames[0]);
    else
        ui->track->insert(trackNames[qPlaylist.getPlaylist()->currentIndex()]);
}

void Widget::on_progressSlider_sliderMoved(int position)
{
    player->setPosition(position);
}

void Widget::on_positionChanged(qint64 position)
{
    ui->progressSlider->setValue(position);
}

void Widget::on_durationChanged(qint64 position)
{
    ui->progressSlider->setMaximum(position);
}

void Widget::on_volumeSlider_sliderMoved(int position)
{
    player->setVolume(position);
}

void Widget::on_play_clicked()
{
    player->play();
}

void Widget::on_addToPlaylist_clicked()
{
    //Takes the tracks selected by the user and adds them to the playlist
    selectedTracks = ui->selectMusic->selectedItems();
    for (int i = 0; i < selectedTracks.size(); i++)
    {
            selectedTrackNames.append(selectedTracks[i]->text());
            ui->playlistItems->addItem(selectedTrackNames[i]);
    }

    selectedTracks.clear();
}

void Widget::on_playlistSave_accepted()
{
    qPlaylist = newPlaylist(ui->enteredName->text());
}

void Widget::on_playlistSave_rejected()
{
    ui->playlistItems->clear();
}


void Widget::on_confirmSelected_clicked()
{
    //Loads the currently displayed playlist in the dropdown into the player, allowing user to confirm their playlist selection
    qPlaylist.getPlaylist()->clear();
    qDebug () << qPlaylist.getPlaylist()->clear();

    qPlaylist.getPlaylist()->load(QUrl::fromLocalFile("../Playlists/" + ui->selectPlaylist->currentText() + ".m3u"), "m3u");
    if(qPlaylist.getPlaylist()->error())
        qDebug() << qPlaylist.getPlaylist()->errorString();

    player->setPlaylist(qPlaylist.getPlaylist());
    qDebug() << "../Playlists/" + ui->selectPlaylist->currentText();
}
