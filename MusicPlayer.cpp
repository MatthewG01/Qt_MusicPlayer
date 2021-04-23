#include "MusicPlayer.h"
#include "ui_MusicPlayer.h"

QVector<QString> mp3Files {};
QVector<QString> allTrackNames {};
QList<QListWidgetItem *> selectedTracks {};
QVector<QString> selectedTrackNames {};
QVector<QString> selectedTrackPaths {};
QStringList playlistNames {};
QVector<QString> playlistTracks {};
QString filename {};
QString item {};



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
        allTrackNames.append(music.fileName());
        mp3Files.append(music.filePath());
    }

    for (int i = 0; i < mp3Files.size(); i++)
    {
        qPlaylist.getPlaylist()->addMedia(QUrl(mp3Files[i]));
    }

    qPlaylist.setTracks(allTrackNames);

    filename = "../Playlist Contents/" + qPlaylist.getName();
    QFile file(filename + ".txt");
    file.remove();
    for (int i = 0; i < allTrackNames.size(); i++)
    {
        file.open(QIODevice::Append);
        QTextStream out(&file);
        out << allTrackNames[i] + "\n";
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
    ui->track->insert(qPlaylist.getTrack(0));

    QValidator *validator = new QRegularExpressionValidator(this);
    ui->enteredName->setValidator(validator);

    enteredName = new QLineEdit;
    QRegularExpression mask("[a-zA-Z0-9_]+( [a-zA-Z0-9_]+)*$");
    ui->enteredName->setValidator(new QRegularExpressionValidator(mask, this));
    ui->enteredName->setPlaceholderText("Enter Playlist Name");


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
        //playlistTracks.append(selectedTrackNames[i]);
        selectedTrackPaths.append("../MP3 Files/" + selectedTrackNames[i]);
        newPlaylist.getPlaylist()->addMedia(QUrl(selectedTrackPaths[i]));
        qDebug() << selectedTrackPaths[i];
    }

    newPlaylist.setTracks(selectedTrackNames);
    filename = "../Playlist Contents/" + newPlaylist.getName();
    QFile file(filename + ".txt");
    file.remove();
    for (int i = 0; i < selectedTrackNames.size(); i++)
    {
        file.open(QIODevice::Append);
        QTextStream out(&file);
        out << selectedTrackNames[i] + "\n";
    }
    playlistNames.append(newPlaylist.getName());
    ui->selectPlaylist->clear();
    loadPlaylistNames();

    ui->playlistTitle->clear();
    ui->playlistTitle->insert(newPlaylist.getName());

    newPlaylist.getPlaylist()->save(QUrl::fromLocalFile("../Playlists/" + newPlaylistName + ".m3u"), "m3u");
    qDebug() << newPlaylist.getPlaylist()->save((QUrl::fromLocalFile("../Playlists/" + newPlaylistName + ".m3u")), "m3u");

    player->setPlaylist(newPlaylist.getPlaylist());

    ui->track->clear();
    ui->track->insert(selectedTrackNames[newPlaylist.getPlaylist()->currentIndex()]);

    //Clears the selected track names and paths so that they don't interfere with the next new playlist created by user
    selectedTrackNames.clear();
    selectedTrackPaths.clear();



    return newPlaylist;
}

void Widget::loadPlaylistNames()
{
    //Used to load existing playlist names and prevent duplicate playlists appearing in the dropdown box
    QDirIterator names("../Playlists", QDir::Files);
    while (names.hasNext())
    {
        names.next();
        playlistNames.append(names.fileName());
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

    qDebug() << player->playlist()->currentIndex();

    if (qPlaylist.getPlaylist()->currentIndex() < 0)
    {
        ui->track->insert(qPlaylist.getTrack(qPlaylist.getPlaylist()->mediaCount() - 1));
    }
    else
    {
        ui->track->insert(qPlaylist.getTrack(qPlaylist.getPlaylist()->currentIndex()));
    }
}

void Widget::on_skipForwardButton_clicked()
{
    //Allows you to skip song. Playlist will loop back around if you skip forwards at the beginning of the playlist
    (player->playlist())->next();
    ui->track->clear();

    qDebug() << player->playlist()->currentIndex();

    if (qPlaylist.getPlaylist()->currentIndex() < 0)
    {
        ui->track->insert(qPlaylist.getTrack(0));
    }
    else
    {
        ui->track->insert(qPlaylist.getTrack(qPlaylist.getPlaylist()->currentIndex()));
    }
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
            ui->playlistItems->addItem(selectedTracks[i]->text());
    }

    selectedTracks.clear();

}

void Widget::on_playlistSave_accepted()
{
    ui->playlistItems->count();

    for (int i = 0; i < ui->playlistItems->count(); i++)
    {
        item = (ui->playlistItems->item(i)->text());
        selectedTrackNames.append(item);
    }

    qDebug() << ui->playlistItems->count();

    if(ui->playlistItems->count() == 0)
    {
        QMessageBox::critical(this, "Invalid", "Please add music to your playlist");
    }
    else if(!ui->enteredName->hasAcceptableInput())
    {
        QMessageBox::critical(this, "Invalid", "Please enter a playlist name");
    }
    else if(ui->enteredName->hasAcceptableInput())
    {
        qPlaylist = newPlaylist(ui->enteredName->text());
    }
}

void Widget::on_playlistSave_rejected()
{
    ui->playlistItems->clear();
    ui->selectMusic->clearSelection();
    selectedTrackNames.clear();
}


void Widget::on_confirmSelected_clicked()
{
    //Loads the currently displayed playlist in the dropdown into the player, allowing user to confirm their playlist selection
    qPlaylist.getPlaylist()->clear();
    qDebug () << qPlaylist.getPlaylist()->clear();

    qPlaylist.getPlaylist()->load(QUrl::fromLocalFile("../Playlists/" + ui->selectPlaylist->currentText() + ".m3u"), "m3u");

    player->playlist()->currentMedia();


    ui->track->clear();
    ui->track->insert(qPlaylist.getTrack(0));
    if(qPlaylist.getPlaylist()->error())
        qDebug() << qPlaylist.getPlaylist()->errorString();

    player->setPlaylist(qPlaylist.getPlaylist());
    qDebug() << "../Playlists/" + ui->selectPlaylist->currentText();
}
