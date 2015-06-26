#pragma once


#include <iostream>
#include <fstream>
#include <iterator>
#include "Plist.hpp"
using namespace std;

// The PListModel namespace
namespace PListModel
{
	//数据类型
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

	//定义通用数据类型
	// #define vector<boost::any>  Data_Array
	// #define boost::any				 Data_Any


	//解析PList 数据，得到一个链表
	class CParsePlistModel
	{
	public:
		CParsePlistModel(void);
		~CParsePlistModel(void);

		//导入解析文件
		bool OnLoadFile(string strPath);

		//获取“字典记录”中的子元素
		boost::any GetObjectForKey(boost::any dict,string key); 

		//判断 子元素是否存在
		bool IsObjectExist(boost::any dict,string key);


		//判断对象类型
		ObjectType GetObjectType(boost::any object);



		//获取根目录字典
		map<string, boost::any> GetRoot();
	private:
		//读取dict
		map<string, boost::any> m_Alldict; 

		//读取Array
		vector<boost::any> m_AllArray;

	};
} /// end namespace PListModel

