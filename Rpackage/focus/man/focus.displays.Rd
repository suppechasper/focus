\name{focus.displays}
\alias{focus.displays}
\alias{focus.add.projection.display}
\alias{focus.add.profile.display}
\alias{focus.add.pca.display}
\alias{focus.add.group.pca.display}
\alias{focus.add.image.display}
\alias{focus.add.multi.index.image.display}
\alias{focus.add.point.as.image.display}

\title{Focus - Interactive exploratory visualization for high-dimensional data}
\description{
Display elements for the focus framework.

The projection display is a scatterplot of the projected points. The background
of the scatterplot can be customized with the methods in
\link{focus.displays.projection.background}.

The profile displays shows the selected points of a data conatiner, e.g. through
interactive selection in a scatterplot, as parallel coordinates.

The pca displays do a principal component analysis of the data or a group of the
data, associated with the projector and add a barplot that can be used to select
principal components as focus points.  

The image display is for datasets for which each point is associated with a
pixel location in the image. The selected points of the data object are
hoighlighted at the corresponding image location. The multiIndex options allows
to map a single point to multiple image locations.

The point-as-image display is for data sets for which each point can be
represented as an image. An image of the selected points of the data container
is drawn.  

}
\usage{
  focus.add.projection.display(fprojector, x, y, w, h, drawPoints=TRUE,
    drawLines=TRUE, passiveSelect = TRUE)
  focus.add.profile.display(fdata, x, y, w, h, showBG = TRUE)
  focus.add.pca.display(fprojector, x, y, w, h, showBG = TRUE)
  focus.add.group.pca.display(fdata, gid, x, y, w, h, showBG = TRUE)
  focus.add.image.display(fdata, image, conv, x, y, w, h, showBG = TRUE)
  focus.add.multi.index.image.display(fdata, image, conv, multiIndex, x, y, w,
    h, showBG = TRUE) 
  focus.add.point.as.image.display(fdata, m, n, columnMajor=TRUE,
    transpose = FALSE, keepAspect=TRUE, x, y, w, h )
  focus.add.point.range.as.image.display(fdata, m, n, indices, columnMajor=TRUE,
    transpose = FALSE, keepAspect=TRUE, x, y, w, h )
}

\arguments{
  \item{fprojector}{Focus projector handle}
  \item{fdata}{Focus data container}
  \item{x}{Horizontal location of top left corner of display. Ranging from 0 leftmost to 1 right most of the window.}
  \item{y}{Vertical location of top left corner of display. Ranging from 0 leftmost to 1 right most of the window.}
  \item{w}{Width of display. Ranging from 0 leftmost to 1 right most of the window.}
  \item{h}{Height of display. Ranging from 0 leftmost to 1 right most of the window.}
  \item{showBG}{Draw the background of the display element}
  \item{gid}{Group identification }
  \item{image}{Image to display}
  \item{conv}{Converter from point index to image location}
  \item{multiIndex}{A two column vector with the first the point index and the
  second the index that is passed to the converter.}
  \item{m}{Height of image}
  \item{n}{Width of image}
  \item{indices}{Indices of the points that are filled into an image}
  \item{columnMajor}{IF \code{TRUE} fill image columnwise from the point cooridnates}
  \item{transpose}{Transpose the resulting image}
  \item{keepAspect}{Keep the aspect ratio}
}
\value{
}

\author{
  Samuel Gerber
}
\seealso{
}
\references{
In preparation
}
\examples{

}
\keyword{visualization, interaction, exploratory data analysis, high dimensional data}
