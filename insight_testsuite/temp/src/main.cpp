#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <math.h>
#include <iomanip>
using namespace std;

void rLine(string in, int& t, string& sym, double& price){
  //cout << "Begin func: " << in << endl;
  string delim = "|";
  size_t pos = 0;
  string token;
  //while((pos = in.find(delim)) != string::npos){
    pos = in.find(delim);
    token = in.substr(0,pos);
    //    cout << token << endl;
    stringstream ss1(token);
    ss1 >> t;
    //cout << t;
    in.erase(0,pos+delim.length());

    //loop 
    pos = in.find(delim);
    sym = in.substr(0,pos);
    stringstream ss2(sym);
    //cout << sym;
    in.erase(0,pos+delim.length());


    //}
    stringstream ss3(in);
    ss3 >> price;
    //cout << price  << endl;
  


}

void printAll(vector<map<string,double> > vect){
  cout << "printing whole vector of maps:" <<endl;
  for(int i=0; i < vect.size(); i++){
    cout << "i:" << i << endl;
    
    for (std::map<string,double>::iterator it=vect[i].begin(); it!=vect[i].end(); ++it)
      std::cout << it->first << " => " << it->second << '\n';

  }

}


void build(string filename, vector< map<string,double> > &out){

  /*
  //  ifstream file("../insight_testsuite/tests/test_0/input/window.txt");
  ifstream file(filename + "window.txt");
  string line;
  int window;
  while (std::getline(file, line)) {
    // line contains the current line
    //cout << line;
    stringstream ss(line);
    ss >> window;
  }
  */
  //array of maps (only size ~ window?)                                                                                                                                         
  //map<string,double> actual[window];
  //map<pair<int,string>,double> actual;
  vector <map<string,double> > actual;

  //actual[0]["ATAYJP"] = 25.74;
  //cout << actual[0]["ATAYJP"];
  //  cout << window;  
  
  string line;

  int t;
  string sym;
  double price;
  //  ifstream actfile("../insight_testsuite/tests/test_0/input/actual.txt");
  //ifstream actfile(filename + "actual.txt");
  ifstream actfile(filename);
  int n=1;
  //int n=54;
  while (std::getline(actfile, line)) {
    //cout << "Time = " << n+1 << endl;
    // line contains the current line                                                                                                                                         
    //    cout << line << endl;
    

    
    //    do{
      rLine(line,t,sym,price);
      //cout << t << "," << sym << "," << price << endl;
      //actual[make_pair(t,sym)] = price;
      //actual[t-1][sym] = price;

      //      cout << t<< ":" << n << endl;
      if(actual.size() == 0){
	map<string,double> temp;
        temp[sym] = price;
        actual.push_back(temp);
	//cout << "First element" << endl;
      }
      else{
	if(t == n ){ 
	  //actual[t-1][sym] = price;
	  actual.back()[sym] = price;
	  //cout << "added to " << t-1 << endl;
	}
	else{
	  map<string,double> temp;
	  temp[sym] = price;
	  actual.push_back(temp);
	  //cout << "pushed to back"<< endl;
	  n = t;
	}
      }
  }
  out = actual;
  //  printAll(out);
}

double calcTotErrorHour(map<string,double> actual,map<string,double> predicted,int &nSamples){
  //cout << "begin error for one time: " << endl;
  double err =0;
  nSamples = 0;
  std::map<string,double>::iterator it2;

  for (std::map<string,double>::iterator it=actual.begin(); it!=actual.end(); ++it){
    //std::cout << it->first << " => " << it->second << '\n';
    it2 = predicted.find(it->first);
    if(it2!= predicted.end()){
      err += fabs(it->second - it2->second);
      nSamples++;
      //cout << it->second << ":" <<  it2->second << "=" << err << endl;
    }
  }
  //  err /= n;
  return err;
}

double round2(double var)
{
  double value = (int)(var * 100 + .5);
  return (double)value / 100;
}
 


int main(int argc, char* argv[]){
  //window,act,pred,out


  //window
  //   ifstream file("../insight_testsuite/tests/test_0/input/window.txt");                                                                                                  
  //  ifstream file("../insight_testsuite/tests/test_1/input/window.txt");                                                                                                   
  ifstream file(argv[1]);

  string line;
  int window;                                                                                                                                                                 
  while (std::getline(file, line)) {                                                                                                                                          
    stringstream ss(line); 
    ss >> window;         
  }

  //    string filename = "../insight_testsuite/tests/test_0/input/actual.txt";
  //    string filename = "../insight_testsuite/tests/test_1/input/actual.txt";
    string filename = argv[2];
  vector <map<string,double> > actual;
  build(filename,actual);
  //printAll(actual);
  //exit(1);

  //    filename = "../insight_testsuite/tests/test_0/input/predicted.txt";
  //  filename = "../insight_testsuite/tests/test_1/input/predicted.txt";
  filename = argv[3];
  vector <map<string,double> > predicted;
  build(filename,predicted);
  //printAll(predicted);

  if(window > max(predicted.size(),actual.size())){
    cout << "Warning: window larger than predicted, actual, setting window to max";
    window = max(predicted.size(),actual.size());
  }

  
  int nDiv = actual.size() - window+1;
  //cout << "nDIV" << nDiv<<endl;

  

  ofstream clearFile;
  //   clearFile.open("../insight_testsuite/tests/test_0/output/comparison.txt");
  //clearFile.open("../insight_testsuite/tests/test_1/output/comparison2.txt");
  clearFile.open(argv[4]);
  
  clearFile.close();

  for(int k = 0; k <nDiv;k++){
    //    int k=0;

    int t = k;
    int nTotSamples =0;
    double err = 0;
    for(int j=0; j < window; j++){
      int nSamples;
      err += calcTotErrorHour(actual[t],predicted[t],nSamples);
      //cout << nSamples << ":" << err << endl;
      nTotSamples += nSamples;
      t++;
    }

    //    cout << err/nTotSamples;
    ofstream myfile;
    //        myfile.open("../insight_testsuite/tests/test_0/output/comparison.txt",std::ios_base::app);
    //	myfile.open("../insight_testsuite/tests/test_1/output/comparison2.txt",std::ios_base::app);
	myfile.open(argv[4],std::ios_base::app);
    myfile << k+1 << "|" << k+window << "|";



        myfile <<std::fixed << std::setprecision(2) << round2(err/nTotSamples)<<endl;
    //    myfile  << (err/nTotSamples)<<endl;

    myfile.close();
    }

    //  cout << std::fixed << std::setprecision(2) << round2(2.1049)<<endl;
    //cout << std::fixed << std::setprecision(2) << round2(2.1050)<<endl;
}
