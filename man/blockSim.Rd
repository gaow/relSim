\name{blockSim}
\alias{blockSim}
\title{Perform relatives simulations using large memory blocks in C}
\description{
  Generate N pairs with a given relationship, calculate the LR for
  sibs, parent-child and the number of matching alleles and count the
  number of pairs that meet the threshold criteria.
}
\usage{
blockSim(N, Freqs, rel = "UN", ibsthresh = NULL, kithresh = NULL,
        code = 1, falseNeg = TRUE, BlockSize = N / 10)
}
\arguments{
    \item{N}{The number of iterations to carry out}
    \item{Freqs}{A list containing two lists labelled loci and freqs. The
second list is a list of vectors containing the allele frequencies of
each allele at each locus in the multiplex.}
  \item{rel}{generate unrelated (\code{rel = 'UN'}), full-sibs
    (\code{rel = 'FS'}), or parent child (\code{rel = 'PC'}) pairs}
  \item{ibsthresh}{A vector of one or more IBS thresholds}
  \item{kithresh}{A vector of one or more KI/LR thresholds}
  \item{code}{A code from 1 to 6 which dictates the events that will be
    counted.
    \enumerate{
      \item the LR for siblings will be compared to the values in
      \code{kithresh} and incremented if the LR is greater than the
      threshold
      \item the LR for parent/child will be compared to the values in
      \code{kithresh} and incremented if the LR is greater than the
      threshold
      \item the number of matching alleles (IBS)  will be compared to
      the values in  \code{ibsthresh} and incremented if the IBS is
      greater than the threshold
      \item the LR for siblings and the number of matching alleles will
      be compared to the values in \code{kithresh} and \code{ibsthresh}
      and incremented if both the LR and IBS is greater than the
      thresholds. \code{ibsthresh} and \code{kithresh} must be of equal
      length for this option to work
      \item the LR for parent/child and the number of matching alleles
      will be compared to the values in \code{kithresh} and
      \code{ibsthresh} and incremented if both the LR and IBS is greater
      than the thresholds. \code{ibsthresh} and \code{kithresh} must be
      of equal length for this option to work
      \item this option is equivalent to performing code 4 and 5
      simulataneously. It is not currently implemented
    }}
  \item{falseNeg}{if TRUE then the number of results that DO NOT satisfy
      the conditions are counted, otherwise the number of results DO
      satisfy the conditions are counted}
  \item{BlockSize}{Sets the number of random profiles to be generated in
    each iteration. By default the block size is set to 10 percent of the
    total sample size. It is unclear whether the procedure is more
    efficient if a bigger percentage of the total is used. Users must
    take care to make sure that the block size evenly divides \code{N}
    otherwise the procedure will exit. Users must also make sure that
    they have enough memory.}
}
\value{
  A vector containing the number of profile pairs that satisfied the
  threshold conditions
}
\details{
  This function is used for fast accurate estimation of false positive
  and false negative rates. It achieves part of its speed by block
  exectution in C, and part by not saving the LR or IBS results. It can
  do 1 billion iterations in about an hour.
}
\seealso{
  sim
}
\examples{
## not run
## this counts the number of unrelated pairs that are falsely identified
## as siblings using the policy that there are 16 or more matching
## alleles, and the LR/KI is greater than 100,000
## this is a very rare event for the FBI Caucasians with a frequency of
## about 4-5 times in 10 million pairs
\dontrun{
data(fbiCaucs)
N = 1e8
ki = 1e5
ibs = 16
code = 5
BlockSize = 1e6
blockSim(N, fbiCaucs, rel = "UN", ibsthresh = ibs, kithresh = ki,
         code = code, falseNeg = FALSE, BlockSize = BlockSize)
}
}
\author{James M. Curran}

