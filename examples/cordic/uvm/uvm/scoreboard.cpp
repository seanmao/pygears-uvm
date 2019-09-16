#include <iostream>
#include "scoreboard.hpp"
#include <systemc>
#include <cmath>

#include <iostream>
#include <sstream>
#include <string>

#define W_PHASE 16
#define W_SIN 12
#define W_COS 12

double round_to(double val, unsigned int digit){
  unsigned int tmp = pow(10, digit);

  return roundf(val * tmp) / tmp;
}

bool cmp_val(double ref, double dut, int digit){
  double ref_rnd = round_to(ref, digit);
  double dut_rnd = round_to(dut, digit);

  if( ref_rnd != dut_rnd ){
    return false;
  }
  return true;
}

std::ostringstream display_cmp(double ref, double dut){
  std::ostringstream str;
  str << "expected : " << ref << " received: " << dut;
  return str;
}

void scoreboard::check(){
  int i = 0;
  unsigned int pi = pow(2, 16) / 2;

  while(true){
    sc_core::wait(sin_e & cos_e);
    unsigned int phase = phase_data[i];
    i++;
    int sinus = sin_data.back();
    int cosinus = cos_data.back();

    double phase_rad = ((double)phase / pi) * M_PI;
    double ref_sin = sin(phase_rad);
    double ref_cos = cos(phase_rad);

    double dut_sin = (double)sinus / (pow(2,W_SIN-1) / 2);
    double dut_cos = (double)cosinus / (pow(2,W_COS-1) / 2);

    if(!cmp_val(ref_sin, dut_sin, 2)){
      UVM_INFO("Mismatch on sin", display_cmp(ref_sin, dut_sin).str() , uvm::UVM_LOW);
    }
    else{
        UVM_INFO("Match on sin", display_cmp(ref_sin, dut_sin).str() , uvm::UVM_LOW);
    }

    if(!cmp_val(ref_cos, dut_cos, 2)){
      UVM_INFO("Mismatch on cos", display_cmp(ref_cos, dut_cos).str() , uvm::UVM_LOW);
    }
    else{
      UVM_INFO("Match on cos", display_cmp(ref_cos, dut_cos).str() , uvm::UVM_LOW);
    }

  }

}
