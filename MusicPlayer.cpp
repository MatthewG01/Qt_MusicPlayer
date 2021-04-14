#include "MusicPlayer.h"
#include "ui_MusicPlayer.h"

QVector<QString> mp3Files {};
QVector<QString> trackNames {};

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

Playlist Widget::newPlaylist()
{
    //Playlist creation code goes here
    return Playlist(this);
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
}

void Widget::on_skipForwardButton_clicked()
{
    //Allows you to skip song. Uses a CONSTANT so will have to check this works when new playlists are created.
    (player->playlist())->next();
    ui->track->clear();
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
