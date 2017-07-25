#include "HttpComm.h"

#include "../Curl/curl.h"
#include "../Json/json.h"
#include "../utils.h"
#include "../MyTTrace.h"


/**
 * ���ܣ�ͨ��curl��post���ͱ���
 * ��Σ�pStrUrl�����͵�url��ַ
 *		pStrData�����͵�����
 *		nDataSize���������ݳ���
 * ����ֵ��0-�ɹ�������ʧ��
 **/ 
int SendHttp1(char *pStrUrl, const char *pStrData, int nDataSize)
{
	if (pStrUrl == NULL || pStrData == NULL || nDataSize < 0)
	{
		return -1000;
	}
	BOOL ret = true;
	// url ��ӡ����־��
	GtWriteTrace(30, "[%s][%d]: ���͵�ַ��[%s]", __FUNCTION__, __LINE__, pStrUrl);

	// post��ʽ����
	curl_global_init(CURL_GLOBAL_ALL);
	CURL *conn = curl_easy_init();
	curl_slist *plist = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8");
	// ��ʱʱ��
	curl_easy_setopt(conn, CURLOPT_TIMEOUT, 5);
	// url��ַ
	curl_easy_setopt(conn, CURLOPT_URL, pStrUrl);
	// ����˷��ر��ĵĻص�����
	curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, &write_data1);
	// ����http����ͷ
	curl_easy_setopt(conn, CURLOPT_HTTPHEADER, plist);
	// ���͵�����
	curl_easy_setopt(conn, CURLOPT_POSTFIELDS, pStrData);
	// �������ݵĳ���
	curl_easy_setopt(conn, CURLOPT_POSTFIELDSIZE, nDataSize);
	// ����post���ͷ�ʽ
	curl_easy_setopt(conn, CURLOPT_POST, true);

	CURLcode code = curl_easy_perform(conn);
	if(code != CURLE_OK)
	{ 
		GtWriteTrace(30, "[%s][%d]: ����ʧ�ܣ�code=[%d]��", __FUNCTION__, __LINE__, code);
	}
	else
	{
		GtWriteTrace(30, "[%s][%d]: ���ͳɹ���", __FUNCTION__, __LINE__);
	}
	// ������Դ
	curl_easy_cleanup(conn);

	return code;
}

/**
 * ���ܣ�ͨ��curl��post���ͱ��ĵĻص����������շ���˵ķ���
 **/ 
size_t write_data1(void *pBuff, size_t nSize, size_t nmemb, void *pUserp)
{
	std::string* str = dynamic_cast<std::string*>((std::string *)pUserp);
	if( NULL == str || NULL == pBuff )
	{
		return -1;
	}
	GtWriteTrace(30, "[%s][%d]: �ӵ������ݣ�UTF-8��size=[%d]��", __FUNCTION__, __LINE__, nSize * nmemb);
	GtWriteTrace(30, "[%s][%d]: �ӵ������ݣ�UTF-8��buff=[%s]��", __FUNCTION__, __LINE__, pBuff);

	// printf("pBuff%s\n",pBuff);
	//sprintf((char *)pBuff, "{\"code\":\"0\", \"msg\":\"�ɹ�\", \"url\":\"http://www.baidu.com\"}");//�����ַ���
	//GtWriteTrace(30, "[ScreenDlg]Receive pBuff = [%s], nSize = [%d]", pBuff, strlen((char *)pBuff));

	// utf-8תgbk
	CString utf8String = (char *)pBuff;
	ConvertUtf8ToGBK(utf8String);
	GtWriteTrace(30, "[%s][%d]: �ӵ������ݣ�gbk��size=[%d]��", __FUNCTION__, __LINE__, utf8String.GetLength());
	GtWriteTrace(30, "[%s][%d]: �ӵ������ݣ�gbk��buff=[%s]��", __FUNCTION__, __LINE__, utf8String.GetBuffer());
	utf8String.ReleaseBuffer();

	string recvBuff = (char *)pBuff;
	// ��������˷��ص�json�������ݣ���ȡ��������
	//json����
	Json::Reader reader;
	//��ʾһ��json��ʽ�Ķ���
	Json::Value value;
	// ��ȡ������Ϣ
	string code;
	string msg;
	string url;
	//����json���ģ��浽value��
	if(reader.parse(recvBuff, value))
	{
		// ��ȡ������
		code = value["code"].asString();
		if (code != "")
		{
			// ��ȡ������Ϣ
			msg = value["msg"].asString();
			// ��ȡurl����ʧ����Ϣ
			url = value["url"].asString();
			//if (code == "0")
			if (code.compare("0") == 0)
			{
				// �����ɹ�����ȡ��һ��"|"ǰ����ַ
				int ret;
				char strUrl[512] = {0};
				ret = splitString(strUrl, url.c_str(), 0);
				if (ret != 0)
				{
					// ��ȡurl��ַʧ��
					GtWriteTrace(30, "[ScreenDlg]splitString() failed! buff=[%s], ret=[%d]!", url.c_str(), ret);
				}
				else 
				{
					GtWriteTrace(30, "[ScreenDlg]splitString() succeed! url=[%s]!", strUrl);
				}
			}
			else if(code.compare("1") == 0)
			{
				// ��ά��ͼƬʶ��ʧ��
				GtWriteTrace(30, "[ScreenDlg]Ret=[%s], msg=[%s], Server recognizing images failed! Will send message to MainFrm!", code.c_str(), msg.c_str());
			}else if(code.compare("2") == 0)
			{
				// ��ά���Ų�����
				GtWriteTrace(30, "[ScreenDlg]Ret=[%s], msg=[%s], QR code number is not exist! Will send message to MainFrm!", code.c_str(), msg.c_str());
			}
			else
			{
				// ������������
				GtWriteTrace(30, "[ScreenDlg]Recvive illegal code[%d]!", code.c_str());
				//::MessageBoxA(NULL, "web�˷���δ���彻�����ͣ�", "��ʾ", MB_OK);
			}
		}

		string type = value["TYPE"].asString();
		string ret_code = value["RET_CODE"].asString();
		if (type != "")
		{
			CString msg;
			msg.Format("web�˷��أ�%s", ret_code.c_str());
			::MessageBoxA(NULL, msg, "��ʾ", MB_OK);
		}
	}
	else
	{
		// json����ʧ��
		GtWriteTrace(30, "[ScreenDlg]Read json message failed!");
		::MessageBoxA(NULL, "����json����ʧ�ܣ�", "��ʾ", MB_OK);
	}

	return nmemb;
}
