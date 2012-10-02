library(plyr);
library(ggplot2);

data <- read.csv(file="route_pheromone_log.csv",header=FALSE);

packet <- c(as.numeric(data[,1]));
route <- c(as.numeric(data[,2]));
phi <- c(as.numeric(data[,3]));

plotData <- data.frame(packet, route, phi);
result <- data.frame();

for(p in 1:length(unique(packet))){
  counter <- 0;

  temp <- subset(plotData, plotData$packet == p)
  if(temp[1,3] > 0.09){
    counter <- counter + 1;
  }

  if(temp[2,3] > 0.09){
    counter <- counter + 1;
  }

  result <- rbind(result, data.frame(packet = p, active = counter));
}

png(file=paste("network_lifetime.png"),width=1280, height=1280);
x_axis_breaks <- seq(1, max(packet), by = 10);
plot <- ggplot() + geom_step(data=result, size = 1.5, mapping=aes(x=result$packet, y=result$active), direction="vh");
#plot <- ggplot(data=result, aes(x=result$packet, y=result$active, group=result$active)) + geom_step(size = 1.5, mapping=aes(x=result$packet, y=result$active));
plot <- plot + scale_x_discrete(breaks=x_axis_breaks, name="packet") + scale_y_discrete(name="#routes");
plot <- plot + opts(title="network life time") 
print(plot);
dev.off();
