/*                             -*- C++ -*-
 * Copyright (C) 2016 Felix Salfelder
 * Author: same
 *
 * This file is part of "Gnucap", the Gnu Circuit Analysis Package
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

// implements (a)gauss, (a)unif and limit as per ngspice manual.
/*--------------------------------------------------------------------------*/
#include <u_parameter.h>
#include <u_function.h>
#include <globals.h>
#include <gsl/gsl_randist.h>

// -uf compat hack
#ifndef _U_FUNC
typedef std::string fun_t;
#define to_fun_t to_string
#endif
/*--------------------------------------------------------------------------*/
namespace{
/*--------------------------------------------------------------------------*/
struct myrng{
  myrng(){
    const gsl_rng_type * T;
    T = gsl_rng_default;

    gsl_rng_env_setup();
    _rng = gsl_rng_alloc(T);
  }
  gsl_rng *_rng;
} rng;
/*--------------------------------------------------------------------------*/
class agauss : public FUNCTION {
public:
	fun_t eval(CS& Cmd, const CARD_LIST* Scope)const
	{
		PARAMETER<double> mean;
		PARAMETER<double> abs_dev_in; // standard deviation. NOT variance
		PARAMETER<double> sigma; // some weird scaling. NOT standard deviation
		Cmd >> mean >> abs_dev_in;
		if(!Cmd.more()){ untested();
		}else{ untested();
			Cmd >> sigma;
			sigma.e_val(NOT_INPUT, Scope);
		}
		mean.e_val(NOT_INPUT, Scope);
		abs_dev_in.e_val(NOT_INPUT, Scope);
		double abs_dev = abs_dev_in;

		if(!sigma.has_hard_value()){ untested();
		}else if(double(sigma)==0){
			incomplete();
		}else{ untested();
			abs_dev = abs_dev/sigma;
		}

		return to_fun_t(gsl_ran_gaussian(rng._rng, abs_dev) + mean);
	}
private:
} p_gauss;
DISPATCHER<FUNCTION>::INSTALL d_gauss(&function_dispatcher, "agauss", &p_gauss);
/*--------------------------------------------------------------------------*/
class rgauss : public FUNCTION {
public:
	fun_t eval(CS& Cmd, const CARD_LIST* Scope)const
	{
		PARAMETER<double> mean;
		PARAMETER<double> rel_dev; // relative std deviation. NOT variance
		PARAMETER<double> sigma; // some weird scaling. NOT standard deviation
		Cmd >> mean >> rel_dev;
		mean.e_val(NOT_INPUT, Scope);
		rel_dev.e_val(NOT_INPUT, Scope);
		if(!Cmd.more()){ untested();
		}else{ untested();
			Cmd >> sigma;
			sigma.e_val(NOT_INPUT, Scope);
		}
		mean.e_val(NOT_INPUT, Scope);

		double abs_dev = 0;

		if(!mean.has_hard_value()){
			incomplete();
		}else{ untested();
			abs_dev = rel_dev*mean;
		}

		if(!sigma.has_hard_value()){ untested();
		}else if(double(sigma)==0){
			incomplete();
		}else{
			abs_dev = abs_dev/sigma;
		}

		return to_fun_t(gsl_ran_gaussian(rng._rng, abs_dev) + mean);
	}
private:
} p_rgauss;
DISPATCHER<FUNCTION>::INSTALL d_rgauss(&function_dispatcher, "gauss|rgauss", &p_rgauss);
/*--------------------------------------------------------------------------*/
class runif : public FUNCTION {
public:
	fun_t eval(CS& Cmd, const CARD_LIST* Scope)const
	{
		PARAMETER<double> mean;
		PARAMETER<double> rel_dev; // relative std deviation. NOT variance
		Cmd >> mean >> rel_dev;

		mean.e_val(NOT_INPUT, Scope);
		rel_dev.e_val(NOT_INPUT, Scope);
		double abs_dev = 0;

		if(!mean.has_hard_value()){
			incomplete();
		}else{ untested();
			abs_dev = rel_dev*mean;
		}

		double rnd=gsl_rng_uniform_pos (rng._rng);
		rnd *= 2;
		rnd -= 1;

		return to_fun_t( rnd*abs_dev + mean);
	}
private:
} p_runif;
DISPATCHER<FUNCTION>::INSTALL d_runif(&function_dispatcher, "unif|runif", &p_runif);
/*--------------------------------------------------------------------------*/
class aunif : public FUNCTION {
public:
	fun_t eval(CS& Cmd, const CARD_LIST* Scope)const
	{
		PARAMETER<double> mean;
		PARAMETER<double> abs_dev; // relative std deviation. NOT variance
		Cmd >> mean >> abs_dev;

		mean.e_val(NOT_INPUT, Scope);
		abs_dev.e_val(NOT_INPUT, Scope);

		double rnd=gsl_rng_uniform_pos (rng._rng);
		rnd *= 2;
		rnd -= 1;

		return to_fun_t( rnd*abs_dev + mean);
	}
private:
} p_aunif;
DISPATCHER<FUNCTION>::INSTALL d_aunif(&function_dispatcher, "aunif", &p_aunif);
/*--------------------------------------------------------------------------*/
class limit : public FUNCTION {
public:
	fun_t eval(CS& Cmd, const CARD_LIST* Scope)const
	{
		PARAMETER<double> mean;
		PARAMETER<double> abs_dev; // relative std deviation. NOT variance
		Cmd >> mean >> abs_dev;

		mean.e_val(NOT_INPUT, Scope);
		abs_dev.e_val(NOT_INPUT, Scope);

		if(gsl_rng_uniform_int(rng._rng, 2)){
			abs_dev =- abs_dev;
		}

		return to_fun_t( abs_dev + mean);
	}
private:
} p_limit;
DISPATCHER<FUNCTION>::INSTALL d_limit(&function_dispatcher, "limit", &p_limit);
/*--------------------------------------------------------------------------*/
}
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
