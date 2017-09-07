#!/bin/bash
cd ~/e3trafficStats
cp /home/noferini/traffic/*.png .
cp /home/noferini/traffic/trafficSummary.txt .
cp ./templatesTrafficPage/index.html .
cp trafficSummary.txt trafficSummary.tmp
cat trafficSummary.tmp |grep -v total|awk '{print "./templatesTrafficPage/addEff.sh",$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,$16,$17}'|bash

export TIME=$(date "+%A %d %B %Y")
sed  -i "s/TIME_TEMP/$TIME/g" index.html

