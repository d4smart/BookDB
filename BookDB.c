/*************************************************************************
  > File Name:    BookDB.c
  > Author:       d4smart 
  > Mail:         d4smart@foxmail.com
  > Created Time: 2016年06月21日 星期二 13时16分56秒
  > Version:      0.0.1
  > LastChange:   
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _book
{
    //int Prikey;
    char *name;
    char *category;
    char *author;
    int date;

    float price;
    struct _book *pre;
    struct _book *next;
} Book;

Book *head, *tail;
char filename[] = "archive.txt";

void PrintBooks();
void InsertBook(Book *book, Book *pos);
void SaveInFile();
void GetBooks();
int Sort(Book *insert, Book *pos, int way);
void SortBooks(int way);
void AddBooks();
Book *FindBooks();
void ChangeInfo();
void DeleteBooks();
int IsBook(int way, char *key, Book *pos);

int main()
{
    int operation, processing = 1;

    printf("\n欢迎来到图书管理系统，本系统由d4smart开发\n\n");

    GetBooks();

    while(processing)
    {
        printf("输入数字选择操作： 1.查找图书记录 2.添加图书记录 3.修改图书记录 4.删除图书记录 5.打印全部图书记录 6.对图书记录排序并保存\n");
        printf("请输入你要进行的操作，按 ctrl+z(Windows)/ctrl+d(Linux) 退出：");

        if (scanf("%d", &operation) == EOF)
        {
            processing = 0;
            continue;
        }

        if (operation < 1 || operation > 6)
        {
            printf("选择的操作不合法！\n\n");
            continue;
        }

        if (operation == 1)
        {
            FindBooks();
        }
        else if (operation == 2)
        {
            AddBooks(); 
        }
        else if (operation == 3)
        {
            ChangeInfo();
        }
        else if (operation == 4)
        {
            DeleteBooks();
        }
        else if (operation == 5)
        {
            PrintBooks();
        }
        else
        {
            SortBooks(5);
        }
    }

    printf("正在退出操作...\n\n");
    SaveInFile();
}

void AddBooks()
{
    char name[60] = "", category[40] = "", author[40] = "";
    int date, ch;
    float price;
    Book *book = NULL;
    Book *pos = tail->pre;

    printf("Now add books...\n");
    printf("Enter book's name, category, author, purchase date, price in order: \n=> ");
    while((ch = scanf("%s %s %s %d %f", name, category, author, &date, &price)) != EOF)
    {
        if (ch != 5)
        {
            printf("Input format invalid, please try again.\n");
            printf("Enter book's name, category, author, purchase date, price in order: \n=> ");
            //printf("%d", ch);
            continue;
        }

        book = (Book *)malloc(sizeof(Book));

        book->name = name;
        book->category = category;
        book->author = author;
        book->date = date;
        book->price = price;

        InsertBook(book, pos);
        pos = pos->next;

        printf("Enter book's name, category, author, purchase date, price in order: \n=> ");
    }

    printf("Add complete. Bye!\n\n");
}

Book *FindBooks()
{
    char key[60] = "";
    int date, way;
    Book *pos = head->next;
    Book *find = NULL;

    printf("Please input the way to find: ");
    scanf("%d", &way);
    printf("Please input key: ");
    scanf("%s", key);

    while(pos != tail)
    {
        if (IsBook(way, key, pos) == 0)
        {
            printf("%-30s%-20s%-20s%8d\t%.2f\n", pos->name, pos->category, pos->author, pos->date, pos->price);
            find = pos;
        }

        pos = pos->next;
    }

    return find;
}

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

void ChangeInfo()
{
    char info[60];
    int date, way;
    float price;
    Book *change = NULL;

    printf("CHANGE INFO\n");
    printf("Choose which book to change...");
    change = FindBooks();

    printf("Choose which info to change, (name,category,author,date,price)|(1,2,3,4,5): ");
    scanf("%d", &way);
    printf("Change to: ");

    if (way == 1 || way == 2 || way == 3)
    {
        scanf("%s", info);

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
    else if (way == 4)
    {
        scanf("%d", &date);
        change->date = date;
    }
    else
    {
        scanf("%f", &price);
        change->price = price;
    }

    printf("Record changed to: ");
    printf("%-30s%-20s%-20s%8d\t%.2f\n", change->name, change->category, change->author, change->date, change->price);
}

void DeleteBooks()
{
    Book *del = NULL;

    printf("DELETE INFO\n");
    printf("Choose which book to delete...");
    del = FindBooks();

    del->pre->next = del->next;
    del->next->pre = del->pre;
    free(del);
}
    
void InsertBook(Book *book, Book *pos)
{
    Book *tmp = NULL, *pt = NULL;
        
    tmp = (Book *)malloc(sizeof(Book));

    tmp->name = (char *)malloc(strlen(book->name)+1);
    strcpy(tmp->name, book->name);

    tmp->category = (char *)malloc(strlen(book->category)+1);
    strcpy(tmp->category, book->category);

    tmp->author = (char *)malloc(strlen(book->author)+1);
    strcpy(tmp->author, book->author);
        
    tmp->date = book->date;
    tmp->price = book->price;

    pt = pos->next;
    pos->next = tmp;
    tmp->pre = pos;
    tmp->next = pt;
    pt->pre = tmp;
}

void SaveInFile()
{
    FILE *fp;
    Book *pos = head->next;

    printf("正在打开文件，请稍后...\n");
    if((fp=fopen(filename,"w"))==NULL)
    {
        perror("文件打开失败！\n\n");
        exit(1);
    }

    printf("文件已打开，正在写入数据...\n");
    while(pos != tail)
    {
        fprintf(fp, "%s\t%s\t%s\t%8d\t%.2f\n", pos->name, pos->category, pos->author, pos->date, pos->price);
        pos = pos->next;
    }

    fclose(fp);
    printf("文件写入完毕！\n\n");
}

void GetBooks()
{
    FILE *fp;
    char name[60] = "", category[40] = "", author[40] = "";
    int date, ch;
    float price;

    head = (Book *)malloc(sizeof(Book));
    tail = (Book *)malloc(sizeof(Book));
    head->next = tail;
    tail->pre = head;

    Book *book = NULL;
    Book *pos = tail->pre;
    
    printf("正在打开文件，请稍后...\n");
    if((fp=fopen(filename,"r"))==NULL)
    {
        perror("文件打开失败！\n\n");
        exit(1);
    }

    printf("文件已打开，正在读取数据...\n");
    while((ch = fscanf(fp, "%s %s %s %d %f", name, category, author, &date, &price)) != EOF)
    { 
        book = (Book *)malloc(sizeof(Book));

        book->name = name;
        book->category = category;
        book->author = author;
        book->date = date;
        book->price = price;

        InsertBook(book, pos);
        pos = pos->next;
    }

    fclose(fp);
    printf("数据读取完毕！\n\n");
}

int Sort(Book *insert, Book *pos, int way)
{
    if (way == 1)
    {
        return strcmp(insert->name, pos->name);
    }
    else if (way == 2)
    {
        return strcmp(insert->category, pos->category);
    }
    else if (way == 3)
    {
        return strcmp(insert->author, pos->author);
    }
    else if (way == 4)
    {
        return insert->date - pos->date;
    }
    else if (way == 5)
    {
        if (insert->price > pos->price)
            return 1;

        else if (insert->price == pos->price)
            return 0;

        else
            return -1;
    }
    else
    {
        printf("Sort: Input invalid!\n");
        return 0;
    }
}

void SortBooks(int way)
{
    if (head->next == tail || head->next == tail->pre)
        return;

    Book *pos, *insert;

    for(pos = head->next->next; pos != tail; pos = pos->next)
    {
        Book *tmp1, *tmp2;

        insert = pos->pre;

        while(insert != head && Sort(insert, pos, way) > 0)
        {
            insert = insert->pre;
        }

        pos->pre->next = pos->next;
        pos->next->pre = pos->pre;
        tmp1 = pos->next;

        tmp2 = insert->next;
        insert->next = pos;
        pos->pre = insert;
        pos->next = tmp2;
        tmp2->pre = pos;

        pos = tmp1->pre;
    }
}

void PrintBooks()
{
    Book *pos = head->next;

    printf("书名\t\t\t类别\t\t作者\t\t购买日期\t价格\n");
    while(pos != tail)
    {
        printf("%-30s%-20s%-20s%8d\t%.2f\n", pos->name, pos->category, pos->author, pos->date, pos->price);

        pos = pos->next;
    }
    printf("\n");
}
