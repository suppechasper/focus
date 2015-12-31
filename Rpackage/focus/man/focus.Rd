\name{focus}
\alias{focus}
\alias{focus.start}
\alias{focus.close}
\title{Focus - Interactive exploratory visualization for high-dimensional data}
\description{
Focus is an interactive visualization framework for exploratory analysis of
high-dimensional data. Focus is based on two dimensional scatterplots of the
high-dimensional data. Several linear and non-linear projection methods are
implemented that highlight different aspects of the data set. The projections
can be linked to domain specific visualization for the individual points in the
scatterplot.

Focus is thightly integrated with the package \code{gmra}. The package
\code{gmra} computes hierarchical decomposition of data sets. Focus can display
and interactively explore the tree structure of the hierarchical decompsotion of
the data provided by \code{gmra} data structures.

The core of Focus is projection methods to reduced the dimension reduction of
the data for visualization. Interactive visualizations provide unique
oppurtunities and challanges for the design of dimension reduction. In machine
learning dimension reduction are designed to minimize an objective, e.g.
preserve neighborhoods, geodesic, densities etc, on average over all data
points. Furthermore, the dimensionality of the reduced space is often not
required to be fixed but can be adaptively choosen  such that an overall good
fit is achieved. In visualizations a low dimensional reduced space is
imperative, in particual if the visualization should be restricted to a single
plot at most three dimension, preferably two dimensions, are feasible. If the
data set is not well approximated by such a low dimensional space distortions
are inevitable. For visuailzations it is desirable to handle distortions
in a controlled manner. This is the goal of several of the projections
implemnted in this package. To this end the projections are designed to
\emph{focus} on a subset of the projection area for which pairwise relations are
represented \emph{exact} and have controlable levels of distortions in the
reamining areas of the projections. 

The setup of focus visualization involves multiple steps:
  \enumerate{
    \item Creating one or multiple data objects (see \link{focus.data} ).
    \item Setting up projections of the data (see \link{focus.projections} )
    \item Adding projections and data displays (see \link{focus.displays})
    \item Starting the visualiztion with \link{focus.start}
  }
A demonstration of various setups is documented in \link{focus.setups} and several
examples are in \link{focus.examples}.

Finally, the viisualization supports interaction through the mouse as well as
through the R console (see \link{focus.interaction}).
}

\usage{
  focus.start()
  focus.close()
}

\arguments{
}
\value{
}

\author{
  Samuel Gerber
}
\seealso{
  focus.data,
  focus.displays,
  focus.displays.projection.background,
  focus.projections,
  focus.projections.setup,
  focus.interaction,
  focus.setups,
  focus.examples,

}
\references{
In preparation
}
\examples{

 library(focus)
 #setup a correlation display for the iris data
 X = t(scale(t( scale(iris[, 1:4]))))
 X = X/sqrt(sum(X[1,]^2))
 data <- focus.create.matrix.data( X )
 proj <- focus.create.planar.projection( ncol(X) )
 
 focus.add.projection.display(data, proj, 0.5, 0, 0.5, 05)
 
 X = iris[, 1:4]

 data2 <- focus.create.matrix.data( X )
 proj2 <- focus.create.single.point.focus.projection( ncol(X) )
 focus.add.projection.display(data2, proj2, 0, 0, 0.5, 0.5)
 
 proj3 <- focus.create.three.point.focus.projection( ncol(X) )
 focus.add.projection.display(data2, proj3, 0, 0.5, 0.5, 1)

 species = levels(iris$Species)
 for(i in 1:length(species)){
   group = which(iris$Species == species[i])
   focus.set.group(data, i-1, group)
   focus.set.group(data2, i-1, group)
 }
 
 
 focus.start()

}
\keyword{visualization, interaction, exploratory data-analysis, high-dimensional data}
