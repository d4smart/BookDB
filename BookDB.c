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
void InsertBook(Book *tmp);
void SaveInFile();
void AddBook()
{
    char name[50] = "", category[20] = "", author[30] = "";
    int date, ch;
    float price;
    //head = (Book *)malloc(sizeof(Book));
    head = NULL, tail = NULL;
    Book *tmp = NULL;

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

        tmp = (Book *)malloc(sizeof(Book));

        tmp->name = (char *)malloc(strlen(name)+1);
        strcpy(tmp->name, name);

        tmp->category = (char *)malloc(strlen(category)+1);
        strcpy(tmp->category, category);

        tmp->author = (char *)malloc(strlen(author)+1);
        strcpy(tmp->author, author);
        

        tmp->date = date;
        tmp->price = price;

        if (!head)
        {
            head = tmp;
            tail = head;
        }
        else
        {
            tail->next = tmp;
            tmp->pre = tail;
            tail = tail->next;
        }


        printf("Enter book's name, category, author, purchase date, price in order: \n=> ");
    }

    printf("Add complete. Bye!\n\n");
}

int main()
{
    char name[] = "金瓶梅(插图珍藏版)";
    char category[] = "中国古典小说";
    char author[] = "兰陵笑笑生";
    int date = 20160620;
    float price = 898.20;

    AddBook();
    PrintBooks();
    SaveInFile();
}

void InsertBook(Book *tmp)
{
    
}

void SaveInFile()
{
    FILE *fp;
    Book *pos = head;

    if((fp=fopen(filename,"w"))==NULL)
    {
        perror("fopen");
        exit(1);
    }

    while(pos)
    {
        fprintf(fp, "%s\t%s\t%s\t%8d\t%.2f\n", pos->name, pos->category, pos->author, pos->date, pos->price);
        pos = pos->next;
    }

    fclose(fp);
    printf("Write to file!\n");
}

void PrintBooks()
{
    Book *pos = head;

    printf("书名\t类别\t作者\t购买日期\t价格\n");
    while(pos)
    {
        printf("%s\t%s\t%s\t%8d\t%.2f\n", pos->name, pos->category, pos->author, pos->date, pos->price);

        pos = pos->next;
    }
    printf("\n");
}
