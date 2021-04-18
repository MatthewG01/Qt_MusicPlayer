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

    QDirIterator combo("../Playlists", QDir::Files);
    while (combo.hasNext())
    {
        combo.next();
        ui->selectPlaylist->addItem(combo.fileName().left(combo.fileName().size() - 4));
    }


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

    qPlaylist.playlist->save(QUrl::fromLocalFile("../Playlists/All Music.m3u"), "m3u");
    qDebug() << qPlaylist.playlist->save(QUrl::fromLocalFile("../Playlists/All Music.m3u"), "m3u");

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

    Playlist newPlaylist(this, newPlaylistName);
    newPlaylist.setName(newPlaylistName);

    for(int i = 0; i < selectedTrackNames.size(); i++)
    {
        selectedTrackPaths.append("../MP3 Files/" + selectedTrackNames[i]);
        newPlaylist.playlist->addMedia(QUrl(selectedTrackPaths[i]));
        qDebug() << selectedTrackPaths[i];
        /*for(int j =0; j < selectedTrackNames.size(); j++)
        {
            newPlaylist.playlist->addMedia(QUrl(selectedTrackPaths[j]));
        }*/
    }

    selectedTrackNames.clear();
    selectedTrackPaths.clear();

    ui->selectPlaylist->addItem(newPlaylist.getName());
    ui->playlistTitle->clear();
    ui->playlistTitle->insert(newPlaylist.getName());

    newPlaylist.playlist->save(QUrl::fromLocalFile("../Playlists/" + newPlaylistName + ".m3u"), "m3u");
    qDebug() << newPlaylist.playlist->save((QUrl::fromLocalFile("../Playlists/" + newPlaylistName + ".m3u")), "m3u");

    //player->setPlaylist(newPlaylist.playlist);

    player->setPlaylist(newPlaylist.playlist);

    return newPlaylist;

}






void Widget::on_pause_clicked()
{
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
    qPlaylist.playlist->clear();
    qDebug () << qPlaylist.playlist->clear();

    //Playlist playlist = Playlist(this, "Test");

    //playlist.playlist->clear();

    qPlaylist.playlist->load(QUrl::fromLocalFile("../Playlists/" + ui->selectPlaylist->currentText() + ".m3u"), "m3u");
    if(qPlaylist.playlist->error())
        qDebug() << qPlaylist.playlist->errorString();

    //player->playlist()->load(QUrl::fromLocalFile("../Playlists/" + ui->selectPlaylist->currentText()), "m3u");
    //qPlaylist.playlist->load(QUrl::fromLocalFile("../Playlists/" + ui->selectPlaylist->currentText()), "m3u");
    player->setPlaylist(qPlaylist.playlist);
    qDebug() << "../Playlists/" + ui->selectPlaylist->currentText();



}
