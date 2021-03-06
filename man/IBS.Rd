\name{IBS}
\alias{IBS}
\title{Identity by state}
\description{
  Calculates the total number of alleles that are shared by two
  profiles. If the two profiles in question are indeed relatives then
  the matching alleles may be identical by descent, or by random chance
  alone, hence identity by state.
}
\usage{
IBS(prof1, prof2, nLoci = length(prof1)/2, bPrint = FALSE)
}
\arguments{
\item{prof1}{A matrix consisting of 2 columns and nLoci rows. Each entry
in the matrix is the (coded) allele held by the individual.}
\item{prof2}{See \code{prof1}}
\item{nLoci}{The number of loci in the profiles. Specifying this value
  speeds up computation enormously.}
\item{bPrint}{If true then the result is printed locus by locus. This
feature exists primarily for debugging purposes.}
}
\value{
An integer between 0 and 2*nLoci representing the total number of
alleles that match in the two profiles.
}
\examples{
data(fbiCaucs)
P1 = randomProfile(fbiCaucs)
C1 = randomChild(P1, fbiCaucs)
IBS(P1, C1)
IBS(P1, C1, bPrint = TRUE)
}
\author{James M. Curran}
