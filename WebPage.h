#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;
class WebPage
{
private:
	int ID, Impressions, Clicks;
	string URL,Content;
	double PageRank,Overall, CTR;
public:
	//Lists
	vector<WebPage*> Connections;
	vector<string> Keywords;
	//Constructor
	WebPage(int, string, string);
	//Setters
	void setPageRank(double);
	void setOverall(double);
	void setImpression(int);
	void setCTR(double);
	void setClicks(int);
	//Getters
	int getID();
	double getPageRank();
	double getOverall();
	int getImpressions();
	double getCTR();
	string getURL();
	int getClicks();
	string getContent();
	// ADD Functions
	void addConnection(WebPage*);
	void addKeyWord(string);
	void addImpression();
	void addClick();
	//Miscellaneous
	void Display();	
	bool containsKeyWord(string) const;
};