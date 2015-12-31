\name{focus.projections.setup}
\alias{focus.projections.setup}
\alias{focus.create.projection.group}
\alias{focus.set.tangent.rotation.animation}
\alias{focus.set.rotation.animation}
\alias{focus.set.no.animation}

\title{Focus - Interactive exploratory visualization for high-dimensional data}
\description{


Methods to change the behaviour of projections created with any of the methods
in \link{focus.projections}. 

In particular changing the animations between different projections:
\enumerate{
 \item no animation.
 \item Standard rotation animation.
 \item Tangent rotation animation. Uses local PCA when the origin is moved. The
       local PCA is done on all points within unit distance as defined by the
        current scale.
}

Finally projections can be group such that if the projection is changed in any
of the projections in the group all projections are chnaged to the same focus
points.
 
}
\usage{
  focus.create.projection.group(projections)
  focus.set.tangent.rotation.animation(fprojector, t1=1, t2=2)
  focus.set.rotation.animation(fprojector)
  focus.set.no.animation(fprojector)
}

\arguments{
  \item{projections}{A lsit of projections handles created with the methods in
  \link{focus.projections} }
  \item{fprojector}{A projection handle created with the methods in
  \link{focus.projections} }
  \item{t1}{Which principal component to use for the first tangent direction}
  \item{t2}{Which principal component to use for the second tangent direction}
}
\value{
}

\author{
  Samuel Gerber
}
\seealso{
 focus,focus.projections
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
