#/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#*
#* Copyright (c) 2013,2014 TELEMATICS LAB, DEI - Politecnico di Bari
#*
#* This program is free software; you can redistribute it and/or modify
#* it under the terms of the GNU General Public License version 2 as
#* published by the Free Software Foundation;
#*
#* This program is distributed in the hope that it will be useful,
#* but WITHOUT ANY WARRANTY; without even the implied warranty of
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#* GNU General Public License for more details.
#*
#* You should have received a copy of the GNU General Public License
#* along with this program; if not, write to the Free Software
#* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#*
#* Author: Giuseppe Piro <peppe@giuseppepiro.com>, <g.piro@poliba.it>
#*/



#
#   THIS SCRIPT WOULD REPRODUCE THE WORK PRESENTED IN
#
#   Giuseppe Piro, Luigi Alredo Grieco, Gennaro Boggia, and Pietro Camarda,"
#   Nano-Sim: simulating electromagnetic-based nanonetworks in the Network Simulator 3",
#   in Proc. of Workshop on NS- 3 (held in conjunction with SIMUTools 2013),
#   Cannes, France, Mar., 2013
#
#   PLEASE, CITE THE WORK IF YOU OUR SIMULATOR
#

set -x

mkdir RESULTS
rm RESULTS/*

for nbNanoNodes in 50 100 150 200 250
do
for txRangeNanoNodes in 0.001 0.01 0.015 0.02
do
for seed in 1 2 3 4 5
do

    #selective flooding with transparent mac
    ./waf --run "scratch/health-care --seed=${seed} --nbNanoNodes=${nbNanoNodes} --txRangeNanoNodes=${txRangeNanoNodes} --macType=1 --l3Type=1"


    #selective flooding with smart mac
    ./waf --run "scratch/health-care --seed=${seed} --nbNanoNodes=${nbNanoNodes} --txRangeNanoNodes=${txRangeNanoNodes} --macType=2 --l3Type=1"

    #random routing with smart  mac
    ./waf --run "scratch/health-care --seed=${seed} --nbNanoNodes=${nbNanoNodes} --txRangeNanoNodes=${txRangeNanoNodes} --macType=2 --l3Type=2"

    mv RES_* RESULTS

done
done
done
