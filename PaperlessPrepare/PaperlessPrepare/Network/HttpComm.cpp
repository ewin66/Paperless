#include "HttpComm.h"

#include "../Curl/curl.h"
#include "../utils.h"
#include "../MyTTrace.h"
#include "../resource.h"
#include "../PaperlessPrepareDlg.h"
#include "../CBase64.h"

/**
 * ���ܣ�ͨ��curl��post���ͱ���
 * ���룺pStrUrl�����͵�url��ַ
 *		pStrData�����͵�����
 *		nDataSize���������ݳ���
 *		nSendRet:ʧ��ԭ���ַ����ɴ�Ŵ�С
 * ���������ֵ��0-�ɹ�������ʧ��
 *		pSendRet��ʧ��ԭ��
 **/ 
int SendData(const char *pStrUrl, const char *pStrData, int nDataSize, int nSendRet, char *pSendRet)
{
	if (pStrUrl == NULL || pStrData == NULL || nDataSize < 0 || pSendRet == NULL || nSendRet <= 0)
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
	curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, &RecvData);
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
		GtWriteTrace(30, "[%s][%d]: ����ʧ�ܣ�code=[%d] msg=[%s]��", __FUNCTION__, __LINE__, code, curl_easy_strerror(code));
		memcpy(pSendRet, curl_easy_strerror(code), nSendRet - 1);
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
size_t RecvData(void *pBuff, size_t nSize, size_t nmemb, void *pUserp)
{
	std::string* str = dynamic_cast<std::string*>((std::string *)pUserp);
	if( NULL == str || NULL == pBuff )
	{
		return -1;
	}
	GtWriteTrace(30, "[%s][%d]: �ӵ������ݣ�UTF-8��size=[%d]��", __FUNCTION__, __LINE__, nSize * nmemb);
	GtWriteTrace(30, "[%s][%d]: �ӵ������ݣ�UTF-8��buff=[%s]��", __FUNCTION__, __LINE__, pBuff);

	// printf("pBuff%s\n",pBuff);
	//GtWriteTrace(30, "[ScreenDlg]Receive pBuff = [%s], nSize = [%d]", pBuff, strlen((char *)pBuff));

	// utf-8תgbk
	CString utf8String = (char *)pBuff;
	ConvertUtf8ToGBK(utf8String);
	GtWriteTrace(30, "[%s][%d]: �ӵ������ݣ�gbk��size=[%d]��", __FUNCTION__, __LINE__, utf8String.GetLength());
	GtWriteTrace(30, "[%s][%d]: �ӵ������ݣ�gbk��buff=[%s]��", __FUNCTION__, __LINE__, utf8String.GetBuffer());
	utf8String.ReleaseBuffer();
	//sprintf((char *)pBuff, "{\"type\":\"2\", \"code\":\"2\", \"msg\":\"�ɹ�\", \"filename\":\"XXbgService.exe\", \"version\":\"1.1\", \"application\":\"eyJCV0taTFgiOiI0IiwgIkpZRE0iOiIxMDEwMTAifQ==\"}");//�����ַ���
	sprintf((char *)pBuff, "{\"type\":\"2\", \"code\":\"0\", \"msg\":\"�ɹ�\", \"filename\":\"XXbgService.exe\", \"version\":\"1.1\", \"application\":\"eyJCV0taTFgiOiI0IiwgIkpZRE0iOiIxMDEwMTAifQ==\"}");//�����ַ���

	string recvBuff = (char *)pBuff;
	// ��������˷��ص�json�������ݣ���ȡ��������
	//json����
	Json::Reader reader;
	//��ʾһ��json��ʽ�Ķ���
	Json::Value value;
	// ��ȡ������Ϣ
	string type;
	//����json���ģ��浽value��
	if(reader.parse(recvBuff, value))
	{
		// ��ȡ��������
		if (value.size() != 0)
		{
			type = value["type"].asString();
			if (type == "2")
			{
				// Ӧ�ó�����µķ��ر��Ĵ���
				DealUpdateRet(value);
			}
			else
			{
				GtWriteTrace(30, "[%s][%d]����������δ����Ľ������ͣ�type=[%s]!", __FUNCTION__, __LINE__, type);
				//::MessageBoxA(NULL, "����������δ����Ľ������ͣ�", "��ʾ", MB_OK);
				((CPaperlessPrepareDlg *)(AfxGetApp()->m_pMainWnd))->MyRetryWin("�������������");
			}
		}
		else
		{
			// json����ʧ��
			GtWriteTrace(30, "[%s][%d]���������ط�json��ʽ����,��������json�ṹ��size=0!", __FUNCTION__, __LINE__);
			//::MessageBoxA(NULL, "���������ط�json��ʽ����!��", "��ʾ", MB_OK);
			((CPaperlessPrepareDlg *)(AfxGetApp()->m_pMainWnd))->MyRetryWin("�����������쳣��");
		}
		
	}
	else
	{
		// json����ʧ��
		GtWriteTrace(30, "[%s][%d]���������ط�json��ʽ����!", __FUNCTION__, __LINE__);
		//::MessageBoxA(NULL, "���������ط�json��ʽ����!��", "��ʾ", MB_OK);
		((CPaperlessPrepareDlg *)(AfxGetApp()->m_pMainWnd))->MyRetryWin("�����������쳣2��");
	}

	return nmemb;
}

// ����Ӧ�ó�����µķ���
int DealUpdateRet(Json::Value &sDataValue)
{
	string code;
	string msg;
	string filename;
	string version;
	string application;

	// ��ȡ������
	code = sDataValue["code"].asString();
	if ("0" == code)
	{
		// �����������°汾
		GtWriteTrace(30, "[%s][%d]���������ظ��°汾!", __FUNCTION__, __LINE__);
		filename = sDataValue["filename"].asString();
		version = sDataValue["version"].asString();
		application = sDataValue["application"].asString();

		char sSrcFile[256] = {0};
		char sDestFile[256] = {0};
		sprintf_s(sSrcFile, sizeof(sSrcFile)-1, "%s/%s", GetFilePath().GetBuffer(), filename.c_str());
		sprintf_s(sDestFile, sizeof(sDestFile)-1, "%s/%s.loadtmp", GetFilePath().GetBuffer(), filename.c_str());
		GtWriteTrace(30, "[%s][%d]Դ�ļ�=[%s]��Ŀ���ļ�=[%s]!", __FUNCTION__, __LINE__, sSrcFile, sDestFile);

		// Դ�ļ����ݣ����Ǽ���
		MoveFileEx(sDestFile, sSrcFile, MOVEFILE_REPLACE_EXISTING);

		// ���ļ�����base64���룬��д�뵽�ļ���
		int nRet = 0;
		ZBase64 zBase;
		int nBase64AfterLens = 0;
		string sBase64After = zBase.Decode(application.c_str(), application.length(), nBase64AfterLens);

		GtWriteTrace(30, "[%s][%d]��д�� len=[%d]!", __FUNCTION__, __LINE__, nBase64AfterLens);

		FILE *fp = NULL;
		fp = fopen(sSrcFile, "wb");
		if (fp != NULL)
		{
			GtWriteTrace(30, "[%s][%d]���°汾���浽����!", __FUNCTION__, __LINE__);
			nRet = fwrite(sBase64After.c_str(), 1, nBase64AfterLens, fp);
			fclose(fp);
			if (nRet != nBase64AfterLens)
			{
				// д�ļ�ʧ�ܣ�Դ�ļ����¿�������
				MoveFileEx(sSrcFile, sDestFile, MOVEFILE_REPLACE_EXISTING);
				((CPaperlessPrepareDlg *)(AfxGetApp()->m_pMainWnd))->MyRetryWin("�������°汾����ʧ�ܣ�");
				return -1;
			}
			// Ӧ�ó�����³ɹ����汾��д�������ļ���
			WritePrivateProfileString("Information", "Version", version.c_str(), GetFilePath()+"\\win.ini");
		}
		// ���������򣬳ɹ������󽫹رձ�����
		StartPaperless();
	}
	else if ("1" == code)
	{
		// ��ǰ�ļ������°汾һ��
		GtWriteTrace(30, "[%s][%d]���°汾�͵�ǰ�汾һ��!", __FUNCTION__, __LINE__);
		// ���������򣬳ɹ������󽫹رձ�����
		StartPaperless();
	}
	else
	{
		GtWriteTrace(30, "[%s][%d]����������ʧ��:%s", __FUNCTION__, __LINE__, msg.c_str());
		// ����������ʧ��
		msg = sDataValue["msg"].asString();
		char tmp[128] = {0};
		sprintf_s(tmp, sizeof(tmp)-1, "����������ʧ��:%s", msg.c_str());
		((CPaperlessPrepareDlg *)(AfxGetApp()->m_pMainWnd))->MyRetryWin(tmp);
	}
	return 0;
}



// ���������򣬳ɹ������󽫹رձ�����
int StartPaperless()
{
	char sAppDir[256] = {0};
	char sRetMsg[128] = {0};

	// ��Ӧ�ó���·��
	GetPrivateProfileString("Information", "AppDir", "../Paperless.exe", sAppDir, sizeof(sAppDir)-1, GetFilePath()+"\\win.ini");
	GtWriteTrace(30, "[%s][%d]��������=[%s]!", __FUNCTION__, __LINE__, sAppDir);
	// ����������
	LPCSTR lpcsDir = _T(sAppDir);
	ShellExecute(NULL, "open", sAppDir, NULL, NULL, SW_SHOWNORMAL);
	// �رձ�����
	((CPaperlessPrepareDlg *)(AfxGetApp()->m_pMainWnd))->OnBnClickedCancel();
	return 0;
}