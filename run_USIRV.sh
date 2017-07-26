#!/bin/sh

# Run without lower bounding rules (MRSW)
./marginirv -ballots USIRV/Aspen_2009_CityCouncil.txt -logfile  log_Aspen_2009_CityCouncil.txt   -tlimit 500 >  margin_Aspen_2009_CityCouncil.txt

./marginirv -ballots USIRV/Aspen_2009_Mayor.txt -logfile  log_Aspen_2009_Mayor.txt   -tlimit 500  >  margin_Aspen_2009_Mayor.txt

./marginirv -ballots USIRV/Berkeley_2010_D1CityCouncil.txt -logfile  log_Berkeley_2010_D1CityCouncil.txt  -tlimit 500   >  margin_Berkeley_2010_D1CityCouncil.txt

./marginirv -ballots USIRV/Berkeley_2010_D4CityCouncil.txt -logfile  log_Berkeley_2010_D4CityCouncil.txt   -tlimit 500  >  margin_Berkeley_2010_D4CityCouncil.txt

./marginirv -ballots USIRV/Berkeley_2010_D7CityCouncil.txt -logfile  log_Berkeley_2010_D7CityCouncil.txt  -tlimit 500   >  margin_Berkeley_2010_D7CityCouncil.txt

./marginirv -ballots USIRV/Berkeley_2010_D8CityCouncil.txt -logfile  log_Berkeley_2010_D8CityCouncil.txt  -tlimit 500   >  margin_Berkeley_2010_D8CityCouncil.txt

./marginirv -ballots USIRV/Oakland_2010_D4CityCouncil.txt -logfile  log_Oakland_2010_D4CityCouncil.txt  -tlimit 500   >  margin_Oakland_2010_D4CityCouncil.txt

./marginirv -ballots USIRV/Oakland_2010_D6CityCouncil.txt -logfile  log_Oakland_2010_D6CityCouncil.txt   -tlimit 500  >  margin_Oakland_2010_D6CityCouncil.txt

./marginirv -ballots USIRV/Oakland_2010_Mayor.txt -logfile  log_Oakland_2010_Mayor.txt  -tlimit 500   >  margin_Oakland_2010_Mayor.txt

./marginirv -ballots USIRV/Pierce_2008_CityCouncil.txt -logfile  log_Pierce_2008_CityCouncil.txt  -tlimit 500   >  margin_Pierce_2008_CityCouncil.txt

./marginirv -ballots USIRV/Pierce_2008_CountyAuditor.txt -logfile  log_Pierce_2008_CountyAuditor.txt   -tlimit 500  >  margin_Pierce_2008_CountyAuditor.txt

./marginirv -ballots USIRV/Pierce_2008_CountyAssessor.txt -logfile  log_Pierce_2008_CountyAssessor.txt  -tlimit 500   >  margin_Pierce_2008_CountyAssessor.txt

./marginirv -ballots USIRV/Pierce_2008_CountyExecutive.txt -logfile  log_Pierce_2008_CountyExecutive.txt   -tlimit 500  >  margin_Pierce_2008_CountyExecutive.txt

./marginirv -ballots USIRV/SanFran_2007_Mayor.txt -logfile  log_SanFran_2007_Mayor.txt  -tlimit 500   >  margin_SanFran_2007_Mayor.txt

# Run with basic lower bounding rule
./marginirv -score -ballots USIRV/Aspen_2009_CityCouncil.txt -logfile  sc_log_Aspen_2009_CityCouncil.txt   -tlimit 500 >  sc_margin_Aspen_2009_CityCouncil.txt

./marginirv -score -ballots USIRV/Aspen_2009_Mayor.txt -logfile  sc_log_Aspen_2009_Mayor.txt   -tlimit 500  >  sc_margin_Aspen_2009_Mayor.txt

./marginirv -score -ballots USIRV/Berkeley_2010_D1CityCouncil.txt -logfile  sc_log_Berkeley_2010_D1CityCouncil.txt  -tlimit 500   >  sc_margin_Berkeley_2010_D1CityCouncil.txt

./marginirv -score -ballots USIRV/Berkeley_2010_D4CityCouncil.txt -logfile  sc_log_Berkeley_2010_D4CityCouncil.txt   -tlimit 500  >  sc_margin_Berkeley_2010_D4CityCouncil.txt

./marginirv -score -ballots USIRV/Berkeley_2010_D7CityCouncil.txt -logfile  sc_log_Berkeley_2010_D7CityCouncil.txt  -tlimit 500   >  sc_margin_Berkeley_2010_D7CityCouncil.txt

./marginirv -score -ballots USIRV/Berkeley_2010_D8CityCouncil.txt -logfile  sc_log_Berkeley_2010_D8CityCouncil.txt  -tlimit 500   >  sc_margin_Berkeley_2010_D8CityCouncil.txt

./marginirv -score -ballots USIRV/Oakland_2010_D4CityCouncil.txt -logfile  sc_log_Oakland_2010_D4CityCouncil.txt  -tlimit 500   >  sc_margin_Oakland_2010_D4CityCouncil.txt

./marginirv -score -ballots USIRV/Oakland_2010_D6CityCouncil.txt -logfile  sc_log_Oakland_2010_D6CityCouncil.txt   -tlimit 500  >  sc_margin_Oakland_2010_D6CityCouncil.txt

./marginirv -score -ballots USIRV/Oakland_2010_Mayor.txt -logfile  sc_log_Oakland_2010_Mayor.txt  -tlimit 500   >  sc_margin_Oakland_2010_Mayor.txt

./marginirv -score -ballots USIRV/Pierce_2008_CityCouncil.txt -logfile  sc_log_Pierce_2008_CityCouncil.txt  -tlimit 500   >  sc_margin_Pierce_2008_CityCouncil.txt

./marginirv -score -ballots USIRV/Pierce_2008_CountyAuditor.txt -logfile  sc_log_Pierce_2008_CountyAuditor.txt   -tlimit 500  >  sc_margin_Pierce_2008_CountyAuditor.txt

./marginirv -score -ballots USIRV/Pierce_2008_CountyAssessor.txt -logfile  sc_log_Pierce_2008_CountyAssessor.txt  -tlimit 500   >  sc_margin_Pierce_2008_CountyAssessor.txt

./marginirv -score -ballots USIRV/Pierce_2008_CountyExecutive.txt -logfile  sc_log_Pierce_2008_CountyExecutive.txt   -tlimit 500  >  sc_margin_Pierce_2008_CountyExecutive.txt

./marginirv -score -ballots USIRV/SanFran_2007_Mayor.txt -logfile  sc_log_SanFran_2007_Mayor.txt  -tlimit 500   >  sc_margin_SanFran_2007_Mayor.txt


# Run with tightest lower bounding rule
./marginirv -tight -ballots USIRV/Aspen_2009_CityCouncil.txt -logfile  tg_log_Aspen_2009_CityCouncil.txt   -tlimit 500 >  tg_margin_Aspen_2009_CityCouncil.txt

./marginirv -tight -ballots USIRV/Aspen_2009_Mayor.txt -logfile  tg_log_Aspen_2009_Mayor.txt   -tlimit 500  >  tg_margin_Aspen_2009_Mayor.txt

./marginirv -tight -ballots USIRV/Berkeley_2010_D1CityCouncil.txt -logfile  tg_log_Berkeley_2010_D1CityCouncil.txt  -tlimit 500   >  tg_margin_Berkeley_2010_D1CityCouncil.txt

./marginirv -tight -ballots USIRV/Berkeley_2010_D4CityCouncil.txt -logfile  tg_log_Berkeley_2010_D4CityCouncil.txt   -tlimit 500  >  tg_margin_Berkeley_2010_D4CityCouncil.txt

./marginirv -tight -ballots USIRV/Berkeley_2010_D7CityCouncil.txt -logfile  tg_log_Berkeley_2010_D7CityCouncil.txt  -tlimit 500   >  tg_margin_Berkeley_2010_D7CityCouncil.txt

./marginirv -tight -ballots USIRV/Berkeley_2010_D8CityCouncil.txt -logfile  tg_log_Berkeley_2010_D8CityCouncil.txt  -tlimit 500   >  tg_margin_Berkeley_2010_D8CityCouncil.txt

./marginirv -tight -ballots USIRV/Oakland_2010_D4CityCouncil.txt -logfile  tg_log_Oakland_2010_D4CityCouncil.txt  -tlimit 500   >  tg_margin_Oakland_2010_D4CityCouncil.txt

./marginirv -tight -ballots USIRV/Oakland_2010_D6CityCouncil.txt -logfile  tg_log_Oakland_2010_D6CityCouncil.txt   -tlimit 500  >  tg_margin_Oakland_2010_D6CityCouncil.txt

./marginirv -tight -ballots USIRV/Oakland_2010_Mayor.txt -logfile  tg_log_Oakland_2010_Mayor.txt  -tlimit 500   >  tg_margin_Oakland_2010_Mayor.txt

./marginirv -tight -ballots USIRV/Pierce_2008_CityCouncil.txt -logfile  tg_log_Pierce_2008_CityCouncil.txt  -tlimit 500   >  tg_margin_Pierce_2008_CityCouncil.txt

./marginirv -tight -ballots USIRV/Pierce_2008_CountyAuditor.txt -logfile  tg_log_Pierce_2008_CountyAuditor.txt   -tlimit 500  >  tg_margin_Pierce_2008_CountyAuditor.txt

./marginirv -tight -ballots USIRV/Pierce_2008_CountyAssessor.txt -logfile  tg_log_Pierce_2008_CountyAssessor.txt  -tlimit 500   >  tg_margin_Pierce_2008_CountyAssessor.txt

./marginirv -tight -ballots USIRV/Pierce_2008_CountyExecutive.txt -logfile  tg_log_Pierce_2008_CountyExecutive.txt   -tlimit 500  >  tg_margin_Pierce_2008_CountyExecutive.txt

./marginirv -tight -ballots USIRV/SanFran_2007_Mayor.txt -logfile  tg_log_SanFran_2007_Mayor.txt  -tlimit 500   >  tg_margin_SanFran_2007_Mayor.txt

