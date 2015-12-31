\name{focus.example.data}
\alias{focus.hypercube.vertices}
\alias{focus.torus}
\alias{focus.sphere}
\alias{focus.sinusoids.grid}
\alias{focus.sinusoids}

\title{Focus - Interactive exploratory visualization for high-dimensional data}
\description{
Example data sets for focus projection visualizations.

The sinusoids method creates data set where each point (row) is a time series
of a combination of n sinusoids with different frequencies sampled with
different phase shifts.
}
\usage{
focus.hypercube.vertices(d)
  focus.torus(n, r=1, R=2)
  focus.sphere(n, d=2)
  focus.sinusoids.grid(n, d=100, f = c(0.5, 1, 2)  )
  focus.sinusoids(n, d=100, f = c(0.5, 1, 2) ) 
}

\arguments{
  \item{d}{Dimensionality}
  \item{n}{NUmber of points}
  \item{r}{Inner radius}
  \item{R}{Outer radus}
  \item{f}{Frequenceis of sinusoids.}
}
\value{
}

\author{
  Samuel Gerber
}
\seealso{
  focus,focus.examples

}
\references{
In preparation
}
\examples{

}
\keyword{visualization, interaction, exploratory data analysis, high dimensional data}
