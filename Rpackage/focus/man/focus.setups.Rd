\name{focus.setups}
\alias{focus.setups}
\alias{focus.setup.groups}
\alias{focus.setup.correlation.plot}
\alias{focus.setup.single.point.focus.plot}
\alias{focus.setup.two.point.focus.plot}
\alias{focus.setup.line.focus.plot}
\alias{focus.setup.three.point.focus.plot}
\alias{focus.setup.single.point.focus.versus.orthogonal.plot}
\alias{focus.setup.4way}
\alias{focus.setup.6way}
\alias{focus.setup.6way.data}

\title{Focus - Interactive exploratory visualization for high-dimensional data}

\description{
  Setups of different focused projections or combination of focused projections
  with backgrounds.
}
\usage{
  focus.setup.groups(data, groups)
  focus.setup.correlation.plot (X, groups = as.factor(rep(0,
        nrow(X))), vt=0, oblique=FALSE, drawPoints=TRUE, drawLines= FALSE,
    passiveSelect=TRUE  )
  focus.setup.single.point.focus.plot(X, groups = as.factor(rep(0,
        nrow(X))), hyperbolic=TRUE, drawPoints=TRUE, drawLines= FALSE,
    passiveSelect=TRUE )
  focus.setup.two.point.focus.plot(X, groups = as.factor(rep(0,
        nrow(X))), hyperbolic=TRUE, drawPoints=TRUE, drawLines= FALSE,
    passiveSelect=TRUE )
  focus.setup.line.focus.plot(X, groups = as.factor(rep(0, nrow(X))),
    hyperbolic=TRUE, drawPoints=TRUE, drawLines= FALSE, passiveSelect=TRUE )
  focus.setup.three.point.focus.plot(X, groups = as.factor(rep(0,
        nrow(X))), drawPoints=TRUE, drawLines= FALSE, passiveSelect=TRUE )
  focus.setup.heat.triangulation.plot(X, groups = as.factor(rep(0,
        nrow(X))), drawPoints=TRUE, drawLines= FALSE, passiveSelect=TRUE )
  focus.setup.single.point.focus.versus.orthogonal.plot(X, groups=
    as.factor(rep(0, nrow(X))), radii = c(0.5, 1, 2, 4), drawPoints=TRUE,
    drawLines= FALSE, passiveSelect=TRUE, fhyperbolic=TRUE, ohyperbolic=FALSE, grouped=TRUE)
  focus.setup.4way <-function(X, groups = as.factor(rep(0, nrow(X))) )
  focus.setup.6way <-function(X, groups = as.factor(rep(0, nrow(X))) )
  focus.setup.6way.data <-function(data, groups = as.factor(rep(0, nrow(X))) )
}
\arguments{
  \item{data}{Focus data container handle}
  \item{groups}{Assignment of group for each data point in teh container (a
        factor) }.
  \item{X}{Data matrix or frame}
  \item{vt}{Remove data points with variance less than vt}
  \item{oblique}{Use an oblique projection}
  \item{drawPoints}{Draw location of each point}
  \item{drawLines}{Draw lines added with \link{focus.add.line}}
  \item{passiveSelect}{Select points as mouse hovers over them. Hampers
    interactivity for very lareg data sets.}
  \item{hyperbolic}{Ude hyprbolic scaling}
  \item{fhyperbolic}{Use hyperbolic focus plot}
  \item{ohyperbolic}{Use hyperbolic orthogonal plot}
  \item{radii}{Radii at whcih to draw concentric circles.}
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
