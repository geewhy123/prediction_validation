// main.cpp - Driver and functions for prediction and validation coding project
// August 27, 2018 by Gary Yan
// See README.md for description.  Compiled and run by executing script ./run.sh in the main level directory

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <math.h>
#include <iomanip>
using namespace std;

// Function that reads one line in the data from a specified file (actual or predicted)
// Arguments: string in - input file string
//            int & t - will contain time (hour), upon exiting function
//            string& sym - will contain ticker symbol, upon exiting function
//            double& price - will containg price, upon exiting function
// Return: None
void rLine(string in, int& t, string& sym, double& price){
  
  string delim = "|"; //pipe delimited
  size_t pos = 0; 
  string token;
  // splices line into three parts : time|ticker|price
  pos = in.find(delim);
  token = in.substr(0,pos);
  
  stringstream ss1(token);
  ss1 >> t;

  in.erase(0,pos+delim.length()); //second part
  pos = in.find(delim);
  sym = in.substr(0,pos);
  stringstream ss2(sym);
  
  in.erase(0,pos+delim.length()); //third part
  stringstream ss3(in);
  ss3 >> price;
  
}

// Function used for testing, prints out the whole vector of maps
// Arguments:  vector<map<string,double> > vect - vector of maps for time,ticker,price data
// Return: none
void printAll(vector<map<string,double> > vect){
  cout << "printing whole vector of maps:" <<endl;
  for(int i=0; i < vect.size(); i++){
    cout << "i:" << i << endl;
    
    for (std::map<string,double>::iterator it=vect[i].begin(); it!=vect[i].end(); ++it)
      std::cout << it->first << " => " << it->second << '\n';
  }
  
}

// Build the vector of maps for the data. Each element of the vector is for a specific time,  
// the keys are the ticker, and the values are the prices
// Arguments: string filename - input file name
//            vector<map<string,double> > & out - vector of maps
// Return: none
void build(string filename, vector< map<string,double> > &out){

  vector <map<string,double> > actual;
  string line;
  int t;
  string sym;
  double price;

  ifstream actfile(filename);
  int n=1;
  
  while (std::getline(actfile, line)) { //loop over each line
    rLine(line,t,sym,price); //read line by line

    if(actual.size() == 0){ //first element, push back
      map<string,double> temp;
      temp[sym] = price;
      actual.push_back(temp);
    }
    else{ //existing vector element
      if(t == n ){ 
	actual.back()[sym] = price; //add to current map
      }
      else{ //next time, create new map and append it to the vector
	map<string,double> temp;
	temp[sym] = price;
	actual.push_back(temp);
	n = t;
      }
    }
  }
  out = actual;

}

// Calculates the total error for a given time. Valid samples counted only if in both actual AND predicted
// Arguments: map<string,double> actual - map containint actual values for the current time
//            map<string,double> predicted - map containing predicted values for the current time
//            int &nSamples - will contain number of valid samples, upon returning from function
// Return: total error (|actual-predicted|) - double
double calcTotErrorHour(map<string,double> actual,map<string,double> predicted,int &nSamples){

  double err =0;
  nSamples = 0;
  std::map<string,double>::iterator it2; //iterator for predicted

  for (std::map<string,double>::iterator it=actual.begin(); it!=actual.end(); ++it){//iterate through actual map
    it2 = predicted.find(it->first); //find ticker in predicted
    if(it2!= predicted.end()){ //if it exists in both
      err += fabs(it->second - it2->second);
      nSamples++;
    }
  }
  return err; //total error

}

// Custom function to round to two decimal places
// Arguments : double var - value to round
// Return : rounded to two decimal places - double
double round2(double var)
{
  double value = (int)(var * 100 + .5);
  return (double)value / 100;
}
 
// Function main, driver for whole process
int main(int argc, char* argv[]){
  //read window
  ifstream file(argv[1]);

  string line;
  int window;                                                                                                                                                                 
  while (std::getline(file, line)) {                                                                                                                                          
    stringstream ss(line); 
    ss >> window;         
  }

  //read actual
  string filename = argv[2];
  vector <map<string,double> > actual;
  build(filename,actual);
  
  //read predicted
  filename = argv[3];
  vector <map<string,double> > predicted;
  build(filename,predicted);
  
  //if window is too big..
  if(window > max(predicted.size(),actual.size())){
    cout << "Warning: window larger than predicted, actual, setting window to max";
    window = max(predicted.size(),actual.size());
  }

  //number of divisions to calculate
  int nDiv = actual.size() - window+1;

  //preparing output file
  ofstream clearFile;
  clearFile.open(argv[4]);
  clearFile.close();
  
  for(int k = 0; k <nDiv;k++){//for each window (division)
    int t = k;
    int nTotSamples =0;
    double err = 0;
    for(int j=0; j < window; j++){
      int nSamples;
      err += calcTotErrorHour(actual[t],predicted[t],nSamples);
      nTotSamples += nSamples;
      t++;
    }

    //write comparison file
    ofstream myfile;
    myfile.open(argv[4],std::ios_base::app);
    myfile << k+1 << "|" << k+window << "|";
    myfile <<std::fixed << std::setprecision(2) << round2(err/nTotSamples)<<endl;
    myfile.close();
  }
  
  return 0;
}
