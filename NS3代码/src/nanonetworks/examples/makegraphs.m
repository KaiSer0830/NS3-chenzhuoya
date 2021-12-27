%#-*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
%#*
%#* Copyright (c) 2013,2014 TELEMATICS LAB, DEI - Politecnico di Bari
%#*
%#* This program is free software; you can redistribute it and/or modify
%#* it under the terms of the GNU General Public License version 2 as
%#* published by the Free Software Foundation;
%#*
%#* This program is distributed in the hope that it will be useful,
%#* but WITHOUT ANY WARRANTY; without even the implied warranty of
%#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%#* GNU General Public License for more details.
%#*
%#* You should have received a copy of the GNU General Public License
%#* along with this program; if not, write to the Free Software
%#* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
%#*
%#* Author: Giuseppe Piro <peppe@giuseppepiro.com>, <g.piro@poliba.it>
%#*/



%#
%#   THIS SCRIPT WOULD REPRODUCE THE WORK PRESENTED IN
%#
%#   Giuseppe Piro, Luigi Alredo Grieco, Gennaro Boggia, and Pietro Camarda,"
%#   Nano-Sim: simulating electromagnetic-based nanonetworks in the Network Simulator 3",
%#   in Proc. of Workshop on NS- 3 (held in conjunction with SIMUTools 2013),
%#   Cannes, France, Mar., 2013
%#
%#   PLEASE, CITE THE WORK IF YOU OUR SIMULATOR
%#



close all;
clc;
clear all;

nbNanoNodes = [50 100 150 200 250]
txRangeNanoNodes = [0.001 0.01 0.015 0.02]


for n=1:length(nbNanoNodes)
for tx=1:length(txRangeNanoNodes)


    % Transparent-MAC with Selective Flooding
    dat=load(strcat('TX_N_',num2str(nbNanoNodes(n)),'_R_50_G_1_nTxRange_',num2str(txRangeNanoNodes(tx)),'_macType_1_l3Type_1'));
    tx_1 (n,tx) = mean(dat);
    dat=load(strcat('RX_N_',num2str(nbNanoNodes(n)),'_R_50_G_1_nTxRange_',num2str(txRangeNanoNodes(tx)),'_macType_1_l3Type_1'));
    rx_1 (n,tx) = mean(dat);
    dat=load(strcat('PHYTX_N_',num2str(nbNanoNodes(n)),'_R_50_G_1_nTxRange_',num2str(txRangeNanoNodes(tx)),'_macType_1_l3Type_1'));
    ptx_1 (n,tx) = mean(dat);
    
   
    
    % Smart-MAC with Selective Flooding        
    dat=load(strcat('TX_N_',num2str(nbNanoNodes(n)),'_R_50_G_1_nTxRange_',num2str(txRangeNanoNodes(tx)),'_macType_2_l3Type_1'));
    tx_2 (n,tx) = mean(dat);
    dat=load(strcat('RX_N_',num2str(nbNanoNodes(n)),'_R_50_G_1_nTxRange_',num2str(txRangeNanoNodes(tx)),'_macType_2_l3Type_1'));
    rx_2 (n,tx) = mean(dat);
    dat=load(strcat('PHYTX_N_',num2str(nbNanoNodes(n)),'_R_50_G_1_nTxRange_',num2str(txRangeNanoNodes(tx)),'_macType_2_l3Type_1'));
    ptx_2 (n,tx) = mean(dat);
    
    
    
    % Smart-MAC with Random Routing               
    dat=load(strcat('TX_N_',num2str(nbNanoNodes(n)),'_R_50_G_1_nTxRange_',num2str(txRangeNanoNodes(tx)),'_macType_2_l3Type_2'));
    tx_3 (n,tx) = mean(dat);
    dat=load(strcat('RX_N_',num2str(nbNanoNodes(n)),'_R_50_G_1_nTxRange_',num2str(txRangeNanoNodes(tx)),'_macType_2_l3Type_2'));
    rx_3 (n,tx) = mean(dat);
    dat=load(strcat('PHYTX_N_',num2str(nbNanoNodes(n)),'_R_50_G_1_nTxRange_',num2str(txRangeNanoNodes(tx)),'_macType_2_l3Type_2'));
    ptx_3 (n,tx) = mean(dat);
    
    
end
end


figure('Name', 'TX packets, Transparent-MAC with Selective Flooding')
bar(nbNanoNodes, tx_1)
legend ('tx range = 0.001','tx range = 0.01','tx range = 0.015','tx range = 0.02');
xlabel ('Number of Nano nodes');
ylabel ('Number of transmitted packets');

figure('Name', 'RX packets, Transparent-MAC with Selective Flooding')
bar(nbNanoNodes, rx_1)
legend ('tx range = 0.001','tx range = 0.01','tx range = 0.015','tx range = 0.02');
xlabel ('Number of Nano nodes');
ylabel ('Number of received packets');

figure('Name', 'Physical TX packets, Transparent-MAC with Selective Flooding')
bar(nbNanoNodes, ptx_1)
legend ('tx range = 0.001','tx range = 0.01','tx range = 0.015','tx range = 0.02');
xlabel ('Number of Nano nodes');
ylabel ('Number of physical transmissions');





figure('Name', 'TX packets, Smart-MAC with Selective Flooding')
bar(nbNanoNodes, tx_2)
legend ('tx range = 0.001','tx range = 0.01','tx range = 0.015','tx range = 0.02');
xlabel ('Number of Nano nodes');
ylabel ('Number of transmitted packets');

figure('Name', 'RX packets, Smart-MAC with Selective Flooding')
bar(nbNanoNodes, rx_2)
legend ('tx range = 0.001','tx range = 0.01','tx range = 0.015','tx range = 0.02');
xlabel ('Number of Nano nodes');
ylabel ('Number of received packets');

figure('Name', 'Physical TX packets, Smart-MAC with Selective Flooding')
bar(nbNanoNodes, ptx_2)
legend ('tx range = 0.001','tx range = 0.01','tx range = 0.015','tx range = 0.02');
xlabel ('Number of Nano nodes');
ylabel ('Number of physical transmissions');






figure('Name', 'TX packets, Smart-MAC with Random Routing')
bar(nbNanoNodes, tx_3)
legend ('tx range = 0.001','tx range = 0.01','tx range = 0.015','tx range = 0.02');
xlabel ('Number of Nano nodes');
ylabel ('Number of transmitted packets');

figure('Name', 'RX packets, Smart-MAC with Random Routing')
bar(nbNanoNodes, rx_3)
legend ('tx range = 0.001','tx range = 0.01','tx range = 0.015','tx range = 0.02');
xlabel ('Number of Nano nodes');
ylabel ('Number of received packets');

figure('Name', 'Physical TX packets, Smart-MAC with Random Routing')
bar(nbNanoNodes, ptx_3)
legend ('tx range = 0.001','tx range = 0.01','tx range = 0.015','tx range = 0.02');
xlabel ('Number of Nano nodes');
ylabel ('Number of physical transmissions');


