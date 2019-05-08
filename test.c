#include <stdio.h>
#include <pthread.h> // khai bao cac ham xu li tuyen
#include <stdlib.h> // dung ham system
#include <unistd.h> // dung fork()
#include <sys/types.h>
#include <sys/wait.h>
// dung thu vien lk dong libpthread

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1, cond2, cond3, cond4; // bien dieu kien kieu
int count = 1; // bien dung de dieu khien luong
#define COUNT_DONE 5

// process 1
void * thread1()
{
	fork();
	for(;;)
	{
		pthread_mutex_lock( &mutex1); // khoa vao de doi giai ph
		pthread_cond_wait( &cond1, &mutex1); // giong nhu nha khoa mutex cho vao trang thai ch
		printf("Ma hoa video cua process1 \n");
		system("ffmpeg -i Hung.mp4 -c:v libx264 -preset fast -crf 22 -c:a copy en_hung.mp4"); // ma hoa video theo chuan h264
		printf("Play back video cua process5 \n");		
		system("ffplay -autoexit en_hieu.mp4");
		printf("Dung luong file video:\n");
		system("du -ah en_hieu.mp4");
		printf("Chieu dai file video:\n");
		system ("mediainfo --Inform='Video;%Duration/String3%' en_hieu.mp4");
		count++;
		pthread_mutex_unlock( &mutex1); // mo khoa de giai phong tai nguyen
		if(count >= COUNT_DONE) return(NULL);
	}
}

// process 2
void * thread2()
{
	fork();
	for(;;)
	{
		pthread_mutex_lock( &mutex1);
		if( count == 2 )
        {
		pthread_cond_wait( &cond2, &mutex1);
		printf("Ma hoa video cua process2 \n");
		system("ffmpeg -i Hiep.mp4 -c:v libx264 -preset fast -crf 22 -c:a copy en_hiep.mp4");
		printf("Play back video cua process1 \n");
		system("ffplay -autoexit en_hung.mp4"); // playback video
		printf("Dung luong file video:\n");
		system("du -ah en_hung.mp4");
		printf("Chieu dai file video:\n");
		system ("mediainfo --Inform='Video;%Duration/String3%' en_hung.mp4");
		count++;
		}
		else
		{	
		pthread_cond_signal( &cond1);
		}
		pthread_mutex_unlock( &mutex1);
		if(count >= COUNT_DONE) return(NULL);
	}
}

// process 3
void * thread3()
{
	fork();
	for(;;)
	{
		pthread_mutex_lock( &mutex1);
		if( count == 3 )
        {
		pthread_cond_wait( &cond3, &mutex1);
		printf("Ma hoa video cua process3 \n");
		system("ffmpeg -i Huong.mp4 -c:v libx264 -preset fast -crf 22 -c:a copy en_huong.mp4");
		printf("Play back video cua process2 \n");
		system("ffplay -autoexit en_hiep.mp4"); // playback video
		printf("Dung luong file video:\n");
		system("du -ah en_hiep.mp4");
		printf("Chieu dai file video:\n");
		system ("mediainfo --Inform='Video;%Duration/String3%' en_hiep.mp4");
		count++;
		}
		else
		{	
		pthread_cond_signal( &cond2);
		}
		pthread_mutex_unlock( &mutex1);
		if(count >= COUNT_DONE) return(NULL);
	}
}

// process 4
void * thread4()
{
	fork();
	for(;;)
	{
		pthread_mutex_lock( &mutex1);
		if( count == 4 )
        {
		pthread_cond_wait( &cond4, &mutex1);
		printf("Ma hoa video cua process4 \n");
		system("ffmpeg -i Hoang.mp4 -c:v libx264 -preset fast -crf 22 -c:a copy en_hoang.mp4");
		printf("Play back video cua process3 \n");
		system("ffplay -autoexit en_huong.mp4"); // playback video
		printf("Dung luong file video:\n");
		system("du -ah en_huong.mp4");
		printf("Chieu dai file video:\n");
		system ("mediainfo --Inform='Video;%Duration/String3%' en_huong.mp4");
		count++;
		}
		else
		{	
		pthread_cond_signal( &cond3);
		}
		pthread_mutex_unlock( &mutex1);
		if(count >= COUNT_DONE) return(NULL);
	}
}

// process 5
void * thread5()
{
	fork();
	for(;;)
	{
		pthread_mutex_lock( &mutex1);
		if( count == 5 )
        {
		printf("Ma hoa video cua process5 \n");
		system("ffmpeg -i Hieu.mp4 -c:v libx264 -preset fast -crf 22 -c:a copy en_hieu.mp4");
		printf("Play back video cua process4 \n");
		system("ffplay -autoexit en_hoang.mp4"); // playback video
		printf("Dung luong file video:\n");
		system("du -ah en_hoang.mp4");
		printf("Chieu dai file video:\n");
		system ("mediainfo --Inform='Video;%Duration/String3%' en_hoang.mp4");
		count++;
		}
		else
		{	
		pthread_cond_signal( &cond4);
		}
		pthread_mutex_unlock( &mutex1);
		if(count >= COUNT_DONE) return(NULL);
	}
}

int main()
{
	pthread_t th1, th2, th3, th4, th5;

	printf("Thanh vien trong nhóm: \n");
	system("cat teamhdh");
	
	pthread_create(&th1,NULL,&thread1,NULL); // tao thread moi 
	pthread_create(&th2,NULL,&thread2,NULL);
	pthread_create(&th3,NULL,&thread3,NULL);
	pthread_create(&th4,NULL,&thread4,NULL);
	pthread_create(&th5,NULL,&thread5,NULL);

	pthread_join(th1, NULL); // sử dụng để đợi 1 thread kết thúc hoặc chờ tái kết hợp với các luồng chính
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	pthread_join(th4, NULL);
	pthread_join(th5, NULL);
	system("top -p 0"); // Hien thi %CPU

// thu vien lien ket dong
	
	/*
	system("gcc -c -fpic test1.c"); // tao duoi .o de shared
	system("gcc –shared test1.o -olibfoo.so"); // tao thu vien lien ket dong voi duoi .so
	system("gcc test1.c –oprogram –L. –lfoo"); // co thu vien lk dong va bat dau bien dich lai chuong tr
	system("LD_LIBRARY_PATH=.:"); // dua thu muc hien hanh vao bien moi tr
	system("export LD_LIBRARY_PATH");
	system("ldd test"); // check OS co tim tat ca thu vien lk dong hay khong
	system("./test"); // chay chuong trinh su dung lk - k c
	system("");
	system("");
	*/
    
	printf("Ty le phan tram dong goi tung thanh vien: \n");
	printf("1. Duong Tuan Hung  - 25 \n");
	printf("2. Vi Quang Hiep    - 20 \n");
	printf("3. Nguyen Thi Huong - 20 \n");
	printf("4. Pham Xuan Hoang  - 17.5 \n");
	printf("5. Do Van Hieu      - 17.5 \n");
	// wait();

	return 0;
}
