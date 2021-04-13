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



}

Widget::~Widget()
{
    delete ui;
    delete (player);
    delete (defaultPlaylist);
}

void Widget::init()
{
    player = new QMediaPlayer(this);
    defaultPlaylist = new QMediaPlaylist(this);


    QDirIterator music("../MP3 Files", QDir::Files);
    while (music.hasNext())
    {
        music.next();
        ui->selectMusic->addItem(music.fileName());
        mp3Files.append(music.filePath());
    }

    for (int i = 0; i < mp3Files.size(); i++)
    {
        defaultPlaylist->addMedia(QUrl(mp3Files[i]));
    }

    player->setPlaylist(defaultPlaylist);


}





void Widget::on_pushButton_clicked()
{
    player->play();
}
