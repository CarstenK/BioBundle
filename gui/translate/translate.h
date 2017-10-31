#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <QMainWindow>

namespace Ui {
class translate;
}

class translate : public QMainWindow
{
    Q_OBJECT

public:
    explicit translate(QWidget *parent = 0);
    ~translate();

private:
    Ui::translate *ui;
};

#endif // TRANSLATE_H
