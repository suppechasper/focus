\name{focus.interaction}
\alias{focus.interaction}
\alias{focus.set.projection}
\alias{focus.set.active}
\alias{focus.clear.active}
\alias{focus.set.group}
\alias{focus.get.group}
\alias{focus.clear.group}
\alias{focus.add.paired}
\alias{focus.get.paired}
\alias{focus.clear.paired}
\alias{focus.get.density}
\alias{focus.set.density}
\alias{focus.gmra.expand.to.scale}
\alias{focus.set.background.color}
\alias{focus.set.default.color}
\alias{focus.set.group.color}
\alias{focus.set.group.colors}
\alias{focus.set.group.colors.ramp}
\alias{focus.add.line}
\alias{focus.get.selection}

\title{Focus - Interactive exploratory visualization for high-dimensional data}
\description{
Interaction methods to control the focus visualization from the R console.
}
\usage{
focus.set.projection(fprojector, v, p)
focus.set.active(fdata, active)
focus.clear.active(fdata )
focus.set.group(fdata, gid, indices)
focus.get.group(fdata, indices)
focus.clear.group(fdata, gid)
focus.add.paired(fdata, from, to)
focus.get.paired(fdata, from, gid)
focus.clear.paired(fdata)
focus.get.density(fdata)
focus.set.density(fdata, density)
focus.gmra.expand.to.scale(fdata, scale)
focus.set.background.color(r, g, b)
focus.set.default.color(fdata, r, g, b)
focus.set.group.color(fdata, gid, r, g, b)
focus.set.group.colors(fdata, r, g, b)
focus.set.group.colors.ramp(fdata, cRamp, ngroups)
focus.add.line(fdata, line)
focus.get.selection(fdata)
}

\arguments{
  \item{fdata}{Focus data container handle.}
  \item{fprojectpr}{Focus projector handle.}
  \item{v}{Vector that matches the dimension of the data object associated with
  projector. }
  \item{p}{WHich projection focus point to set to v.}
  \item{gid}{Group identification}
  \item{indices}{Indices to change}
  \item{active}{Indices to set active}  
  \item{r}{Red component (0-1).}
  \item{g}{Green component (0-1). }
  \item{b}{Blue component (0-1).}
  \item{cRamp}{Colorramp function create with \link{colorRamp}.}
  \item{nGroups}{Number of groups to create, each group is assigned a color
    through a uniform spacing on cRamp.}
  \item{scale}{Expand gmra structure to given scale.}
  \item{from}{Adds line on selection to from indices to to indices}
  \item{to}{Adds line on selection  to from indices to to indices}

}
\value{
}

\author{
  Samuel Gerber
}
\seealso{
  focus
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
