//This code was written last year, but some issues fixed recently to comply with Conway's original rule.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <getopt.h>
#include <string>
#include <iostream>

std::string m="";

const char *Icon[] =
{
    " .",/*□*/
    " X"
};//图素

class Cell{
private:
    int currStatus_;
    int nextStatus_;
public:
    Cell()
    {
        currStatus_ = 0;
        nextStatus_ = ((rand() % 2) && (rand() % 2));
    }
    void showCell()
    {
        currStatus_ = nextStatus_;
        m.append(Icon[currStatus_]);
    }
    void setAlive()
    {
        nextStatus_ = 1;
    }
    void setKilled()
    {
        nextStatus_ = 0;
    }
    int isAlive()
    {
        return currStatus_;
    }
};

class Map{
private:
    int width=19;
    int high=19;
    int a=3;
    int b=2;
    Cell *worldMatrix_[100][100];
    int dayCount_;
    int totalAlive_;
public:
    void setwidth(int w1){
        width=w1;
    }
    void sethigh(int h1){
        high=h1;
    }
    void seta(int a1){
        a=a1;
    }
    void setb(int b1){
        b=b1;
    }
    int getwidth(){
        return width;
    }
    int gethigh(){
        return high;
    }
    int geta(){
        return a;
    }
    int getb(){
        return b;
    }
    Map()
    {
        width=getwidth();
        high=gethigh();
        dayCount_ = 1;
        totalAlive_ = 0;
        for (int i = 0; i < high; i++)
        {
            for (int j = 0; j < width; j++)
                worldMatrix_[i][j] = new Cell;
        }
    }
    ~Map()
    {
        for (int i = 0; i < high; i++)
        {
            for (int j = 0; j < width; j++)
                delete worldMatrix_[i][j];
        }
    }
    void displayMap()
    {
        m="";
        totalAlive_ = 0;
        for (int i = 0; i < high; i++)
        {
            for (int j = 0; j < width; j++)
            {
                worldMatrix_[i][j]->showCell();
                totalAlive_ += worldMatrix_[i][j]->isAlive();
            }
            m.append("\r\n");
            //printf("\r\n");
        }
        m.append("第"+std::to_string(dayCount_)+"天\t地图中有"+std::to_string(totalAlive_)+"个细胞\r\n\n");
        std::cout<<m;
        //printf("第%d天\t地图中有%d个细胞\r\n\n", dayCount_, totalAlive_);
    }
    void startIterating()
    {
        int count = 0;
        for (int i = 0; i < high; i++)
        {
            for (int j = 0; j < width; j++)
            {
                count = 0;
                if ((i - 1 >= 0) && (worldMatrix_[i - 1][j]->isAlive()))//i-1 , j
                    count++;
                if ((i + 1 < high) && (worldMatrix_[i + 1][j]->isAlive()))//i+1 , j
                    count++;
                if ((j - 1 >= 0) && (worldMatrix_[i][j - 1]->isAlive()))//i , j-1
                    count++;
                if ((j + 1 < width) && (worldMatrix_[i][j + 1]->isAlive()))//i , j+1
                    count++;
                if ((i - 1 >= 0) && (j - 1 >= 0) && (worldMatrix_[i - 1][j - 1]->isAlive()))//i-1 , j-1
                    count++;
                if ((i - 1 >= 0) && (j + 1 < width) && (worldMatrix_[i - 1][j + 1]->isAlive()))//i-1 , j+1
                    count++;
                if ((i + 1 < high) && (j - 1 >= 0) && (worldMatrix_[i + 1][j - 1]->isAlive()))//i+1 , j-1
                    count++;
                if ((i + 1 < high) && (j + 1 < width) && (worldMatrix_[i + 1][j + 1]->isAlive()))//i+1 , j+1
                    count++;
                if (count == geta())
                {
                    worldMatrix_[i][j]->setAlive();
                }
                else if (count == getb())
                {
                    if (worldMatrix_[i][j]->isAlive())
                    {
                        worldMatrix_[i][j]->setAlive();
                    }
                    else
                    {
                        worldMatrix_[i][j]->setKilled();
                    }
                }
                else
                {
                    worldMatrix_[i][j]->setKilled();
                }
            }
        }
        dayCount_++;
    }
};

int main(int argc, char* argv[])
{
    SetWindowText(GetForegroundWindow(), "Life Game");

    Map map;
    int w1,h1,a1,b1;

    int opt; // getopt_long() 的返回值
    int digit_optind = 0; // 设置短参数类型及是否需要参数

    // 如果option_index非空，它指向的变量将记录当前找到参数符合long_opts里的
    // 第几个元素的描述，即是long_opts的下标值
    int option_index = 0;
    // 设置短参数类型及是否需要参数
    const char *optstring = "w:h:a:b:";  

    // 设置长参数类型及其简写，比如 --reqarg <==>-r
    /*
    struct option {
             const char * name;  // 参数的名称
             int has_arg; // 是否带参数值，有三种：no_argument， required_argument，optional_argument
             int * flag; // 为空时，函数直接将 val 的数值从getopt_long的返回值返回出去，
                         // 当非空时，val的值会被赋到 flag 指向的整型数中，而函数返回值为0
             int val; // 用于指定函数找到该选项时的返回值，或者当flag非空时指定flag指向的数据的值
        };
    其中：
        no_argument(即0)，表明这个长参数不带参数（即不带数值，如：--name）
        required_argument(即1)，表明这个长参数必须带参数（即必须带数值，如：--name Bob）
        optional_argument(即2)，表明这个长参数后面带的参数是可选的，（即--name和--name Bob均可）
     */
    static struct option long_options[] = {
        {"width",required_argument,NULL,'w'},
        {"high",required_argument,NULL,'h'}//,
        //{0, 0, 0, 0}  // 添加 {0, 0, 0, 0} 是为了防止输入空值
    };

    while ( (opt = getopt_long(argc,argv,optstring,long_options,&option_index)) != -1) {
        switch (opt) {
            case 'w':
                w1=atoi(argv[optind-1]); 
                map.setwidth(w1);
                break;
            case 'h':
                h1=atoi(argv[optind-1]); 
                map.sethigh(h1);
                break;
            case 'a':
                a1=atoi(argv[optind-1]); 
                map.seta(a1);
                break;
            case 'b':
                b1=atoi(argv[optind-1]); 
                map.setb(b1);
                break;
            case '?':
                printf("Unknown option: %c\n",(char)opt);
                break;
        }
    }

/*    srand([]()->unsigned int
    {
        _asm
        {
            rdtsc;
        }
    }());*/

    while (true)
    {
        Sleep(200);
        map.displayMap();
        map.startIterating();
    }
    return 0;
}