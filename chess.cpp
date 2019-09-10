#include "chess.h"

Chess::Chess(Thetype ptype,int x,int y,int tcolor)
{
    type=ptype;
    color=tcolor;
    curpoint.setX(x);
    curpoint.setY(y);
    status=true;

}
Thetype Chess::getType() const
{
    return type;
}
