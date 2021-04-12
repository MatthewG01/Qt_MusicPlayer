#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QtMultimedia>
#include <QMediaPlayer>
#include <QModelIndex>
#include <QFileSystemModel>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_directoryView_activated(const QModelIndex &index);

private:
    Ui::Widget *ui;
    QMediaPlayer *player;

    QFileSystemModel dirModel;
    QFileSystemModel fileModel;

    void init();
};
#endif // WIDGET_H
