// httpͨѶ
#include "../stdafx.h"
#pragma once

#define CURL_STATICLIB 
#if defined(_DEBUG) 
#pragma comment(lib, "libcurld.lib") 
#else
#pragma comment(lib, "libcurl.lib") 
#endif
#pragma comment ( lib, "winmm.lib" ) 
#pragma comment ( lib, "ws2_32.lib" ) 
#pragma comment ( lib, "wldap32.lib" ) 
#include <string>
#include <string.h>
using std::string;

/**
 * ���ܣ�ͨ��curl��post���ͱ���
 * ��Σ�pStrUrl�����͵�url��ַ
 *		pStrData�����͵�����
 *		nDataSize���������ݳ���
 * ����ֵ��0-�ɹ�������ʧ��
 **/ 
int SendHttp1(char *pStrUrl, const char *pStrData, int nDataSize);


/**
 * ���ܣ�ͨ��curl��post���ͱ��ĵĻص����������շ���˵ķ���
 **/ 
size_t write_data1(void *pBuff, size_t nSize, size_t nMemb, void *pUserp);