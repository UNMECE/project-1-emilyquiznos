#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>  

struct Capacitor {
    double *time; //time array
    double *voltage; //voltage array
    double *current; //current array
    double C;  // capacitance value
};


void constant_current_charge(Capacitor&cap,double I,double dt,int steps){
    for (int t=1;t<steps;++t) {
        cap.time[t]=t*dt;
        cap.voltage[t]=cap.voltage[t-1]+I*dt/cap.C;
        if (t%200==0){
            std::cout<<"Timestep: "<<t<<"Time: "<<cap.time[t]<<" s"<<"Voltage: "<<cap.voltage[t]<<" V"<<std::endl;
        }
    }
}


void constant_voltage_charge(Capacitor&cap,double V0,double R,double dt,int steps){
    for (int t=1;t<steps;++t){
        cap.time[t]=t*dt;

        cap.current[t]=cap.current[t-1]-(cap.current[t-1]*dt/(R*cap.C));
		cap.voltage[t]=V0*(1-exp(-cap.time[t]/(R*cap.C)));
		// NaN is not a number
		//this was placed because previously, it wasn't able to print to max timestep
		//output was just NaN
        if (std::isnan(cap.current[t])) {
            std::cerr<<"Current became NaN at timestep"<<t<<std::endl;
            return;
        }

        if (t%200==0){
            std::cout<<"Timestep: "<<t<<"Time: "<<cap.time[t]<<" s"<<"Current: "<<cap.current[t]<<" A"<<std::endl;
        }
    }
}

int main(){
	//givens from instructions
    const double dt=1e-10;  
    const double final_time=5e-6; 
    const int steps=static_cast<int>(final_time/dt);  
    const double R=1e3;  
    const double C=100e-12;
    const double I=1e-2;  
    const double V0=10.0; 
    

    Capacitor cap;
    cap.time=(double*)malloc(steps*sizeof(double));
    cap.voltage=(double*)malloc(steps*sizeof(double));
    cap.current=(double*)malloc(steps * sizeof(double));
    cap.C=C;

    if (!cap.time||!cap.voltage||!cap.current) {
        std::cerr<<"Memory allocation did not process"<<std::endl;
		free(cap.time);
		free(cap.voltage);
		free(cap.current);
        return 1;
    }

    cap.time[0]=0.0; // intitial condition for time
    cap.voltage[0]=0.0;  // intitial condition for voltage
    cap.current[0]=V0/R; // initial condition for current

    std::cout<<"Constant Current Charging: "<< std::endl;
    constant_current_charge(cap,I,dt,steps);

    std::cout<<"\nConstant Voltage Charging: "<< std::endl;
    constant_voltage_charge(cap,V0,R,dt,steps);
	
    free(cap.time);
    free(cap.voltage);
    free(cap.current);

    return 0;
}
