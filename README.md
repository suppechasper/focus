# Focused Maps for Interactive Visualization of High-Dimensional Data

![focus-small.png](https://bitbucket.org/repo/46o8jG/images/3641832854-focus-small.png)

Focus is an interactive visualization framework for exploratory analysis of high-dimensional data. Focus is based on two dimensional scatterplots of the high-dimensional data. Several linear and non-linear projection methods are implemented that highlight different aspects of the data set. The projections can be linked to domain specific visualization for the individual points in the scatterplot. Focus is tightly integrated with the package gmra. The package gmra computes hierarchical decomposition of data sets. Focus can display and interactively explore the tree structure of the hierarchical decomposition of the data provided by gmra data structures. 

For more information and a paper with examples see: http://pages.uoregon.edu/sgerber/research/focus.htm


The focus package requires pthreads and freeglut. On mac systems freeglut is typically installed with X11. Additionally you might need to edit the Makevars file in the src folder of the package and add the -L option to point to your OpenGL library. Detailed instructions for mac system are here. 

There is a commandline tool for testing but focus is meant to be used within R.