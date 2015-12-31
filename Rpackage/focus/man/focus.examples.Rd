\name{focus.examples}
\alias{focus.examples}
\alias{focus.example.normals}
\alias{focus.example.hypercube}
\alias{focus.example.normal.plus.uniform}
\alias{focus.example.planes}

\title{Focus - Interactive exploratory visualization for high-dimensional data}
\description{
Example focus projection visualizations.
}
\usage{
focus.example.normals(n=1000, r=c(1, 2, 3), dim=10, focusColor=F)
focus.example.hypercube(n=100, dim=4, r=0.1)
focus.example.normal.plus.uniform(n=1000, dim=4, r=0.1, sigma=1)
focus.example.planes(n=1000, dim=30, nPlanes=6, pDim=2)
}

\arguments{
  \item{n}{Number of points.}
  \item{r}{Support for uniform distribution ([-r, r]^dim).}
  \item{dim}{Data dimenionality.}
  \item{focusColor}{Add focus colormap.}
  \item{sigma}{Standard deviation of normal.}
  \item{nPlans}{Number of planes.}
  \item{pDim}{Dimension of planes.}
}
\value{
}

\author{
  Samuel Gerber
}
\seealso{
  focus,focus.example.data

}
\references{
In preparation
}
\examples{

}
\keyword{visualization, interaction, exploratory data analysis, high dimensional data}
