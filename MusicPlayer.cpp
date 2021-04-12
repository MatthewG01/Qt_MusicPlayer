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
    //Filesystem code from Bryan Cairns Qt 5 Widgets for Beginners with C++, QFileSystemModel tutorial.
    dirModel.setRootPath(QDir::currentPath());
    dirModel.setFilter(QDir::Filter::Drives | QDir::Filter::AllDirs | QDir::Filter::NoDotAndDotDot);

    //Displays directories without unnecessary columns such as size and date modified
    ui->directoryView->setModel(&dirModel);

    for(int i = 1; i < dirModel.columnCount(); i++)
    {
        ui->directoryView->hideColumn(i);
    }

    //Shows files from chosen directories within the list view
    fileModel.setRootPath(dirModel.rootPath());
    fileModel.setFilter(QDir::Filter::Files);

    ui->fileView->setModel(&fileModel);

    defaultPlaylist = new QMediaPlaylist(this);
    player->setPlaylist(defaultPlaylist);

    QDirIterator music("../MP3 Files", QDir::Files);
    while (music.hasNext())
    {
        qDebug() << music.next();
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



void Widget::on_directoryView_activated(const QModelIndex &index)
{
    QString path = dirModel.filePath(index);
    fileModel.setFilter(QDir::Filter::Files);

    ui->fileView->setRootIndex(fileModel.setRootPath(path));
}


