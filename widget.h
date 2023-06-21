#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUrl>

QT_BEGIN_NAMESPACE
class QAudioOutput;
class QMediaPlayer;
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    //void on_pushButton_clicked();//音量

    //void on_pushButton_4_clicked();//菜单

    void on_pushButton_3_clicked();//上一首

    void on_pushButton_5_clicked();//下一首


    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_2_clicked();//文件夹

    void on_pushButton_6_clicked();//播放

    void on_pushButton_clicked(bool checked);

    void on_pushButton_4_clicked();//单曲循环

private:
    Ui::Widget *ui;
    QList<QUrl> playList;//播放列表
    int curPlayIndex = 0;
    QAudioOutput* audioOutput;
    QMediaPlayer* mediaPlayer;
};
#endif // WIDGET_H
