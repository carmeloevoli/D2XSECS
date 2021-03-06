// Copyright (c) 2017 Carmelo Evoli - MIT License
#ifndef INCLUDE_XS4GCR_LEPTONS_LEPTONS_H_
#define INCLUDE_XS4GCR_LEPTONS_LEPTONS_H_

#include <cmath>
#include <memory>
#include <string>

#include "XS4GCR/cgs.h"
#include "XS4GCR/grid.h"
#include "XS4GCR/pid.h"

namespace XS4GCR {

class Secondary_Leptons {
   public:
    virtual ~Secondary_Leptons() {}

    virtual void print() const = 0;

    virtual std::shared_ptr<Secondary_Leptons> clone() = 0;

    virtual double get(const PID &projectile, const TARGET &target, const double &T_n,
                       const double &T_lepton) const = 0;

    virtual double annihilation_xsec(const PID &target, const double &T_electron) const;

   protected:
    PID lepton;
    std::string model_name;

    inline void set_model_name(const std::string &m) { model_name = m; }
};

class Kamae2006_Secondary_Leptons : public Secondary_Leptons {
   public:
    explicit Kamae2006_Secondary_Leptons(const PID &lepton);

    void print() const override;

    std::shared_ptr<Secondary_Leptons> clone() override;

    double get(const PID &projectile, const TARGET &target, const double &T_n,
               const double &T_lepton) const override;

    double He_function(const double &T_n, const double &T_lepton) const;

   private:
    double He_correction = std::pow(4., 2.2 / 3.);
    double HeHe_correction = std::pow(4. * 4., 2.2 / 3.);

    double get_cparamlib_sigma(const PID &lepton_, const double &T_proton_GeV_,
                               const double &T_lepton_GeV_) const;
};

class HuangPohl2007_Secondary_Leptons : public Secondary_Leptons {
   public:
    explicit HuangPohl2007_Secondary_Leptons(const PID &lepton);

    void print() const override;

    std::shared_ptr<Secondary_Leptons> clone() override;

    double get(const PID &projectile, const TARGET &target, const double &T_n,
               const double &T_lepton) const override;

   private:
    std::string eminus_p_datafile = "data/espectra_eminus.decay.p.matrix.data";
    std::string eminus_he_datafile = "data/espectra_eminus.decay.he.matrix.data";
    std::string eplus_p_datafile = "data/espectra_eplus.decay.p.matrix.data";
    std::string eplus_he_datafile = "data/espectra_eplus.decay.he.matrix.data";
    std::string prodxsec_p_datafile = "data/prodxsection.p.matrix.data";
    std::string prodxsec_he_datafile = "data/prodxsection.he.matrix.data";
    double E_proj_min = 1.24 * pow(1.05, 1.) * cgs::GeV;
    double E_proj_max = 1.24 * pow(1.05, 374.) * cgs::GeV;
    size_t E_proj_size = 374;
    double T_lepton_min = 1.058950e-02 * cgs::GeV;
    double T_lepton_max = 9.443313e+07 * cgs::GeV;
    size_t T_lepton_size = 201;
    grid2d dsigma_dT_H;
    grid2d dsigma_dT_He;

    void init();

    void read_data_file();

    void check_datafiles_exist();

    void resize_sigma_grids();
};

}  // namespace XS4GCR

#endif  // INCLUDE_XS4GCR_LEPTONS_LEPTONS_H_
