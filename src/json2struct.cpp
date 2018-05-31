#include <string>    
#include <iostream>     // std::cout  
#include <algorithm>    // std::sort  
#include <vector>       // std::vector  
#include "x2struct.hpp" 
#include "json/json.h"   
	
using namespace std;  

struct UniversalItemBean{
	string id;
	string name;
	string value;
	XTOSTRUCT(O(id,name,value));
};

struct UniversalBean{

	string devid;
	string name;
	string value;
	vector<UniversalItemBean> itemList;
	XTOSTRUCT(A(devid, "id"),O(name,value,itemList));
};	



bool UniversalItemBeanSort(UniversalItemBean i,UniversalItemBean j) { return (i.id < j.id);}



int main(int argc, char*argv[]) 
{   
	UniversalBean universalBean;    
	x2struct::X::loadjson("{\"id\":\"1\",\"itemList\":[{\"id\":\"3\",\"name\":\"长度\",\"value\":\"10\"},{\"id\":\"2\",\"name\":\"起点X\",\"value\":\"20\"},{\"id\":\"1\",\"name\":\"起点Y\",\"value\":\"30\"},{\"id\":\"4\",\"name\":\"终点X\",\"value\":\"40\"},{\"id\":\"5\",\"name\":\"终点Y\",\"value\":\"50\"}],\"name\":\"线\",\"value\":\"red\"}", universalBean, false); // json转结构体     
	cout<<universalBean.devid<<endl;  
	cout<<universalBean.name<<endl; 
	cout<<universalBean.value<<endl; 
	
	
	cout<<universalBean.itemList.size()<<endl;
	std::sort(universalBean.itemList.begin(), universalBean.itemList.end(), UniversalItemBeanSort);
	for (vector<UniversalItemBean>::iterator it = universalBean.itemList.begin(); it != universalBean.itemList.end(); it++){
         cout << "id    : " << it->id << endl;
         // cout << "name  : " << it->name << endl;
         // cout << "value : " << it->value << endl;
		// cout << endl;
    }
	
	string jstr=x2struct::X::toxml(universalBean, "root");//结构体转json字符串    
	cout<<jstr<<endl;    
	
	
	Json::Value send_data;
	send_data["devipaddr"] = "192.168.0.10";
	send_data["devtype"] = "VZ1001";
	string tmp_buf = send_data.toStyledString();
	cout<<"jsonstr : " << tmp_buf<<endl;
			
	
	return 0;    
}  