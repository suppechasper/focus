\name{focus.data}
\alias{focus.data}
\alias{focus.create.matrix.data}
\alias{focus.create.matrix.data.proxy}
\alias{focus.cretae.sparse.matrix.data}
\alias{focus.create.gmra.data}
\title{Focus - Interactive exploratory visualization for high-dimensional data}
\description{
Methods to create data container for focus visualizations.  The proxy method
allows to show projections of preprocessed data, but still link data-displays to
the orginal data (see \link{focus.displays}).
}
\usage{
  focus.create.matrix.data(data)
  focus.create.matrix.data.proxy(fdata, data)
  focus.create.sparse.matrix.data(m ,n, rowI, colI, x)
  focus.create.gmra.data(gmra, dim = length( gmra.centers(gmra, 0) ) )
}

\arguments{
  \item{data}{ A numeric matrix or object that can be converted into a matrix. Each ro
  corresponds to a data point}
  \item{fdata}{A focus data handle}
  \item{m}{Number of rows of the sparse matrix}
  \item{n}{Numbers of columns of the sparse matrix}
  \item{rowI}{Row indicies of nonzer elements}
  \item{colI}{Column indices of nozero elements}
  \item{x}{Values of nonzero elements. I.e. the sparse matrix has nonzeros at
  [rowI[i], colI[i] ] = x[i]}
  \item{gmra}{A gmra object (see package \link{gmra}}
  \item{dim}{Number of dimensions of the gmra object used for the focused
  projections. This is useful if the gmra was constructed using addional
  dimension not related to the comparions of interst for the focus visualization
  (e.g. constructing the gmra jointly with spatial information) }
}
\value{
  Returns a focus data handle, that holds a pointer to the data strcuture. Note,
  the data structure will not be saved when R is closed and will have to be
  recreated when R is started.
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
 
 X = iris[, 1:4]

 data1 <- focus.create.matrix.data( X )
 
 cX = cmdscale(X) 
 data2 <- focus.create.matrix.data.proxy( data1, cX )

}
\keyword{visualization, interaction, exploratory data-analysis, high-dimensional data}
