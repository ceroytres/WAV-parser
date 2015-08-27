#include<math.h>
#include<string>
#include<iostream>
#include<stdio.h>
#include <stdlib.h> 



using namespace std;

/****Variables*****/
FILE* fptr;
FILE* fout;
string riff_id="RIFF";
string type_id="WAVE";
string fmt_id="fmt ";
string data_id="data";
int audio_fmt=1;
int size_fmt=16;
int channels,datSize,dat_RIFF,fs,byte_r,bps, ba;
int samples;
/****Functions*****/
int get_DataSize(void);
void write_headers(void);
void write_data(void);

/*****Main*****/
int main()
{
   system("color C1");
   fptr=fopen("sound.bin","rb");
   fout=fopen("out.wav","wb");
   cout<<endl;
   
   datSize=get_DataSize();
   dat_RIFF=datSize+36;
   cout<<"NUMBER OF CHANNELS (MONO=1....STEREO=2)= ";
   cin>>channels;
   
   if((channels!=1)||(channels!=2)) channels=1;
   
   
   cout<<"SAMPLING FREQENCY= ";
   cin>>fs;
   cout<<"BITS PER SAMPLE= ";
   cin>>bps;
   
   byte_r=fs*channels*bps/8;
   cout<<"BYTE RATE= "<<byte_r<<endl;
   
   ba=channels*bps/8;
   cout<<"BLOCK ALIGN= "<<ba<<endl;
   
   samples=(8*datSize)/(channels*bps);
   write_headers();
   write_data();
   fclose(fptr);
   fclose(fout);
   return 0;  
}


int get_DataSize(void)
{
   long lSize=0;
   fseek (fptr , 0 , SEEK_END);
   lSize = ftell (fptr);
   rewind (fptr);
   cout<<"DATA SIZE="<<lSize<<endl;
   return lSize;
}  

void write_headers(void)
{
	//RIFF
	fwrite(riff_id.c_str(),sizeof(unsigned char),riff_id.length(),fout);
	fwrite(&dat_RIFF,sizeof(long),1,fout);
	fwrite(type_id.c_str(),sizeof(unsigned char),type_id.length(),fout);
	//FMT
	fwrite(fmt_id.c_str(),sizeof(unsigned char),type_id.length(),fout);
	fwrite(&size_fmt,sizeof(long),1,fout);
	fwrite(&audio_fmt,sizeof(short int),1,fout);
	fwrite(&channels,sizeof(short int),1,fout);
	fwrite(&fs,sizeof(long),1,fout);
	fwrite(&byte_r,sizeof(long),1,fout);
	fwrite(&ba,sizeof(short int),1,fout);
	fwrite(&bps,sizeof(short int),1,fout);
	//DATA
	fwrite(data_id.c_str(),sizeof(unsigned char),data_id.length(),fout);
	fwrite(&datSize,sizeof(long),1,fout);
}

void write_data(void)
{
	int i;
	short int data;
	for(i=0;i<samples;i++)
	{
		fread(&data,sizeof(short int),1,fptr);
		fwrite(&data,sizeof(short int),1,fout);
	}
	
}