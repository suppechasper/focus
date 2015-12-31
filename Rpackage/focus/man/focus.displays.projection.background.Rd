\name{focus.displays.projection.background}
\alias{focus.displays.projection.background}
\alias{focus.add.circle.background}
\alias{focus.add.vertical.circle.lines.background}
\alias{focus.add.horizontal.circle.lines.background}
\alias{focus.add.projector.shadow.background}
\alias{focus.add.center.shadow.background}
\alias{focus.set.projection.background.color}

\title{Focus - Interactive exploratory visualization for high-dimensional data}
\description{
Methods to add background drawings to projection displays.

Circles positioned at the display origin and with radius relative to the
display. 

Vertical and horizontal lines with endpoints on circles(for setting up correlation display).

Outline of the projection shadow of balls located at the focus points used to
setup the focus projections.

}
\usage{
focus.add.circle.background(fdel, radius, n=200)
focus.add.vertical.circle.lines.background(fdel, radius, n)
focus.add.horizontal.circle.lines.background(fdel, radius, n)
focus.add.projector.shadow.background(fdel, radius, pIndex, n=200,
    scaled=TRUE)i
focus.add.center.shadow.background(fdel, radius, n=200,
    scaled=TRUE)i
focus.set.projection.background.color(fbg, r, g, b, alpha=1)
}

\arguments{
  \item{fdel}{Focus display handle}
  \item{fbg}{Focus background handle}
  \item{radius}{Radius of circle relative to display size (1 = minimum over display
      width, height). For shadow methods radius is relative to current scaling
    of the projector.}
  \item{n}{Nymber of points for drawing the circle witha polyline. Or number of
    vertical / horizontal lines withing a cricle of radius radius}
  \item{pIndex}{Shadow for which projector.}
  \item{scaled}{Use radius relative to current scaling.}
  \item{r}{Red component (0-1).}
  \item{g}{Green component (0-1). }
  \item{b}{Blue component (0-1).}
  \item{alpha}{Transparancy (0-1).}
}
\value{
  Background handle or NULL
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
