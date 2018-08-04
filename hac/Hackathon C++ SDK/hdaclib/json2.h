//
// Copyright (c) 2018 Hdac Technology AG
// Hdac code distributed under the GPLv3 license
//============================================================================================
// json2.h
//	Simple JSON class
//
// History
//	2018/03/10	Initial code
//	2018/05/20	Parsing bug fixed
//============================================================================================
//

#ifndef __JSON2_H__
#define __JSON2_H__


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#include <iostream>
#include <string>
#include <array>
#include <vector>


using namespace std;


#define BOOL_STR(v)		((v) ? "true" : "false")

#define JSON_MAX_NEST		100

typedef enum {
	JSON_NONE	= 0,
	JSON_OBJECT,
	JSON_OBJECT_END,
	JSON_ARRAY,
	JSON_ARRAY_END,
	JSON_PAIR,
	JSON_STRING,
	JSON_NUMBER,
	JSON_TRUE,
	JSON_FALSE,
	JSON_NULL,
}	jsontype;


typedef struct {
	int	nobject;
	int	narray;
	int	npair;

	int	nstring;
	int	nnumber;
	int	ntrue;
	int	nfalse;
	int	nnull;

	int	length;
}	jsonstat;


class tokeninfo {
public:
	string	name;
	int	line;		// line number 
	int	linepos;	// position in a line
	int	pos;		// position in a source string

	tokeninfo()
		{ clear(); }
	void	clear()
		{ name.clear(); line = pos = 0; }
};


class jsonnode {
public:
	jsontype type;
	string	name;
	string	value;
	jsontype valuetype;
	short	depth;

	jsonnode()	{ type = JSON_NONE; name = ""; value = ""; valuetype = JSON_NONE; depth = 0; }
};


bool	json_rpc_error(const char *jsonstr);

int	isnumstr(const char *data);
int	isconstant(const char *data);		// true false null


class json {
public:
	vector<jsonnode> m_list;
private:
	int m_count[JSON_MAX_NEST];

	int parser(const string jsonstr);
	int tokenizer(const string jsonstr);

private:
	string	m_srcstr;		// source json string
	vector<tokeninfo> m_tokenlist;	// token list
//	vector<int> m_poslist;		// position list

	void printerr(int pos, string msg);

	void calcsize();
	int getstartindex(const int nth);	// index of m_list[]
	int getstartindex(const string name);	// index of m_list[]
	int getlastindex(int fromidx);

public:
	json()		{ clear(); }
	~json()		{ clear(); }

	void clear();

	int size(int depth = 1)	{ return m_count[depth]; }
	size_t listsize()	{ return m_list.size(); }

	json& parse(const string jsonstr);
	json find(const string name, const string opt = "");
	json get(const int nth)	{ return operator [](nth); }

	json& setname(const int nth, const string name);
	json& setvalue(const int nth, const string name, const long value);
	json& setvalue(const int nth, const string name, const double value);
	json& setvalue(const int nth, const string name, const string value);
	json& setvalue_common(const int nth, const string name, const string value, const jsontype type);

	json& replace(const int nth, const string name, const string value);
	json& replace(const int nth, const string name, const double value);
	json& replace(const int nth, const string name, const long value);
	json& replace(const int nth, jsonnode node);
	json& replace_common(const int nth, const string name, const string value, jsontype type);
	json& replace(const string name, const string value);
	json& replace(const string name, const double value);
	json& replace(const string name, const long value);

	json& add(json js);
	json& add(const jsonnode node);

	json& insert(const int nth, const string name, const string value);
	json& insert(const int nth, const string name, const double value);
	json& insert(const int nth, const string name, const long value);
	json& insert(const int nth, json sub);
	json& insert_common(const int nth, const string name, const string value, jsontype type);

	json& remove(const int nth);
	json& remove(const string name);

	json dup(int from);
	void fixdepth();
	jsonnode getnode(const int nth);

	string tostr(const bool isstrip = true);
	char *tostr(char *buf, size_t bufsz, const bool isstrip = true);
	string dump(FILE *outfp = NULL);
	string print(const int tab = 4, FILE *outfp = stdout);
	void dumplist(FILE *outfp = NULL);

	jsonstat stat();
	string type(const jsontype type);
	string strip_quote(const string token);
	bool isjsonobj(const string token);
	bool isarray(const string token);
	bool isnumber(const string token);

	json operator [](const int nth);
	json operator [](const string name);

	json& operator =(const string jsonstr)
		{ m_list.clear(); parse(jsonstr); calcsize(); return *this; }
	json& operator =(const json newjs)
		{ m_list.clear(); m_list = newjs.m_list; calcsize(); return *this; }
	json& operator =(const jsonnode node)
		{ m_list.clear(); m_list.push_back(node); calcsize(); return *this; }

	bool cmp(const json& cmpjson);
	bool operator ==(const json& cmpjson)
		{ return cmp(cmpjson); }
};


#endif	// __JSON2_H__
