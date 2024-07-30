#include "WebPage.h"
#include <iostream>
using namespace std;
WebPage::WebPage(int ID, string URL, string Content){
	this->ID = ID;
	this->Content = Content;
	this->URL = URL;
	this->CTR = 0.0;
	this->PageRank = 0.0;
	Impressions = 0;
	Clicks = 0;
	Overall = 0.0;
}
void WebPage::setPageRank(double pr) { PageRank = pr; }
void WebPage::setOverall(double o) { Overall = o; }
void WebPage::setImpression(int i) { Impressions = i; }
void WebPage::setCTR(double ctr) { CTR = ctr; }
void WebPage::setClicks(int c) { Clicks = c; }
//Getters
int WebPage::getID() { return ID; }
double WebPage::getPageRank() { return PageRank; }
double WebPage::getOverall() { return Overall; }
int WebPage::getImpressions() { return Impressions; }
double WebPage::getCTR() { return CTR; }
string WebPage::getURL() { return URL; }
int WebPage::getClicks() { return Clicks; }
string WebPage::getContent() { return Content; }
// ADD Functions
void WebPage::addConnection(WebPage* W) { Connections.push_back(W); }
void WebPage::addKeyWord(string str) { Keywords.push_back(str); }
void WebPage::addImpression() { Impressions++; }
void WebPage::addClick() { Clicks++; }
//Miscellaneous
void WebPage::Display() {
	cout << "URL : " << URL << endl;
	cout << "Clicks : " << Clicks << endl;
	cout << "Impressions : " << Impressions << endl;
	cout << "PageRank = " << PageRank << endl;
	cout << "CTR = " << CTR << endl;
	cout << "Overall = " << Overall << endl;
	cout << "Connected to : ";
	for (auto x : Connections) {cout << x->getURL() << ", ";}
	cout << endl;
	cout << "KeyWords : ";
	for (auto x : Keywords) {cout << x << ", ";}
	cout << endl;
	cout << "--------------------------" << endl;
}

bool WebPage::containsKeyWord(string kw) const {
	string temp = "";

	for (int i = 0; i < Keywords.size(); i++) {
		temp = Keywords[i];
		if (temp == kw) {return true;}
	}
	return false;
}

