#ifndef ESP_IOTOOLS_H

#define ESP_IOTOOLS_H "ESP_IOTOOLS_HEADER_FILE"

// #include <fstream>
// #include <string>

// #include <sys/stat.h>
//#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <algorithm> //2023-06-24 15:32:37 - Needed to call std::find()
#include <string.h>
#include <vector>
#include <map>
// #include <deque>

using namespace std;

#include <unistd.h> //unlink

// color for konsole, use: printf("%sRED\n",KRED);
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define KGreen KGRN
/*
 ATTENTION,WARNING:
 La couleur ne s'affichera pas dans la console intégrée de KDevelop !
 */
//////------------------------ Console, Terminal functions
void printErr(string greenText)
{
  string green="\x1B[31m", white="\x1B[37m";
  printf("%s%s%s",green.c_str(),greenText.c_str(),white.c_str());
}

void printCool(string greenText)
{
  string green="\x1B[32m", white="\x1B[37m";
  printf("%s%s%s",green.c_str(),greenText.c_str(),white.c_str());
}

void printlnErr(string greenText)
{
  string green="\x1B[31m", white="\x1B[37m";
  printf("%s%s%s\n",green.c_str(),greenText.c_str(),white.c_str());
}
void printErrLn(string redText)
{
  printlnErr(redText);
}

void println(string greenText, string color=KNRM)
{
  string green=color, white=KWHT;
  printf("%s%s%s\n",green.c_str(),greenText.c_str(),white.c_str());
}

void printLn(string greenText, string color)
{
  string green=color, white=KWHT;
  printf("%s%s%s\n",green.c_str(),greenText.c_str(),white.c_str());
}

void printlnCool(string greenText)
{
  string green="\x1B[32m", white="\x1B[37m";
  printf("%s%s%s\n",green.c_str(),greenText.c_str(),white.c_str());
}
void printCoolLn(string greenText)
{
  string green="\x1B[32m", white="\x1B[37m";
  printf("%s%s%s\n",green.c_str(),greenText.c_str(),white.c_str());
}

void print(string whiteText)
{
  string green="\x1B[37m", white="\x1B[0m";
  printf("%s%s%s",green.c_str(),whiteText.c_str(),white.c_str());
}

void printLn(string whiteText)
{
  string green="\x1B[37m", white="\x1B[0m";
  printf("%s%s%s\n",green.c_str(),whiteText.c_str(),white.c_str());
}
//convert string to double
double strToDouble(string str)
{
	return strtod(str.c_str(),NULL);
}

long double strToLongDouble(string str)
{
	return stold(str.c_str(),NULL);
}

// cout<<strToFloat("9.43");
float strToFloat(string str)
{
	return (float) strToDouble(str);
}

int strToInt(string str)
{
   return atoi(str.c_str());
}

long strToLong(string str)
{
   return atol(str.c_str());
}

long long strToLongLong(string str)
{
   return atoll(str.c_str());
}

//convert hexadecimal to integer
int strToHex(string str)
{
    int x;
    std::stringstream ss;
    ss << std::hex << str;
    ss >> x;
    return x;
}

//convert string to int
bool strToInt(string str, int *i, int base=10)
{
 char *end;
 long lnum = strtol(str.c_str(), &end,base);
 *i=lnum;
 bool success=false;
 if(strlen(end)==0)success = true;

 return success;
}

string intToStr(int n)
{
    std::ostringstream oss;
    oss << n;
    return oss.str();
}

string floatToStr(float f,  int precision = 18)
{
    std::ostringstream oss;
    oss.precision(precision);
    oss << f;
    return oss.str();
}


#ifndef boolToStr

string boolToStr(bool val)
{
  if(val) return "true"; else return "false";
}

#endif


string longToStr(long long l)
{
  std::ostringstream oss;
    oss << l;
    return oss.str();
}

string intToStr(long l)
{
   std::ostringstream oss;
    oss << l;
    return oss.str();
}

string doubleToStr(double n)
{
    std::ostringstream oss;
    oss << n;
    return oss.str();
}
/*
 #include <sstream>

template<typename T>
std::string toString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
 */


//HERE renvoie le numéro de ligne dans le programme
//#define HERE (std::string(__FILE__) + "(" + itos(__LINE__) + "): ")

//détect si une chaine débute avec une autre.
bool strStartsWith(string str, string pattern)
{
	bool result= false;
	if( str.length()>=pattern.length())
	{
		if(! str.substr(0,pattern.length()).compare(pattern) )
		{
			result = true;
		}
	}
	return result;
}


//to lowerCase
string strToLower(string s)
{
	string result="";
	for(int i=0; i<(int)s.length(); i++)
	{
		result+=tolower( s.at(i) );
	}
	return result;
}

string stdStrToLower(string str)
{
  string output="";
  std::locale loc;
  for (std::string::size_type i=0; i<str.length(); ++i)
    output+=std::tolower(str[i],loc);
  return output;
}


bool startsWith(string str, string pattern)
{
	return strStartsWith(str,pattern);
}


string lastLeftStr(string line,string pattern, bool includePattern=false)
{
	int l=0,r,len;
	if(line.empty())return "";
	string str;
	r = line.length()+line.find_last_of(pattern,l);
	len=pattern.length();
	if(includePattern)
	{
		str=line.substr(0,r+len);
	}else
	{
		str=line.substr(0,r);
	}
	return str;
}


string leftStr(string line,string pattern, bool includePattern=false)
{
	int l=0,r,len;
	if(line.empty())return "";
	string str;
	r=line.find(pattern,l);
	len=pattern.length();
	if(includePattern)
	{
		str=line.substr(0,r+len);
	}else
	{
		str=line.substr(0,r);
	}
	return str;
}

string rightStr(string line,string pattern, bool includePattern=false)
{
	int l,len;
	if(line.empty())return "";
	string str;
	l=line.find(pattern);
	len=pattern.length();

	if(includePattern)
	{
		str=line.substr(l);
	}else
	{
		str=line.substr(l+len);
	}
	return str;
}


//does a string end with ....?
bool endsWith(string s,string pattern)
{
	int plen=pattern.length();
	int slen=s.length();
	if( (plen>slen)|| pattern.empty() )return false;
	string str=s.substr(slen-plen);
	if(!str.compare(pattern)) return true; else return false;
}



//si une chaine est présente dans une autre
bool inStr(string line, string toFind, bool ignoreCase=false)
{
	if(ignoreCase)
	{
		line=strToLower(line);
		toFind=strToLower(toFind);
	}
	if(line.find(toFind)!=-1)return true;
	else return false;
}



void trimLeft(std::string& str, const char* chars2remove=" \r\n")
{
	if (!str.empty())
	{
	std::string::size_type pos = str.find_first_not_of(chars2remove);

	if (pos != std::string::npos)
	str.erase(0,pos);
	else
	str.erase( str.begin() , str.end() );
	}
}

void trimRight(std::string& str, const char* chars2remove=" \r\n")
{
	if (!str.empty())
	{
	std::string::size_type pos = str.find_last_not_of(chars2remove);

	if (pos != std::string::npos)
	str.erase(pos+1);
	else
	str.erase( str.begin() , str.end() ); // make empty
	}
}

void trim(string &str,const char* chars2remove=" \r\n")
{
	trimLeft(str,chars2remove);
	trimRight(str,chars2remove);
}

string trimStr(string line)
{
     trim(line);
	return line;
}

///verified:
//  string innerBrack = between_last( trimStr( " !{int:20}" ),"!{","}");
//    cout<<"["<<innerBrack<<"]"<<endl; //[int:20]
string between_last2(string line, string leftPattern, string rightPattern, bool includePattern=false)
{
	string r,str="";
	if( ( (int)line.find(leftPattern) ) <0 ){ return str; }
	if( ( (int)line.find(rightPattern) ) <0 ){ return str; }
	//if( line.find(leftPattern) >= line.find(rightPattern)  ){ return str; }  //line = "<w f="10">sous</w>" recherche de "sous"

	if(leftPattern.compare(rightPattern))
	r=rightStr(line,leftPattern,includePattern);
	else
	r=rightStr(line,leftPattern,false);

	if(leftPattern.compare(rightPattern))
    {  //pattern different
        str=lastLeftStr(r,rightPattern,includePattern);
           int rn = str.find_last_of(rightPattern) - rightPattern.length();
           str = str.substr(0,rn+1);
    }
	else
	{
        str=lastLeftStr(r,rightPattern,includePattern);

        int rn = str.find_last_of(rightPattern) - rightPattern.length();
        str = str.substr(0,rn+1);

        string str2=str;
        if(includePattern)
        {   str2=leftPattern;
            str2.append(str);
            str2.append(rightPattern);
        }
        return str2;
	}

	return str;
}//between_last2

//2023-07-14 12:11:38 - cast int or use  if(  line.find(leftPattern) != std::string::npos ){...}
string between_last(string line, string leftPattern, string rightPattern, bool includePattern=false)
{
	string str="";
	if( ((int)line.find(leftPattern) ) <0 ){ return str; }
	if( ((int)line.find(rightPattern) ) <0 ){ return str; }

	int l = line.find(leftPattern)+leftPattern.length();
    int r = line.length() - line.find_last_of(rightPattern) + rightPattern.length()-1;

    int len = line.length() -l - r;
	str = line.substr(l, len );

    if(includePattern)
    {
        str=leftPattern + str + rightPattern;
    }

	return str;
}//between_last

//changé le 13:48 31/03/2009
//non vérifiée voir between_last
string between_manual(string line, string leftPattern, string rightPattern, bool includePattern=false)
{
	string r,str="";
	//2023-07-14 12:11:38 - cast int or use  if(  line.find(leftPattern) != std::string::npos ){...}
	if( ((int)line.find(leftPattern)) <0 ){ return str; }
	if( ((int)line.find(rightPattern)) <0 ){ return str; }
	//if( line.find(leftPattern) >= line.find(rightPattern)  ){ return str; }  //line = "<w f="10">sous</w>" recherche de "sous"

	if(leftPattern.compare(rightPattern))
	r=rightStr(line,leftPattern,includePattern);
	else
	r=rightStr(line,leftPattern,false);

	if(leftPattern.compare(rightPattern))
	str=leftStr(r,rightPattern,includePattern);
	else
	{
	str=leftStr(r,rightPattern,includePattern);
	string str2="";
	if(includePattern) str2=leftPattern;
	str2.append(str);
	return str2;
	}
	return str;
}

//Création 23/03/2018
string between_escape(string line,string left, string right, int &rightPos)
{
    int l, r; string s = "";
    l=line.find(left,0);
    if (l < 0) return s;
    r = line.find(right, l+1);
    while( line.at(r-1)=='\\' )
    {   //find the next ending
        r=line.find(right, r+1);
        if (r < 0) return s;
    }
    rightPos=r; //save right position
    if (r < 0) return s;
    int bigl=l+left.length();
    int bigr = r-l - left.length();

    s=line.substr(bigl, bigr);
    return s;
}//end between_escape




//création 16/08/2017 //messageBox(between("<<<1234567]]fin>","<<<","]]") );return 0;
string between(string line,string left, string right)
        {
            int l, r; string s = "";
            l=line.find(left,0);
            if (l < 0) return s;
            r = line.find(right, l+1);
            if (r < 0) return s;
	    int bigl=l+left.length();
	    int bigr = r-l - left.length();
            s=line.substr(bigl, bigr);
            return s;
        }//endbetween (verified)


//création 11/10/2017 //messageBox(between("<<<1234567]]fin>","<<<","]]") );return 0;
string between(string line,string left, string right, int &posLastRight)
        {
            int l, r; string s = "";
            l=line.find(left,0);
            if (l < 0) return s;
            r = line.find(right, l+1);
	    posLastRight=r;
            if (r < 0) return s;
	    int bigl=l+left.length();
	    int bigr = r-l - left.length();
            s=line.substr(bigl, bigr);
            return s;
        }//endbetween (verified)



//string clean = removeStrBetween("12[*ABasourdi*]4bonjour","[*","*]");
//returns 124bonjour
string removeStrBetween(string line, string left, string right)
{
    //code from function between
            int l, r; string s = "";
            l=line.find(left,0);
            if (l < 0) return s;
            r = line.find(right, l+1);
            if (r < 0) return s;
            int bigl=l+left.length();
            int bigr = r-l - left.length();
            s=line.substr(bigl, bigr);
            //cout<<"r:"<<r<<endl; cout<<"l:"<<l<<endl;            cout<<"length:"<<len<<endl;
            string clean = line.erase(l, r+right.length() - l );
    return clean;
}//removeStrBetween


//string.split
vector<string> splitStr(string str, char sep)
{
    vector<string> strings;
    istringstream f(str);
    string s;
    while (getline(f, s, sep)) {
        //cout << s << endl;
        strings.push_back(s);
    }
	return strings;
}//end split


// return number of occurences replaced, and updates the &whole variable
//   utilisation:
//   string ident = "";
//   string somefile = "/home/boony/dev/c++/myheaders/myfile";
//   string tofind ="/";
//   int occurences =  replaceStr(somefile,"/","(!)");
//   disp(occurences);
//   box(ident+"\nfirstvar:"+somefile);

//2020-07-13 00:18:30 - ReplaceAll fonctionne correctement
string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}
#define replaceString ReplaceAll

void replaceStr(string &whole,  const std::string& from, const std::string& to)
{
 	whole = replaceString(whole, from, to );
}



/*************************   exemple map pour stockage hashtable de long?    ************************************/

//utilisation de map comme hastable avec pointer (exemple pour stocker des long)
// map<string, long long*>timeMap2;
//  // timeMap2["test inexistant"] = new long long;
//   //*(timeMap2["test inexistant"])=132;
//  long long *res =  timeMap2["test inexistant"];
//  if(res == NULL)
//  {
//    box("NULL");
// }else
// {
//   box("ok");
//   disp(*res);
// }
//


/*************Ma classe stringmap********************/
/*Utilisation:
StringMap sm;
sm.add("text1");
sm.add("text2");
*/
class StringMap
{
public:
	map <int,string> m;
	void init()
	{
		text="";
	}
public:
	StringMap(){init();}
	StringMap(vector<string> arr){init(); addRange(arr);}
	~StringMap(){}
	string text;

	int count(){ return m.size(); }
	int size(){ return m.size(); }

	void add(string s)
	{
		m[count()]=s;
		if(m.size()-1>0)//si ya une ligne avant:
			text.append("\r\n");
		text.append(s);
	}

	void add(char cstr[])
	{
		string s=cstr;
		add(s);
	}

	void add(int i)
	{
		add(intToStr(i));
	}
	void add(string before,int i)
	{
		string s=before;
		s.append(intToStr(i));
		add(s);
	}

	void add(bool b)
	{
		add((b)?"true":"false");
	}
	void add(string before,bool b)
	{
		string s=before;
		s.append((b)?"true":"false");
		add(s);
	}

	void add(string before,string str)
	{
		string s=before;
		s.append(str);
		add(s);
	}

	void addRange(vector<string> arr)
	{
		for(int i=0; i<arr.size(); i++)
		{
			add(arr[i]);
		}
	}

	/*
	void add(WCHAR wstr[])
	{
		char cstr[sizeof(wstr)];
		wcharToChar(cstr,wstr);
		string s=cstr;
		add(s);
	}*/

	void add(StringMap sm0)
	{
		for(int i=0; sm0.count(); i++)
		{
			add(sm0.item(i));
		}
	}

	void add(string before,StringMap sm0)
	{string str;
		int countFix=sm0.count();
		for(int i=0; i<countFix; i++)
		{
			str=before;
			str.append(sm0.item(i));
			add(str);
		}
	}

	int findItem(string strEq)
	{string str;
		int c=count();//gain de vitesse
		for(int i=0; i<c; i++)
		{
			str=item(i);
			if(!str.compare(strEq))return i;
		}
		return -1;
	}

	string item(int i){return m[i];}
	string last(){return m[m.size()-1];}


	string &operator[](int i) {
	  return this->m[i];
	}

	//virtual string toString()	{	}


	//retourner une seule chaine avec toutes les lignes séparées par "\r\n"
	string all()
	{
		string result="";
		int c=count();//gain de vitesse
		for(int i=0; i<c; i++)
		{
			result.append(item(i));
			if(i+1<c)result.append("\r\n");
		}
		return result;
	}

	bool contains(bool (*compareFn)(string item, string item2), string value, int *ifound=NULL)
	{
	      int key = -1;
	      map<int, string>::iterator it;
		    it=m.begin();
	      for (; it != m.end(); it++ )
	      {
		//  (*it).first
		  string vv =(*it).second;
		  if( compareFn(vv,value) )
		  {
		    key=(*it).first;
		    if(ifound!= NULL)(*ifound)=key;
		    return true; //break;
		  }
	      }
	}//contains overload

	bool contains(string value, int *ifound=NULL)
	{

	   // map<int, string> m = {  make_pair(0, "zero"), make_pair(1, "one"), make_pair(2, "two") };
/*
	      int key = -1;
	      for (auto &i : m) {
		  if (i.second == value) {
		    key = i.first;
		    break; // to stop searching
		  }
	      }

	     ifound=key;
	     printf("key found:%d\n",key);
	     */
	      int key = -1;
	      map<int, string>::iterator it;
		    it=m.begin();
	      for (; it != m.end(); it++ )
	      {
		//  (*it).first
		  string vv =(*it).second;
		  if(vv==value)
		  {
		    key=(*it).first;
		   if(ifound!= NULL)(*ifound)=key;
		   return true; //break;
		  }
	      }


		    /*
		      if(std::find(v.begin(), v.end(), x) != v.end()) {
			// v contains x
			} else {
			    // v does not contain x
			}
		      */

		    return false;
	}//contains


  //operator std::string() const { return "Hi"; }

    string toString()
    {
      string s="";
      int key = -1;
	      map<int, string>::iterator it;
		    it=m.begin();
	      for (; it != m.end(); it++ )
	      {
		  string vv =(*it).second;
		  key=(*it).first;
		  ostringstream oss;
		  oss<<key;
		  s+="["+oss.str()+"]=\""+vv+"\"";
	      }
	      return s;
  }//toString
};
#define StringList StringMap

//----------------------    vector helper functions     -------------------------------------
bool vectorContains( std::vector<int>& vec, int element )
{
    if (std::find(vec.begin(), vec.end(), element) != vec.end())
        return true;

    return false;
}
bool vectorContains( std::vector<string>& vec, string element )
{
    if (std::find(vec.begin(), vec.end(), element) != vec.end())
        return true;

    return false;
}

void vectorRemove(vector<string> &vect, string value)
{
  vect.erase(std::remove(vect.begin(), vect.end(), value), vect.end());
}


void vectorRemove(vector<int> &vect, int value)
{
  vect.erase(std::remove(vect.begin(), vect.end(), value), vect.end());
}


string vectorFind(vector<string> vect, string tofind, bool (*compareFn)(string item, string item2))
{
  string vline="";
  for(int i=0; i<vect.size(); i++)
	 {
	   if(compareFn!=NULL)
	   {
	     if(  (*compareFn)(vect[i], tofind) )
	     {
	       vline = vect[i];
	       return vline;
	       break;
	     }
	   }
	 }//next
	 return vline;
}//vectorFind

int vectorFind(vector<int> vect, int tofind, bool (*compareFn)(int item, int item2))
{
  for(int i=0; i<vect.size(); i++)
	 {
	   if(compareFn!=NULL)
	   {
	     if(  (*compareFn)(vect[i], tofind) )
	     {
	        return vect[i];
	       break;
	     }
	   }
	 }//next

	 return -1;
}//vectorFind

string vectorContains(vector<string> vect, string tofind, bool (*compareFn)(string item, string item2))
{
  return vectorFind(vect,tofind,compareFn);
}//vectorFind

int vectorContains(vector<int> vect, int tofind, bool (*compareFn)(int item, int item2))
{
  return vectorFind(vect,tofind,compareFn);
}//vectorFind

string vectorJoin(vector<string> vect, string join_sep=",")
{
  string output="";
  for(int i=0; i<vect.size(); i++)
  {
    if(output.length()>0) output+=join_sep;
    output+=vect[i];
  }
  return output;
}

//-----------------------------------------------------------

/*
 //énum map key and value
	map <int,string> m;
	m[9]="lundi";
	m[10]="mardi";
m[11]="mercredi";

	map<int,string>::iterator  it;

	it=m.begin();
  for (; it != m.end(); it++ )
  {
	  disp( (*it).first, (char*) (*it).second.c_str() );
  }
box("exit");exit(9);
*/

template <typename tKey, typename tValue>
class HashTable
{

private:


public:
map<tKey,tValue>m;
void put(tKey key,tValue value)
{
	m[key]=value;
}

void get(tKey key)
{
	return m[key];
}

//surcharge d'opérateur [
	tValue& operator[](tKey key)
	{
		return m[key];
	}

	int size()
	{
		return m.size();
	}
	//enum and get key list

/*
	deque <tKey> enumKeys()
	{
		deque <tKey> d;
		 map<tKey,tValue>::iterator  it;

		  it=m.begin();
		  for (; it != m.end(); it++ )
		  {
			  d.push_back( (*it).first);
			  //d.push_back((*it).second);
		  }
		return d;
	}

	deque <tValue> enumValues()
	{
		deque <tValue> d;
		 map<tKey,tValue>::iterator it;

		  it=m.begin();
		  for (; it != m.end(); it++ )
		  {
			  //d.push_back( (*it).first);
			  d.push_back((*it).second);
		  }
		  return d;
	}
	*/

}; //HashTable



//2023-06-24 15:08:36 - à modifier pour FreeRtos
void msSleep(int ms) {
usleep(ms*1000); //convert to microseconds
return;
}

void sleepMs(int ms) {
usleep(ms*1000); //convert to microseconds
return;
}

//clock sleep or delay
void csleep(unsigned int msecs) {
clock_t goal = msecs*CLOCKS_PER_SEC/1000 + clock();  //convert msecs to clock count
while ( goal > clock() );
}


//not tested
void lsleep(int value)
{

    clock_t goal = value*CLOCKS_PER_SEC/1000 + clock();  //convert msecs to clock count
    while ( goal > clock() )
    {
            usleep(10);
    }

}



//2023-06-24 22:53:05 - Explode with char delimiter
  // Utilisation
  // std::vector<std::string> tokens = explode("abc:def:ghi",':');
  // for(string part:tokens)
  // {
  //   cout<<"part="<<part<<endl;
  // }
  // // cout<<tokens[0]<<endl;
  // // cout<<tokens[1]<<endl;
std::vector<std::string> explode(std::string str, char split_char)
{
  std::vector<std::string> tokens;
  std::istringstream split(str);
   for (std::string each; std::getline(split, each, split_char); tokens.push_back(each));
  return tokens;
}//explode



//2023-06-24 23:15:14 - Convert uint_8* to String : 
string converter(uint8_t *str){
    return string((char *)str);
}


#endif
