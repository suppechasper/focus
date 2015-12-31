.onLoad <- function(libname, pkgname){
    .Call("focus_load");
}
.onUnload <- function(libname){
    .Call("focus_unload");
}

focus.start <- function (){
  .Call("focusStart");
}

focus.close <-  function(){
 .Call("focusClose")
}






#----------- data objects

focus.create.matrix.data <-function(data){

  data=as.matrix(data)
    data[] = as.double(data[])
   if( is.null( colnames(data) ) ){
     colnames(data) = as.character(1:ncol(data))
   }
   if( is.null( rownames(data) ) ){
     rownames(data) = as.character(1:nrow(data))
   }

  dptr = .Call("focusMatrixData", ncol(data), nrow(data), t(data) )
  dptr
}

focus.create.matrix.data.proxy <-function(fdata, data){
  data = as.matrix(data)
  dptr = .Call("focusMatrixDataProxy", fdata, ncol(data), nrow(data), t(data) )
  dptr
}


focus.create.sparse.matrix.data <-function(m, n, rowI, colI, x){

  dptr = .Call("focusSparseMatrixData", n, m, length(colI), as.integer(colI),
      as.integer(rowI), as.double(x))
  dptr
}


focus.create.gmra.data <- function(gmra, dim = length( gmra.centers(gmra, 0) )){

  dptr = .Call("focusSingleGMRA", gmra, as.integer( dim ) )
  
  dptr

}


#----------- projections 

focus.create.orthogonal.projection <- function(data, scaled=TRUE, tripod=FALSE,
    fixed=FALSE, hyperbolic=TRUE, animated=TRUE){

  pptr = .Call("focusOrthogonalProjection", data, as.integer(scaled),
      as.integer(tripod), as.integer(fixed), as.integer(hyperbolic) )
  
  if(animated){
    focus.set.rotation.animation(pptr)
  }
  
  pptr

}


focus.create.linear.projection <- function(data, scaled=TRUE, fixed=FALSE, animated=TRUE){

  pptr = .Call("focusLinearProjection", data, as.integer(scaled), as.integer(fixed)   ) 
 
  if(animated){
    focus.set.rotation.animation(pptr)
  }

  pptr

}


focus.create.correlation.projection <- function(data, animated=TRUE){

  pptr = .Call("focusCorrelationProjection", data  )
  
  if(animated){
    focus.set.rotation.animation(pptr)
  }
  
  pptr

}



focus.create.single.point.focus.projection <- function(data, scaled=TRUE, tripod=FALSE,
    fixed=FALSE, hyperbolic=TRUE, animated=TRUE){

  pptr = .Call("focusSinglePointFocusProjection", data, as.integer(scaled),
      as.integer(tripod), as.integer(fixed), as.integer(hyperbolic)   )
  
  
  if(animated){
    focus.set.rotation.animation(pptr)
  }
  
  pptr

}



focus.create.two.point.focus.projection <- function(data, scaled=TRUE,
    hyperbolic=TRUE, animated=TRUE){

  pptr = .Call("focusTwoPointFocusProjection", data, as.integer(scaled),
      as.integer(hyperbolic)  )
  
  
  if(animated){
    focus.set.rotation.animation(pptr)
  }
  
  pptr

}




focus.create.line.focus.projection <- function(data, hyperbolic=TRUE, animated=TRUE){

  pptr = .Call("focusLineFocusProjection", data, as.integer(hyperbolic)  )
  
  if(animated){
    focus.set.rotation.animation(pptr)
  }
  
  
  pptr

}

focus.create.three.point.focus.projection <- function(data, animated=TRUE){

  pptr = .Call("focusThreePointFocusProjection", data  )
  
  if(animated){
    focus.set.rotation.animation(pptr)
  }
  
  
  pptr

}

focus.create.heat.triangulation.projection <- function(data, hyperbolic=FALSE, animated=TRUE){

  pptr = .Call("focusHeatTriangulation", data, as.integer(hyperbolic)  )
  
  if(animated){
    focus.set.rotation.animation(pptr)
  }
  
  
  pptr

}





#grouping
focus.create.projection.group <- function(projections){
  .Call("focusProjectionGroup", as.list(projections) )
}


#animations
focus.set.tangent.rotation.animation <- function(fprojector, t1=1, t2=2){
  .Call("focusTangentRotationAnimation", fprojector, as.integer(t1), as.integer(t2)   )
}

focus.set.rotation.animation <- function(fprojector){
  .Call("focusRotationAnimation", fprojector  )
}

focus.set.no.animation <- function(fprojector){
  .Call("focusNoAnimation", fprojector  )
}


#other projector related stuff
focus.add.focus.color <- function(fprojector, pIndex){
    .Call( "focusAddFocusColor", fprojector, as.integer(pIndex) )
}






#----------- display elements


#projection display
focus.add.projection.display <- function(fprojector, x, y, w, h,
    drawPoints=TRUE, drawLines=TRUE, passiveSelect = TRUE){
  
  delptr = .Call( "focusProjectionDEL", fprojector, as.double(x), as.double(y),
      as.double(w), as.double(h), as.integer(drawPoints), as.integer(drawLines),
      as.integer(passiveSelect) ) 
  
  delptr
}


#projection display backgrounds
focus.add.circle.background <- function(fdel, radius, n=200){
  .Call( "focusCircleBackground", fdel, as.double(radius), as.integer(n) )
}

focus.add.vertical.circle.lines.background <- function(fdel, radius, n){
  .Call( "focusVerticalCircleLinesBackground", fdel, as.double(radius),
      as.integer(n) )
}

focus.add.horizontal.circle.lines.background <- function(fdel, radius, n){
  .Call( "focusHorizontalCircleLinesBackground", fdel, as.double(radius),
      as.integer(n) )
}

focus.add.projector.shadow.background <- function(fdel, radius, pIndex, n=200,
    scaled=TRUE){
  .Call( "focusProjectorShadowBackground", fdel, as.double(radius),
      as.integer(pIndex), as.integer(n), as.integer(scaled) )
}

focus.add.center.shadow.background <- function(fdel, radius, n=200,
    scaled=TRUE){ 
  .Call( "focusCenterShadowBackground", fdel, as.double(radius),
      as.integer(n), as.integer(scaled) )
}

focus.set.projection.background.color <- function(fbg, r, g, b, alpha=1){
  .Call( "focusSeProjectionBackgroundColor", fbg, as.double(r), as.double(g),
      as.double(b), as.double(alpha) )
}



#other display elements
focus.add.profile.display <- function(fdata, x, y, w, h, showBG = TRUE){
  delptr = .Call( "focusProfileDEL", fdata, as.double(x), as.double(y),
      as.double(w), as.double(h), as.integer(showBG) )
  delptr
}

focus.add.points.display <- function(fdata, x, y, w, h, showBG = TRUE){
  delptr = .Call( "focusPointsDEL", fdata, as.double(x), as.double(y),
      as.double(w), as.double(h), as.integer(showBG) )
  delptr
}

focus.points.display.set.points<- function(delptr, id, X){
  delptr = .Call( "focusPointsDELaddPoints", delptr, as.integer(nrow(X)),
      as.double(t(X)), as.integer(id-1) )
  delptr
}

focus.add.pca.display <- function(fprojector, x, y, w, h, showBG = TRUE){
  delptr = .Call("focusPCADEL", fprojector, as.double(x), as.double(y),
      as.double(w), as.double(h), as.integer(showBG) )
  delptr
}


focus.add.group.pca.display <- function(fprojector, gid, x, y, w, h, showBG = TRUE){
  delptr = .Call( "focusGroupPCADEL", fprojector, as.integer(gid), as.double(x), as.double(y),
      as.double(w), as.double(h), as.integer(showBG) )
  delptr
}


focus.add.image.display <- function(fdata, image, conv, x, y, w, h, showBG = TRUE){
  delptr = .Call("focusImageDEL", fdata, as.double(image), dim(image),
      as.integer( t(conv-1) ), as.double(x), as.double(y), as.double(w),
      as.double(h), as.integer(showBG) ) 
  delptr
}


focus.add.multi.index.image.display <- function(fdata, image, conv, multiIndex, x, y, w, h, showBG = TRUE){
  delptr = .Call("focusMultiIndexImageDEL", fdata, as.double(image), dim(image),
      as.integer( t(conv-1) ), as.integer(nrow(conv)), as.integer(multi - 1),
      as.integer(nrow(multi)),  as.double(x), as.double(y), as.double(w),
      as.double(h), as.integer(showBG) ) 
  delptr
}



focus.add.point.as.image.display <- function(fdata, m, n, columnMajor=TRUE,
    transpose = FALSE, keepAspect=TRUE, x, y, w, h ){
  delptr = .Call("focusMatrixDEL", fdata, as.integer(m), as.integer(n),
      as.integer(columnMajor), as.integer(transpose), as.integer(keepAspect),
      as.double(x), as.double(y), as.double(w), as.double(h) )

  delptr
}


focus.add.point.as.image.patch.display <- function(fdata, m, n, mAll, nAll,
                                                    offsets, columnMajor=TRUE,
    transpose = FALSE, keepAspect=TRUE, x, y, w, h ){
  delptr = .Call("focusOffsetMatrixDEL", fdata, as.integer(m), as.integer(n),
                 as.integer(mAll), as.integer(nAll), as.integer( t( offsets-1 ) ),
                 as.integer(columnMajor), as.integer(transpose),
                 as.integer(keepAspect), as.double(x), as.double(y),
                 as.double(w), as.double(h) )

  delptr
}





focus.add.point.range.as.image.display <- function(fdata, m, n, indices, columnMajor=TRUE,
    transpose = FALSE, keepAspect=TRUE, x, y, w, h ){
  delptr = .Call("focusSubMatrixDEL", fdata, as.integer(m), as.integer(n), as.integer(indices),
      as.integer(columnMajor), as.integer(transpose), as.integer(keepAspect),
      as.double(x), as.double(y), as.double(w), as.double(h) )

  delptr
}



#---------- interaction methods
focus.set.projection <- function(fprojector, v, p){
  .Call( "focusSetProjection", fprojector, as.double(v), as.integer(length(v)),
      as.integer(p) )
}


focus.set.active <- function(fdata, active){
  .Call( "focusSetActive", fdata, as.integer(active), as.integer(length(active)))
}


focus.clear.active <- function(fdata ){
  .Call( "focusClearActive", fdata)
}


focus.set.group <- function(fdata, gid, indices){
  .Call( "focusSetGroup", fdata, as.integer(indices), as.integer(length(indices)),
      as.integer(gid) )
}


focus.get.group <- function(fdata, indices){
  .Call( "focusGetGroup", fdata, as.integer(indices), as.integer(length(indices)) )
}


focus.clear.group <- function(fdata, gid){
  .Call( "focusClearGroup", as.integer(fdata, gid) )
}


focus.add.paired <- function(fdata, from, to){
  .Call( "focusAddPaired", fdata, as.integer(from), as.integer(to),
      as.integer(length(from)))
}


focus.get.paired <- function(fdata, from, gid){
  .Call( "focusGetPaired", fdata, as.integer(from), length(from),
      as.integer(gid))
}


focus.clear.paired <- function(fdata){
  .Call( "focusClearPaired", fdata )
}


focus.get.density <- function(fdata){
  .Call( "focusGetDensity", fdata)
}


focus.set.density <- function(fdata, density){
  .Call( "focusSetDensity", fdata, as.double(density), as.integer(length(density) ) )
}


focus.gmra.expand.to.scale <- function(fdata, scale){
  .Call( "focusExpandToScale", fdata, as.integer(scale) )
}


focus.set.background.color <- function(r, g, b){
  .Call( "focusSetBackgroundColor", as.double(r), as.double(g), as.double(b) )
}


focus.set.default.color <- function(fdata, r, g, b){
  .Call( "focusSetDefaultColor", fdata, as.double(r), as.double(g), as.double(b) )
}


focus.set.group.color <- function(fdata, gid, r, g, b){
  .Call( "focusSetGroupColor", fdata, as.integer(gid), as.double(r), as.double(g), as.double(b) )
}


focus.set.group.colors <- function(fdata, r, g, b){
  for(i in 1:length(r)){
    focus.set.group.color(fdata, i-1, r[i], g[i], b[i]);
  }
}


focus.set.group.colors.ramp <- function(fdata, cRamp, ngroups){
  cols = cRamp( seq(0, 1, length.out=ngroups) ) / 255
  focus.set.group.colors( fdata, cols[,1], cols[,2], cols[,3] )
}

focus.add.line <- function(fdata, line){
  .Call( "focusAddLine", fdata, as.integer(line), as.integer(length(line)))
}


focus.get.selection <- function(fdata){
  .Call( "focusGetSelection", fdata)+1
}
