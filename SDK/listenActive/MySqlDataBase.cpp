// MySqlDataBase.cpp: implementation of the CMySqlDataBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MySqlDataBase.h"

#include <stdio.h>
//#include <WinSock.h>  //һ��Ҫ�������������winsock2.h
#include<WinSock2.h>
#include "mysql.h"    //����mysqlͷ�ļ�(һ�ַ�ʽ����vcĿ¼�������ã�һ�����ļ��п�������Ŀ¼��Ȼ����������)
#include <Windows.h>

//�������������Ҳ�����ڹ���--������������
#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"./lib/libmysql.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySqlDataBase::CMySqlDataBase()
{

}

CMySqlDataBase::~CMySqlDataBase()
{

}

MYSQL mysql;

bool CMySqlDataBase::connectDB()
{
	/*
	CString m_Dsn;
		//�������ݿ�
		try{
			m_Dsn.Format("DSN=%s","WEST100V2");
			m_db.OpenEx(m_Dsn,CDatabase::openReadOnly|CDatabase::noOdbcDialog);
		}catch (CDBException *ex) {
			ex->Delete();
			return false;
		}
		m_db.SetQueryTimeout(30);
		m_db.SetLoginTimeout(3);
		return TRUE;*/
	CoInitialize(NULL);
	//�����������ݿ��ַ���
	//strConn.Format("Provider=MSDAORA.1;Password=%s;User ID=%s;Data Source=%s;Persist Security Info=True","windstorm","west100","WEST100V2");
	//strConn.Format("DRIVER={MySQL ODBC 3.51 Driver};SERVER=61.178.85.43;PORT=3306;DATABASE=west100v2; USER=west100;PASSWORD=windstorm;OPTION=3");
	/*try{
		//����ʵ��
		m_db.CreateInstance("ADODB.Connection");
		//HRESULT hr=m_db->Open((_bstr_t)connStr,"","",adConnectUnspecified); 
		HRESULT hr=m_db->Open((_bstr_t)connStr,"root","1",adConnectUnspecified);
		
		if (FAILED( hr)){
            return false;
        }
		return true;
	}catch(_com_error e){
		PrintComError(e);
		return false;		
	}*/

	try
	{
		//��ʼ��mysql
		mysql_init(&mysql);  //����mysql�����ݿ�

		//����false������ʧ�ܣ�����true�����ӳɹ�
		if (!(mysql_real_connect(&mysql, "localhost", "root", "1", "mwusers", 3308, NULL, 0))) //�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ
		{
			//AfxMessageBox("mysql_real_connect!");
			//printf("Error connecting to database:%s\n", mysql_error(&mysql));
			return false;
		}
		else
		{
			mysql_close(&mysql);

			//AfxMessageBox("mysql_real_connect success!");
		//	AfxGetMainWnd()->PostMessage(WM_CLOSE);
			//printf("Connected...\n");
			return true;
		}
	}
	catch (...)
	{
		return false;
	}
	
}

bool CMySqlDataBase::close()
{
	try{
		if(m_db!=NULL)
			m_db->Close();
		return true;
	}catch (CException *ex) {
		ex->Delete();
		return false;
	}
}

CStringArray* CMySqlDataBase::excuteQuery(CString strSql,long *lngRowCount)
{
	/*
	CStringArray* m_result=NULL;
	CRecordset m_pset(&m_db);
	try{
		m_pset.Open(CRecordset::forwardOnly,LPCSTR(strSql),CRecordset::readOnly);
		
		long longCount = m_pset.GetRecordCount();
		if(longCount==0){
			//��¼Ϊ��
			m_pset.Close();
			*lngRowCount=-1;
			return NULL;
		}
		*lngRowCount = longCount;
		m_result = new CStringArray[longCount];
		int intCol = m_pset.GetODBCFieldCount();
		int i =0;
		CString strColValue;
		m_pset.Move(-1);
		while(!m_pset.IsEOF())
		{
			for(int j=0;j<intCol;j++){
				m_pset.GetFieldValue(j,strColValue);
				m_result[i].Add(strColValue);
			}	
			i++;
			m_pset.MoveNext();
		}
		m_pset.Close();
		return m_result;
	}catch (CException *ex) {
		*lngRowCount=-1;
		ex->Delete();
		m_pset.Close();
		return NULL;
	}
	*/
	_RecordsetPtr   pRstAuthors   = NULL;//��¼��
	try{
		pRstAuthors.CreateInstance(__uuidof(Recordset));
		pRstAuthors->CursorLocation=adUseClient;
		pRstAuthors->Open((_variant_t)strSql, _variant_t((IDispatch*)m_db,true),adOpenKeyset, adLockReadOnly, adCmdText);
		try{
			_variant_t column;
			long logColNum=0;
			long i=0;
			logColNum   = pRstAuthors->GetFields()->Count;
			*lngRowCount = pRstAuthors->RecordCount;
			if(*lngRowCount==0){
				//��¼������
				pRstAuthors->Close();
				return NULL;
			}
			CStringArray *m_result = new CStringArray[*lngRowCount];
			while(!pRstAuthors->adoEOF){
				for (long j = 0; j < logColNum; j++){
					try{
						column = pRstAuthors->GetFields()->Item[j]->Value;
					}catch (_com_error &e0) {
						CString strTemp = m_result[i].GetAt(0);
						m_result[i].Add("NULL");
						PrintComError(e0);
						continue;
					}
					
					if(column.vt != VT_NULL){
						m_result[i].Add((char*)_bstr_t(column));
					}else{
						m_result[i].Add("");
					}	
				}
				i+=1;
				pRstAuthors->MoveNext();
			}
			return m_result;
		}catch(_com_error &e){
			PrintComError(e);
			*lngRowCount=-1;
			pRstAuthors->Close();
			return NULL;
		}
	}catch(_com_error &e){
		PrintComError(e);
		if(e.Error()==-2147467259)
			*lngRowCount=-1;
		//MessageBox("ִ��sql����!");
		return NULL;
	}
}

int CMySqlDataBase::excuteSql(CString strSql)
{
	CDatabase m_db;
	CString m_Dsn;
	//�������ݿ�
	m_Dsn.Format("DSN=%s;",this->connStr);
	try
	{
		m_db.OpenEx(m_Dsn,CDatabase::openReadOnly|CDatabase::noOdbcDialog);
		
	}
	catch(CDBException *e)
	{
		e->Delete();
		//�����ݿ����
		return -1;
	}
	try
	{
		m_db.ExecuteSQL(strSql);
		m_db.Close();
	}
	catch(CDBException *e)
	{
		e->Delete();
		m_db.Close();
		//�����ݿ����
		return -1;
	}
	return 1;
}

void CMySqlDataBase::PrintComError(_com_error &e)
{
	_bstr_t bstrSource(e.Source());
    _bstr_t bstrDescription(e.Description());
    
    // Print Com errors.
    printf("Error\n");
    printf("\tCode = %08lx\n", e.Error());
    printf("\tCode meaning = %s\n", e.ErrorMessage());
	printf("\tSource = %s\n", (LPCSTR) bstrSource);
	printf("\tDescription = %s\n", (LPCSTR) bstrDescription);
}

void CMySqlDataBase::setDbName(CString strCon)
{
	this->connStr = strCon;
}
