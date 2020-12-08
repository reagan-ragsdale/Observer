#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

// base class used by classes that do OBSERVING (i.e., a "display")!
class Observer {
  public:
    virtual void update(int runs, int hits, int wins, int losses, float win_average)=0;
};

class Subject {
  public:
    virtual void registerObserver(Observer* o)=0;
    virtual void unregisterObserver(Observer* o)=0;
    virtual void notifyObservers()=0;
};

class SportsData:public Subject {
  private:
    int runs;
    int hits;
    int wins;
    int losses;
    float win_average;
    vector<Observer*> observerList;

    void notifyObservers() {
      for (int i = 0; i < observerList.size(); i++ ) {
        if (observerList.at(i) != nullptr) {
          observerList.at(i)->update(runs,hits,wins,losses,win_average);
        }
      }
    }
    
    int getLatestRuns() {
      int value = rand() % 10;
      return (value > hits) ? hits : value;
    }

    int getLatestHits() {
      return (rand() % 10);
    }

    int getLatestWins() {
      return (rand() % 10);
    }

    int getLatestLosses() {
      return (rand() % 10);
    }

    float getLatestWinAverage() {
      return floorf(((float) wins) / (float)(losses) * 1000) / 1000;
    }

  public:
    SportsData() {
        srand(time(0));  // Initialize random number generator.
    }

    void registerObserver(Observer* o) {
      observerList.push_back(o);
    }

    void unregisterObserver(Observer* o) {
      for (int i = 0; i < observerList.size(); i++ ) {
        if (observerList.at(i) == o) {
          observerList.erase(observerList.begin()+i);
        }
      }
    }

    // This method is used update data and displays
    void dataChanged() {
      
      //get latest data
      hits = getLatestHits();
      runs = getLatestRuns();
      wins = getLatestWins();
      losses = getLatestLosses();
      win_average = getLatestWinAverage();

      notifyObservers();
    }

};

class CalcScoreDisplay: public Observer {
  private:
    float runRate;

  public:
    void update(int runs, int hits, int wins, int losses, float win_average) {
                         
      runRate = (hits > 0)
       ? (ceilf(((float)runs /  (float)hits)*1000)/1000) 
       : 0;
      display();
    }

    void display() {
      cout << "\nRuns Per Hit: \n";
      cout << runRate << "\n";
    }
};

class CurrentScoreDisplay: public Observer {
  private:
    int runs, hits, wins, losses;
    float win_average;

  public:
    void update(int new_runs, int new_hits, int new_wins, int new_losses,
                       float new_win_average) {
      runs = new_runs;
      hits = new_hits;
      wins = new_wins;
      losses = new_losses;
      win_average = new_win_average;
      display();
    }

    void display() {
      cout << "\nCurrent Score Display:\n";
      cout << "Runs: " << runs;
      cout << "\nHits:" << hits;
      cout << "\nWins:" << wins;
      cout << "\nLosses:" << losses;
      cout << "\nWin Average: " << win_average << "\n";
    }
};

int main() {
  // create objects for display
  CalcScoreDisplay calcScoreDisplay = CalcScoreDisplay();
  CurrentScoreDisplay currentScoreDisplay = CurrentScoreDisplay();

  SportsData sportsData = SportsData();

  // register display elements
  sportsData.registerObserver(&calcScoreDisplay);
  sportsData.registerObserver(&currentScoreDisplay);

  sportsData.dataChanged();

  //remove an observer and re-run dataChanged()
  //sportsData.unregisterObserver(&currentScoreDisplay);
  //sportsData.unregisterObserver(&calcScoreDisplay);
  //sportsData.dataChanged();

} 