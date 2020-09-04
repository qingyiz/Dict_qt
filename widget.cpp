#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QString>
#include <QFont>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <QTextCodec>//实现字符串转化的类
#include <QDebug>


using namespace std;

unsigned long long Widget::get_dict_size(const char *dict_filename)//得到字典文件中词条总数
{
    FILE *pfile = fopen(dict_filename, "r");
    if (pfile == nullptr){
        qDebug()<<"get_dict_size() open file fair";
        return 0;
    }


    unsigned long long i = 0;
    char buf[2048];
    while (!feof(pfile)){
        fgets(buf, sizeof(buf), pfile);
        fgets(buf, sizeof(buf), pfile);
        i++;//读取两行后，计数器加1
    }
    return i;
}
//打开字典文件，并读取文件内容
unsigned long long Widget::open_dict(struct dict **p, const char *dict_filename)
{
    FILE *pfile = fopen(dict_filename, "r");
    if (pfile == nullptr)
        return 0;//打开文件失败，函数返回
    unsigned long long size = get_dict_size(dict_filename);//得到字典文件中词条总数
    if (size == 0)
        return 0;

    *p = (struct dict *)malloc(sizeof(struct dict) * size);//根据字典文件词条总数分配内存
    memset(*p, 0, sizeof(struct dict) * size);//将分配内存初始化为0

    struct dict *pD = *p;//pD指向数组p的首地址

    char buf[2048] = { 0 };
    size_t len = 0;
    unsigned long long i = 0;
    fseek(pfile, 0L, SEEK_SET);//设置读取位置为字典文件开始
    while (!feof(pfile))//循环读取文件，直到文件末尾
    {
        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf), pfile);//读取文件一行
        len = strlen(buf);//得到读取到字符串长度
        if (len > 0)
        {
            pD->key = (char *)malloc(len);//根据字符串长度分配内存
            memset(pD->key, 0, len);
            strcpy(pD->key, &buf[1]);//将读取到的内容拷贝到key中，去掉#
        }

        memset(buf, 0, sizeof(buf));
        fgets(buf, sizeof(buf), pfile);
        len = strlen(buf);
        if (len > 0){
            pD->content = (char *)malloc(len);
            memset(pD->content, 0, len);
            strcpy(pD->content, &buf[6]);//去掉无效字符
        }
        pD->next = (struct dict *)calloc(sizeof(struct dict), 1);
        pD = pD->next;
        i++;
    }
    pD->next = nullptr;
    fclose(pfile);//关闭字典文件

    return i;//返回读取到的字典词条数
}

//根据关键字key,在字典中查找内容
int Widget::search_dict(const struct dict *p,const char *key, char *content)
{

    const struct dict *pD=p;
    unsigned long long i;
    //while(pD){//遍历字典
    for(i = 0;i < this->dict_size;i++){
        if ((pD->key == nullptr) || (pD->content == nullptr))
            continue;

        if (strncmp(pD->key, key, strlen(key)) == 0){
            strcpy(content, pD->content);
            return 1;//找到符合条件记录，返回1
        }
        pD = pD->next;
    }

    return 0;//没有找到符合条件记录，返回0
}

//释放内存
void Widget::free_dict(struct dict *p)
{
    struct dict *pD = p;
        while (pD)
        {
            if (pD->key)
                free(pD->key);
            if (pD->content)
                free(pD->content);
            struct dict *tmp = pD->next;
            free(pD);
            pD = tmp;
        }
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{

    this->dict_size = get_dict_size("../Dict_qt/dict.txt");
    open_dict(&(this->p),"../Dict_qt/dict.txt");
    if(this->dict_size == 0){
        exit(0);
    }

    ui->setupUi(this);
    QFont font("LiHei Pro Medium ", 13, 100, true);
    QPalette palette;



    //去掉标题栏
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    //按钮样式设置
    setSysleSheetButton();
    //输入框样式设置
    setSysleSheetEdit();
    ui->searchEdit->setPlaceholderText(QString("请输入单词或汉字"));  // 设置默认文字
    //容器样式设置
    setsetSysleSheetBox();

    //自动换行
    ui->toLable->setWordWrap(true);

    ui->Authorlable->setFont(font);
    ui->QQlable->setFont(font);
    palette.setColor(QPalette::WindowText,Qt::white);
    ui->Authorlable->setPalette(palette); //这里设置控件字体颜色
    ui->QQlable->setPalette(palette); //这里设置控件字体颜色

    ui->searchButton->setShortcut(Qt::Key_Return);//设置按钮关联到Enter键

}

void Widget::paintEvent(QPaintEvent *)
{
    //QPainter p(this);

    QPainter p;//创建画家对象
    p.begin(this);//指定当前窗口为绘图设备

    //绘图操作
    //p.drawxxx();
    //画背景图
    //p.drawPixmap(0, 0, width(), height(), QPixmap("../Image/bk.png"));
    p.drawPixmap(rect(), QPixmap("../Dict_qt/image/2000973.jpg"));
}

//设置按钮样式
void Widget::setSysleSheetButton()
{
    QString styleSheet;
    //这是在Qt的资源下的文件,可以不用在资源下
    QFile file1("../Dict_qt/qss/serachButton.qss");
    //只读方式打开文件
    file1.open(QFile::ReadOnly);
    //读取文件的所有内容，并转换成QString类型
    styleSheet = QString(file1.readAll());
    //设置样式表
    ui->searchButton->setStyleSheet(styleSheet);

    QFile file2("../Dict_qt/qss/statusButton.qss");
    file2.open(QFile::ReadOnly);
    styleSheet = QString(file2.readAll());
    ui->closeButton->setStyleSheet(styleSheet);
    ui->minsizeButton->setStyleSheet(styleSheet);
    ui->maxsizeButton->setStyleSheet(styleSheet);


}
//设置输入框样式
void Widget::setSysleSheetEdit()
{
    QFile file("../Dict_qt/qss/serachEdit.qss");
    file.open(QFile::ReadOnly);
    //读取文件的所有内容，并转换成QString类型
    QString styleSheet = QString(file.readAll());
    ui->searchEdit->setStyleSheet(styleSheet);
}

void Widget::setsetSysleSheetBox()
{
    QFile file("../Dict_qt/qss/groupBox.qss");
    file.open(QFile::ReadOnly);
    //读取文件的所有内容，并转换成QString类型
    QString styleSheet = QString(file.readAll());
    ui->groupBox->setStyleSheet(styleSheet);
}


Widget::~Widget()
{
    free_dict(p);
    delete ui;
}

void Widget::on_closeButton_clicked()
{
    this->close();
}

void Widget::on_maxsizeButton_clicked()
{

}

void Widget::on_minsizeButton_clicked()
{
    this->showMinimized();
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
}
void Widget::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx, y()+dy);
}
void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}


void Widget::on_searchButton_clicked()
{

    QFont font("楷体",15,10);
    char key[2048];
    char content[2048];
    memset(key, 0, sizeof(key));
    memset(content, 0, sizeof(content));
    QTextCodec *codec = QTextCodec::codecForName("GBK");//要实现GBK和utf8之间编码的转化
    ui->toLable->setText(" ");

    ui->toLable->setFont(font);
    ui->toLable->setText(codec->toUnicode(key));
    strcpy(key, codec->fromUnicode(ui->searchEdit->text()));//把用户的输入转化为GBK编码，拷贝到key中
    if (strlen(key) == 0)//当输入为空时
        ui->toLable->setText("");
    else if (search_dict(p,key,content))//根据用户输入，在字典中检索
        ui->toLable->setText(codec->toUnicode(content));
    else
        ui->toLable ->setText("Not Found");



}
