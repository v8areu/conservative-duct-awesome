#include "../global.hpp"
#include "main_final.hpp"

//calculate dU_dt_average (time derivative average)
std::vector<double> Main_Final::calc_dU_dt_average(Parameters pars, std::vector<double> dU_dt_predictor, std::vector<double> dU_dt_corrector) {

	//local pars
	const int max_node	=	pars.max_node;

	//processed variable
	std::vector<double> dU_dt_average(max_node);

	for (auto i = 1; i < max_node - 1; i++) {
		
		dU_dt_average[i]	=	0.5*(dU_dt_predictor[i] + dU_dt_corrector[i]);
	}

	return dU_dt_average;
}

//calculate new v
std::vector<double> Main_Final::calc_new_v(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;

	//local vars
	std::vector<double> U_1	=	vars.U_1;
	std::vector<double> U_2	=	vars.U_2;

	//processed variable
	std::vector<double> v(max_node);

	for (auto i = 0; i < max_node; i++) {
		
		v[i]	=	U_2[i]/U_1[i];
	}

	return v;
}

//error calculation
double Main_Final::calc_error(std::vector<double> old_F, std::vector<double> new_F) {

	//get the size
	const int max_node	=	old_F.size();

	//processed variable
	double error_var = 0;

	for (auto i = 0; i < max_node; i++) {
	
		if (old_F[i] != 0) {
			error_var = error_var + std::abs(old_F[i] - new_F[i])/old_F[i];
		} else {
			error_var = error_var;
		}

	}
	
	return error_var;
}

//mach calculation
std::vector<double> Main_Final::calc_mach(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;

	//local vars
	std::vector<double> v		=	vars.v;
	std::vector<double> sound_speed	=	vars.sound_speed;

	//processed variable
	std::vector<double> mach(max_node);

	for (auto i = 0; i < max_node; i++) {
		
		mach[i]	=	v[i]/sound_speed[i];
	}

	return mach;
}

//pressure calculation
std::vector<double> Main_Final::calc_pressure(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;
	const double gamma	=	pars.gamma;

	//local vars
	std::vector<double> T		=	vars.T;

	//processed variable
	std::vector<double> pressure(max_node);

	double temp	=	gamma/(gamma - 1);
	for (auto i = 0; i < max_node; i++) {
	
		pressure[i]	=	pow(T[i], temp);
	}

	return pressure;
}

//mass flow calculation
std::vector<double> Main_Final::calc_mass_flow(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;

	//local vars
	std::vector<double> v		=	vars.v;
	std::vector<double> rho		=	vars.rho;
	std::vector<double> area	=	vars.area;

	//processed variable
	std::vector<double> mass_flow(max_node);

	for (auto i = 0; i < max_node; i++) {
	
		mass_flow[i]	=	v[i]*rho[i]*area[i];
	}

	return mass_flow;
}

//error_rms calculation
double Main_Final::calc_error_rms(std::vector<double> old_F, std::vector<double> new_F) {

	//get the size
	const int max_node	=	old_F.size();

	//processed variable
	double error_rms_var = 0;

	//square
	for (auto i = 0; i < max_node; i++) {
		
		if (old_F[i] != 0) {
			error_rms_var = error_rms_var + pow(((old_F[i] - new_F[i])/old_F[i]),2);
		} else {
			error_rms_var = error_rms_var;
		}
	
	}

	//mean
	error_rms_var = error_rms_var/max_node;

	//root
	error_rms_var = sqrt(error_rms_var);
	
	return error_rms_var;
}

//pressure_v2 calculation
std::vector<double> Main_Final::calc_pressure_v2(Parameters pars, Variables vars) {

	//local pars
	const int max_node	=	pars.max_node;
	const double gamma	=	pars.gamma;

	//local vars
	std::vector<double> v		=	vars.v;
	std::vector<double> U_2		=	vars.U_2;
	std::vector<double> U_3		=	vars.U_3;
	std::vector<double> area	=	vars.area;

	//processed variable
	std::vector<double> pressure_v2(max_node);

	for (auto i = 0; i < max_node; i++) {
		double temp_1	=	U_3[i] - 0.5*gamma*U_2[i]*v[i];
		double temp_2	=	area[i] / (gamma - 1);

		pressure_v2[i]	=	temp_1/temp_2;
	
	}

	return pressure_v2;
}
