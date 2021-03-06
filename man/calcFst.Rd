\name{calcFst}
\alias{calcFst}
\title{Caculate locus-wise and population \eqn{F_{ST}}{Fst} values}
\description{
  This procedure uses the method of Weir and Cockerham to estimate
  \eqn{theta}{\theta} (\eqn{Fst}{F_{ST}}) for a population with substructure
}
\usage{
calcFst(Pop, subPopIdx = NULL)
}
\arguments{
  \item{Pop}{An object type 'population'}
  \item{subPopIdx}{If this vector is not null, then it must consist of
    \eqn{N}{N} elements with values from 1 to \eqn{n_s}{ns} representing
    which subpopulation each member of \code{Pop$profiles} belongs to. If it is
    null then it is assumed that the population consists of
    \eqn{n_s}{ns} subpopulations of equal size \eqn{N_s}{Ns} so that
    \eqn{n_s\times N_s = N}{ns*Ns = N}}
}
\value{
  A vector of length \eqn{n_{loci}+1}{nloci+1} with locus-wise
  \eqn{\theta}{theta} values and an overall \eqn{\theta}{theta} value
  for the population
}
\references{
  Weir, B.S., Genetic Data Analysis II, (1996) p.173--179, Sinauer,
  Sunderland, MA.
}
\examples{
data(USCaucs)
p = breedFst(USCaucs)
fst = calcFst(p)
fst
}
\seealso{breedFst}
\author{James M. Curran}
