#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int max[20][20],MAXR[20];
int allocated[20][20];
int need[20][20];
int available[20];
int n, m;
int safe[20];
int issafe();
int memcp(int*, int*, int);

void print(int (*a)[20], int n, int m)
{
    int i, j;
    for(i = 0; i < n; i++)
    {
        printf("\n");
        for(j = 0; j < m; j++)
        {
            printf("%d\t", a[i][j]);
        }
    }
}

int main()
{
    int i, j, p, request[20];
    int *avail, *alloc, *nd;
    printf("Enter the no. of processes and resources: ");
    scanf("%d%d", &n, &m);
    printf("Enter the maximum resources available: \n");
    for(i=0; i<m; i++)
    {
        printf("Enter MAX[%d]:",i+1);
        scanf("%d",&MAXR[i]);
    }

    printf("Enter the max. matrix\n");
    for(i = 0; i < n; i++)
        for(j = 0; j < m; j++)
        {
            printf("Max[%d][%d]: ", i+1, j+1);
            scanf("%d", &max[i][j]);
        }
    //printf("Enter the available resource vector: \n");
    /* for(i = 0; i < m; i++)
     {
         printf("Resource[%d]: ", i+1);
         scanf("%d", &available[i]);
     }*/
    printf("Enter the  allocation matrix\n");
    for(i = 0; i < n; i++)
        for(j = 0; j < m; j++)
        {
            printf("Allocated[%d][%d]: ", i+1, j+1);
            scanf("%d", &allocated[i][j]);
        }
    for(i = 0; i < n; i++)
        for(j = 0; j < m; j++)
        {
            need[i][j] = max[i][j] - allocated[i][j];
        }

    for(i=0; i<m; i++)
    {
        available[i] = 0;
    }
    for(i=0; i<m; i++)
    {
        for(j=0; j<n; j++)
        {
            available[i] += allocated[j][i];
        }
    }
    for(i=0; i<m; i++)
    {
        available[i] = MAXR[i] - available[i];
    }

    if(issafe())
    {
        printf("Safe State");
    }
    else
    {
        printf("Not a safe state");
    }
}
int memcp(int *a,int *b, int n)
{
    int i;
    for(i = 0; i < n; i++)
        a[i] = b[i];
}

int isless(int *a,int *b)
{
    int i;
    for(i = 0; i < m; i++)
        if(a[i] > b[i])
            return 0;
    return 1;
}

int issafe()
{
    int i, *work, *finish;
    char out[50];
    work = malloc(m*4);
    finish = malloc(n*4);
    memset(finish, 0, n);
    memcp(work, available, m);
    sprintf(out, "<");
    for(i = 0; i < n; i++)
    {
        if(isless(need[i], work) && !finish[i])
        {
            add(work, allocated[i]);
            finish[i] = 1;
            sprintf(out + strlen(out), "P%d, ", i+1);
            i = -1;
        }
    }
    printf("\n");
    for(i = 0; i < n; i++)
        if(!finish[i])
            break;
    free(work);
    free(finish);
    if(i == n)
    {
        sprintf(out + strlen(out) - 2, ">");
        printf("%s\n", out);
        return 1;
    }
    else
        return 0;
}

int add(int *a, int *b)
{
    int i;
    for(i = 0; i < m; i++)
        a[i] += b[i];
    return 1;
}

int sub(int *a, int *b)
{
    int i;
    for(i = 0; i < m; i++)
        a[i] -= b[i];
    return 1;
}
