library(plyr);
library(ggplot2);

# read date for route jump plot
src <- read.csv(file="jumps.csv",header=FALSE);
colnames(src) <- c("packet","route");
packet <- c(as.numeric(src[,1]));
route <- c(as.numeric(src[,2]));

# The following lines generate a line plot depicting which route has been taken on a per packet level
result <- data.frame(packet, route)
png(file=paste("jumps_stats.png"),width=1280, height=1280);


x_axis_breaks <- seq(0, 10, by = 1)

if(length(unique(packet)) > 10){
  x_axis_breaks <- seq(0, length(unique(packet)), by = 10)
}

if(length(unique(packet)) > 500){
  x_axis_breaks <- seq(0, length(unique(packet)), by = 50)
}

plot <- ggplot(data=result, aes(x=result$packet, y=result$route)) + geom_line();
plot <- plot + scale_x_discrete(breaks=x_axis_breaks, name="packet") + scale_y_discrete(name="route");
plot <- plot + opts(title="routes taken on each iteration") 
#+ stat_summary(fun.y=mean, geom="point", shape=5, size=4);
print(plot);
dev.off();
