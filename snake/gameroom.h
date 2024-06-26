#ifndef GAMEROOM_H
#define GAMEROOM_H

#include <QWidget>
#include<QRectF>
#include<QList>
#include<QSound>
enum snakemove{
    up=0,
    down,
    left,
    right
};
class gameroom : public QWidget
{
    Q_OBJECT
public:
    explicit gameroom(QWidget *parent = nullptr);
void paintEvent(QPaintEvent *event);

void moveup();
void movedown();
void moveleft();
void moveright();
bool gameover();
void creatfood();
void setmovetime(int x){snakemovetime=x;}
private:
    const int snakebodywidth=20;
    const int snakebodyheight=20;
    const int movetime=200;//蛇单次移动时间（ms）
    //即定时器的数值，200ms移动一次

    QList<QRectF> snakelist;//以链表作为蛇的连接方式
    QRectF foodrect;//食物
    snakemove move=snakemove::up;//默认移动方向
    QTimer* timer;//定时器

    bool isgame=false;//是否开始游戏
    QSound* sound;
    int snakemovetime=move;
};

#endif // GAMEROOM_H
