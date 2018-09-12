// Copyright 2018 Ralf Stubner and Victor Reijs
//
// This file is part of swephR.
//
// swephR is free software: you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// swephR is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with swephR.  If not, see <http://www.gnu.org/licenses/>.

#include <Rcpp.h>
#include <array>
#include "swephapi.h"


// Compute information of planet (UT)
// [[Rcpp::export]]
Rcpp::List calc_ut(Rcpp::NumericVector tjd_ut, Rcpp::IntegerVector ipl, int iflag) {
  if (tjd_ut.length() != ipl.length())
    Rcpp::stop("The number of bodies in 'ipl' and the number of dates in 'tjd_ut' must be identical!");

  Rcpp::IntegerVector rc_(ipl.length());
  Rcpp::CharacterVector serr_(ipl.length());
  Rcpp::NumericMatrix xx_(ipl.length(), 6);

  for (int i = 0; i < ipl.length(); ++i) {
    std::array<double, 6> xx{0.0};
    std::array<char, 256> serr{'\0'};
    rc_(i) = swe_calc_ut(tjd_ut[i], ipl(i), iflag, xx.begin(), serr.begin());
    Rcpp::NumericVector tmp(xx.begin(), xx.end());
    xx_(i, Rcpp::_) = tmp;
    serr_(i) = std::string(serr.begin());
  }

  // remove dim attribute to return a vector
  if (ipl.length() == 1)
    xx_.attr("dim") = R_NilValue;

  return Rcpp::List::create(Rcpp::Named("return") = rc_,
			    Rcpp::Named("xx") = xx_,
			    Rcpp::Named("serr") = serr_);
}

// Compute information of planet (ET)
// [[Rcpp::export]]
Rcpp::List calc(Rcpp::NumericVector tjd_et, Rcpp::IntegerVector ipl, int iflag) {
  if (tjd_et.length() != ipl.length())
    Rcpp::stop("The number of bodies in 'ipl' and the number of dates in 'tjd_et' must be identical!");

  Rcpp::IntegerVector rc_(ipl.length());
  Rcpp::CharacterVector serr_(ipl.length());
  Rcpp::NumericMatrix xx_(ipl.length(), 6);

  for (int i = 0; i < ipl.length(); ++i) {
    std::array<double, 6> xx{0.0};
    std::array<char, 256> serr{'\0'};
    rc_(i) = swe_calc(tjd_et[i], ipl(i), iflag, xx.begin(), serr.begin());
    Rcpp::NumericVector tmp(xx.begin(), xx.end());
    xx_(i, Rcpp::_) = tmp;
    serr_(i) = std::string(serr.begin());
  }
  // remove dim attribute to return a vector
  if (ipl.length() == 1)
    xx_.attr("dim") = R_NilValue;

  return Rcpp::List::create(Rcpp::Named("return") = rc_,
                            Rcpp::Named("xx") = xx_,
                            Rcpp::Named("serr") = serr_);
}

// Compute information of star (UT)
// [[Rcpp::export]]
Rcpp::List fixstar2_ut(Rcpp::CharacterVector star, Rcpp::NumericVector tjd_ut, int iflag) {
  if (tjd_ut.length() != star.length())
    Rcpp::stop("The number of stars in 'star' and the number of dates in 'tjd_ut' must be identical!");

  Rcpp::IntegerVector rc_(star.length());
  Rcpp::CharacterVector serr_(star.length());
  Rcpp::NumericMatrix xx_(star.length(), 6);

  for (int i = 0; i < star.length(); ++i) {
    std::array<double, 6> xx{0.0};
    std::array<char, 256> serr{'\0'};
    std::string star_(star(i));
    star_.resize(41);
    rc_(i) = swe_fixstar2_ut(&star_[0], tjd_ut(i), iflag, xx.begin(), serr.begin());
    Rcpp::NumericVector tmp(xx.begin(), xx.end());
    xx_(i, Rcpp::_) = tmp;
    serr_(i) = std::string(serr.begin());
    star(i) = star_;
  }

  // remove dim attribute to return a vector
  if (star.length() == 1)
    xx_.attr("dim") = R_NilValue;

  return Rcpp::List::create(Rcpp::Named("return") = rc_,
                            Rcpp::Named("star") = star,
                            Rcpp::Named("xx") = xx_,
                            Rcpp::Named("serr") = serr_);
}


// Compute information of star
// [[Rcpp::export]]
Rcpp::List fixstar2(Rcpp::CharacterVector star, Rcpp::NumericVector tjd_et, int iflag) {
  if (tjd_et.length() != star.length())
    Rcpp::stop("The number of stars in 'star' and the number of dates in 'tjd_et' must be identical!");

  Rcpp::IntegerVector rc_(star.length());
  Rcpp::CharacterVector serr_(star.length());
  Rcpp::NumericMatrix xx_(star.length(), 6);

  for (int i = 0; i < star.length(); ++i) {
    std::array<double, 6> xx{0.0};
    std::array<char, 256> serr{'\0'};
    std::string star_(star(i));
    star_.resize(41);
    rc_(i) = swe_fixstar2(&star_[0], tjd_et(i), iflag, xx.begin(), serr.begin());
    Rcpp::NumericVector tmp(xx.begin(), xx.end());
    xx_(i, Rcpp::_) = tmp;
    serr_(i) = std::string(serr.begin());
    star(i) = star_;
  }

  // remove dim attribute to return a vector
  if (star.length() == 1)
    xx_.attr("dim") = R_NilValue;

  return Rcpp::List::create(Rcpp::Named("return") = rc_,
                            Rcpp::Named("star") = star,
                            Rcpp::Named("xx") = xx_,
                            Rcpp::Named("serr") = serr_);
}

//' @title Calculation of planets and stars
//' @param tjd_ut  Julian day, UT time
//' @param tjd_et  Julian day, Ephemeris time
//' @param ipl  body/planet number (-1 for no planet possible with \code{swe_rise_trans_true_hor})
//' @param iflag  a 32 bit integer containing bit flags that indicate what
//'               kind of computation is wanted
//' @param star  star name
//' Compute the magnitude of star
//' @return \code{swe_fixstar2_mag} returns a list with named entries \code{return},
//'         \code{star} updated star name, \code{mag} magnitude of star, and \code{serr} for error message.
//' @rdname calculation
//' @export
// [[Rcpp::export(swe_fixstar2_mag)]]
Rcpp::List fixstar2_mag(Rcpp::CharacterVector star) {
  Rcpp::IntegerVector rc_(star.length());
  Rcpp::CharacterVector serr_(star.length());
  Rcpp::NumericVector mag_(star.length());

  for (int i = 0; i < star.length(); ++i) {
    double mag;
    std::array<char, 256> serr{'\0'};
    std::string star_(star(i));
    star_.resize(41);
    rc_(i) = swe_fixstar2_mag(&star_[0], &mag, serr.begin());
    mag_(i) = mag;
    serr_(i) = std::string(serr.begin());
    star(i) = star_;
  }

  return Rcpp::List::create(Rcpp::Named("return") = rc_,
                            Rcpp::Named("star") = star,
                            Rcpp::Named("mag") = mag_,
                            Rcpp::Named("serr") = serr_);
}

//' Set the topocentric location (lon, lat, height)
//' @rdname calculation
//' @param geolon  Topocentric Longitude (deg)
//' @param geolat  Topocentric Latitude (deg)
//' @param altitude  the height (m)
//' @export
// [[Rcpp::export(swe_set_topo)]]
void set_topo(double geolon, double geolat, double altitude) {
  swe_set_topo(geolon, geolat, altitude);
}
