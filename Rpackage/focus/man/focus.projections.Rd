\name{focus.projections}
\alias{focus.projections}
\alias{focus.create.orthogonal.projection}
\alias{focus.create.linear.projection}
\alias{focus.create.single.point.focus.projection}
\alias{focus.create.two.point.focus.projection}
\alias{focus.create.three.point.focus.projection}
\alias{focus.create.line.focus.projection}
\alias{focus.create.correlation.projection}

\title{Focus - Interactive exploratory visualization for high-dimensional data}
\description{

Methods to create projections of focus data containers. 

The \pkg{focus} package conatins several projection methods to visualize and
explore data in 2-dimensional scatterplots.  The focus projections are designed
to \emph{focus} on a subset of the projection area for which pairwise relations
are represented \emph{exact} and have controlable levels of distortions in the
reamining areas of the projections. For more details about the \pkg{focus}
framework see \link{focus}.

For a detail description of the projections see the referenced paper.

The focus point for projections are set using the left mouse button for setting
the center or first focus point and shift and ctrl left mouse click for setting
additonal focus points or cutting planes if available in the respective
projection display. Right mouse click controls navigation for the mutliscale
structure if available: Unmodified click expands a node on level, shift expands
the node completly, ctrl collpases a node and alt removes/deletes the node.
 
}
\usage{
  focus.create.orthogonal.projection(data, scaled=TRUE, tripod=FALSE,
                                     fixed=FALSE, hyperbolic=TRUE, animated=TRUE) 
  focus.create.linear.projection(data, scaled=TRUE, fixed=FALSE, animated=TRUE) 
  focus.create.correlation.projection(data, animated=TRUE) 
  focus.create.single.point.focus.projection(data, scaled=TRUE, tripod=FALSE,
                                     fixed=FALSE, hyperbolic=TRUE, animated=TRUE) 
  focus.create.two.point.focus.projection(data, scaled=TRUE, hyperbolic=TRUE, animated=TRUE) 
  focus.create.three.point.focus.projection(data, animated=TRUE) 
  focus.create.line.point.focus.projection(data, hyperbolic=TRUE, animated=TRUE) 
}

\arguments{
  \item{data}{Focus data container, see \link{focus.data} }
  \item{scaled}{Scale projected point such that the further of the two points
  defining the projection plane maps to unit distance form the origin}
  \item{tripod}{If set to \code{FALSE} keep projection plane when moving origin}
  \item{fixed}{Fix projection at the orgin}
  \item{hyperbolic}{USe hyperbolic scaling}
  \item{animated}{Use animations to move between projections (See also
  \link{focus.projections.setup}}

}
\value{
}

\author{
  Samuel Gerber
}
\seealso{
 focus,focus.projections.setup,focus.data,focus.displays
}

\references{
Focused Maps - Dimensionality Reduction for Interactive Visualization
Samuel Gerber, IEEE TVCG 2015
}
\examples{

 library(focus)
 #setup a correlation display for the iris data
 X = t(scale(t( scale(iris[, 1:4]))))
 X = X/sqrt(sum(X[1,]^2))
 data <- focus.create.matrix.data( X )
 proj <- focus.create.planar.projection( ncol(X) )
 focus.start()
 focus.add.correlation.display(data, proj, 0.5, 0, 0.5, 05)
 
 X = scale( iris[, 1:4] )

 data2 <- focus.create.matrix.data( X )
 proj2 <- focus.create.hyperbolic.line.projection( ncol(X) )
 focus.add.line.projection.display(data2, proj2, 0, 0, 0.5, 0.5)
 
 proj3 <- focus.create.hyperbolic.voronoi.projection( ncol(X) )
 focus.add.hyperbolic.voronoi.projection.display(data2, proj3, 0, 0.5, 0.5, 1)

 species = levels(iris$Species)
 for(i in 1:length(species)){
   group = which(iris$Species == species[i])
   focus.set.group(data, i-1, group)
   focus.set.group(data2, i-1, group)
 }

}
\keyword{visualization, interaction, exploratory data analysis, high dimensional data}
