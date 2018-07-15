library(ggplot2)
library(viridis)

pn <- read.csv("/home/thu/CLionProjects/likelihood/output/pmax_negll.csv")
pn[which.min(pn$sum_negll),1:3]
pn[which(pn$sum_negll>255 & pn$sum_negll<265),1:3]
hist(pn$sum_negll, breaks = 100)
plot(x=pn$pmax_LM, y=pn$sum_negll)
points(x=pn$pmax_AM, y=pn$sum_negll, col="blue")

# hot zone where we observe a clear trouph of pmax_LM and sum_negll
pn.sub <- pn[which(pn$sum_negll>255 & pn$sum_negll<265),]
plot(x=pn.sub$pmax_AM, y=pn.sub$sum_negll)
points(x=pn.sub$pmax_LM, y=pn.sub$sum_negll, col="blue")
hist(pn.sub$pmax_AM, breaks=50)

pn.am <- read.csv("/home/thu/CLionProjects/likelihood/output/pmax_negll_monoAM.csv")
pn.am[which.min(pn.am$sum_negll),1:3]
plot(x=pn.am$pmax_AM, y=pn.am$sum_negll, cex=0.5)
points(x=pn.am$pmax_LM, y=pn.am$sum_negll, col='blue', cex=0.5)
lm.am <- lm(data=pn.am, formula=sum_negll~pmax_AM)
par(mfrow=c(2,2))
plot(lm.am)
par(mfrow=c(1,1))

pn.amal <- read.csv("/home/thu/CLionProjects/likelihood/output/pmax_negll_monoAMAL.csv")
pn.amal[which.min(pn.amal$sum_negll),1:3]
plot(x=pn.amal$pmax_AM, y=pn.amal$sum_negll, cex=0.5)
points(x=pn.amal$pmax_LM, y=pn.amal$sum_negll, col='blue', cex=0.5)

pn.lm <- read.csv("/home/thu/CLionProjects/likelihood/output/pmax_negll_monoLM.csv")
pn.lm[which.min(pn.lm$sum_negll),1:3]
plot(x=pn.lm$pmax_AM, y=pn.lm$sum_negll, cex=0.5)
points(x=pn.lm$pmax_LM, y=pn.lm$sum_negll, col='blue', cex=0.5)
lm.lm <- lm(data=pn.lm, formula=sum_negll~pmax_LM)
par(mfrow=c(2,2))
plot(lm.lm)
par(mfrow=c(1,1))

pn.lmal <- read.csv("/home/thu/CLionProjects/likelihood/output/pmax_negll_monoLMAL.csv")
pn.lmal[which.min(pn.lmal$sum_negll),1:3]
plot(x=pn.lmal$pmax_AM, y=pn.lmal$sum_negll, cex=0.5)
points(x=pn.lmal$pmax_LM, y=pn.lmal$sum_negll, col='blue', cex=0.5)

pn.al <- read.csv("/home/thu/CLionProjects/likelihood/output/pmax_negll_combiAL.csv")
pn.al[which.min(pn.al$sum_negll),1:3]
plot(x=pn.al$pmax_LM, y=pn.al$sum_negll)
plot(x=pn.al$pmax_AM, y=pn.al$sum_negll, col="blue")



ggplot(data=pn) +
  geom_tile(aes(x=pmax_AM, y=pmax_LM, fill=sum_negll)) +
  # scale_fill_gradient(name = 'sum negll', low = 'red', high = 'white')
  scale_fill_gradient2(name = 'sum negll', low = 'red', high = 'black', midpoint = 300)
  # geom_point(aes(x=pmax_AM, y=pmax_LM, color=round(sum_negll), size=round(-sum_negll/200)))

ggplot(data=pn) + 
    geom_raster(aes(y=pmax_AM, x=pmax_LM, fill=sum_negll)) + 
    coord_fixed(expand = FALSE) +
    scale_fill_viridis()
  
ggplot(data=pn.al.sub) +
  geom_point(aes(x=pmax_LM, y=sum_negll, color='LM')) +
  geom_point(aes(x=pmax_AM, y=sum_negll, color='AM', alpha=0.5))

pn.al.sub <- pn.al[which(pn.al$pmax_LM>0.08 & pn.al$pmax_LM<0.25),]
pn.al.sub <- pn.al[which(pn.al$sum_negll < 80),]

pn.al[which.min(pn.al$sum_negll),]


ggplot(data=pn) +
# ggplot(data=pn[which(pn$pmax_LM>0.05 & pn$pmax_LM<0.26),]) +
  geom_raster(aes(x=pmax_AM, y=pmax_LM, fill=sum_negll)) +
  labs(title="Sum negative log-likelihood for all trials", x="AM pmax", y="LM pmax", fill="sum\nnegtive\nlog-likelihood") +
  # theme(plot.margin = unit(c(1,1,1,1), "cm")) +
  coord_fixed(xlim=c(0,1), ylim=c(0.05,0.25)) +
  scale_fill_viridis(direction=-1, begin=0, end=1, option='D', alpha=0.5) 
  # scale_fill_gradientn(colors=c("#00ff00","#67a140", "#4ebd39", "#bbf7bb","#8bbb2d", "#b0b237", "#968b26", 
  #                               "#dca639", "#bc802b", "#e57e20", "#da7743", "#ea531f", "#000000")) +
  # scale_fill_gradient2(low=scales::muted("#00ff00"), high=scales::muted("red"), midpoint=120) +
ggsave(filename="/home/thu/CLionProjects/likelihood/output/plot-sum_negll-AMAL.jpeg")
  