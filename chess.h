#ifndef CHESS_H
#define CHESS_H

#include <QObject>
#include <vector>
#include <QPoint>

using namespace std;
enum Thetype
{
    bing=0,
    pao,
    ju,
    ma,
    xiang,
    shi,
    jiang,
    none

};
class Chess
{
private:
    Thetype type;
    bool    status;
    int    color;       //1 for red and 2 for black
    QPoint curpoint;    //9*9

    vector<QPoint> available;
    void calculate_available();
public:
    Thetype getType() const;
    bool    getStatus() const   {return status;}
    void    setStatus(bool tem) {status=tem;}
    explicit Chess(Thetype ptype,int x,int y,int tcolor);
};

#endif // CHESS_H
