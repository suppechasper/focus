
focus.hypercube.vertices <- function(d)
{
	numv=2^d
	vertices=matrix(0,nrow=numv,ncol=d)
	for (j in 1:numv) {
		count=j-1
		for (i in 1:d) {
			vertices[j,i]=count%%2
			count=count%/%2
		}	
	}
	vertices
}



focus.torus <- function(n, r=1, R=2){
   alpha= runif(n)*2*pi;
   beta= runif(n)*2*pi;
   data.frame(
        x = ( R + r*cos(alpha) ) * cos(beta),
        y = ( R + r*cos(alpha) ) * sin(beta),
        z = r * sin(alpha)
       )

}


 
focus.sphere <- function(n, d=2){
   X = matrix( rnorm(n*(d+1)), nrow=n );
   X = t(scale(t(X), center=F))
}



focus.sinusoids.grid <- function(n, d=100, f = c(0.5, 1, 2)  ){

  t = seq(0, 2*pi, length.out=d)
  s = list()
  for(i in 1:length(f) ){
    s[[i]] = seq(0, 2*pi, length.out=n+1)[1:n]
  }
  alpha = expand.grid(s)
  
  X = matrix(0, nrow=nrow(alpha), ncol=d)
  for( i in 1:nrow(alpha) ){
    for(j in 1:length(f)){
      X[i, ] = X[i, ]+sin(alpha[i, j] + f[j]*t)
    }
  }

  list(X=X, alpha=alpha)

}



focus.sinusoids <- function(n, d=100, f = c(0.5, 1, 2) ){

  t = seq(0, 2*pi, length.out=d)
  X = matrix(0, nrow=n, ncol=d)
  alpha = matrix(0, nrow=n, ncol=length(f) )
  for( i in 1:n ){
    for(j in 1:length(f)){
      a = runif(1)*2*pi
      alpha[i, j] = a;
      X[i, ] = X[i, ]+sin(a + f[j]*t)
    }
  }

  list(X=X, alpha=alpha)

}




