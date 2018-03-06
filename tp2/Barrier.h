#include <condition_variable>
#include <mutex>  

#ifndef B_H
#define	B_H

class Barrier
{
private:
  std::mutex m;
  std::condition_variable cv;
  unsigned int nbThreads;
  unsigned int tmpNbThreads;
  unsigned int nbGenerations;
  
public:
  explicit Barrier(unsigned int p_nbThreads): 
    nbThreads(p_nbThreads), 
    tmpNbThreads(p_nbThreads),
    nbGenerations(p_nbThreads)
    {}
  
    //Return true if the thread is the last thread to call wait 
    bool wait()
    {
      std::unique_lock<std::mutex> lock{m};
      unsigned int tmpNbGenerations = nbGenerations;
      
      if (--tmpNbThreads==0){
        nbGenerations++;//This generation is finished
        tmpNbThreads=nbThreads;//Fix the good number of threads for the next generation
        cv.notify_all();
        return true;
      }
      //Sleep while the generation has not changed
      cv.wait(lock,[&tmpNbGenerations,this] {return tmpNbGenerations != nbGenerations;});
      return false;
    }
};
#endif
