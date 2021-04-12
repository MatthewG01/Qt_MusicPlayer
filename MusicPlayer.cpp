#include "MusicPlayer.h"
#include "ui_MusicPlayer.h"

QVector<QString> mp3Files {};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
    //load();

    player = new QMediaPlayer(this);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::init()
{
    defaultPlaylist = new QMediaPlaylist(this);
    player->setPlaylist(defaultPlaylist);

    QDirIterator music("../MP3 Files", QDir::Files);
    while (music.hasNext())
    {
        ui->selectMusic->addItem(music.fileName());
        mp3Files.append(music.filePath());
    }

    for (int i = 0; i < mp3Files.size(); i++)
    {
        defaultPlaylist->addMedia(QUrl(mp3Files[i]));
    }

}

void Widget::load()
{

}

void Widget::save()
{

}


void Widget::on_play_clicked()
{
    player->setPlaylist(defaultPlaylist);
    player->play();
}
