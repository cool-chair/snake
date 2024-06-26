#include "gameroom.h"
#include<QPainter>
#include<QPixmap>
#include<QIcon>
#include<QTimer>
#include<QPushButton>
#include<QMessageBox>
#include"gamemenu.h"
#include<QFile>
#include<QTextStream>
gameroom::gameroom(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(1000,800);
    this->setWindowIcon(QIcon(":/res/ico.png"));
    this->setWindowTitle("游戏房间");

    //初始化蛇的位置
    //链表不能为空，先尾差一个矩形结点，在painter里，对头尾进行绘画
    snakelist.push_back(QRectF(this->width()*0.5,this->height()*0.5,snakebodywidth,snakebodyheight));
    //尾差完毕，现在只有一节身子，让其向上移动
    //移动一次，多一个节点
    moveup();
    moveup();
    //初始化蛇为3节，头，身，尾

    creatfood();

    timer=new QTimer(this);//定时器
    //定时器超时，调用lambda表达式，即200ms调用一次
    //每200ms走一格，同时对蛇是否吃到食物进行判断
    connect(timer,&QTimer::timeout,[=](){
        QFile file("C:/Users/13911/Desktop/QT_greedy_snake/snake/1.txt");

        if(file.open(QIODevice::WriteOnly))
        {
            int m=snakelist.size();
            QTextStream out(&file);
            out<<m;
            file.close();
        }
        int c=1;
        if(snakelist.front().intersects(foodrect))
         //蛇和食物相交，创造出一个新的食物，让蛇多一个节点
        {
            creatfood();
            QSound::play(":res/eatfood.wav");//制造吃东西音效
            c++;
        }

       //因为移动也会多一个节点，所以吃食物会多两个节点
        while(c--)
        {
            switch(move)
            {
                case snakemove::up :
                   moveup();
                   break;
                case snakemove::down :
                   movedown();
                   break;
                case snakemove::left :
                   moveleft();
                   break;
                case snakemove::right :
                   moveright();
                   break;
            }
        }
        snakelist.pop_back();//尾删一个移动多出来的结点
        update();//更新
    });


    QPushButton* button1=new QPushButton(this);
    QPushButton* button2=new QPushButton(this);
    button1->move(860,100);
    button2->move(860,200);
    QFont font("宋体",20);
    button1->setFont(font);
    button2->setFont(font);
    button1->setText("开始");
    button2->setText("暂停");
    connect(button1,&QPushButton::clicked,[=](){
        isgame=true;
       timer->start(snakemovetime);
       sound=new QSound(":/res/Trepak.wav");
       sound->play();
       sound->setLoops(-1);//循环播放
    });
    connect(button2,&QPushButton::clicked,[=](){
        isgame=false;
        sound->stop();
        timer->stop();
    });

    QPushButton* up=new QPushButton(this);
    QPushButton* down=new QPushButton(this);
    QPushButton* left=new QPushButton(this);
    QPushButton* right=new QPushButton(this);

    up->move(880,400);
    down->move(880,480);
    left->move(840,440);
    right->move(920,440);

    up->setText("↑");
    down->setText("↓");
    left->setText("←");
    right->setText("→");

    QFont ft("宋体",30);
    up->setFont(ft);
    down->setFont(ft);
    left->setFont(ft);
    right->setFont(ft);

    up->setStyleSheet("QPushButton{border:0px;}");
    down->setStyleSheet("QPushButton{border:0px;}");
    left->setStyleSheet("QPushButton{border:0px;}");
    right->setStyleSheet("QPushButton{border:0px;}");

    up->setShortcut(QKeySequence("w"));
    down->setShortcut(QKeySequence("s"));
    left->setShortcut(QKeySequence("a"));
    right->setShortcut(QKeySequence("d"));


    connect(up,&QPushButton::clicked,[=](){
        if(move!=snakemove::down)
            move=snakemove::up;
    });
    connect(down,&QPushButton::clicked,[=](){
        if(move!=snakemove::up)
            move=snakemove::down;
    });
    connect(left,&QPushButton::clicked,[=](){
        if(move!=snakemove::right)
            move=snakemove::left;
    });
    connect(right,&QPushButton::clicked,[=](){
        if(move!=snakemove::left)
            move=snakemove::right;
    });

    QFont fot("宋体",20);
    QPushButton* button=new QPushButton(this);
    button->setText("退出游戏");
    button->setFont(fot);
    button->move(830,700);
    connect(button,&QPushButton::clicked,[=](){
        QSound::play(":/res/clicked.wav");
        int res=QMessageBox::critical(this,"贪吃蛇","是否退出",QMessageBox::Yes|QMessageBox::No);
        if(res==QMessageBox::Yes)
        {
            this->close();
            gamemenu* gm=new gamemenu;
            gm->setGeometry(this->geometry());
            sound->stop();
            gm->show();
        }
        else if(res==QMessageBox::No)
        {
            return;
        }
    });
}
void gameroom::paintEvent(QPaintEvent *event)
{
    (void) event;
    QPainter painter(this);
    QPixmap pix;

    pix.load(":/res/game_room.png");
    painter.drawPixmap(0,0,800,800,pix);

    pix.load(":/res/bg1.png");
    painter.drawPixmap(800,0,200,900,pix);

    //绘制蛇:蛇头+身体+尾巴
    //蛇头
    if(move==snakemove::up)
    {
        pix.load(":/res/up.png");
    }
    else if(move==snakemove::down)
    {
        pix.load(":/res/down.png");
    }
    else if(move==snakemove::left)
    {
        pix.load(":/res/left.png");
    }
    else
    {
        pix.load(":/res/right.png");
    }
    auto head=snakelist.front();
    painter.drawPixmap(head.x(),head.y(),head.width(),head.height(),pix);
    //蛇身
    pix.load(":/res/Bd.png");
    for(int i=1;i<snakelist.size()-1;i++)//减去头和尾
    {
        auto body=snakelist.at(i);
        painter.drawPixmap(body.x(),body.y(),body.width(),body.height(),pix);
    }
    //蛇尾
    pix.load(":/res/Bd.png");
    auto tail=snakelist.back();
    painter.drawPixmap(tail.x(),tail.y(),tail.width(),tail.height(),pix);

    //绘制食物
    pix.load(":/res/food.bmp");
    painter.drawPixmap(foodrect.x(),foodrect.y(),foodrect.width(),foodrect.height(),pix);

    //绘制分数
    pix.load(":/res/sorce_bg.png");
    painter.drawPixmap(850,30,100,50,pix);
    QPen pen;
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    QFont fot("宋体",25);
    painter.setFont(fot);
    painter.drawText(900,72,QString("%1").arg(snakelist.size()));
    //%1获取arg函数里的数字，转为字符串形式


    //绘制游戏失败效果
    if(gameover())
    {
        pen.setColor(Qt::red);
        painter.setPen(pen);
        QFont font("宋体",50);
        painter.setFont(font);

        painter.drawText(this->width()*0.3,this->height()*0.45,QString("GAME OVER"));
        timer->stop();
        QSound::play(":res/gameover.wav");

        sound->stop();
    }
}

void gameroom::moveup()
{
    QPointF lefttop;
    QPointF rightbottom;
    //左上，右下坐标，用来生成矩形

   auto head =snakelist.front();//获取头部的矩形数据

   int headx=head.x();//头矩形的横坐标
   int heady=head.y();//头矩形的纵坐标
   if(heady<=0)
   {
       lefttop=QPointF(headx,this->height()-snakebodyheight);
   }
   else
   {
       lefttop=QPointF(headx,heady-snakebodyheight);
   }
   rightbottom=lefttop+QPointF(snakebodywidth,snakebodyheight);
    //每次移动，头插一个蛇头矩形，把其余节点改为身子矩形
   snakelist.push_front(QRectF(lefttop,rightbottom));
    move=snakemove::up;
}

void gameroom::movedown()
{
    QPointF lefttop;
    QPointF rightbottom;

    auto head=snakelist.front();
    int headx=head.x();
    int heady=head.y();

    if((heady+snakebodyheight)>=this->height())
    {
        lefttop=QPointF(headx,0);
    }
    else
    {
        lefttop=QPointF(headx,heady+snakebodyheight);
    }
    rightbottom=lefttop+QPointF(snakebodywidth,snakebodyheight);

    snakelist.push_front(QRectF(lefttop,rightbottom));
    move=snakemove::down;

}

void gameroom::moveleft()
{
    QPointF lefttop;
    QPointF rightbottom;

    auto head=snakelist.front();
    int headx=head.x();
    int heady=head.y();
    if(headx<=0)
    {
        lefttop=QPointF(800-snakebodywidth,heady);
    }
    else
    {
        lefttop=QPointF(headx-snakebodywidth,heady);
    }
    rightbottom=lefttop+QPointF(snakebodywidth,snakebodyheight);

    snakelist.push_front(QRectF(lefttop,rightbottom));
    move=snakemove::left;
}

void gameroom::moveright()
{
    QPointF lefttop;
    QPointF rightbottom;

    auto head=snakelist.front();
    int headx=head.x();
    int heady=head.y();
    if((headx+snakebodywidth)>=800)
    {
        lefttop=QPointF(0,heady);
    }
    else
    {
        lefttop=QPointF(headx+snakebodywidth,heady);
    }
    rightbottom=lefttop+QPointF(snakebodywidth,snakebodyheight);

    snakelist.push_front(QRectF(lefttop,rightbottom));
    move=snakemove::right;
}

bool gameroom::gameover()
{
    for(int i=0;i<snakelist.size();i++)
    {
        for(int j=i+1;j<snakelist.size();j++)
        {
            if(snakelist.at(i)==snakelist.at(j))
        //at,获取链表节点，如果i和j相同，说明蛇头碰到蛇尾了，游戏结束
            {

                return true;
            }
        }
    }
    return false;
}

void gameroom::creatfood()
{
    foodrect=QRectF(qrand()%(800/snakebodywidth)*snakebodywidth,qrand()%(this->height()/snakebodyheight)*snakebodyheight,snakebodywidth,snakebodyheight);

}
