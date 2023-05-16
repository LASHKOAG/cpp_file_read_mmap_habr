#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

//Как ускорить чтение строк из файла?
//https://qna.habr.com/q/1275888
//egor_nullptr @egor_nullptr

//test.txt (разместить в каталог Debug)
//AAA BBB 1111
//AAA CCC 22222
//BBB CCC 333333
//DDD FFF 55555


int main(int argc, char** argv)
{
    printf("argv[1]: %s\n\n", argv[1]);
    printf("argc: %d\n\n", argc);
    if (argc != 2)
    {
        printf("Too many arguments.\n");
        exit(EXIT_FAILURE);//1
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 1){
        printf("ERROR: no open file\n");
        exit(EXIT_FAILURE);
    }

    //    int fd = open("/home/expert/file_read_habr/for_debug/test.txt", O_RDONLY);
    printf("fd: %d\n\n", fd);
    //    struct stat finfo = {0};
    //
    struct stat finfo;
    fstat(fd, &finfo);

    char *map = (char *)mmap(0, finfo.st_size, PROT_READ, MAP_SHARED, fd, 0);

    char src[4] = {0};
    char dst[4] = {0};
    int cost;
    int i;
    int off = 0;

    while (off < finfo.st_size)
    {
        memcpy(src, map + off, 3);
        memcpy(dst, map + off + 4, 3);
        i = 0;
        cost = 0;
        printf("src: %s\n\n", src); //AAA
        printf("dst: %s\n\n", dst); //BBB

        while (map[off + 8 + i] != 10) {
            cost = cost * 10 + map[off + 8 + i] - 48;
            ++i;
        };
        off += 9 + i;
        printf("cost: %d\n\n", cost); //1111
    };

    munmap(map, finfo.st_size);
    close(fd);

    return EXIT_SUCCESS;
}



//Wataru @wataru
//Разработчик на С++, экс-олимпиадник.
//Может, ручной разбор будет быстрее. Читайте весь файл в большой буфер.
//Трехсимвольные строки - это пусть будут тупо char* на начало строк в этом буфере.
//Там пробелы замените на '\0' только.
//Вот вы уже сэкономили на разборе строк и выделении под них памяти.
//Числа руками разобрать придется правда (умножение на 10 + следующий символ - '0').

//Еще, вместо чтения файла какой-нибудь mmap может быть быстрее.

//Потом это можно еще и распараллелить каким-нибудь OpenMP, например.
//Разбейте буфер на примерно равные части, чуть подвигайте границы,
//чтоб они на переводы строк приходились и каждый кусок разбирайте отдельно.
