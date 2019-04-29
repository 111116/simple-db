#include <map>
#include <string>

#include "client.h"

void create(std::string dbName){
	dblist[dbName]=new Database;
}

void drop(std::string dbName){
	delete t.second;
	dblist.erase(t);
}

void use(std::string dbName){
	selected=dblist[dbName];
}

void show(){
	cout << "Database" << endl;
	for (auto t:dblist)
		cout << t.first << endl;
}

std::string read(){
	std::string str;
	if (str.back()==';') str.pop_back();
	return str;
}

int main(){
	while (true){
		auto str1=read(),str2=read();
		if (str1=="CREATE"	&&str2=="DATABASE") create(read());
		if (str1=="DROP"	&&str2=="DATABASE") drop(read());
		if (str1=="USE"		&&str2=="DATABASE") use(read());
		if (str1=="SHOW"	&&str2=="DATABASE") show();
		
		if (str1=="CREATE"	&&str2=="TABLE"){
			auto str=read();
			auto t=str.find('(');
			auto tableName=str.substr(0,t);
			auto traits=str.substr(t+1,str.size())-t-2);
			selected->create(tableName,traits);
		}
		if (str1=="DROP"	&&str2=="TABLE") selected->drop(read());
		if (str1=="SHOW"	&&str2=="columns") selected->show(read());
		
		if (str1=="INSERT"	&&str2=="INTO"){
			auto str=read();
			auto t=str.find('(');
			auto tableName=str.substr(0,t);
			auto attrlist=str.substr(t+1,str.size()-t+1);
			str=read();
			auto datalist=str.substr(1,str.size()-2);
			auto tmp=selected->table[tableName]->buildEntry(attrlist,datalist);
			selected->table[tableName]->insert(tmp);
		}
		if (str1=="DELETE"	&&str2=="FROM"){
			auto tableName=read();read();
			std::string str;getline(cin,str);str.pop_back();
			auto tmp=selected->table[tableName]->buildCond(str);
			selected->table[tableName]->remove(tmp);
		}
		if (str1=="UPDATE"){
			
		}
		if (str1=="SELECT"){
			
		}
	}
}
