#!/bin/bash
cd /home/noferini/traffic
cat stats/tel |awk '{print "root -b -q -d drawLD.C\\\(\\\""$1"\\\"\\\)"}'|bash
