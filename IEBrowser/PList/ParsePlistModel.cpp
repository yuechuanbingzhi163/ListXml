#include "StdAfx.h"
#include "ParsePlistModel.h"

namespace PListModel
{
	CParsePlistModel::CParsePlistModel(void)
	{
	}


	CParsePlistModel::~CParsePlistModel(void)
	{
	}

	//导入解析文件
	bool CParsePlistModel::OnLoadFile(string strPath)
	{
		//得到数据
		Plist::readPlist(strPath, m_Alldict, m_Alldict);

		//	Plist::readPlist(strPath, m_AllArray);

		return true;
	}
	map<string, boost::any> CParsePlistModel::GetRoot()
	{
		return m_Alldict;
	}
	boost::any CParsePlistModel::GetObjectForKey(boost::any dict,string key)
	{
		ObjectType nTypePre = GetObjectType(dict);
		if(nTypePre == PLS_ARRAY)
		{
			vector<boost::any>& plistDict = boost::any_cast<vector<boost::any>& >(dict);
			return plistDict;
		}

		map<string, boost::any>& plistDict = boost::any_cast<map<string, boost::any>&>(dict);

		if(plistDict.find(key) == plistDict.end())
			return NULL;

		boost::any& obj = plistDict.find(key)->second;

		ObjectType nType = GetObjectType(obj);

		switch(nType)
		{
		case PLS_MAP:
			{
				map<string, boost::any>& plistDictSub = boost::any_cast<map<string, boost::any>&>(obj);
				return plistDictSub;
			}
			break;
		case PLS_ARRAY:
			{
				vector<boost::any>& plistArray = boost::any_cast<vector<boost::any>&>(obj);
				return plistArray;
			}
			break;
		case PLS_INT32:
			{
				return (boost::any_cast<const int32_t&>)(obj);
			}
		case PLS_INT64:
			return (boost::any_cast<const int64_t&>)(obj);
			break;
		case PLS_FLOAT:
			return (boost::any_cast<const float&>)(obj);
			break;
		case PLS_DOUBLE:
			return (boost::any_cast<const double&>)(obj);
			break;
		case PLS_LONG:
			return (boost::any_cast<const long&>)(obj);
			break;
		case PLS_SHORT:
			return (boost::any_cast<const short&>)(obj);
			break;
		case PLS_STRING:
			return (boost::any_cast<const string&>)(obj);
			break;
		case PLS_PLISTBOOL:
			return (boost::any_cast<const bool&>)(obj);
			break;
		case PLS_PLISTDATE:
			return (boost::any_cast<const PlistDate&>)(obj);
			break;
		case PLS_BYTEARRAY:
			{
				return boost::any_cast<const vector<char>& >(obj);
			}
			break;
		}

		return NULL;
	}
	//判断 子元素是否存在
	bool CParsePlistModel::IsObjectExist(boost::any dict,string key)
	{
		map<string, boost::any>& plistDict = boost::any_cast<map<string, boost::any>&>(dict);
		if(plistDict.find(key) == plistDict.end())
			return false;
		else
			return true;
	}
	//判断对象类型
	ObjectType CParsePlistModel::GetObjectType(boost::any object)
	{
		const boost::any& obj = object;

		static boost::any aString = string();
		static boost::any aDouble = double(0);
		static boost::any aFloat = float(0);
		static boost::any anInt32 = int32_t(0);
		static boost::any anInt64 = int64_t(0);
		static boost::any aLong = long(0);
		static boost::any aShort = short(0);
		static boost::any anArray = vector<boost::any>();
		static boost::any aMap = map<string, boost::any>();
		static boost::any aByteArray = vector<char>();
		static boost::any aPlistDate = PlistDate();
		static boost::any aPlistBool = bool(true);

		if(obj.type() == anInt32.type())
		{	
			return PLS_INT32;
		}
		else if(obj.type() == anInt64.type())
		{	
			return PLS_INT64;
		}
		else if(obj.type() == aLong.type())
		{	
			return PLS_LONG;
		}
		else if(obj.type() == aShort.type())
		{	
			return PLS_SHORT;
		}
		else if(obj.type() == aMap.type())
		{	
			return PLS_MAP;
		}
		else if(obj.type() == aString.type())
		{	
			return PLS_STRING;
		}
		else if(obj.type() == anArray.type())
		{	
			return PLS_ARRAY;
		}
		else if(obj.type() == aByteArray.type())
		{
			return PLS_BYTEARRAY;
			// 		string dataEncoded;
			// 			base64Encode(dataEncoded, boost::any_cast<const vector<char>& >(obj));
			// 			{	
			// 			}
		}
		else if(obj.type() == aDouble.type())
		{	
			return PLS_DOUBLE;
		}
		else if(obj.type() == aFloat.type())
		{	
			return PLS_FLOAT;
		}
		else if(obj.type() == aPlistDate.type())
		{	
			return PLS_PLISTDATE;
		}
		else if(obj.type() == aPlistBool.type())
		{
			return PLS_PLISTBOOL;
		}
		return UNKNOW;
	}
}	/// end namespace PListModel