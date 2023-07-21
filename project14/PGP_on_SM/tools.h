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
	功能：程序计时初始化
	[输入] testTime：测试次数
	*/
#define T_INIT(testTime) \
	clock_t start;	\
	int TestTime = testTime; \
	float time; \
	printf("\nTestTime: %d\n",TestTime); 

	/*
	功能：程序lable计时开始
	[输入] lable：计时的程序名
	[前提] 定义clock_t start
	*/
#define T_STR(lable) \
	printf("\n======== %s ========\n",lable);	\
	start = clock(); \
	for(int i = 0 ;i < TestTime; i++){ 
	/*
	功能：程序lable计时结束
	[输入] lable：计时的程序名
	[输入] Rbyte：一次测试处理的字节数
	[前提] 定义clock_t start
	[前提] 定义int TestTime表示程序测试次数
	*/
#define T_END(lable,Rbyte) \
	}					   \
	time = clock() - start;		  \
	printf("耗时: %.2f ms\n", time/TestTime);\
	if(Rbyte)\
	printf("吞吐: %.4f Mbps\n",(((double)(TestTime*Rbyte*8)/(1024*1024))/time)*1000);	\
	printf("======== %s ========\n",lable);		




	/*
	功能：根据块的字节大小padLen，对数据data进行padding,模式为ZERO
	[输入] dataLen：*data指向的数据的长度
	[输入] padLen：	块的字节大小
	[输出] data： realloc并填充\x00
	*/
	void Paddata(uint8_t** data, size_t* dataLen, int padLen);




	/*
	功能：根据输入的data,输出16进制格式数据
	[输入] s：需要说明的字符串，在数据前输出“%s:”
	[输入] data: 需要转换输出的数据
	[输入] len:  data的字节长度
	[定义] LINE_BYTE: 每行字节数
	[定义] BLOCK_BYTE: 每块字节数
	*/
#define LINE_BYTE 16
#define BLOCK_BYTE 8
	void printHex(const char* s, const uint8_t* data, size_t len);

	//具体同printHex，在右侧多输出ASCII转码
	void printHex_ASCII(const char* s, const uint8_t* data, size_t len);


	#ifdef __cplusplus
	}
	#endif

#endif // !TOOLS_H
