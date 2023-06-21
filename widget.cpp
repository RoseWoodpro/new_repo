#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QListWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <qslider.h>

int temp=0;


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->verticalSlider->close();
    //播放音乐
    audioOutput = new QAudioOutput(this);
    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setAudioOutput(audioOutput);
    //mediaPlayer->setSource(QUrl::fromLocalFile("D:\\MP3qt6\\lib\\Summer 久石让.mp3"));
    //播放
    //mediaPlayer->play();
    //获取当前媒体时长 通过信号关联
    connect(mediaPlayer,&QMediaPlayer::durationChanged,this,[=](qint64 duration)
    {
        ui->totallabel->setText(QString("%1:%2").arg(duration/1000/60,2,10,QChar('0')).arg((duration/1000%60)));
        ui->playCourseSlider->setRange(0,duration);
    });
    //获取当前播放时长
    connect(mediaPlayer,&QMediaPlayer::positionChanged,this,[=](qint64 pos)
    {
        ui->currrentlabel->setText(QString("%1:%2").arg(pos/1000/60,2,10,QChar('0')).arg((pos/1000%60)));
        ui->playCourseSlider->setValue(pos);//让进度滑块动起来
    });
    connect(ui->playCourseSlider,&QSlider::sliderMoved,mediaPlayer,&QMediaPlayer::setPosition);
    connect(ui->verticalSlider,&QSlider::sliderMoved,audioOutput,[&](int position)
            {audioOutput->setVolume(position/100.);});
    //设置音量
    audioOutput->setVolume (0.5);
    ui->verticalSlider->setValue(50);


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_2_clicked()
{
    //qInfo()<<"hello qt";
    //打开文件对话框
    auto path=QFileDialog::getExistingDirectory(this,"请选择音乐所在目录","D:\\MP3qt6\\lib");
    QDir dir(path);
    auto musicList = dir.entryList(QStringList()<<"*.mp3");
    //把音乐名字存到listWidget
    ui->listWidget->addItems(musicList);
    //默认选中第一首歌
    ui->listWidget->setCurrentRow(0);
    //把音乐完整路径保存
    for(auto file:musicList)
        playList.append((QUrl::fromLocalFile(path+"/"+file)));

}

//上一曲
void Widget::on_pushButton_3_clicked()
{
    curPlayIndex = (curPlayIndex-1)%playList.size();
    ui->listWidget->setCurrentRow(curPlayIndex);
    mediaPlayer->setSource(playList[curPlayIndex]);
    mediaPlayer->play();
}

//下一曲
void Widget::on_pushButton_5_clicked()
{
    //curPlayIndex++;
    //if(curPlayIndex>=playList.size())
   // {
    //    curPlayIndex = 0;
    //}
    //更简洁的是
    curPlayIndex = (curPlayIndex+1)%playList.size();
    ui->listWidget->setCurrentRow(curPlayIndex);
    mediaPlayer->setSource(playList[curPlayIndex]);
    mediaPlayer->play();
}

void Widget::on_pushButton_6_clicked()//播放
{
    if(playList.empty())
    {
        return;
    }
    switch(mediaPlayer->playbackState())
    {
    case QMediaPlayer::PlaybackState::StoppedState://停止状态
    {
        //播放当前选中的音乐(若正在播放）
        //获取选中的行号
        curPlayIndex = ui->listWidget->currentRow();
        //播放对应下标的音乐
        mediaPlayer->setSource(playList[curPlayIndex]);
        mediaPlayer->play();
        break;
    }
    case QMediaPlayer::PlaybackState::PlayingState://播放状态
    {
        //暂停音乐（若正在播放）
        mediaPlayer->pause();
        break;
    }
    case QMediaPlayer::PlaybackState::PausedState://暂停状态
    {
        //播放音乐（若暂停）
        mediaPlayer->play();
        break;
    }
    }

}

void Widget::on_pushButton_clicked(bool checked)//音量键展示与隐藏
{
    if(temp==0)
    {
        ui->verticalSlider->show();
        temp++;

    }
    else if(temp==1)
    {
        ui->verticalSlider->close();
        temp=0;

    }




}
//双击列表播放
void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    curPlayIndex = index.row();
    mediaPlayer->setSource(playList[curPlayIndex]);
    mediaPlayer->play();
}
//单曲循环
void Widget::on_pushButton_4_clicked()
{
    mediaPlayer->setLoops(-1);
    mediaPlayer->play();
}

