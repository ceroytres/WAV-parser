#include<math.h>
#include<string>
#include<iostream>
#include<stdio.h>
#include <stdlib.h> 

#define NUM_OF_BYTE 4



using namespace std;

/*******Variables********/
typedef struct 
{
	unsigned char riff[NUM_OF_BYTE];
	unsigned char wave[NUM_OF_BYTE+1];
	long size;
} riff_hdr;

typedef struct
{
	unsigned char fmt_txt[NUM_OF_BYTE];
	long size;
	short int audio_fmt;
	short int channel;
	long int sampling_f;
	long int byte_rate;
	short int block_align;
	short int bits_per_sample;
} format_hdr;

typedef struct
{
	unsigned char id[NUM_OF_BYTE];
	long size;
	int num_samples;
} data_hdr;


FILE* fptr;
FILE* fid;
riff_hdr R_hdr={"",0};
format_hdr F_hdr;
data_hdr D_hdr;

/*********Functions Declarations******/
void check_riff_hdr(void);
void check_fmt(void);
void check_data(void);
void monoTo_bin(void);
void StereoTo_bin(void);
void Stereo_mono(void);
/*****Main************/
int main(int argc, char* argv[])
{
	string filename;
	char reply='n';

	system("color b0");

	if(argc<=1)
	{
		printf("Error -1:No enough arguments argc= %d\n",argc);;  
		system("pause");
		return -1;
	}

	filename=argv[1];
	cout<<"\nLoading......"+filename<<endl;
	fptr=fopen(filename.c_str(),"rb");
	fid=fopen("sound_parameters.txt","w");
	
	
	fprintf(fid,"PARAMETERS FOR SOUND FILE\n");
	
	if(fptr==NULL)
		{
			cout<<"Error 1: Reader is null"<<endl;
			return 1;
		}
	else cout<<"Successfully opened....."+filename<<endl;
	
	check_riff_hdr();
	check_fmt();
	check_data();

	cout<<"\nReading data from "+filename+"........"<<endl;

	switch(F_hdr.channel)
	{
	case 1:
		cout<<"Converting mono audio data to bin file"<<endl;
	    monoTo_bin();
		cout<<"Done!"<<endl;
		break;
	case 2:
		printf("Convert from stereo to mono(y/n):");
		scanf("%c",&reply);
		if(reply=='n')
		{
		cout<<"Converting stereo audio data to bin file"<<endl;
		StereoTo_bin();
		cout<<"Done!"<<endl;}
		else
		{
		cout<<"Converting stereo audio data to mono, and then to bin"<<endl;
		Stereo_mono();
		cout<<"Done!"<<endl;
		fprintf(fid,"!!!!!!!!!!FILE WAS CONVERTED TO MONO FROM STEREO!!!!!!!!!!\n");
		}	
		break;
	default: 
	    cout<<"DATA is in another format"<<endl;
		break;
	}
	
	fclose(fid);
	fclose(fptr);
	return 0;
}


void check_riff_hdr(void)
{
	
	cout<<"\nChecking RIFF header"<<endl;
	fprintf(fid,"\nRIFF header\n");
	
	fread(&R_hdr.riff,sizeof(unsigned char),NUM_OF_BYTE,fptr);
	cout<<"ID= "<<R_hdr.riff<<endl;
	fprintf(fid,"ID=%s\n",R_hdr.riff);
	
	fread(&R_hdr.size,sizeof(long),1,fptr);
	cout<<"SIZE="<<R_hdr.size<<endl;
	fprintf(fid,"SIZE=%d\n",R_hdr.size);

	fread(&R_hdr.wave,sizeof(unsigned char),NUM_OF_BYTE,fptr);
	cout<<"TYPE= "<<R_hdr.wave<<endl;
	fprintf(fid,"TYPE=%s\n",R_hdr.wave);
	
	
	cout<<"\n"<<endl;
	
}

void check_fmt(void)
{
	cout<<"Checking FMT header"<<endl;
	fprintf(fid,"\n\nFMT header\n");
	
	fread(&F_hdr.fmt_txt,sizeof(unsigned char),NUM_OF_BYTE,fptr);
	cout<<"ID= "<<F_hdr.fmt_txt<<endl;
	fprintf(fid,"ID=%s\n",F_hdr.fmt_txt);
	
	fread(&F_hdr.size,sizeof(long),1,fptr);
	cout<<"SIZE="<<F_hdr.size<<endl;
	fprintf(fid,"SIZE=%d\n",F_hdr.size);
	
	fread(&F_hdr.audio_fmt,sizeof(short int),1,fptr);
	cout<<"AUDIO_FMT="<<F_hdr.audio_fmt<<endl;
	fprintf(fid,"AUDIO_FMT=%d\n",F_hdr.audio_fmt);
	
	
	fread(&F_hdr.channel,sizeof(short int),1,fptr);
	
	if(F_hdr.channel==1)
		{cout<<"CHANNEL= MONO"<<endl;
		fprintf(fid,"CHANNEL= MONO\n");}
	else if(F_hdr.channel==2)
		{cout<<"CHANNELS= STEREO"<<endl;
		fprintf(fid,"CHANNEL= STEREO\n");}
	else
		{cout<<"CHANNELS= OTHER"<<endl;
		fprintf(fid,"CHANNEL= OTHER\n");}
		
	fread(&F_hdr.sampling_f,sizeof(long int),1,fptr);
	cout<<"SAMPLING FREQUENCY="<<F_hdr.sampling_f<<" Hz"<<endl;
	fprintf(fid,"SAMPLING FREQUENCY=%d\n",F_hdr.sampling_f);
	
	fread(&F_hdr.byte_rate,sizeof(long int),1,fptr);
	cout<<"BYTE RATE="<<F_hdr.byte_rate<<endl;
	fprintf(fid,"BYTE RATE=%d\n",F_hdr.byte_rate);
	
	fread(&F_hdr.block_align,sizeof(short int),1,fptr);
	cout<<"BLOCK ALIGN="<<F_hdr.block_align<<endl;
	fprintf(fid,"BLOCK ALIGN=%d\n",F_hdr.block_align);
	
	fread(&F_hdr.bits_per_sample,sizeof(short int),1,fptr);
	cout<<"BITS PER SAMPLE="<<F_hdr.bits_per_sample<<endl;
	fprintf(fid,"BITS PER SAMPLE=%d\n\n",F_hdr.bits_per_sample);
	cout<<endl;
}

void check_data(void)
{
	cout<<endl;
	cout<<"Checking data header...."<<endl;
	fprintf(fid,"Data\n\n");
	
	fread(&D_hdr.id,sizeof(unsigned char),NUM_OF_BYTE,fptr);
	cout<<"ID= "<<D_hdr.id<<endl;
	fprintf(fid,"ID=%s\n",D_hdr.id);
	
	fread(&D_hdr.size,sizeof(long),1,fptr);
	cout<<"SIZE OF DATA= "<<D_hdr.size<<endl;
	fprintf(fid,"SIZE=%d\n",D_hdr.size);
	
	D_hdr.num_samples=(D_hdr.size*8)/(F_hdr.channel*F_hdr.bits_per_sample);
	cout<<"Number of samples="<<D_hdr.num_samples<<endl;
	fprintf(fid,"SAMPLES=%d\n",D_hdr.num_samples);
}

 void monoTo_bin(void)
{
	FILE* fp;
	int i;
	short int data=0;
	fp=fopen("sound.bin","wb");
	for(i=0;i<D_hdr.num_samples;i++)
	{
		fread(&data,sizeof(short int),1,fptr);
		fwrite(&data,sizeof(short int),1,fp);
	}
	fclose(fp);	
}

 void StereoTo_bin(void)
{
	FILE* fp;
	int i;
	short int data=0;
	fp=fopen("sound.bin","wb");
	for(i=0;i<D_hdr.num_samples*2;i++)
	{
		fread(&data,sizeof(short int),1,fptr);
		fwrite(&data,sizeof(short int),1,fp);
	}
	fclose(fp);
}

void Stereo_mono(void)
{
	FILE* fp;
	int i;
	short int data=0;
	fp=fopen("sound.bin","wb");
	for(i=0;i<D_hdr.num_samples;i+=1)
	{
		fread(&data,sizeof(short int),1,fptr);
		fseek(fptr,sizeof(short int),SEEK_CUR);
		fwrite(&data,sizeof(short int),1,fp);
	}
	fclose(fp);
}
