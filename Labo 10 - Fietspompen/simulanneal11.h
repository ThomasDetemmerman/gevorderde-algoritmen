#ifndef __SIMULANNEAL
#define __SIMULANNEAL
#include <cstdlib>
#include <memory>

//abstracte klasse voor simulated annealing
//Hier worden GEEN probleemafhankelijke functies gedefinieerd
//deze zijn allemaal virtual.
//Notaties: zie Blum and Roli
//Parameter S: De oplossing (zowel de voorlopige als de uiteindelijke)
//is een S. S staat voor Search space


template <class S>
class SimulatedAnnealing{
public:
    S oplossing(){
        T=T0();
        unsigned int iter=0;
        s=initialSolution();
        best=s;
        double fbest=f(best);
//        std::cout <<" Xfbest "<<fbest<<"\n";
        double fs=fbest;
        while (!terminateCond()){
            iter++;
            S sprime=pickAtRandom(s);
            double fsprime=f(sprime);
            if (fsprime < fbest){
//                std::cout <<"oude fbest "<<fbest  <<" fsprime "<<fsprime   <<" iter "<<iter <<" T "<<T   <<"\n";
                best=sprime;
                fbest=fsprime;
            }
                
            double deltaf=fsprime-fs;
            if (deltaf < 0 || rand()<p(T,deltaf)*RAND_MAX){
                s=std::move(sprime);
                fs=fsprime;
            }            
            updateT();
        }
        return best;
    }

protected:
    double T;//temperatuur;
    S s;//voorlopige en eindoplossing.
    S best;
    virtual double T0()=0;//initi"ele temperatuur
    virtual bool terminateCond()=0;//true als stoppen met zoeken
    virtual double f(const S&)=0;//objectieffunctie. Moet > 0 blijven
    virtual S initialSolution()=0;
    virtual S pickAtRandom(const S&)=0;
    virtual double p(double T, double deltaf)=0;//probabiliteit verandering
    virtual void updateT()=0;
};

#endif
