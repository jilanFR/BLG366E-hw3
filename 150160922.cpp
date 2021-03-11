#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>


using namespace std;

class Suites{
  public:
    string name;
    int value;
    int weight;
    vector<string> freq;
};

struct KnapscakData{
    vector<string> suiteNames; 
    int numOfBugs;
};

vector<Suites> suites;
int max(int a, int b) 
{ 
    return (a > b) ? a : b; 
}

int min(int a, int b) 
{ 
    return (a < b) ? a : b; 
} 

KnapscakData knapSack(int W) 
{ 
    int M[suites.size()+1][W+1]; 
    vector<string> suiteNames[suites.size()+1][W+1];

    for (int w = 0; w <= W; ++w) {
        M[0][w] = 0;
    }

    for (int i = 1; i <= suites.size(); ++i) { 
        for (int w = 0; w <= W; ++w) { 
            if (suites[i-1].weight > w){
                M[i][w] = M[i - 1][w];
                suiteNames[i][w] = suiteNames[i - 1][w];
            }
            else{
                if( M[i - 1][w] > suites[i-1].value + M[i - 1][w - suites[i-1].weight]){
                  M[i][w] = M[i - 1][w];
                  suiteNames[i][w] = suiteNames[i - 1][w];
                }
                else{
                  M[i][w] = suites[i-1].value + M[i - 1][w - suites[i-1].weight];
                  suiteNames[i][w] = suiteNames[i - 1][w - suites[i-1].weight];
                  suiteNames[i][w].push_back(suites[i-1].name);
                }

            }
        } 
    } 
      
    KnapscakData knapscakData;
    knapscakData.suiteNames = suiteNames[suites.size()][W];
    knapscakData.numOfBugs = M[suites.size()][W]; 
    return knapscakData;
} 

int readFile(){
    ifstream readFile("data.txt");
    
    string temp;
    int time;
    readFile >>temp>>temp>>temp>>temp>>time;
    readFile >>temp>>temp>>temp>>temp;

    while(!readFile.eof()){

      Suites suite;

      readFile >> suite.name >> suite.value >> suite.weight;

      
      string freq;
      while ((readFile.peek()!='\n') && (readFile>>freq)){
        suite.freq.push_back(freq);
      }
      suites.push_back(suite);
      
    }
    return time;
}

struct Freq{
  int size;
  vector<int>numberOfNonZeros;
  int maxElementIndex;
  vector<string> str;
  

};
map<string,Freq> freq;


void levinshtein(KnapscakData knapscakData){

    for(int m = 0 ; m < knapscakData.suiteNames.size(); m++){
      for(int i = 0; i < suites.size() ; i++){
        if( suites[i].name == knapscakData.suiteNames[m] ){
            freq[knapscakData.suiteNames[m]];
            freq[knapscakData.suiteNames[m]].size = suites[i].freq.size();
            for(int k = 0 ; k < suites[i].freq.size(); k++){
                stringstream ss (suites[i].freq[k]);
                string item;

                vector< int > vec;
                vector< int > vecOr;

                int counter = 0;
                int ord = 1;
                while (getline (ss, item, '-')) {
                  vec.push_back(stoi(item));
                  vecOr.push_back(ord);
                  if(item != "0")
                    counter++;
                  ord++;
                }
                freq[knapscakData.suiteNames[m]].numberOfNonZeros.push_back(counter);

                bool swap = true;
                while(swap){
                  swap = false;
                  for (size_t i = 0; i < vec.size()-1; i++) {
                      if (vec[i]>vec[i+1] ){
                          vec[i] += vec[i+1];
                          vecOr[i] += vecOr[i+1];
                          vec[i+1] = vec[i] - vec[i+1];
                          vecOr[i+1] = vecOr[i] - vecOr[i+1];
                          vec[i] -=vec[i+1];
                          vecOr[i] -=vecOr[i+1];
                          swap = true;
                      }
                  }
              }
              string st = "";
              for(int i = 0; i < vecOr.size(); i++){

                st = st + to_string(vecOr[i]);
              }
              freq[knapscakData.suiteNames[m]].str.push_back(st);

              freq[knapscakData.suiteNames[m]].maxElementIndex = max_element(freq[knapscakData.suiteNames[m]].numberOfNonZeros.begin(),freq[knapscakData.suiteNames[m]].numberOfNonZeros.end()) - freq[knapscakData.suiteNames[m]].numberOfNonZeros.begin();


            }
        }
      }
    }
}


int distanceCalc(string s, string t){

    int m,n,temp,tracker;
    int arr[100][100];
    m = s.size();
    n = t.size();
 
    for(int i=0;i<=m;i++)
      arr[0][i] = i;
    for(int j=0;j<=n;j++)
      arr[j][0] = j;
 
    for (int j=1;j<=m;j++)
    {
        for(int i=1;i<=n;i++)
        {
            if(s[i-1] == t[j-1])
            {
                tracker = 0;
            }
            else
            {
                tracker = 1;
            }
            temp = min((arr[i-1][j]+1),(arr[i][j-1]+1));
            arr[i][j] = min(temp,(arr[i-1][j-1]+tracker));
        }
    }

    return arr[n][m];
}

int main() 
{ 
    int W = readFile();

    KnapscakData knapscakData;
    knapscakData = knapSack( W );
    cout << "Optimum selected set:";
    for(int i = 0 ; i < knapscakData.suiteNames.size(); i++){
      cout << " " << knapscakData.suiteNames[i];
    }
    cout<<endl;
    cout<< "Total number of bugs in these suits: " << knapscakData.numOfBugs <<endl;
    levinshtein(knapscakData);

    for(int i = 0 ; i < knapscakData.suiteNames.size(); i++){
      cout << knapscakData.suiteNames[i] ;

      if(freq[knapscakData.suiteNames[i]].size == 1 ){
        cout << " 1" << endl;
      }else{
          vector <int>indexSet;
          vector <int>index;
          //differences
          int value = 1;
          for(int k=0 ; k < freq[knapscakData.suiteNames[i]].str.size() ; k++){
              if(k != freq[knapscakData.suiteNames[i]].maxElementIndex){
                indexSet.push_back(distanceCalc(freq[knapscakData.suiteNames[i]].str[freq[knapscakData.suiteNames[i]].maxElementIndex],
                freq[knapscakData.suiteNames[i]].str[k]
                ) );  
                index.push_back(value);
              }
              value++;
              
          }

          bool swap = true;
          while(swap){
            swap = false;
              for (size_t i = 0; i < indexSet.size()-1; i++) {
                if (indexSet[i]>indexSet[i+1] ){
                  indexSet[i] += indexSet[i+1];
                  index[i] += index[i+1];
                  indexSet[i+1] = indexSet[i] - indexSet[i+1];
                  index[i+1] = index[i] - index[i+1];
                  indexSet[i] -=indexSet[i+1];
                  index[i] -=index[i+1];
                  swap = true;
                }
              }
          }
          cout << " " << freq[knapscakData.suiteNames[i]].maxElementIndex+1 << " " ;
          for( int i = index.size()-1 ; i >= 0; --i){
            cout << " " << index[i] << " " ;

          }     
          cout << endl;     

      }

    }

    return 0;
} 