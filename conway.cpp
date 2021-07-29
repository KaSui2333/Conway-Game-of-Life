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
    " .",/*��*/
    " X"
};//ͼ��

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
        m.append("��"+std::to_string(dayCount_)+"��\t��ͼ����"+std::to_string(totalAlive_)+"��ϸ��\r\n\n");
        std::cout<<m;
        //printf("��%d��\t��ͼ����%d��ϸ��\r\n\n", dayCount_, totalAlive_);
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

    int opt; // getopt_long() �ķ���ֵ
    int digit_optind = 0; // ���ö̲������ͼ��Ƿ���Ҫ����

    // ���option_index�ǿգ���ָ��ı�������¼��ǰ�ҵ���������long_opts���
    // �ڼ���Ԫ�ص�����������long_opts���±�ֵ
    int option_index = 0;
    // ���ö̲������ͼ��Ƿ���Ҫ����
    const char *optstring = "w:h:a:b:";  

    // ���ó��������ͼ����д������ --reqarg <==>-r
    /*
    struct option {
             const char * name;  // ����������
             int has_arg; // �Ƿ������ֵ�������֣�no_argument�� required_argument��optional_argument
             int * flag; // Ϊ��ʱ������ֱ�ӽ� val ����ֵ��getopt_long�ķ���ֵ���س�ȥ��
                         // ���ǿ�ʱ��val��ֵ�ᱻ���� flag ָ����������У�����������ֵΪ0
             int val; // ����ָ�������ҵ���ѡ��ʱ�ķ���ֵ�����ߵ�flag�ǿ�ʱָ��flagָ������ݵ�ֵ
        };
    ���У�
        no_argument(��0)���������������������������������ֵ���磺--name��
        required_argument(��1)�����������������������������������ֵ���磺--name Bob��
        optional_argument(��2)���������������������Ĳ����ǿ�ѡ�ģ�����--name��--name Bob���ɣ�
     */
    static struct option long_options[] = {
        {"width",required_argument,NULL,'w'},
        {"high",required_argument,NULL,'h'}//,
        //{0, 0, 0, 0}  // ��� {0, 0, 0, 0} ��Ϊ�˷�ֹ�����ֵ
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