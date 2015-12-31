
focus.setup.groups <- function(data, groups){
  groups = as.factor(groups);
  
  gids = levels(groups);
  for(i in 1:length(gids)){
    focus.set.group(data, i-1, which(groups == gids[i]) )
  }
}





focus.setup.correlation.plot <- function (X, groups = as.factor(rep(0,
        nrow(X))), vt=0, oblique=FALSE, drawPoints=TRUE, drawLines= FALSE,
    passiveSelect=TRUE  ){

  focus.start()
  
  #threshold based on variance
  v <- apply(X, 1, var)
  ind = which( v > vt  )
  Xs = t(scale(t(X[ind, ]) ) ) 
  Xs = Xs / sqrt( sum(Xs[1, ]^2) )



  data <- focus.create.matrix.data(Xs)

  if(oblique){
    proj <- focus.create.linear.projection(data, T, F)
  }else{
    proj <- focus.create.orthogonal.projection(data, F, F, T, F)
  }
   
  
  pdel = focus.add.projection.display(proj, 0.25, 0.25, 0.75, 0.75, drawPoints=drawPoints,
      drawLines=drawLines, passiveSelect=passiveSelect)

  if(oblique){
    focus.add.center.shadow.background(pdel, 1.0)
  }else{
    focus.add.circle.background(pdel, 1.0)
    focus.add.vertical.circle.lines.background(pdel, 1.0, 10)
  }

  focus.add.profile.display(data, 0.25, 0, 0.75, 0.25)

   gids = levels(groups);
   a <- min( 1 / (length(gids)+1), 0.1)  
   focus.add.pca.display(proj, 0, 0, 0.25, a)
   for(i in 1:length(gids)){
     focus.set.group(data, i-1, which(groups == gids[i]) )
     focus.add.group.pca.display(proj, (i-1), 0, i*a, 0.25, a)
   }


   list(data=data, proj=proj, pdel=pdel)

}




focus.setup.single.point.focus.plot <- function(X, groups = as.factor(rep(0,
        nrow(X))), hyperbolic=TRUE, drawPoints=TRUE, drawLines= FALSE,
    passiveSelect=TRUE, animation=TRUE){

  focus.start()
  

  data <- focus.create.matrix.data(X)
  proj <- focus.create.single.point.focus.projection( data, TRUE, TRUE, FALSE,
      hyperbolic=hyperbolic )
 
  if(animation){
    focus.set.tangent.rotation.animation(proj, 1, 2); 
  }
  pdel = focus.add.projection.display(proj, 0, 0, 1, 1, drawPoints=drawPoints,
      drawLines=drawLines, passiveSelect=passiveSelect)

  if(hyperbolic){
    for( r in c(0.25, 0.5, 1.0, 2,0) ){
      focus.add.center.shadow.background(pdel, r)
    }
  }

  focus.add.circle.background(pdel, 1.0)
  
  focus.setup.groups(data, groups)

  list(data=data, proj=proj, pdel=pdel)

}


focus.setup.two.point.focus.plot <- function(X, groups = as.factor(rep(0,
        nrow(X))), hyperbolic=TRUE, drawPoints=TRUE, drawLines= FALSE,
    passiveSelect=TRUE ){

  focus.start()
  

  data <- focus.create.matrix.data(X)
  proj <- focus.create.two.point.focus.projection( data, hyperbolic=hyperbolic )
  
  
  pdel = focus.add.projection.display(proj, 0, 0, 1, 1, drawPoints=drawPoints,
      drawLines=drawLines, passiveSelect=passiveSelect)
  
  for( r in c(0.25, 0.5, 1.0, 2,0) ){
    focus.add.projector.shadow..background(pdel, r, 0)
    focus.add.projector.shadow.background(pdel, r, 1)
    focus.add.center.shadow.background(pdel, r)
  }
  
  focus.setup.groups(data, groups)

  list(data=data, proj=proj, pdel=pdel)

}




focus.setup.line.focus.plot <- function(X, groups = as.factor(rep(0, nrow(X))),
    hyperbolic=TRUE, drawPoints=TRUE, drawLines= FALSE, passiveSelect=TRUE ){

  focus.start()
  

  data <- focus.create.matrix.data(X)
  proj <- focus.create.line.focus.projection(data, hyperbolic=hyperbolic)
  
  
  pdel = focus.add.projection.display(proj, 0, 0, 1, 1, drawPoints=drawPoints,
      drawLines=drawLines, passiveSelect=passiveSelect)
    
  for( r in c(0.25, 0.5, 1.0, 2,0) ){
    focus.add.center.shadow.background(pdel, r)
  }
  
  focus.setup.groups(data, groups)


  list(data=data, proj=proj, pdel=pdel)

}





focus.setup.three.point.focus.plot <- function(X, groups = as.factor(rep(0,
        nrow(X))), drawPoints=TRUE, drawLines= FALSE, passiveSelect=TRUE ){

  focus.start()
  
  data <- focus.create.matrix.data(X )
  proj <- focus.create.three.point.focus.projection( data )
  
  pdel = focus.add.projection.display(proj, 0, 0, 1, 1, drawPoints=drawPoints,
      drawLines=drawLines, passiveSelect=passiveSelect) 
  
  for( r in c(0.25, 0.5, 1.0, 2,0) ){
    focus.add.projector.shadow.background(pdel, r, 0)
    focus.add.projector.shadow.background(pdel, r, 1)
    focus.add.projector.shadow.background(pdel, r, 2)
  }

  focus.setup.groups(data, groups)


  list(data=data, proj=proj, pdel=pdel)

}


focus.setup.heat.triangulation.plot <- function(X, groups = as.factor(rep(0,
        nrow(X))), drawPoints=TRUE, drawLines= FALSE, passiveSelect=TRUE ){

  
  data <- focus.create.matrix.data(X )
  proj <- focus.create.heat.triangulation.projection( data )
  
  pdel = focus.add.projection.display(proj, 0, 0, 1, 1, drawPoints=drawPoints,
      drawLines=drawLines, passiveSelect=passiveSelect) 
  

  focus.setup.groups(data, groups)

  focus.start()

  list(data=data, proj=proj, pdel=pdel)

}





focus.setup.single.point.focus.versus.orthogonal.plot <- function(X, groups=
    as.factor(rep(0, nrow(X))), radii = c(0.5, 1, 2, 4), drawPoints=TRUE,
    drawLines= FALSE, passiveSelect=TRUE, fhyperbolic=TRUE, ohyperbolic=FALSE, grouped=TRUE){


  focus.start()
  
  data <- focus.create.matrix.data(X)
  
  proj1 <- focus.create.single.point.focus.projection( data, TRUE, TRUE,
      hyperbolic=fhyperbolic )
  focus.set.tangent.rotation.animation(proj1, 1, 2); 
  pdel1 = focus.add.projection.display(proj1, 0, 0, 0.5, 1,
      drawPoints=drawPoints, drawLines=drawLines, passiveSelect = passiveSelect)
  
  proj2 <- focus.create.orthogonal.projection( data, TRUE, TRUE, FALSE,
      hyperbolic=ohyperbolic )
  pdel2 = focus.add.projection.display(proj2, 0.5, 0, 0.5, 1,
      drawPoints=drawPoints, drawLines=drawLines, passiveSelect=passiveSelect)

  #setup backgrounds
#if(hyperbolic){
#    for(r in radii){
#      focus.add.center.shadow.background(pdel1, r)
#      focus.add.center.shadow.background(pdel2, r) 
#    }
#  }
  focus.add.circle.background(pdel1, 1.0)
  focus.add.circle.background(pdel2, 1.0)
  

  if(grouped){
    focus.create.projection.group( list(proj1, proj2) )
  }

  focus.setup.groups(data, groups)

  list(data=data, proj1=proj1, pdel1=pdel1, proj2 = proj2, pdel2=pdel2)


}

focus.setup.4way <-function(X, groups = as.factor(rep(0, nrow(X))) ){

    data <- focus.create.matrix.data(X)

    hproj <- focus.create.single.point.focus.projection( data, scaled=TRUE, fixed=FALSE,
        tripod=TRUE )
    cproj <- focus.create.correlation.projection( data )
    pproj <- focus.create.orthogonal.projection( data, scaled=TRUE, fixed=FALSE,
        tripod=TRUE )
    vproj <- focus.create.three.point.focus.projection( data )

    focus.create.projection.group( list(hproj, cproj, pproj, vproj) )

    focus.start()

    hdel = focus.add.projection.display(hproj, 0, 0, 0.5, 0.5)
    focus.add.center.shadow.background(hdel, 0.25)
    focus.add.center.shadow.background(hdel, 0.5)
    focus.add.center.shadow.background(hdel, 1.0)
    focus.add.circle.background(hdel, 1.0)

    cdel = focus.add.projection.display(cproj, 0, 0.5, 0.5, 0.5)
    focus.add.circle.background(cdel, 1.0)
    focus.add.vertical.circle.lines.background(cdel, 1.0, 10)

    pdel = focus.add.projection.display(pproj, 0.5, 0, 0.5, 0.5)
    focus.add.center.shadow.background(pdel, 0.25)
    focus.add.center.shadow.background(pdel, 0.5)
    focus.add.center.shadow.background(pdel, 1.0)
    focus.add.circle.background(pdel, 1.0)


    vdel = focus.add.projection.display(vproj, 0.5, 0.5, 0.5, 0.5)
    for(i in 0:2){
      focus.add.projector.shadow.background(vdel, 0.5, i)
      focus.add.projector.shadow.background(vdel, 1.0, i)
      focus.add.projector.shadow.background(vdel, 2.0, i)
    }


    focus.setup.groups(data, groups)


  list(data=data, hproj=hproj, vproj = vproj, pproj=pproj, cproj=cproj,
      hdel=hdel, vdel=vdel, pdel=pdel, cdel=cdel)
}





focus.setup.6way <-function(X, groups = as.factor(rep(0, nrow(X))) ){

    data <- focus.create.matrix.data(X)
    focus.setup.6way.data(data, groups);
}


focus.setup.6way.data <-function(data, groups = as.factor(rep(0, nrow(X))) ){

    hproj <- focus.create.single.point.focus.projection( data, scaled=TRUE,
        tripod=TRUE, hyperbolic=TRUE )
    cproj <- focus.create.correlation.projection( data )
    pproj <- focus.create.orthogonal.projection( data, scaled=TRUE,
        fixed=FALSE, tripod=TRUE )
    vproj <- focus.create.three.point.focus.projection( data )
    lproj <- focus.create.line.focus.projection( data )
    dproj <- focus.create.two.point.focus.projection( data )

    focus.create.projection.group( list(hproj, cproj, pproj, vproj, lproj, dproj) )


    hdel = focus.add.projection.display(hproj, 0, 0, 0.33, 0.5)
    focus.add.center.shadow.background(hdel, 0.25)
    focus.add.center.shadow.background(hdel, 0.5)
    focus.add.center.shadow.background(hdel, 1.0)
    focus.add.circle.background(hdel, 1.0)

    cdel = focus.add.projection.display(cproj, 0, 0.5, 0.33, 0.5)
    focus.add.circle.background(cdel, 1.0)
    focus.add.vertical.circle.lines.background(cdel, 1.0, 10)

    pdel = focus.add.projection.display(pproj, 0.33, 0, 0.33, 0.5)
    focus.add.center.shadow.background(pdel, 0.25)
    focus.add.center.shadow.background(pdel, 0.5)
    focus.add.center.shadow.background(pdel, 1.0)
    focus.add.circle.background(pdel, 1.0)


    vdel = focus.add.projection.display( vproj, 0.33, 0.5, 0.33, 0.5)
    for(i in 0:2){
      focus.add.projector.shadow.background(vdel, 0.5, i)
      focus.add.projector.shadow.background(vdel, 1.0, i)
      focus.add.projector.shadow.background(vdel, 2.0, i)
    }

  ldel = focus.add.projection.display(lproj, 0.66, 0, 0.33, 0.5)
  focus.add.center.shadow.background(ldel, 0.25)
  focus.add.center.shadow.background(ldel, 0.5)
  focus.add.center.shadow.background(ldel, 1.0)
  focus.add.center.shadow.background(ldel, 2.0)

  
  ddel = focus.add.projection.display(dproj, .66, 0.5, 0.33, 0.5)
  focus.add.center.shadow.background(ddel, 0.25)
  focus.add.center.shadow.background(ddel, 0.5)
  focus.add.center.shadow.background(ddel, 1.0)
  focus.add.center.shadow.background(ddel, 2.0)
  
  for(i in 0:1){
    focus.add.projector.shadow.background(ddel, 0.25, i)
    focus.add.projector.shadow.background(ddel, 0.5, i)
    focus.add.projector.shadow.background(ddel, 1.0, i)
    focus.add.projector.shadow.background(ddel, 2.0, i)
  }

  focus.setup.groups(data, groups)

  focus.start()

  list(data=data, hproj=hproj, vproj = vproj, pproj=pproj, cproj=cproj,
      lproj=lproj, dproj=dproj, hdel=hdel, vdel=vdel, pdel=pdel, cdel=cdel,
      ldel=ldel, ddel=ddel)
}

