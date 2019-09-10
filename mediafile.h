#ifndef HEADERFILE_H
#define HEADERFILE_H
#include <QSound>
#include <QUrl>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#define WIN_SOUND QUrl("qrc:/audio/audio/win.wav")
#define LOSE_SOUND QUrl("qrc:/sound/lose.mp3")
#define SET_SOUND QUrl("qrc:/audio/audio/set.wav")
#define CHECKMATE_SOUND QUrl("qrc:/audio/audio/checkmate.wav")
#define ILLEGAL_SOUND QUrl("qrc:/audio/audio/illgeal.wav")
#define CAPUTURE_SOUND QUrl("qrc:/audio/audio/chessone.wav")
#define MESSAGE_SOUND QUrl("qrc:/audio/audio/newmessage.wav")

#define PLAY musicplayer.setMedia
#endif // HEADERFILE_H
