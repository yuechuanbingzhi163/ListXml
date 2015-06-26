#pragma once


#include <iostream>
#include <fstream>
#include <iterator>
#include "Plist.hpp"
using namespace std;

// The PListModel namespace
namespace PListModel
{
	//��������
	enum ObjectType
	{
		UNKNOW,
		PLS_STRING,
		PLS_DOUBLE,
		PLS_FLOAT,
		PLS_INT32,
		PLS_INT64,
		PLS_LONG,
		PLS_SHORT,
		PLS_ARRAY,
		PLS_MAP,
		PLS_BYTEARRAY,
		PLS_PLISTDATE,
		PLS_PLISTBOOL

	};

	//����ͨ����������
	// #define vector<boost::any>  Data_Array
	// #define boost::any				 Data_Any


	//����PList ���ݣ��õ�һ������
	class CParsePlistModel
	{
	public:
		CParsePlistModel(void);
		~CParsePlistModel(void);

		//��������ļ�
		bool OnLoadFile(string strPath);

		//��ȡ���ֵ��¼���е���Ԫ��
		boost::any GetObjectForKey(boost::any dict,string key); 

		//�ж� ��Ԫ���Ƿ����
		bool IsObjectExist(boost::any dict,string key);


		//�ж϶�������
		ObjectType GetObjectType(boost::any object);



		//��ȡ��Ŀ¼�ֵ�
		map<string, boost::any> GetRoot();
	private:
		//��ȡdict
		map<string, boost::any> m_Alldict; 

		//��ȡArray
		vector<boost::any> m_AllArray;

	};
} /// end namespace PListModel

