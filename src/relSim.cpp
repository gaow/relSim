#include <Rcpp.h>
#include <iostream>
using namespace Rcpp;

int profIBS(IntegerVector::const_iterator Prof){
  int nResult = 0;
  
  int nA1 = Prof[0];
  int nA2 = Prof[1];
  int nB1 = Prof[2];
  int nB2 = Prof[3];
  
  if(nA1 == nB1 && nA2 == nB2){
    nResult = 2;
  }else if((nA1 == nB1) || (nA2 == nB1) || (nA1 == nB2) || (nA2 == nB2)){
    nResult = 1;
  } // else nResult is zero
  
  return nResult;
}


double locusLRmix(IntegerVector::const_iterator ProfVic, IntegerVector::const_iterator ProfSus, 
                  NumericVector Freq){ 
  double dLR;
  double dPa, dPb, dPc, dPd;
  
  dPa = dPb = dPc =  dPd = 0;
  
  if(ProfVic[0] == ProfVic[1]){ // hom vic aa
    if(ProfSus[0] == ProfSus[1]){ // hom sus aa or bb
      if(ProfSus[0] == ProfVic[0]){ // sus aa
        // *nCase = 1;
        dPa = Freq[ProfVic[0] - 1];
        dLR = 1 / (dPa * dPa);
      }else{ // sus bb
        // *nCase = 2;
        dPa = Freq[ProfVic[0] - 1];
        dPb = Freq[ProfSus[0] - 1];
        dLR = 1 / (dPb * (2 * dPa + dPb));
      }
    }else{ // suspect ab, bc
      if(ProfSus[0] == ProfVic[0] || ProfSus[1] == ProfVic[0]){ // sus ab
        // *nCase = 4;
        dPa =  Freq[ProfVic[0] - 1];
        dPb =  Freq[ProfSus[ProfSus[0] == ProfVic[0] ? 1 : 0] - 1];
        dLR = 1 / (dPb * (2 * dPa + dPb));
      }else{ // suspect bc
        // *nCase = 5;
        dPb = Freq[ProfSus[0] - 1];
        dPc = Freq[ProfSus[1] - 1];
        dLR = 1 / (2 * dPb * dPc);
      }
    }
  }else{ // vic ab
    if(ProfSus[0] == ProfSus[1]){ // hom sus aa or bb or cc
      if(ProfSus[0] == ProfVic[0] || ProfSus[0] == ProfVic[1]){ // sus aa or bb
        // *nCase = 5;
        dPa = Freq[ProfVic[0] - 1];
        dPb = Freq[ProfVic[1] - 1];
        double dp = dPa + dPb;
        dLR = 1 / (dp * dp);
      }else{ // suspect cc
        // *nCase = 6;
        dPa = Freq[ProfVic[0] - 1];
        dPb = Freq[ProfVic[1] - 1];
        dPc = Freq[ProfSus[0] - 1];
        dLR = 1 / (dPc * (2 * (dPa + dPb) + dPc));
      }
    }else{ // sus ab, ac or bc or cd
      if(ProfSus[0] == ProfVic[0] && ProfSus[1] == ProfVic[1]){ // sus ab
        // *nCase = 7;
        dPa = Freq[ProfSus[0] - 1];
        dPb = Freq[ProfSus[1] - 1];
        double dp = dPa + dPb;
        dLR = 1 / (dp * dp);
      }else if(ProfSus[0] == ProfVic[0] || ProfSus[1] == ProfVic[0]){ // sus ac 
        // *nCase = 8;
        dPa = Freq[ProfVic[0] - 1];
        dPb = Freq[ProfVic[1] - 1];
      
        if(ProfSus[0] == ProfVic[0]){
          dPc = Freq[ProfSus[1] - 1];
        }else{
          dPc = Freq[ProfSus[0] - 1];
        }
        dLR = 1 / (dPc * (2 * (dPa + dPb) + dPc));
      }else if(ProfSus[0] == ProfVic[1] || ProfSus[1] == ProfVic[1]){ // sus bc
        // *nCase = 9;
        dPa = Freq[ProfVic[0] - 1];
        if(ProfSus[0] == ProfVic[1]){
          dPb = Freq[ProfSus[0] - 1];
          dPc = Freq[ProfSus[1] - 1];
        }else{
          dPb = Freq[ProfSus[1] - 1];
          dPc = Freq[ProfSus[0] - 1];
        }
        dLR = 1 / (dPc * (2 * (dPa + dPb) + dPc));
      }else{ // sus cd
        // *nCase = 10;
        dPc = Freq[ProfSus[0] - 1];
        dPd = Freq[ProfSus[1] - 1];
        dLR = 1 / (2 * dPc * dPd);
      }
    }
  }
  //pdF[0] = dPa;
  //pdF[1] = dPb;
  //pdF[2] = dPc;
  //pdF[3] = dPd;
  return dLR;
}

// [[Rcpp::export(".locusLRmix")]]
double locusLRmix_Caller(IntegerVector ProfVic, IntegerVector ProfSus, 
                   NumericVector Freq){ 
  return locusLRmix(ProfVic.begin(), ProfSus.begin(), Freq);
}

// [[Rcpp::export(".LRmix")]]
NumericVector LRmix(IntegerVector ProfVic, IntegerVector ProfSus, List listFreqs){
  int i;
  int nLoci = listFreqs.size();
  NumericVector LocusLRs(nLoci);
  
  for(i = 0; i < nLoci; i++){
    NumericVector Freqs = as<NumericVector>(listFreqs[i]);
    LocusLRs[i] = locusLRmix(ProfVic.begin() + 2 * i, ProfSus.begin() + 2 * i, Freqs);
  }
  
  return LocusLRs;
}

//[[Rcpp::export(".locusIBS")]]
IntegerVector locusIBS(IntegerVector ProfMat, int N){
  
  // assumes pnProfMat is a vector of length 4 * N
  IntegerVector result(N);
  
  int i;
  for(i = 0; i < N; i++){
    int i1 = 4 * i;
    result[i] = profIBS(ProfMat.begin() + i1);
  }
  
  return result;
}

int IBS(IntegerVector::const_iterator Prof1, IntegerVector::const_iterator Prof2, int nLoci){
  int s = 0;
  int i;
  IntegerVector p(4);
  
  for(i = 0; i < nLoci; i++){
    int i1 = 2 * i;
    p[0] = Prof1[i1];
    p[1] = Prof1[i1 + 1];
    p[2] = Prof2[i1];
    p[3] = Prof2[i1 + 1];  
    s += profIBS(p.begin());
  }
  return s;
}

// [[Rcpp::export(".IBS")]]
int IBS_Caller(IntegerVector Prof1, IntegerVector Prof2, int nLoci){
  return IBS(Prof1.begin(), Prof2.begin(), nLoci);
}

// [[Rcpp::export]]
IntegerVector randomProfiles(List listFreqs, int nBlockSize = 1000){
  int nLoci = listFreqs.size();
  int nLoc;
  int nB;
  
  // pull in the sample function from R
  Environment base("package:base");
  Function sample = base["sample"];
  
  // reserve space for result
  IntegerVector Profiles(2 * nLoci * nBlockSize);
  
  for(nLoc = 0; nLoc < nLoci; nLoc++){
    NumericVector locusFreqs = as<NumericVector>(listFreqs[nLoc]);
    int nAlleles = locusFreqs.size();
    IntegerVector randAlleles = sample(seq_len(nAlleles), Named("size", 2 * nBlockSize),
    Named("replace", true),
    Named("prob", locusFreqs));
    
    for(nB = 0; nB < nBlockSize; nB++){
      int nBlockOffset = 2 * nLoci * nB;
      
      int a1 = randAlleles[2 * nB];
      int a2 = randAlleles[2 * nB + 1];
      
      if(a1 > a2){
        Profiles[nBlockOffset + 2 * nLoc] = a2;
        Profiles[nBlockOffset + 2 * nLoc + 1] = a1;
      }else{
        Profiles[nBlockOffset + 2 * nLoc] = a1;
        Profiles[nBlockOffset + 2 * nLoc + 1] = a2;
      }
    }
  }
  
  return Profiles;
}

// [[Rcpp::export]]
IntegerVector randomSibs(IntegerVector ProfSib1, List listFreqs, int nBlockSize){
  int nLoci = listFreqs.size();
  int nLoc;
  int nB;
  
  // pull in the sample function from R
  Environment base("package:base");
  Function sample = base["sample"];
  
  // reserve space for the results
  IntegerVector ProfSib2(2 * nLoci * nBlockSize);
  
  
  // the mean + 10 sd rounded up should mean enough random alleles are available
  int sampleSize = (int)(100 * ceil(floor(nBlockSize + 10 * sqrt(nBlockSize * 0.5)) / 100));
  
  
  for(nLoc = 0; nLoc < nLoci; nLoc++){
    NumericVector U = runif(nBlockSize);
    
    NumericVector locusFreqs = as<NumericVector>(listFreqs[nLoc]);
    int nAlleles = locusFreqs.size();
    IntegerVector randAlleles = sample(seq_len(nAlleles), Named("size", sampleSize),
    Named("replace", true),
    Named("prob", locusFreqs));
    
    int j = 0;
    int a1, a2;
    
    for(nB = 0; nB < nBlockSize; nB++){
      int nBlockOffset = 2 * nLoci * nB;
      
      if(U[nB] < 0.25){
        a1 = ProfSib1[nBlockOffset + 2 * nLoc];
        a2 = ProfSib1[nBlockOffset + 2 * nLoc + 1];
      }else if(U[nB] >= 0.25 && U[nB] < 0.5){
        a1 = ProfSib1[nBlockOffset + 2*nLoc];
        a2 = randAlleles[j++];
      }else if(U[nB] >= 0.50 && U[nB] <= 0.75){
        a1 = randAlleles[j++];
        a2 = ProfSib1[nBlockOffset + 2 * nLoc + 1];
      }else{
        a1 = randAlleles[j++];
        a2 = randAlleles[j++];
      }
      
      if(a1 > a2){
        ProfSib2[nBlockOffset + 2*nLoc] = a2;
        ProfSib2[nBlockOffset + 2*nLoc + 1] = a1;
      }else{
        ProfSib2[nBlockOffset + 2*nLoc] = a1;
        ProfSib2[nBlockOffset + 2*nLoc + 1] = a2;
      }  
    }
  }
  
  return ProfSib2;
}

// [[Rcpp::export]]
IntegerVector randomChildren(IntegerVector ProfParent, List listFreqs, int nBlockSize){
  int nLoci = listFreqs.size();
  int nLoc;
  int nB;
  
  // pull in the sample function from R
  Environment base("package:base");
  Function sample = base["sample"];
  
  // reserve space for the results
  IntegerVector ProfChild(2 * nLoci * nBlockSize);
  
  int sampleSize = (int)ceil(nBlockSize * 0.5);
  
  for(nLoc = 0; nLoc < nLoci; nLoc++){
    NumericVector U = runif(nBlockSize);
    
    NumericVector locusFreqs = as<NumericVector>(listFreqs[nLoc]);
    int nAlleles = locusFreqs.size();
    IntegerVector randAlleles = sample(seq_len(nAlleles), Named("size", sampleSize),
    Named("replace", true),
    Named("prob", locusFreqs));
    
    int j = 0;
    int a1, a2;
    
    for(nB = 0; nB < nBlockSize; nB++){
      int nBlockOffset = 2 * nLoci * nB;
      
      
      if(U[nB] < 0.5){
        a1 = ProfParent[nBlockOffset + 2 * nLoc];
        a2 = randAlleles[j++];
      }else{
        a1 = randAlleles[j++];
        a2 = ProfParent[nBlockOffset + 2 * nLoc + 1];
      }
      
      if(a1 > a2){
        ProfChild[nBlockOffset + 2 * nLoc] = a2;
        ProfChild[nBlockOffset + 2 * nLoc + 1] = a1;
      }else{
        ProfChild[nBlockOffset + 2 * nLoc] = a1;
        ProfChild[nBlockOffset + 2 * nLoc + 1] = a2;
      }
    }
  }
  
  return ProfChild;
}

double locusLRPC(IntegerVector::const_iterator ProfParent, IntegerVector::const_iterator ProfChild, NumericVector& Freq, int & dc){
	double dLR = 1;
	if (ProfChild[0] == -9 || ProfChild[1] == -9 || ProfParent[0] == -9 || ProfParent[1] == -9) {
		// do nothing
	}else if (ProfChild[0] == ProfChild[1]){ // Child is aa
		double dPa = Freq[ProfChild[0] - 1];
  
		if(ProfParent[0] == ProfParent[1]){ // Parent is aa or bb
			if(ProfParent[0] == ProfChild[0]){ // Parent is aa
				dLR /= dPa;
			}else{ // Parent is bb
				dLR = 1;
        			dc++;
			}
		}else{ // Parent is ab or bc
			if(ProfParent[0] != ProfChild[0] && ProfParent[1] != ProfChild[0]){ // Parent is bc
				dLR = 1;
        			dc++;
			}else{ // Parent is ab
				dLR /= 2 * dPa;
			}
		}
	}else{ // Child is ab
		double dPa = Freq[ProfChild[0] - 1];
		double dPb = Freq[ProfChild[1] - 1];
  
		if(ProfParent[0]==ProfParent[1]){ // Parent is aa or bb or cc
			if(ProfParent[0]==ProfChild[0]){ // Parent is aa
				dLR /= 2 * dPa;
			}else if(ProfParent[0]==ProfChild[1]){ // Parent is bb
				dLR /= 2 * dPb;
			}else{ // Parent is cc
				dLR = 1;
        			dc++;
			}
		}else{ // Parent is ab, bc, ac, or cd
			if(ProfParent[0] == ProfChild[0] && ProfParent[1] == ProfChild[1]){ // Parent is ab
				dLR = (dPa + dPb) / (4 * dPa * dPb);
			}else if(ProfParent[0] == ProfChild[0] || ProfParent[1] == ProfChild[0]){ // Parent is ac or ca
				dLR /= 4 * dPa;
			}else if(ProfParent[0] == ProfChild[1] || ProfParent[1] == ProfChild[1]){ // Parent is bc or cb
				dLR /= 4 * dPb;
			}else{ // Parent is cd
				dLR = 1;
        			dc++;
			}
		}
	}
	
  return(dLR);
}

double lrPC(IntegerVector::const_iterator ProfParent, IntegerVector::const_iterator ProfChild, 
            List listFreqs){
	int nLoci = listFreqs.size();
	int nLoc = 0;
  	int denovo_cnt = 0;
	double dLR = 1;
	
	while(nLoc < nLoci){
		int i1 = 2 * nLoc;
		NumericVector Freqs = as<NumericVector>(listFreqs[nLoc]);
    
		dLR *= locusLRPC(ProfParent + i1, ProfChild + i1, Freqs, denovo_cnt);
		nLoc++;
	}
	std::cout << "De novo loci: " << denovo_cnt << std::endl;
  	std::cout << "LR: " << dLR << std::endl;
	return(dLR);
}

// [[Rcpp::export(".lrPC")]]
double lrPC_Caller(IntegerVector ProfParent, IntegerVector ProfChild,
            List listFreqs){
  return lrPC(ProfParent.begin(), ProfChild.begin(), listFreqs);
}

// [[Rcpp::export]]
List maximizeLRPC(List listFreqs, int nBlockSize){  
  int b;
  int nOffset;
  int nLoci = listFreqs.size();
  
  IntegerVector Prof1 = randomProfiles(listFreqs, nBlockSize);
  IntegerVector Prof2 = randomChildren(Prof1, listFreqs, nBlockSize);
  int iMax = 0;
  double dMax = 0;
  
  for(b = 0; b < nBlockSize; b++){
    // calculate the LR
    
    nOffset = 2 * nLoci * b;
    double lr = lrPC(Prof1.begin() + nOffset, Prof2.begin() + nOffset, listFreqs);
    // update
    
    if(lr > dMax){
      dMax = lr;
      iMax = b;
    }
  }
  
  nOffset = 2 *  nLoci * iMax;
  
  List listResult;
  
  listResult["parent"] = IntegerVector(Prof1.begin() + nOffset, Prof1.begin() + nOffset + 2 * nLoci);
  listResult["child"] = IntegerVector(Prof2.begin() + nOffset, Prof2.begin() + nOffset + 2 * nLoci);
  /*for(nLoc = 0; nLoc < nLoci; nLoc++){
    int i1 = 2*nLoc;
    pnProfParent[i1] = pnProf1[nOffset + i1];
    pnProfParent[i1+1] = pnProf1[nOffset + i1 + 1];
    pnProfChild[i1] = pnProf2[nOffset + i1];
    pnProfChild[i1+1] = pnProf2[nOffset + i1 + 1];
  }*/
  
  return listResult;
}

double locusLRSib(IntegerVector::const_iterator ProfSib1, IntegerVector::const_iterator ProfSib2,
                   NumericVector Freq){
  double dLR = 0;
  
  if(ProfSib2[0] == ProfSib2[1]){ // Sib2 is aa
    double dPa = Freq[ProfSib2[0] - 1];
  
    if(ProfSib1[0] == ProfSib1[1]){ // Sib1 is aa or bb
      if(ProfSib1[0] == ProfSib2[0]){ // Sib1 is aa
        dLR = (1 + dPa) * (1 + dPa) / (4 * dPa * dPa);
      }else{ // Sib1 is bb
        dLR = 0.25;
      }
    }else{ // Sib1 is ab or bc
      if(ProfSib1[0] != ProfSib2[0] && ProfSib1[1] != ProfSib2[0]){ // Sib1 is bc
        dLR = 0.25;
      }else{ // Sib1 is ab
        dLR = (1 + dPa) / (4 * dPa);
      }
    }
  }else{ // Sib2 is ab
    double dPa = Freq[ProfSib2[0] - 1];
    double dPb = Freq[ProfSib2[1] - 1];
  
    if(ProfSib1[0] == ProfSib1[1]){ // Sib1 is aa or bb or cc
      if(ProfSib1[0] == ProfSib2[0]){ // Sib1 is aa
        dLR = (1 + dPa) / (4 * dPa);
      }else if(ProfSib1[0] == ProfSib2[1]){ // Sib1 is bb
        dLR = (1 + dPb) / (4 * dPb);
      }else{ // Sib1 is cc
        dLR = 0.25;
      }
    }else{ // Sib1 is ab, bc, ac, or cd
      if(ProfSib1[0] != ProfSib2[0] && ProfSib1[1] != ProfSib2[1] &&
         ProfSib1[1] != ProfSib2[0] && ProfSib1[0] != ProfSib2[1]){ // Sib1 is cd
        dLR = 0.25;
      }else if(ProfSib1[0] == ProfSib2[0] && ProfSib1[1] == ProfSib2[1]){ // Sib1 is ab
        dLR = (1 + dPa + dPb + 2 * dPa * dPb)/(8 * dPa * dPb);
      }else{ // Sib1 is ac or bc
        if((ProfSib1[0] == ProfSib2[0] && ProfSib1[1] != ProfSib2[1]) ||
           (ProfSib1[1] == ProfSib2[0] && ProfSib1[0] != ProfSib2[1])){ // Sib1 is ac
          dLR = (1 + 2 * dPa) / (8 * dPa);
        }else{
          dLR = (1 + 2 * dPb)/(8 * dPb);
        }
      }
    }
  }
  
  return dLR;
}
  
double lrSib(IntegerVector::const_iterator ProfSib1, IntegerVector::const_iterator ProfSib2, 
              List listFreqs){
  double dLR = 1;
  int nLoc;
  int nLoci = listFreqs.size();
  
  for(nLoc = 0; nLoc < nLoci; nLoc++){
    int i1 = 2 * nLoc;
    NumericVector Freq = as<NumericVector>(listFreqs[nLoc]);
    
    dLR *= locusLRSib(ProfSib1 + i1, ProfSib2 + i1, Freq);
  }
  
  return dLR;
}

// [[Rcpp::export(".lrSib")]]
double lrSib_Caller(IntegerVector ProfSib1, IntegerVector ProfSib2, 
              List listFreqs){
  return lrSib(ProfSib1.begin(), ProfSib2.begin(), listFreqs);                
}

double locusProb(IntegerVector::const_iterator Prof, NumericVector Freq){
  double dResult = 0;

  if(Prof[0] == Prof[1]){
    double dPa = Freq[Prof[0] - 1];
    dResult = dPa * dPa;
  }else{
    double dPa = Freq[Prof[0] - 1];
    double dPb = Freq[Prof[1] - 1];
    dResult = 2 * dPa * dPb;
  }
  
  return dResult;
}
  
double prob(IntegerVector Prof, List listFreqs){
  int nLoci = listFreqs.size();
  int nLoc;
  double dProd = 1;
  
  for(nLoc = 0; nLoc < nLoci; nLoc++){
    double lprob;
    int i1 = 2*nLoc;
    NumericVector Freq = as<NumericVector>(listFreqs[nLoc]);
    
    dProd *= locusProb(Prof.begin() + i1, Freq);
    nLoc++;
  }
  
  return dProd;
}
  
// [[Rcpp::export]]  
IntegerVector blockStatCounts(IntegerVector Prof1, IntegerVector Prof2, int nProf,
               List listFreqs, int nCode, bool bFalseNeg,
               IntegerVector IBSthresh, NumericVector LRthresh,
               int nNumResults){
  
  int i, j;
  int nLoci = listFreqs.size(); 
  IntegerVector Result(nNumResults, 0);
  
  if(nCode == 1){ // lrSib only
    for(i = 0; i < nProf; i++){
      int nOffset = 2 * nLoci * i;
      IntegerVector::const_iterator iProf1 = Prof1.begin() + nOffset;
      IntegerVector::const_iterator iProf2 = Prof2.begin() + nOffset;      
      double dLRSib = lrSib(iProf1, iProf2, listFreqs);
      
      if(bFalseNeg){
        for(j = 0; j < nNumResults; j++){
          if(dLRSib < LRthresh[j])
            Result[j] += 1;
        }
      }else{
        for(j = 0; j < nNumResults; j++){
          if(dLRSib >= LRthresh[j])
            Result[j] += 1;
        }
      }
    }
  }else if(nCode == 2){ //lrPC only
    for(i = 0; i < nProf; i++){
      int nOffset = 2 * nLoci * i;
      IntegerVector::const_iterator iProf1 = Prof1.begin() + nOffset;
      IntegerVector::const_iterator iProf2 = Prof2.begin() + nOffset;
      double dLRPC = lrPC(iProf1, iProf2, listFreqs);
      
      if(bFalseNeg){
        for(j = 0; j < nNumResults; j++){
          if(dLRPC < LRthresh[j])
            Result[j] += 1;
        }
      }else{
        for(j = 0; j < nNumResults; j++){
          if(dLRPC >= LRthresh[j])
            Result[j] += 1;
        }
      }
    }
  }else if(nCode == 3){ // ibs only
    for(i = 0; i < nProf; i++){
      int nOffset = 2 * nLoci * i;
      IntegerVector::const_iterator iProf1 = Prof1.begin() + nOffset;
      IntegerVector::const_iterator iProf2 = Prof2.begin() + nOffset;
      int nIBS = IBS(iProf1, iProf2, nLoci);
      
      if(bFalseNeg){
        for(j = 0; j < nNumResults; j++){
          if(nIBS < IBSthresh[j])
          Result[j] += 1;
        }
      }else{
        for(j = 0; j < nNumResults; j++){
          if(nIBS >= IBSthresh[j])
          Result[j] += 1;
        }
      }
    }
  }else if(nCode == 4){ // lrSib and ibs
    for(i = 0; i < nProf; i++){
      int nOffset = 2 * nLoci * i;
      IntegerVector::const_iterator iProf1 = Prof1.begin() + nOffset;
      IntegerVector::const_iterator iProf2 = Prof2.begin() + nOffset;
      double dLRSib = lrSib(iProf1, iProf2, listFreqs);
      int nIBS = IBS(iProf1, iProf2, nLoci);
      
      if(bFalseNeg){
        for(j = 0; j < nNumResults; j++){
          if(dLRSib < LRthresh[j] || nIBS < IBSthresh[j])
            Result[j] += 1;
        }
      }else{
        for(j = 0; j < nNumResults; j++){
          if(dLRSib >= LRthresh[j] && nIBS >= IBSthresh[j])
            Result[j] += 1;
        }
      }
    }
  }else if(nCode == 5){ // lrPC and ibs
    for(i = 0; i < nProf; i++){
      int nOffset = 2 *nLoci * i;
      IntegerVector::const_iterator iProf1 = Prof1.begin() + nOffset;
      IntegerVector::const_iterator iProf2 = Prof2.begin() + nOffset;
      double dLRPC = lrPC(iProf1, iProf2, listFreqs);
      int nIBS = IBS(iProf1, iProf2, nLoci);
      
      if(bFalseNeg){
        for(j = 0; j < nNumResults; j++){
          if(dLRPC < LRthresh[j] || nIBS < IBSthresh[j])
            Result[j] += 1;
        }
      }else{
        for(j = 0; j < nNumResults; j++){
          if(dLRPC >= LRthresh[j] && nIBS >= IBSthresh[j])
            Result[j] += 1;
        }
      }
    }
  }else if(nCode == 6){ // lrSib, lrPC and ibs
  
  }
  return Result;
}
  

// [[Rcpp::export]]  
List blockStats(IntegerVector Prof1, IntegerVector Prof2, int nProf,
                List listFreqs, int nCode){
  
  int i;
  List results;
  int nLoci = listFreqs.size();
  
  if(nCode == 1){ // lrSib only
    NumericVector lrSibResults(nProf);

    for(i = 0; i < nProf; i++){
      int nOffset = 2 * nLoci * i;
      IntegerVector::const_iterator iProf1 = Prof1.begin() + nOffset;
      IntegerVector::const_iterator iProf2 = Prof2.begin() + nOffset;   
      lrSibResults[i] = lrSib(iProf1, iProf2, listFreqs);
    }
    results["sib"] = lrSibResults;
  }else if(nCode == 2){ //lrPC only
    NumericVector lrPCResults(nProf);

    for(i = 0; i < nProf; i++){
      int nOffset = 2 * nLoci * i;
      IntegerVector::const_iterator iProf1 = Prof1.begin() + nOffset;
      IntegerVector::const_iterator iProf2 = Prof2.begin() + nOffset;
      
      lrPCResults[i] = lrPC(iProf1, iProf2, listFreqs);
    }
    results["pc"] = lrPCResults;
  }else if(nCode == 3){ // ibs only
    IntegerVector IBSResults(nProf);
    for(i = 0; i < nProf; i++){
      int nOffset = 2 *nLoci * i;
      IntegerVector::const_iterator iProf1 = Prof1.begin() + nOffset;
      IntegerVector::const_iterator iProf2 = Prof2.begin() + nOffset;
      
      IBSResults[i] = IBS(iProf1, iProf2, nLoci);
    }
    results["ibs"] = IBSResults;
  }else if(nCode == 4){ // lrSib and lrPC
   NumericVector lrPCResults(nProf);
   NumericVector lrSibResults(nProf);
    for(i = 0; i <  nProf; i++){
      int nOffset = 2 * nLoci * i;
      IntegerVector::const_iterator iProf1 = Prof1.begin() + nOffset;
      IntegerVector::const_iterator iProf2 = Prof2.begin() + nOffset;
    
      lrSibResults[i] = lrSib(iProf1, iProf2, listFreqs);
      lrPCResults[i] = lrPC(iProf1, iProf2, listFreqs);
    }
    results["pc"] = lrPCResults;
    results["sib"] = lrSibResults;
  }else if(nCode == 5){ // lrSib, lrPC and ibs
    NumericVector lrPCResults(nProf);
    NumericVector lrSibResults(nProf);
    IntegerVector IBSResults(nProf);
    for(i = 0; i < nProf; i++){
      int nOffset = 2 * nLoci * i;
      IntegerVector::const_iterator iProf1 = Prof1.begin() + nOffset;
      IntegerVector::const_iterator iProf2 = Prof2.begin() + nOffset;
      
      lrSibResults[i] = lrSib(iProf1, iProf2, listFreqs);
      lrPCResults[i] = lrPC(iProf1, iProf2, listFreqs);
      IBSResults[i] = IBS(iProf1, iProf2, nLoci);
    }
    results["pc"] = lrPCResults;
    results["sib"] = lrSibResults;
    results["ibs"] = IBSResults;
  }
  return results;
}
