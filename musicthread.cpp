#include "musicthread.h"
#include <QDebug>
MusicThread::MusicThread(QObject* parent)
{

}

void MusicThread::run()
{
   QMediaPlaylist* playlist=new QMediaPlaylist;
   playlist->addMedia(QUrl("qrc:/audio/audio/backgroud3.wav"));
   playlist->addMedia(QUrl("qrc:/icon/audio/backgroud2.wav"));
   musicplayer.setPlaylist(playlist);
   musicplayer.play();
    //QSound::play(":/audio/audio/backgroud3.mp3");

}
