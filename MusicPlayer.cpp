#include "MusicPlayer.h"
#include "ui_MusicPlayer.h"

QVector<QString> mp3Files {};
QVector<QString> trackNames {};
QList<QListWidgetItem *> selectedTracks {};
//QList<QListWidgetItem *> playlistTracks {};
QVector<QString> selectedTrackNames {};
QVector<QString> selectedTrackPaths {};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
    //load();



}

Widget::~Widget()
{
    delete ui;
    delete (player);
}

void Widget::init()
{


    player = new QMediaPlayer(this);
    qPlaylist = Playlist(this, "All Music");

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
        qPlaylist.playlist->addMedia(QUrl(mp3Files[i]));
    }

    qPlaylist.playlist->save(QUrl::fromLocalFile("../Playlists/All Music"), "m3u");
    qDebug() << qPlaylist.playlist->save(QUrl::fromLocalFile("../Playlists/All Music"), "m3u");

    ui->selectPlaylist->addItem(qPlaylist.playlistName);

    ui->playlistTitle->setReadOnly(true);
    ui->playlistTitle->insert(qPlaylist.playlistName);
    player->setPlaylist(qPlaylist.playlist);


    ui->track->setReadOnly(true);
    ui->track->insert(trackNames[qPlaylist.playlist->currentIndex()]);

    /*Connects the QMediaPlayer signals for positionChanged and durationChanged to their corresponding widget
     *slots, allowing the user to use the position slider to determine where a song plays from. Referenced
     *from a Bryan Cairns tutorial on the VoidRealms YouTube channel, see declaration.
    */
    connect(player, &QMediaPlayer::positionChanged, this, &Widget::on_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &Widget::on_durationChanged);

}

Playlist Widget::newPlaylist(QString newPlaylistName)
{
    //Playlist creation code goes here
    //return Playlist(this);
    for(int i = 0; i < selectedTrackNames.size(); i++)
    {
        selectedTrackPaths.append("../MP3 Files/" + selectedTrackNames[i]);
    }
}






void Widget::on_pause_clicked()
{
    /*Need to add to this section to make pause button work as intended rather than stopping current song
    *and going back to beginning of playlist/song
    */
    player->position();
    player->pause();
}

void Widget::on_skipBackButton_clicked()
{
    //Allows you to skip song. Uses a CONSTANT so will have to check this works when new playlists are created.
    (player->playlist())->previous();
    ui->track->clear();

    qDebug() << qPlaylist.playlist->currentIndex();

    if (qPlaylist.playlist->currentIndex() < 0)
        ui->track->insert(trackNames[qPlaylist.playlist->mediaCount() - 1]);
    else
        ui->track->insert(trackNames[qPlaylist.playlist->currentIndex()]);
}

void Widget::on_skipForwardButton_clicked()
{
    //Allows you to skip song. Uses a CONSTANT so will have to check this works when new playlists are created.
    (player->playlist())->next();
    ui->track->clear();

    qDebug() << qPlaylist.playlist->currentIndex();

    if (qPlaylist.playlist->currentIndex() < 0)
        ui->track->insert(trackNames[0]);
    else
        ui->track->insert(trackNames[qPlaylist.playlist->currentIndex()]);


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

    selectedTracks = ui->selectMusic->selectedItems();
    for (int i = 0; i < selectedTracks.size(); i++)
    {
        //if(int test = QString::compare(selectedTrackNames[i], ui->playlistItems->findItems(selectedTrackNames[i], Qt::MatchExactly) != 0)
            //Add nothing
        //else
            selectedTrackNames.append(selectedTracks[i]->text());
            ui->playlistItems->addItem(selectedTrackNames[i]);
    }

    selectedTrackNames.clear();
}

void Widget::on_playlistSave_accepted()
{
    newPlaylist(ui->enteredName->text());
}

void Widget::on_playlistSave_rejected()
{
    ui->playlistItems->clear();
}

