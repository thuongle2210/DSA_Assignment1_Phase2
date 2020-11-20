#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

///
#include <unordered_map>
#include <sstream>
using namespace std;
class BusSystem{
public:
	class Route{
    public:
        string LP;
        int CASE;
        int TIME_A;
        int TIME_B;
        Route* next;
        Route(string lp,short Case,int time_a,int time_b){
            LP=lp;
            CASE=Case;
            TIME_A=time_a;
            TIME_B=time_b;
            next=NULL;
        }
        Route(Route *Cur, Route *Next){
            *this=*Cur;
            if (this!=NULL)
                this->next=next;
        }
    };
    class Node{
    public:
        Route *HEAD;
        string CODE;
        int NumberRoute;
        Node(Route *root,string code){
            HEAD=root;
            CODE=code;
        }
        Node(const Node &x){
            *this=x;
        }
        Node(){
            HEAD=NULL;
            CODE="";
        }
        void Clear(){
            Route *pTrain=HEAD;
            while (HEAD!=NULL){
                HEAD=HEAD->next;
                delete(pTrain);
                pTrain=HEAD;
            }
            pTrain=NULL;
            HEAD=NULL;
        };
    };
private:
	int N;
	unordered_map<string,Node> ListCode;
    string arrInstruction[6];
    int NumberElement;
    bool Case[2];
public:
    ~BusSystem();
	bool checkInterger(string s);
	int StringToInteger(string s);
	bool checkCase(string s);
	string InsertRoute(string NameRoute,Route *tmpRoute);
    string DeleteAllRoute(Node pNode,string CODE);
    string DeleteAnyRoute(Node pNode,string CODE,int Time_begin,int Time_end);
    string NumberRouteForCS(string CODE,int Time,bool Case[]);
    string NumberRouteForCE(string CODE,int Time,bool Case[]);
    string LPForGS(string CODE,int Time,bool Case[]);
    string LPForGE(string CODE,int Time,bool Case[]);
    bool CompareForGE(Route *pA, Route *pB);
    bool CompareForGS(Route *pA, Route *pB);
    bool CheckPreValid(string instruction);
    bool CheckValidForSQ();
    bool CheckValidForINS();
    bool CheckValidForDEL();
    bool CheckValidForSearch();
    void SetCase();
    string query(string instruction);
};
bool BusSystem::CheckPreValid(string instruction){
    if ((instruction[0]==' ') || (instruction[instruction.length()-1]==' ')) return false;
	int count_space=0;
    for (string::iterator it=instruction.begin();it!=instruction.end();it++) 
        if (*it==' ') 
            count_space++;    
    if (count_space>5 && count_space<1)
        return false;
    instruction=instruction+' ';
    for (int i=0;i<6;i++)
        arrInstruction[i]="";
    NumberElement=0;
    do{
        int pos=instruction.find(" ");
        arrInstruction[NumberElement]=instruction.substr(0,pos);
        NumberElement++;
        instruction.erase(0,pos+1);
    }while (instruction!="");
	//check bus no name
	for (int i=0;i<NumberElement;i++)
		if (arrInstruction[i]=="") return false;
    return true;
}
bool BusSystem::checkInterger(string s){
        for (string::iterator it=s.begin();it!=s.end();it++)
            if (!(*it<='9' && *it>='0')) return false;
        return true;
    }
int BusSystem::StringToInteger(string s){
    int kq=0;
    for (string::iterator it=s.begin();it!=s.end();it++){
        kq=kq*10+(*it)-'0';
    }
    return kq;
}
bool BusSystem::checkCase(string s){
    if (s.length()!=1 || (s[0]!='0' && s[0]!='1'))
        return false;
    return true;
}
string BusSystem::InsertRoute(string NameRoute,BusSystem::Route *tmpRoute){
	unordered_map<string,Node>::iterator it=ListCode.find(NameRoute);
	stringstream ss;
	BusSystem::Node tmpNode;
    if (it==ListCode.end()){
        if (N==0) {
            delete tmpRoute;
            tmpRoute=NULL;
            return "-1";
        }
        tmpNode=Node(tmpRoute,NameRoute); 
        tmpNode.NumberRoute=1;
        ListCode.emplace(NameRoute,tmpNode);
        ss<<1;
    }else{
        if (it->second.NumberRoute>=N) {
            delete tmpRoute;
            tmpRoute=NULL;
            return "-1";
        }
		Route *pTrain=it->second.HEAD;
		while (pTrain!=NULL){
			if (pTrain->TIME_A==tmpRoute->TIME_A && pTrain->CASE==tmpRoute->CASE) return "-1";
			if (pTrain->LP==tmpRoute->LP && tmpRoute->TIME_A<=pTrain->TIME_B) 
                return "-1";
			pTrain=pTrain->next;
		}
		tmpRoute->next=it->second.HEAD;
		it->second.HEAD=tmpRoute;
		it->second.NumberRoute++;
		ListCode[NameRoute]=it->second;
		tmpRoute=NULL;
		pTrain=NULL;
		ss<<it->second.NumberRoute;
	}
	return ss.str();
};
string BusSystem::DeleteAllRoute(Node pNode,string CODE){
    BusSystem::Route *pTrain=pNode.HEAD;
    while (pNode.HEAD!=NULL){
        pNode.HEAD=pNode.HEAD->next;
        delete pTrain;
        pTrain=pNode.HEAD;
    }
    pTrain=NULL;
    stringstream ss;
    ss<<pNode.NumberRoute;
    ListCode.erase(CODE);
    return ss.str();
}
string BusSystem::DeleteAnyRoute(Node pNode,string CODE,int Time_begin,int Time_end){
    int k=0;
    Route *pPre=NULL,*pTrain=pNode.HEAD;
    while (pTrain!=NULL){
        if (pTrain->TIME_A>=Time_begin && pTrain->TIME_A<=Time_end){
            if (pPre==NULL){
                pNode.HEAD=pNode.HEAD->next;
                delete pTrain;
                pTrain=pNode.HEAD;
            }else{
                pPre->next=pTrain->next;
                delete pTrain;
                pTrain=pPre->next;
            }
            k++;
        }else{
            pPre=pTrain;
            pTrain=pTrain->next;
        }
    }
    pNode.NumberRoute-=k;
    if (pNode.NumberRoute==0)
        ListCode.erase(CODE);
    else ListCode[CODE]=pNode;
    stringstream ss;
    ss<<k;
    return ss.str();
}
string BusSystem::NumberRouteForCS(string CODE,int Time,bool Case[]){
    unordered_map<string,Node>::iterator it=ListCode.find(CODE);
    if (it==ListCode.end())
        return "0";
    Route *pTrain=it->second.HEAD;
    stringstream ss;
    int Count=0;
    while (pTrain!=NULL){
        if (pTrain->TIME_A<=Time && pTrain->TIME_B>=Time && Case[pTrain->CASE]==true)
            Count++;
        pTrain=pTrain->next;
    }
    //set pTrain=NULL
    pTrain=NULL;
    ss<<Count;
    return ss.str();
};  
string BusSystem::NumberRouteForCE(string CODE,int Time,bool Case[]){
    unordered_map<string,Node>::iterator it=ListCode.find(CODE);
    if (it==ListCode.end())
        return "0";
    Route *pTrain=it->second.HEAD;
    stringstream ss;
    int Count=0;
    while (pTrain!=NULL){
        if (pTrain->TIME_B<Time && Case[pTrain->CASE]==true)
            Count++;
        pTrain=pTrain->next;
    }
    //set pTrain=NULL
    pTrain=NULL;
    ss<<Count;
    return ss.str();
};
bool BusSystem::CompareForGS(Route *pA, Route *pB){
    if (pA->TIME_A>pB->TIME_A) return true;
    if (pA->TIME_A==pB->TIME_A && pA->CASE<=pB->CASE) return true;
    return false;
};
string BusSystem::LPForGS(string CODE,int Time,bool Case[]){
    unordered_map<string,Node>::iterator it=ListCode.find(CODE);
    if (it==ListCode.end()) return "-1";
    Route *RES=NULL,*pTrain=it->second.HEAD;
    while (pTrain!=NULL){
        if (pTrain->TIME_A<=Time && Case[pTrain->CASE]){
            if (RES==NULL){
                RES=pTrain;
            }
            else if (CompareForGS(pTrain,RES)){
                RES=pTrain;
            }
        }
        pTrain=pTrain->next;
    }
    //set pTrain=NULL
    pTrain=NULL;
    if (RES==NULL) return "-1";
    string kq=RES->LP;
    //set RES=NULL
    RES=NULL;
    return kq;
}
bool BusSystem::CompareForGE(Route *pA, Route *pB){
    if (pA->TIME_B>pB->TIME_B) return true;
    if (pA->TIME_B==pB->TIME_B && pA->CASE<pB->CASE) return true;
    if (pA->TIME_B==pB->TIME_B && pA->CASE==pB->CASE && pA->TIME_A>pB->TIME_A) return true;
    return false;
}
string BusSystem::LPForGE(string CODE,int Time,bool Case[]){
    unordered_map<string,Node>::iterator it=ListCode.find(CODE);
    if (it==ListCode.end()) return "-1";
    Route *RES=NULL,*pTrain=it->second.HEAD;
    while (pTrain!=NULL){
        if (pTrain->TIME_B<Time && Case[pTrain->CASE]){
            if (RES==NULL){
                RES=pTrain;
            }else if (BusSystem::CompareForGE(pTrain,RES)){
                RES=pTrain;
            }
        }
        pTrain=pTrain->next;
    }
    //set pTrain=NULL
    if (RES==NULL) return "-1";
    string kq=RES->LP;
    //set RES=NULL
    RES=NULL;
    return kq;
}
bool BusSystem::CheckValidForSQ(){
    if (NumberElement!=2 || checkInterger(arrInstruction[1])==false) return false;
    return true;
}
bool BusSystem::CheckValidForINS(){
    if (NumberElement!=5 && NumberElement!=6) return false;
    if (checkInterger(arrInstruction[NumberElement-1])==false ||
        checkInterger(arrInstruction[NumberElement-2])==false)
    return false;
    if (NumberElement==6 && checkCase(arrInstruction[3])==false) return false;
    if (arrInstruction[1].length()>5 || arrInstruction[2].length()>10) return false;
    return true;
}
bool BusSystem::CheckValidForDEL(){
    if (!(NumberElement==2 || NumberElement==3 || NumberElement==4))
        return false;
    if (NumberElement>=3 && checkInterger(arrInstruction[2])==false)
        return false;
    if (NumberElement>=4 && checkInterger(arrInstruction[3])==false)
        return false;
    if (arrInstruction[1].length()>5) return false;
    return true;
}
bool BusSystem::CheckValidForSearch(){
    if (!(NumberElement==3 || NumberElement==4))
        return false;
    if (checkInterger(arrInstruction[2])==false)
        return false;
    if (NumberElement==4 && checkCase(arrInstruction[3])==false)
        return false;
    if (arrInstruction[1].length()>5) return false;
    return true;
}
void BusSystem::SetCase(){
    Case[0]=false;
    Case[1]=false;
    if (NumberElement==4)
        Case[StringToInteger(arrInstruction[NumberElement-1])]=true;
    else{
        Case[0]=true;
        Case[1]=true;
    }
}
BusSystem::~BusSystem(){
    for (unordered_map<string,Node>::iterator it=ListCode.begin();it!=ListCode.end();it++){
        it->second.Clear();  
    }
    ListCode.clear();
}
string BusSystem::query(string instruction){
    if (CheckPreValid(instruction)==false) return "-1";
	if (arrInstruction[0]=="SQ"){
        if (CheckValidForSQ()==false) return "-1";
        this->N=StringToInteger(arrInstruction[1]);
        return "1";
    }else if (arrInstruction[0]=="INS"){
		if (CheckValidForINS()==false) return "-1";
		BusSystem::Route *tmpRoute;
        //check time
        int time_begin=StringToInteger(arrInstruction[NumberElement-2]);
        int time_end=StringToInteger(arrInstruction[NumberElement-1]);
        if (time_begin>=time_end) return "-1";
        if (NumberElement==5){
            tmpRoute=new BusSystem::Route(arrInstruction[2],0,time_begin,time_end);
        }else{
            tmpRoute=new BusSystem::Route(arrInstruction[2],StringToInteger(arrInstruction[3]),time_begin,time_end);
        }
		return InsertRoute(arrInstruction[1],tmpRoute);
	}else if (arrInstruction[0]=="DEL"){
        if (CheckValidForDEL()==false) return "-1";
        unordered_map<string,Node>::iterator it=ListCode.find(arrInstruction[1]);
        if (it==ListCode.end()) return "0";
        if (NumberElement==2){
            return DeleteAllRoute(it->second,it->first);
        }
        int Time_begin=StringToInteger(arrInstruction[2]);
        int Time_end=Time_begin;
        if (NumberElement==4)
            Time_end=StringToInteger(arrInstruction[3]);
        //check_time
        if (Time_begin>Time_end) return "-1";
        return DeleteAnyRoute(it->second,it->first,Time_begin,Time_end);
    }else if (arrInstruction[0]=="CS"){
        if (CheckValidForSearch()==false) return "-1";
        SetCase();
        return NumberRouteForCS(arrInstruction[1],StringToInteger(arrInstruction[2]),Case);
    }else if (arrInstruction[0]=="CE"){
        if (CheckValidForSearch()==false) return "-1";
        SetCase();
        return NumberRouteForCE(arrInstruction[1],StringToInteger(arrInstruction[2]),Case);
    }else if (arrInstruction[0]=="GS"){
        if (CheckValidForSearch()==false) return "-1";
        SetCase();
        return LPForGS(arrInstruction[1],StringToInteger(arrInstruction[2]),Case);
    }else if (arrInstruction[0]=="GE"){
        if (CheckValidForSearch()==false) return "-1";
        SetCase();
        return LPForGE(arrInstruction[1],StringToInteger(arrInstruction[2]),Case);
    }
	return "-1";
}
int main(){
    BusSystem *bs=new BusSystem();
    cout<<bs->query("SQ 2")<<endl;
    cout<<bs->query("INS 50 ABC 123 456")<<endl;
    cout<<bs->query("CE 50 457");
}