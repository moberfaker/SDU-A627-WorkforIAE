#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <Windows.h>

	#ifdef __cplusplus
	extern "C" {
	#endif

	/*
	���ܣ������ʱ��ʼ��
	[����] testTime�����Դ���
	*/
#define T_INIT(testTime) \
	clock_t start;	\
	int TestTime = testTime; \
	float time; \
	printf("\nTestTime: %d\n",TestTime); 

	/*
	���ܣ�����lable��ʱ��ʼ
	[����] lable����ʱ�ĳ�����
	[ǰ��] ����clock_t start
	*/
#define T_STR(lable) \
	printf("\n======== %s ========\n",lable);	\
	start = clock(); \
	for(int i = 0 ;i < TestTime; i++){ 
	/*
	���ܣ�����lable��ʱ����
	[����] lable����ʱ�ĳ�����
	[����] Rbyte��һ�β��Դ�����ֽ���
	[ǰ��] ����clock_t start
	[ǰ��] ����int TestTime��ʾ������Դ���
	*/
#define T_END(lable,Rbyte) \
	}					   \
	time = clock() - start;		  \
	printf("��ʱ: %.2f ms\n", time/TestTime);\
	if(Rbyte)\
	printf("����: %.4f Mbps\n",(((double)(TestTime*Rbyte*8)/(1024*1024))/time)*1000);	\
	printf("======== %s ========\n",lable);		




	/*
	���ܣ����ݿ���ֽڴ�СpadLen��������data����padding,ģʽΪZERO
	[����] dataLen��*dataָ������ݵĳ���
	[����] padLen��	����ֽڴ�С
	[���] data�� realloc�����\x00
	*/
	void Paddata(uint8_t** data, size_t* dataLen, int padLen);




	/*
	���ܣ����������data,���16���Ƹ�ʽ����
	[����] s����Ҫ˵�����ַ�����������ǰ�����%s:��
	[����] data: ��Ҫת�����������
	[����] len:  data���ֽڳ���
	[����] LINE_BYTE: ÿ���ֽ���
	[����] BLOCK_BYTE: ÿ���ֽ���
	*/
#define LINE_BYTE 16
#define BLOCK_BYTE 8
	void printHex(const char* s, const uint8_t* data, size_t len);

	//����ͬprintHex�����Ҳ�����ASCIIת��
	void printHex_ASCII(const char* s, const uint8_t* data, size_t len);


	#ifdef __cplusplus
	}
	#endif

#endif // !TOOLS_H
