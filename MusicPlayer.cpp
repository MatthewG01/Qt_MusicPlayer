#include "MusicPlayer.h"
#include "ui_MusicPlayer.h"

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

//Initialisation function which runs as soon as the program is run by the user
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
        allMP3Files.append(music.filePath());
    }
    for (int i = 0; i < allMP3Files.size(); i++)
    {
        qPlaylist.getPlaylist()->addMedia(QUrl(allMP3Files[i]));
    }
    qPlaylist.setTracks(allTrackNames);

    //Saves the playlist contents to a txt file so that contents can be retrieved
    filename = "../Playlist Contents/" + qPlaylist.getName();
    QFile playlistContents(filename + ".txt");
    playlistContents.remove();
    for (int i = 0; i < allTrackNames.size(); i++)
    {
        playlistContents.open(QIODevice::Append);
        QTextStream write(&playlistContents);
        write << allTrackNames[i] + "\n";
    }
    qPlaylist.getPlaylist()->save(QUrl::fromLocalFile("../Playlists/All Music.m3u"), "m3u");
    qDebug() << qPlaylist.getPlaylist()->save(QUrl::fromLocalFile("../Playlists/All Music.m3u"), "m3u");

    //Saves the playlist names so they can be displayed in a dropdown combo box
    playlistNames.append(qPlaylist.getName());
    ui->selectPlaylist->clear();
    loadPlaylistNames();

    ui->playlistTitle->setReadOnly(true);
    ui->playlistTitle->insert(qPlaylist.getName());
    player->setPlaylist(qPlaylist.getPlaylist());

    ui->track->setReadOnly(true);
    ui->track->insert(qPlaylist.getTrack(0));

    /*Validator used to prevent the user from entering a playlist name which is blank or contains special characters.
     * Code for the mask variable used in the validator was referenced from slackwing on stackoverflow, see declaration.
     */
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

//Function that creates new playlists each time the user clicks save after adding music and giving their playlist a name
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
    newPlaylist.setTracks(selectedTrackNames);

    //Saves the playlist contents to a txt file so that contents can be retrieved
    filename = "../Playlist Contents/" + newPlaylist.getName();
    QFile playlistContents(filename + ".txt");
    playlistContents.remove();
    for (int i = 0; i < selectedTrackNames.size(); i++)
    {
        playlistContents.open(QIODevice::Append);
        QTextStream write(&playlistContents);
        write << selectedTrackNames[i] + "\n";
    }

    //Saves the new playlist names so they can be displayed in a dropdown combo box
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

//Used to load existing playlist names and prevent duplicate playlists appearing in the dropdown box
void Widget::loadPlaylistNames()
{   
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

void Widget::on_play_clicked()
{
    player->play();
}

void Widget::on_pause_clicked()
{
    player->pause();
}

//Allows you to skip song. Playlist will loop back around if you skip backwards at the beginning of the playlist
void Widget::on_skipBackButton_clicked()
{
    (player->playlist())->previous();
    ui->track->clear();

    qDebug() << player->playlist()->currentIndex();

    if (qPlaylist.getPlaylist()->currentIndex() < 0)
    {
        ui->track->insert(qPlaylist.getTrack(qPlaylist.getPlaylist()->mediaCount() - 1));
        (player->playlist())->previous();
    }
    else
    {
        ui->track->insert(qPlaylist.getTrack(qPlaylist.getPlaylist()->currentIndex()));
    }
}

//Allows you to skip song. Playlist will loop back around if you skip forwards at the end of the playlist
void Widget::on_skipForwardButton_clicked()
{
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

//Referenced from Bryan Cairns VoidRealms tutorial
void Widget::on_progressSlider_sliderMoved(int position)
{
    player->setPosition(position);
}

//Referenced from Bryan Cairns VoidRealms tutorial
void Widget::on_positionChanged(qint64 position)
{
    ui->progressSlider->setValue(position);
}

//Referenced from Bryan Cairns VoidRealms tutorial
void Widget::on_durationChanged(qint64 position)
{
    ui->progressSlider->setMaximum(position);
}

//Referenced from Bryan Cairns VoidRealms tutorial
void Widget::on_volumeSlider_sliderMoved(int position)
{
    player->setVolume(position);
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

//Clears the user selected tracks, allowing them to reconsider their choices
void Widget::on_playlistSave_rejected()
{
    ui->playlistItems->clear();
    ui->selectMusic->clearSelection();
    selectedTrackNames.clear();
}

//Loads the currently displayed playlist in the dropdown into the player, allowing the user to confirm their playlist selection and listen to it
void Widget::on_confirmSelected_clicked()
{
    qPlaylist.getPlaylist()->clear();
    qDebug () << qPlaylist.getPlaylist()->clear();

    qPlaylist.getPlaylist()->load(QUrl::fromLocalFile("../Playlists/" + ui->selectPlaylist->currentText() + ".m3u"), "m3u");

    //Accesses the currently selected playlists contents so they can be used to display track names
    ui->track->clear();
    songs.clear();
    filename = "../Playlist Contents/" + ui->selectPlaylist->currentText();
    QFile playlistContents(filename + ".txt");
    playlistContents.open(QIODevice::ReadOnly);
    QTextStream read(&playlistContents);
    while (!read.atEnd())
    {
        songs.append(read.readLine());
        qDebug() << songs;
    }
    playlistContents.close();
    ui->track->insert(songs[0]);

    //If the playlist can not be found, a message is displayed informing the user so they know to pick another playlist or create a new one
    if(qPlaylist.getPlaylist()->error())
    {
        QMessageBox::critical(this, "Error", "Playlist not found, select another playlist or create a new one.");
        ui->selectPlaylist->removeItem(ui->selectPlaylist->currentIndex());
        qDebug() << qPlaylist.getPlaylist()->errorString();
    }

    //Sets the loaded playlist as the current playlist and sets its tracks using the songs contained in the corresponding playlist contents file for the playlist
    player->setPlaylist(qPlaylist.getPlaylist());
    qDebug() << "../Playlists/" + ui->selectPlaylist->currentText();
    qPlaylist.setTracks(songs);
}
