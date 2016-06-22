/*************************************************************************
  > File Name:    BookDB.c
  > Author:       d4smart 
  > Mail:         d4smart@foxmail.com
  > Created Time: 2016年06月21日 星期二 13时16分56秒
  > Version:      0.0.1
  > LastChange:   2016.06.22
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//程序工作流程
//文件夹中有对应的图书数据文件（archive.txt)
//程序首先打开文件并读取文件中的图书信息，建立图书链表
//再根据用户的输入执行对应的操作（查找，添加，修改，删除，打印，排序），操作的对象是图书链表的信息
//在用户选择退出时将图书链表的信息写入文件（覆盖）

//用于存储图书信息的结构体
//说明：
//图书信息从文件中读取后存放在一个双向链表中
//链表有一个头指针head和一个尾指针tail，head与tail本身不存放信息，信息存放在链表中间的节点中
//示意图：  0=1=1=1=1=1=1=1=1=1=1=0
//结构体中书名，类别和作者字符指针存储，存储数据需要使用动态malloc和strcpy
typedef struct _book
{
    //int Prikey;
    char *name;     //书名指针
    char *category; //类别指针
    char *author;   //作者指针
    int date;       //购买日期
    float price;    //价格
    struct _book *pre;  //前一本书
    struct _book *next; //后一本书
} Book;

Book *head, *tail;  //图书链表的头指针和尾指针
char filename[] = "archive.txt";    //存放图书信息的文件

Book *Choose();       //选择图书
void InsertBook(Book *book, Book *pos); //插入图书
void SaveInFile();  //保存图书信息到文件中
int Sort(Book *insert, Book *pos, int way); //辅助排序函数
int IsBook(int way, char *key, Book *pos);  //判断pos对应字段的内容是否与用户查询的字段内容相等

void GetBooks();    //从文件中读取图书信息，并建立图书链表
void SortBooks();   //对图书进行排序
void AddBooks();    //添加图书记录
void FindBooks();   //查找图书记录
void ChangeInfo();  //修改图书记录
void DeleteBooks(); //删除图书记录
void PrintBooks();  //打印全部图书记录


int main()
{
    int operation, processing = 1;

    printf("\n欢迎来到图书管理系统，本系统由d4smart开发\n\n");

    GetBooks();     //打开文件并读取图书数据

    //程序主循环
    while(processing)
    {
        printf("输入数字选择操作： 1.查找图书记录 2.添加图书记录 3.修改图书记录 4.删除图书记录 5.打印全部图书记录 6.对图书记录排序并保存\n");
        printf("请输入你要进行的操作，按 ctrl+z(Windows)/ctrl+d(Linux) 退出：");

        //读取用户输入，并判断是否输入EOF退出信息
        if (scanf("%d", &operation) == EOF)
        {
            processing = 0;
            continue;
        }

        //输入数据不合法则结束此次循环
        if (operation < 1 || operation > 6)
        {
            printf("选择的操作不合法！\n\n");
            continue;
        }

        //根据用户输入执行对应的功能
        if (operation == 1) //查找图书
        {
            FindBooks();
        }
        else if (operation == 2)    //添加图书
        {
            AddBooks(); 
        }
        else if (operation == 3)    //修改图书信息
        {
            ChangeInfo();
        }
        else if (operation == 4)    //删除图书
        {
            DeleteBooks();
        }
        else if (operation == 5)    //打印全部图书记录
        {
            PrintBooks();
        }
        else
        {
            SortBooks();   //对图书排序
        }
    }

    printf("\n\n正在退出操作...\n\n");
    SaveInFile();   //将图书信息保存到文件
}

//从文件中读取图书信息，并建立图书链表
//图书链表为全局变量，使用head和tail进行操作
void GetBooks()
{
    FILE *fp;
    char name[60] = "", category[40] = "", author[40] = "";
    int date, ch;
    float price;

    head = (Book *)malloc(sizeof(Book));    //分配头指针
    tail = (Book *)malloc(sizeof(Book));    //分配尾指针

    //建立内容为空的图书链表，头尾指针相连（0=0）
    head->next = tail;
    tail->pre = head;

    Book *book = NULL;      //用于暂时存储要添加的图书信息
    Book *pos = tail->pre;  //游标指针
    
    //打开文件
    printf("正在打开文件，请稍后...\n");
    if((fp=fopen(filename,"r"))==NULL)
    {
        perror("文件打开失败！\n\n");
        exit(1);
    }

    //读取文件内容并添加入图书链表
    printf("文件已打开，正在读取数据...\n");
    while((ch = fscanf(fp, "%s %s %s %d %f", name, category, author, &date, &price)) != EOF)
    { 
        book = (Book *)malloc(sizeof(Book));    //为暂存图书结构体分配内存

        //为暂存图书结构体赋值
        book->name = name;
        book->category = category;
        book->author = author;
        book->date = date;
        book->price = price;

        InsertBook(book, pos);  //将图书插入图书链表
        pos = pos->next;        //移动游标指针
    }

    fclose(fp); //关闭文件
    printf("数据读取完毕！\n\n");
}

//对图书进行排序
//可选择关键字进行排序，对前3项信息按中文拼音排序需要对系统默认排序编码进行修改（环境变量LC_COLLATE改为中文GBK）
//排序方法为链表的插入排序，需要使用辅助排序函数Sort
void SortBooks()
{
    //链表无图书或只有一个图书，直接返回
    if (head->next == tail || head->next == tail->pre)
        return;

    int way;
    Book *pos, *insert; //待排序图书，插入位置

    //选择排序方式
    printf("\n请输入排序方式（1.书名 2.类别 3.作者 4.购买日期 5.价格）：");
    scanf("%d", &way);

    //双向链表插入排序
    printf("Sorting...\n");
    for(pos = head->next->next; pos != tail; pos = pos->next)
    {
        Book *tmp1, *tmp2;

        insert = pos->pre;

        //寻找插入位置
        while(insert != head && Sort(insert, pos, way) > 0)
        {
            insert = insert->pre;   //插入位置前移
        }

        //将待排序图书从原来位置移出
        pos->pre->next = pos->next;
        pos->next->pre = pos->pre;
        tmp1 = pos->next;

        //将带排序图书放入新位置
        tmp2 = insert->next;
        insert->next = pos;
        pos->pre = insert;
        pos->next = tmp2;
        tmp2->pre = pos;

        pos = tmp1->pre;    //将pos值修改为下一个待排序图书
    }

    printf("Sort complete!\n");
}

//添加图书记录，将输入的图书信息加入到图书链表
void AddBooks()
{
    char name[60] = "", category[40] = "", author[40] = "";
    int date, ch, num = 0;
    float price;
    Book *book = NULL;      //待添加图书
    Book *pos = tail->pre;  //添加位置

    //读取图书信息，并添加入图书链表
    printf("\n添加图书记录，按 ctrl+z(Windows)/ctrl+d(Linux) 退出\n");
    printf("Enter book's name, category, author, purchase date, price in order: \n=> ");
    while((ch = scanf("%s %s %s %d %f", name, category, author, &date, &price)) != EOF)
    {
        //输入合法性判断
        if (ch != 5)
        {
            printf("图书输入格式不合法，请重新输入\n");
            printf("Enter book's name, category, author, purchase date, price in order: \n=> ");
            //printf("%d", ch);
            continue;
        }

        book = (Book *)malloc(sizeof(Book));    //为待添加图书分配内存

        //待添加图书赋值
        book->name = name;
        book->category = category;
        book->author = author;
        book->date = date;
        book->price = price;

        InsertBook(book, pos);  //插入待添加图书
        pos = pos->next;        //添加位置后移
        num++;
        printf("添加了%d本图书...\n", num); //输出提示信息

        printf("Enter book's name, category, author, purchase date, price in order: \n=> ");
    }

    printf("Add complete. Bye!\n\n");
}

//查找图书记录
//选择字段在记录中查找，需要使用图书选择函数Choose
void FindBooks()
{
    //char key[60] = "";
    int finding = 1, c; //查找标志，用户选择
    Book *book = NULL;  //找到的图书

    printf("查找图书记录\n");
    while(finding)
    {
        book = Choose();  //查找图书

        if (book == NULL)   //未找到记录
        {
            printf("\n未找到该记录...\n");
        }
        else    //找到记录，输出信息
        {
            printf("\n找到记录！\n");
            printf("书名\t\t\t类别\t\t作者\t\t购买日期\t价格\n");
            printf("%-30s%-20s%-20s%8d\t%.2f\n", book->name, book->category, book->author, book->date, book->price);
        }

        //让用户选择是否中止
        printf("是否继续？（按0退出，其他数字继续）：");
        scanf("%d", &c);
        if (c == 0)
        {
            finding = 0;
        }

        printf("\n");
    }
}

//修改图书信息
//根据用户的输入选择要修改的图书何和修改的内容
//需要使用图书选择函数Choose
void ChangeInfo()
{
    char info[60];  //保存用户输入的前三项字符值
    int date, way;  //购买日期，修改内容标记
    float price;    //价格
    Book *change = NULL;    //要修改的图书

    printf("修改图书信息\n");
    change = Choose();    //选择要修改的图书
    printf("记录原值：");
    printf("%-30s%-20s%-20s%8d\t%.2f\n", change->name, change->category, change->author, change->date, change->price);

    printf("选择修改信息的类型 (name,category,author,date,price)|(1,2,3,4,5): ");
    scanf("%d", &way);  //读取用户修改内容选择
    printf("修改为：");

    if (way == 1 || way == 2 || way == 3)   //修改前三项字符串值（书名，类别，作者）
    {
        scanf("%s", info);  //读取用户的修改内容

        //根据用户选择修改对应的图书记录内容
        if (way == 1)
        {
            free(change->name);
            change->name = (char *)malloc(strlen(info)+1);
            strcpy(change->name, info);
        }
        else if (way == 2)
        {        
            free(change->category);
            change->category = (char *)malloc(strlen(info)+1);
            strcpy(change->category, info);
        }
        else
        {  
            free(change->author);
            change->author = (char *)malloc(strlen(info)+1);
            strcpy(change->author, info);
        }
    }
    else if (way == 4)  //修改购买日期
    {
        scanf("%d", &date);
        change->date = date;
    }
    else    //修改价格
    {
        scanf("%f", &price);
        change->price = price;
    }

    printf("记录被修改为：");
    printf("%-30s%-20s%-20s%8d\t%.2f\n\n", change->name, change->category, change->author, change->date, change->price);
}

//删除图书记录
//根据用户输入选择要删除的图书记录并删除
//需要使用图书选择函数Choose
void DeleteBooks()
{
    Book *del = NULL;

    printf("删除图书记录\n");
    printf("请选择要删除的图书\n");
    del = Choose();   //选择要删除的图书
    printf("要删除的图书：");
    printf("%-30s%-20s%-20s%8d\t%.2f\n", del->name, del->category, del->author, del->date, del->price);

    //从图书链表中删除图书
    del->pre->next = del->next;
    del->next->pre = del->pre;
    free(del);

    printf("\n图书已删除...\n\n");
}

//打印全部图书记录
//遍历图书链表并打印出所有图书信息
void PrintBooks()
{
    Book *pos = head->next; //游标初始指向第一本图书的位置

    printf("所有图书记录\n");
    printf("书名\t\t\t类别\t\t作者\t\t购买日期\t价格\n");
    while(pos != tail)  //遍历图书链表并输出
    {
        printf("%-30s%-20s%-20s%8d\t%.2f\n", pos->name, pos->category, pos->author, pos->date, pos->price);

        pos = pos->next;    //移动游标
    }
    printf("\n");
}

//选择图书
//根据用户输入选择图书记录，并返回指针
Book *Choose()
{
    char key[60] = "";
    int way;
    Book *pos = head->next; //游标指向第一本图书
    Book *find = NULL;      //结果指针

    //用户输入选择方式
    printf("选择方式： 1.书名 2.分类 3.作者\n");
    printf("请输入选择的方式：");
    scanf("%d", &way);
    printf("请输入选择字段的值：");
    scanf("%s", key);

    while(pos != tail)
    {
        if (IsBook(way, key, pos) == 0) //找到记录，记录结果，跳出while循环
        {
            find = pos;
            break;
        }
        pos = pos->next;
    }

    return find;
}

//判断pos对应字段的内容是否与用户查询的字段内容相等
//返回0表示相等，非0表示不等
//使用strcmp进行判断
int IsBook(int way, char *key, Book *pos)
{
    if (way == 1)
    {
        return strcmp(key, pos->name);
    }
    else if (way == 2)
    {
        return strcmp(key, pos->category);
    }
    else if (way == 3)
    {
        return strcmp(key, pos->author);
    }
    else
        return -1;
}

//插入位置
//将book对应的图书插入到pos位置后面
void InsertBook(Book *book, Book *pos)
{
    Book *tmp = NULL, *pt = NULL;
        
    tmp = (Book *)malloc(sizeof(Book)); //为新节点分配内存

    //为新节点赋值
    tmp->name = (char *)malloc(strlen(book->name)+1);
    strcpy(tmp->name, book->name);
    tmp->category = (char *)malloc(strlen(book->category)+1);
    strcpy(tmp->category, book->category);
    tmp->author = (char *)malloc(strlen(book->author)+1);
    strcpy(tmp->author, book->author);        
    tmp->date = book->date;
    tmp->price = book->price;

    //插入新节点
    pt = pos->next;
    pos->next = tmp;
    tmp->pre = pos;
    tmp->next = pt;
    pt->pre = tmp;
}

//保存图书信息到文件中
//将图书链表中的内容写入文件（覆盖）
void SaveInFile()
{
    FILE *fp;
    Book *pos = head->next;

    //打开文件
    printf("正在打开文件，请稍后...\n");
    if((fp=fopen(filename,"w"))==NULL)
    {
        perror("文件打开失败！\n\n");
        exit(1);
    }

    //写入图书信息
    printf("文件已打开，正在写入数据...\n");
    while(pos != tail)
    {
        fprintf(fp, "%s\t%s\t%s\t%8d\t%.2f\n", pos->name, pos->category, pos->author, pos->date, pos->price);
        pos = pos->next;
    }

    fclose(fp); //关闭文件
    printf("文件写入完毕！\n\n");
}

//辅助排序函数
//根据选择的排序方式way，对待定插入点insert和排序点pos对应字段内容进行比较
//对应字段内容： insert > pos，返回正值；insert < pos，返回负值；insert == pos，返回0
//使用了strcoll函数，在系统默认排序编码为中文GBK时，可以对汉字按拼音正确排序
//该函数在SortBooks函数中被调用
int Sort(Book *insert, Book *pos, int way)
{
    if (way == 1)
    {
        return strcoll(insert->name, pos->name);
    }
    else if (way == 2)
    {
        return strcoll(insert->category, pos->category);
    }
    else if (way == 3)
    {
        return strcoll(insert->author, pos->author);
    }
    else if (way == 4)
    {
        return insert->date - pos->date;
    }
    else if (way == 5)  //返回值为int，因此double需要特殊处理
    {
        if (insert->price > pos->price)
            return 1;

        else if (insert->price == pos->price)
            return 0;

        else
            return -1;
    }
    else    //输入排序方式不合法
    {
        printf("Sort: Input invalid!\n");
        return 0;
    }
}
