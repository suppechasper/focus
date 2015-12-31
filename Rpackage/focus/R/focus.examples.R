
focus.example.normals <- function(n=1000, r=c(1, 2, 3), dim=10, focusColor=F){
  
  X = c()
  groups = c()
  
  for(radius in r){
    X = rbind(X, matrix( radius*rnorm(dim*n), ncol=dim ) )
    groups = c(groups, rep(radius, n) )
  }
  groups = as.factor(groups)

  res  = focus.setup.6way(X, groups)

  if(focusColor){
    library(RColorBrewer)
    set = brewer.pal("Spectral", n=11)
    ramp = colorRamp(set)
    focus.set.group.colors.ramp(res$data, ramp, 100)
    focus.add.focus.color(res$hproj, 2) 
  }
  list(res, X)
}




focus.example.hypercube <- function(n=100, dim=4, r=0.1){
 
  library(RColorBrewer)

  V = focus.hypercube.vertices(dim)

  X = c()
  groups = c()
  
  for(i in 1:nrow(V)){
    tmp =  sweep(matrix( r*(runif(dim*n)-0.5), ncol=dim ), 2, V[i,], "+" ) 
    X = rbind(X, tmp)
    groups = c(groups, rep(i-1, n) )
  }

  groups = as.factor(groups)

  
  res = focus.setup.6way(X, groups)
  
  pal = brewer.pal(name = "Spectral", n=11)
  ramp = colorRamp( pal)
  focus.set.group.colors.ramp(res$data, ramp, nrow(V) )

  res
}



focus.example.normal.plus.uniform <- function(n=1000, dim=4, r=0.1, sigma=1){
  X = matrix(sigma*rnorm(dim*n), ncol=dim)
  X = rbind(X, matrix(r*runif(dim*n), ncol=dim) )
  focus.setup.6way(X)  
}



focus.example.planes <- function(n=1000, dim=30, nPlanes=6, pDim=2){

  X = c()
  groups = c()

  for(i in 1:nPlanes){
    tmp = matrix(runif(pDim*n) - 0.5, ncol=pDim) 
    Y = matrix(rnorm(pDim*dim), nrow=pDim)
    X = rbind(X, tmp %*% Y)
    groups = c(groups, rep(i, n) )
  }

  groups = as.factor(groups)
  focus.setup.6way(X, groups)  
}



