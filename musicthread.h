#ifndef MUSICTHREAD_H
#define MUSICTHREAD_H
#include <QThread>
#include <mediafile.h>
class MusicThread : public QThread
{
    Q_OBJECT
private:
    QMediaPlayer musicplayer;
public:
    MusicThread(QObject* parent=0);
public slots:
    void run() override;

};

#endif // MUSICTHREAD_H
