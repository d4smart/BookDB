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
void SortBooks();
int Sort();
void AddBooks();
Book *FindBooks();
void ChangeInfo();
void DeleteBooks();
int IsBook(int way, char *key, Book *pos);

int main()
{
    char name[] = "金瓶梅(插图珍藏版)";
    char category[] = "中国古典小说";
    char author[] = "兰陵笑笑生";
    int date = 20160620;
    float price = 898.20;

    GetBooks();
    AddBooks();
    PrintBooks();
    //FindBooks();
    //ChangeInfo();
    SortBooks();
    //DeleteBooks();
    PrintBooks();
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
    Book *delete = NULL;

    printf("DELETE INFO\n");
    printf("Choose which book to delete...");
    delete = FindBooks();

    delete->pre->next = delete->next;
    delete->next->pre = delete->pre;
    free(delete);
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

    if((fp=fopen(filename,"w"))==NULL)
    {
        perror("fopen");
        exit(1);
    }

    while(pos != tail)
    {
        fprintf(fp, "%s\t%s\t%s\t%8d\t%.2f\n", pos->name, pos->category, pos->author, pos->date, pos->price);
        pos = pos->next;
    }

    fclose(fp);
    printf("Write to file!\n");
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
    
    if((fp=fopen(filename,"r"))==NULL)
    {
        perror("fopen");
        exit(1);
    }
    
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

    printf("Books getted.\n");
}

void SortBooks()
{
    if (head->next == tail || head->next == tail->pre)
        return;

    float key = 0;
    Book *pos, *insert;

    for(pos = head->next->next; pos != tail; pos = pos->next)
    {
        Book *tmp1, *tmp2;
        key = pos->price;
        insert = pos->pre;

        while(insert != head && insert->price > key)
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
