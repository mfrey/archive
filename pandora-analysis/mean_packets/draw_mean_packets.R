library(lattice);
library(plyr,lib.loc="~/.R/library");

data <- read.csv(file="final_mean_packets.csv",header=FALSE);

experiment <- c(as.numeric(data[,1]));
alpha <- c(as.numeric(data[,2]));
beta <- c(as.numeric(data[,3]));
packets <- c(as.numeric(data[,4]));

result <- data.frame(alpha, beta, packets);

png(file=paste("mean_packets.png"),width=1280, height=1280);
meanJumpPlot <- wireframe(result$packets ~ result$alpha * result$beta, data=result,
  scales = list(arrows=FALSE, cex= .45, col = "black", font = 3),
  #scales = list(arrows=FALSE, cex= .45, col = "black", font = 3, x = list(labels = seq(1.0, 5.0, by = 0.5)),
  #y = list(labels = seq(1.0, 5.0, by = 0.5))),
  drape = TRUE, colorkey = TRUE, main = "Mean Packets per Alpha/Beta",
  # set the color
  col.regions = terrain.colors(100),
  # rotate the figure
  screen = list(z = -60, x = -60),
  # set the labels of the axis
  xlab="alpha", ylab="beta", zlab="mean \n jumps");
print(meanJumpPlot);
dev.off();
