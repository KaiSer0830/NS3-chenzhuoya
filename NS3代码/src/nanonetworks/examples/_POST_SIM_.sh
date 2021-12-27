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

mkdir POSTSIM
rm POSTSIM/*

for nbNanoNodes in 50 100 150 200 250
do
for txRangeNanoNodes in 0.001 0.01 0.015 0.02
do
for seed in 1 2 3 4 5
do


	TX=RESULTS/RES_TX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_1_l3Type_1_seed_${seed}
    RX=RESULTS/RES_RX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_1_l3Type_1_seed_${seed}
    PHYTX=RESULTS/RES_PHYTX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_1_l3Type_1_seed_${seed}

    grep -c ".*" $TX >> POSTSIM/TX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_1_l3Type_1
    grep -c ".*" $RX >> POSTSIM/RX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_1_l3Type_1
    grep -c ".*" $PHYTX >> POSTSIM/PHYTX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_1_l3Type_1



	TX=RESULTS/RES_TX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_2_l3Type_1_seed_${seed}
	RX=RESULTS/RES_RX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_2_l3Type_1_seed_${seed}
	PHYTX=RESULTS/RES_PHYTX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_2_l3Type_1_seed_${seed}

	grep -c ".*" $TX >> POSTSIM/TX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_2_l3Type_1
	grep -c ".*" $RX >> POSTSIM/RX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_2_l3Type_1
	grep -c ".*" $PHYTX >> POSTSIM/PHYTX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_2_l3Type_1



        TX=RESULTS/RES_TX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_2_l3Type_2_seed_${seed}
        RX=RESULTS/RES_RX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_2_l3Type_2_seed_${seed}
        PHYTX=RESULTS/RES_PHYTX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_2_l3Type_2_seed_${seed}

        grep -c ".*" $TX >> POSTSIM/TX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_2_l3Type_2
        grep -c ".*" $RX >> POSTSIM/RX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_2_l3Type_2
        grep -c ".*" $PHYTX >> POSTSIM/PHYTX_N_${nbNanoNodes}_R_50_G_1_nTxRange_${txRangeNanoNodes}_macType_2_l3Type_2

done
done
done
