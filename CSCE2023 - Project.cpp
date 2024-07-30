#include<iostream>
#include<vector>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <sstream>
#include<fstream>
#include "WebPage.h"

//#include <windows.h>

using namespace std;

// check if WebPage y is connected to WebPage x
bool isConnectedTo(WebPage* x, WebPage* y) {
    for (int i = 0; i < y->Connections.size(); i++) {
        WebPage* p = y->Connections[i];
        if (p->getURL() == x->getURL()) {
            return true;
        }
    }
    return false;
}
bool compare(WebPage* p1, WebPage* p2) {
    return p1->getOverall() > p2->getOverall();
}

void sortWebPages(vector<WebPage*>& webPages) {
    sort(webPages.begin(), webPages.end(), compare);
}
//calculate the PageRank for all WebPages in the map
void PageRank(unordered_map<int, WebPage>& WebPages) {

    double intiRank = 1.0 / WebPages.size();

    for (auto& x : WebPages) {

        x.second.setPageRank(intiRank);
    }

    double test1 = 0.0, test2 = 0.0, cal = 0.0;
    unordered_map<int, double> updatedPageRanks;
    for (auto& x : WebPages) {
        updatedPageRanks[x.first] = x.second.getPageRank();
    }
    for (int k = 0; k < 2; k++) {
        for (auto& x : WebPages) {
            cal = 0.0;
            test1 = 0.0, test2 = 0.0;
            WebPage* temp = &(x.second);

            for (auto& y : WebPages) {
                WebPage* temp2 = &(y.second);
                if (isConnectedTo(temp, temp2)) {
                    test1 = temp2->getPageRank();
                    test2 = temp2->Connections.size();
                    cal += (test1 / test2);
                }
            }
            updatedPageRanks[x.first] = cal;
        }
        for (auto& x : updatedPageRanks) {
            WebPages.at(x.first).setPageRank(x.second);
        }
        updatedPageRanks.clear();
    }
}


// calculate the Click Through Rate for a WebPage
void calculateCTR(unordered_map<int, WebPage>& WebPages) {
    double C = 0.0, I = 0.0, CTR = 0.0, test = 0.0;
    for (auto& x : WebPages) {
        C = x.second.getClicks();
        I = x.second.getImpressions();
        test = (C / I);
        CTR = test * 100.0;
        x.second.setCTR(CTR);
        CTR = 0.0;
    }
}
int identifyOperator(string x) {
    if (x.find("AND") != string::npos || x.find("and") != string::npos) {return 1;}
    else if (x.find("OR") != string::npos || x.find("or") != string::npos) {return 2;}
    else if (x.find("\"") != string::npos) {return 3;}
    else {return 2;}
    return -1;
}

vector<string> split_string(string x)
{
    vector<string> words;
    string word;
    if (x.find("\"") != string::npos) {
        x.erase(std::remove(x.begin(), x.end(), '\"'), x.end());
    }
    stringstream ss(x);
    while (ss >> word) {
        if (word == "AND" || word == "OR") {continue;}
        words.push_back(word);
    }
    return words;
}

// search the query x
vector<WebPage*> SearchQuery(string x, unordered_map<int, WebPage>& WebPages) {

    vector<string> words = split_string(x);
    vector<WebPage*> SearchResult;
    WebPage* p = nullptr;
    string keyword1 = "", keyword2 = "";
    try {
        keyword1 = words.at(0), keyword2 = words.at(1);
    }
    catch (out_of_range e) {
        return SearchResult;
    }
    string keyword3 = "";
    switch (identifyOperator(x)) {
    case 1:
        //AND
        for (auto& x2 : WebPages) {

            p = &(x2.second);

            if (p->containsKeyWord(keyword1) && p->containsKeyWord(keyword2)) {
                p->addImpression();
                SearchResult.push_back(p);
            }
        }
        sortWebPages(SearchResult);
        return SearchResult;
        delete p;
        break;
    case 2:
        //OR 
        for (auto& x2 : WebPages) {

            p = &(x2.second);

            if (p->containsKeyWord(keyword1) || p->containsKeyWord(keyword2)) {
                p->addImpression();
                SearchResult.push_back(p);
            }
        }
        sortWebPages(SearchResult);
        return SearchResult;
        delete p;
        break;
    case 3:
        //Quotes
        keyword3 = keyword1 + " " + keyword2;
        for (auto& x2 : WebPages) {

            p = &(x2.second);

            if (p->containsKeyWord(keyword3)) {
                p->addImpression();
                SearchResult.push_back(p);
            }
        }

        sortWebPages(SearchResult);
        return SearchResult;
        delete p;
        break;

    case -1:
        break;

    }
    return SearchResult;
}

//Files 
void clearFile(string filename) {
    ofstream ofs;
    ofs.open(filename, ofstream::out | ofstream::trunc);
    ofs.close();
}
void Key_Word_File_Write(unordered_map<int, WebPage>& w) {

    clearFile("KeyWord.txt");
    ofstream KeyWordfileOUT("KeyWord.txt");

    for (auto& kvp : w) {

        const vector<string>& keyword_list = kvp.second.Keywords;
        KeyWordfileOUT << kvp.second.getURL() << ",";

        for (size_t i = 0; i < keyword_list.size(); ++i) {
            KeyWordfileOUT << keyword_list[i];

            if (i != keyword_list.size() - 1) {
                KeyWordfileOUT << ",";
            }
        }

        KeyWordfileOUT << '\n';

    }

    KeyWordfileOUT.close();
}
void Key_Word_File_Read(unordered_map<int, WebPage>& w) {

    ifstream infile("KeyWord.txt");
    string line = "";

    while (getline(infile, line)) {
        stringstream ss(line);
        string first_website = "", keyword = "";
        vector<string> keywords;
        WebPage* p = nullptr, * q = nullptr;
        getline(ss, first_website, ',');
        for (auto& x : w) {
            p = &x.second;
            if (p->getURL() == first_website) {

                p = &x.second;
                break;
            }
        }

        while (getline(ss, keyword, ',')) {

            if (p != nullptr) {

                p->addKeyWord(keyword);

            }
            keyword = "";
        }
    }

    infile.close();

}
void Web_Graph_File_Write(unordered_map<int, WebPage>& w) {
    clearFile("Web Graph.txt");
    ofstream WebGraphfileOUT("Web Graph.txt");

    for (auto& kvp : w) {

         vector<WebPage*>& website_list = kvp.second.Connections;
        WebGraphfileOUT << kvp.second.getURL()<< ",";
     

        for (size_t i = 0; i < website_list.size(); ++i) {
            WebGraphfileOUT << website_list[i]->getURL();

            if (i != website_list.size() - 1) {
                    WebGraphfileOUT << ",";       
            }
        }
        WebGraphfileOUT << '\n';   
    }
    WebGraphfileOUT.close();
}

void Impressions_File_Write(unordered_map<int, WebPage>& w) {
    clearFile("Impressions.txt");
    ofstream ImpressionsfileOUT("Impressions.txt");
    for (auto& kvp : w) {
        ImpressionsfileOUT << kvp.second.getURL() << ",";
        ImpressionsfileOUT << kvp.second.getImpressions();
        ImpressionsfileOUT << '\n';
    }
    ImpressionsfileOUT.close();
}
void Impressions_File_Read(unordered_map<int, WebPage>& w) {
    
    ifstream infile;
    infile.open("Impressions.txt");
    string line = "";

    while (getline(infile, line)) {
        stringstream inputString(line);
        string website = "";
        string impression = "";
        WebPage* p = nullptr;
        getline(inputString, website, ',');
        for (auto& kvp : w) {
            p = &kvp.second;
            if (p->getURL() == website) {
            
                p = &kvp.second;
                break;
            }
        }
        getline(inputString, impression, ',');
        p->setImpression(stoi(impression));
    }

    infile.close();
}

void Clicks_File_Write(unordered_map<int, WebPage>& w) {
    clearFile("Clicks.txt");
    ofstream ClicksfileOUT("Clicks.txt");
    for (auto& kvp : w) {
        ClicksfileOUT << kvp.second.getURL() << ",";
        ClicksfileOUT << kvp.second.getClicks();
        ClicksfileOUT << '\n';
    }
    ClicksfileOUT.close();
}
void Clicks_File_Read(unordered_map<int, WebPage>& w) {

    ifstream infile;
    infile.open("Clicks.txt");
    string line = "";

    while (getline(infile, line)) {
        stringstream inputString(line);
        string website = "";
        string clicks = "";
        WebPage* p = nullptr;
        getline(inputString, website, ',');
        for (auto& kvp : w) {
            p = &kvp.second;
            if (p->getURL() == website) {
                p = &kvp.second;
                break;
            }
        }
        getline(inputString, clicks, ',');
        p->setClicks(stoi(clicks));
    }

    infile.close();
}
void Web_Graph_File_Read(unordered_map<int, WebPage>& w) {
    ifstream infile("Web Graph.txt");
    string line = "";

    while (getline(infile, line)) {
        stringstream ss(line);
        string first_website = "", website = "";
        vector<string> connections;
        WebPage* p = nullptr, *q = nullptr;
        getline(ss, first_website, ','); 
        for (auto& x : w) {
            p = &x.second;
            if (p->getURL() == first_website) {      
                p = &x.second;
                break;
            }
        }
       
        while (getline(ss, website, ',')) {    
            for (auto& x : w) {
                q = &x.second;
                if (q->getURL() == website) {
                    p->addConnection(q);
                    break;
                }
            }
            website = "";   
        } 
    }
    infile.close();
}

void ProgramIntilization(unordered_map<int, WebPage>& w) {
    try {
        Impressions_File_Read(w);
        Web_Graph_File_Read(w);
        Key_Word_File_Read(w);
        Clicks_File_Read(w);
    }
    catch (exception e) { cout << "Could Not Initialize Program:ERROR = " << e.what() << endl; }
}
// calculate the overall score for a WebPage
void OverallScore(unordered_map<int, WebPage>& w) {
    WebPage* temp = nullptr;
    double overall = 0.0;
    for (auto& x : w) {
        temp = &x.second;
        overall = ((0.4 * temp->getPageRank()) + (((1 - ((0.1 * temp->getImpressions()) / (1 + 0.1 * temp->getImpressions()))) * temp->getPageRank()) + ((0.1 * temp->getImpressions()) / (1 + 0.1 * temp->getImpressions()))) * temp->getCTR()) * 0.6;
        temp->setOverall(overall);

    }
    //delete temp;
}

// Update the score of all WebPages
void UpdateScore(unordered_map<int, WebPage>& w) {
    PageRank(w);
    calculateCTR(w);
    OverallScore(w);
}
void test(unordered_map<int, WebPage> WebPages) {
    for (auto& x : WebPages) {x.second.Display();}
}
void Save(unordered_map<int, WebPage>& w) {
    try {
        Impressions_File_Write(w);
        Web_Graph_File_Write(w);
        Key_Word_File_Write(w);
        Clicks_File_Write(w);
        return;
    }
    catch (exception e) {cout << "Could Not Save Program:ERROR = " << e.what() << endl; }
}
int main() {
    //Insertion(Test)
    WebPage A(234, "www.A.com", "This is Webpage A");
    WebPage B(235, "www.B.com", "This is Webpage B");
    WebPage C(236, "www.C.com", "This is Webpage C");
    WebPage D(237, "www.D.com", "This is Webpage D");
    //Connections
 /* A.addConnection(&B);
    A.addConnection(&C);
    B.addConnection(&D);
    D.addConnection(&C);
    C.addConnection(&A);
    C.addConnection(&B);
    C.addConnection(&D);*/
    // Keywords
   // A.addKeyWord("Data");
   // A.addKeyWord("Structure");
   // A.addKeyWord("Data Structure");
   // B.addKeyWord("Data");
   // B.addKeyWord("Algorithm");
   // C.addKeyWord("Science");
   // C.addKeyWord("Algorithm");
   // D.addKeyWord("Data Science");
   // D.addKeyWord("Science");
    //---------------------TEST---------------------
    //Test Impressions and Clicks
  /*A.setImpression(23);
    A.setClicks(25);
    B.setImpression(30);
    B.setClicks(40);
    C.setImpression(65);
    C.setClicks(34);
    D.setImpression(30);
    D.setClicks(40);*/

    /* Web_Graph_File_Write(WebPages);
     Key_Word_File_Write(WebPages);*/

     //Impressions_File_Write(WebPages);
    // Clicks_File_Write(WebPages);
    //---------------------TEST---------------------

    //Appending to graph
    unordered_map<int, WebPage> WebPages = { {1,A}, {2,B}, {3,C}, {4,D} };
    ProgramIntilization(WebPages);
    UpdateScore(WebPages);
    
    
   
    //---------------------TEST---------------------
    // calculate the PageRank for all WebPages
    //PageRank(WebPages);
    //
   // string x;
    //cout << "Enter search" << endl;
   // getline(cin, x);
    //vector<WebPage*> results = SearchQuery(x, WebPages);
    //---------------------TEST---------------------

    int input = -1, input2 = -1, input3 = 0, WID = 0;
    bool flag1 = false;
    string query;
    vector<WebPage*> result;

    // HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << "Note - To Save You Must Select Exit" << endl;
    do {
        //test(WebPages);
        if (input != 1) {
            //    SetConsoleTextAttribute(h, 1);
            cout << "---------------------INDEX---------------------" << endl;
            //   SetConsoleTextAttribute(h, 7);
            cout << "1.Search" << endl;
            cout << "2.Exit" << endl;
            cout << "Input: ";

            cin >> input;
        }
        switch (input) {
        case 1:
            query = "";
            //   SetConsoleTextAttribute(h, 1);
            cout << "---------------------Search---------------------" << endl;
            //  SetConsoleTextAttribute(h, 7);
            cout << "Enter Query :" << endl;
            cin.ignore();
            getline(cin, query);
            //  SetConsoleTextAttribute(h, 1);
            cout << "---------------------Result---------------------" << endl;
            //   SetConsoleTextAttribute(h, 7);
            result = SearchQuery(query, WebPages);
            if (!result.empty()) {
                //  SetConsoleTextAttribute(h, 2);
                cout << "Results Found For: " << query << endl;
                // SetConsoleTextAttribute(h, 7);
                UpdateScore(WebPages);
         //       test(WebPages);
                do
                {
                    for (auto& x : result) {
                        cout << x->getID() << ": " << x->getURL() << endl;
                    }
                    //  SetConsoleTextAttribute(h, 1);
                    cout << "------------------------------------------------" << endl;
                    //   SetConsoleTextAttribute(h, 7);
                    cout << "1.Open WebPage" << endl;
                    cout << "2.New Search" << endl;
                    cout << "3.Exit" << endl;
                    cout << "Input: ";
                    cin >> input2;
                    switch (input2) {
                    case 1:
                        //    SetConsoleTextAttribute(h, 1);
                        cout << "------------------------------------------------" << endl;
                        //    SetConsoleTextAttribute(h, 7);
                        cout << "Input WebPage ID : ";
                        cin >> WID;
                        for (auto& x : result) {

                            if (x->getID() == WID) {
                                flag1 = false;
                                cout << x->getContent() << endl;
                                x->addClick();
                                //        SetConsoleTextAttribute(h, 1);
                                cout << "------------------------------------------------" << endl;
                                //         SetConsoleTextAttribute(h, 7);
                                UpdateScore(WebPages);
                                //test(WebPages);
                                break;
                            }
                            else {flag1 = true;}
                        }
                        if (flag1) {
                            //      SetConsoleTextAttribute(h, 4);
                            cout << "------------------------------------------------" << endl;
                            cout << "Invalid ID" << endl;
                            cout << "------------------------------------------------" << endl;
                            //       SetConsoleTextAttribute(h, 7);
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            break;
                        }
                        cout << "1.Return to Result List" << endl;
                        cout << "2.New Search" << endl;
                        cout << "3.Exit" << endl;
                        cout << "Input: ";
                        cin >> input3;
                        if (input3 == 1) {
                            //    SetConsoleTextAttribute(h, 1);
                            cout << "------------------------------------------------" << endl;
                            //   SetConsoleTextAttribute(h, 7); 
                            break;
                        }
                        else if (input3 == 2) { input2 = 2; result.clear(); break; }
                        else if (input3 == 3) { cout << "GoodBye" << endl; Save(WebPages);  return 0; }
                        else {
                            cout << "------------------------------------------------" << endl;
                            //    SetConsoleTextAttribute(h, 4);
                            cout << "Invalid Input" << endl;
                            //  SetConsoleTextAttribute(h, 7);
                            cout << "------------------------------------------------" << endl;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            break;
                        }
                    case 2:
                        break;
                    case 3:
                        cout << "Goodbye" << endl;
                        Save(WebPages);
                        return 0;
                    default:
                        cout << "------------------------------------------------" << endl;
                        //    SetConsoleTextAttribute(h, 4);
                        cout << "Invalid Input" << endl;
                        //  SetConsoleTextAttribute(h, 7);
                        cout << "------------------------------------------------" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                } while (input2 != 2);
                result.clear();
                input = 1;
                break;
            }
            else {
                //     SetConsoleTextAttribute(h, 4);
                result.clear();
                cout << "No Results Found For : " + query << endl;
                input = -1;
                break;
                //    SetConsoleTextAttribute(h, 7);
            }
            // SetConsoleTextAttribute(h, 1);
            cout << "------------------------------------------------" << endl;
            //   SetConsoleTextAttribute(h, 7);     
        case 2:
            cout << "Goodbye" << endl;
            Save(WebPages);
            return 0;
        default:
            //  SetConsoleTextAttribute(h, 4);
            cout << "------------------------------------------------" << endl;
            cout << "Invalid Input" << endl;
            //   SetConsoleTextAttribute(h, 7);
            input = -1;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    } while (input != 0);

    return 0;
}