#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <QWidget>

class gamemenu : public QWidget
{
    Q_OBJECT
public:
    explicit gamemenu(QWidget *parent = nullptr);
void paintEvent(QPaintEvent *event);
signals:

};

#endif // GAMEMENU_H
