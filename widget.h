#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QFont>


namespace Ui {
class Widget;
}

struct dict
{
    char *key;
    char *content;
    struct dict *next;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QPoint last;
    struct dict *p ;
    unsigned long long dict_size;



protected:

    //重写绘图事件，虚函数
    //如果在窗口绘图，必须放在绘图事件里实现
    //绘图事件内部自动调用，窗口需要重绘的时候（状态改变）
    void paintEvent(QPaintEvent *);

    //设置按钮样式
    void setSysleSheetButton();
    //设置输入框样式
    void setSysleSheetEdit();
    //设置容器样式
    void setsetSysleSheetBox();

    //重写鼠标事件，这样就可以去掉标题栏之后可以通过鼠标移动窗口
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    //dict
    unsigned long long get_dict_size(const char *dict_filename);//得到字典文件中词条总数
    //打开字典文件，并读取文件内容
    unsigned long long open_dict(struct dict **p, const char *dict_filename);
    //根据关键字key,在字典中查找内容
    int search_dict(const struct dict *p,const char *key, char *content);
    //释放内存
    void free_dict(struct dict *p);


private slots:
    void on_closeButton_clicked();
    void on_maxsizeButton_clicked();
    void on_minsizeButton_clicked();
    void on_searchButton_clicked();
};

#endif // WIDGET_H
