/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013,2014 TELEMATICS LAB, DEI - Politecnico di Bari
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Giuseppe Piro <peppe@giuseppepiro.com>, <g.piro@poliba.it>
 */

#include "ns3/object.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/packet-burst.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/double.h"
#include "ns3/mobility-model.h"
#include "ns3/spectrum-phy.h"
#include "ns3/spectrum-propagation-loss-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-delay-model.h"
#include "nano-spectrum-channel.h"
#include "nano-spectrum-phy.h"
#include <iostream>
#include <utility>
#include "simple-nano-device.h"

NS_LOG_COMPONENT_DEFINE("NanoSpectrumChannel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(NanoSpectrumChannel);

NanoSpectrumChannel::NanoSpectrumChannel() {
	NS_LOG_FUNCTION(this);
}

void NanoSpectrumChannel::DoDispose() {
	NS_LOG_FUNCTION(this);
	m_phyList.clear();
	m_spectrumModel = 0;
	m_propagationDelay = 0;
	m_propagationLoss = 0;
	m_spectrumPropagationLoss = 0;
	SpectrumChannel::DoDispose();
}

TypeId NanoSpectrumChannel::GetTypeId(void) {
	NS_LOG_FUNCTION_NOARGS ();
	static TypeId tid = TypeId("ns3::NanoSpectrumChannel").SetParent<SpectrumChannel>().AddConstructor<NanoSpectrumChannel>();
	return tid;
}

void NanoSpectrumChannel::AddRx(Ptr<SpectrumPhy> phy) {
	NS_LOG_FUNCTION(this << phy);
	m_phyList.push_back(phy);
}

void NanoSpectrumChannel::StartTx(Ptr<SpectrumSignalParameters> txParams) {
	NS_LOG_FUNCTION(this << txParams->psd << txParams->duration << txParams->txPhy);
	NS_ASSERT_MSG(txParams->psd, "NULL txPsd");
	NS_ASSERT_MSG(txParams->txPhy, "NULL txPhy");

	if (m_spectrumModel == 0) {
		m_spectrumModel = txParams->psd->GetSpectrumModel();
	} else {
		NS_ASSERT(*(txParams->psd->GetSpectrumModel()) == *m_spectrumModel);
	}

	Ptr<MobilityModel> senderMobility = txParams->txPhy->GetMobility();
	double txRange = txParams->txPhy->GetObject<NanoSpectrumPhy>()->GetTransmissionRange();				//获取物理层的节点传输范围

	for (PhyList::const_iterator rxPhyIterator = m_phyList.begin(); rxPhyIterator != m_phyList.end(); ++rxPhyIterator) {
		if ((*rxPhyIterator) != txParams->txPhy) {
			Time delay = MicroSeconds(0);						//delay为+0.0fs
			Ptr<MobilityModel> receiverMobility = (*rxPhyIterator)->GetMobility();
			double distance = (receiverMobility->GetDistanceFrom(senderMobility));				//计算两个节点之间的距离

			NS_LOG_FUNCTION(this << "check txrange" << distance << txRange);
			if (distance <= txRange) {
				NS_LOG_LOGIC("copying signal parameters " << txParams);
				Ptr<SpectrumSignalParameters> rxParams = txParams->Copy();

				Ptr<NetDevice> netDev = (*rxPhyIterator)->GetDevice();
				if (netDev) {
					uint32_t dstNode = netDev->GetNode()->GetId();
					Simulator::ScheduleWithContext(dstNode, delay, &NanoSpectrumChannel::StartRx, this, rxParams, *rxPhyIterator);
				} else {
					Simulator::Schedule(delay, &NanoSpectrumChannel::StartRx, this, rxParams, *rxPhyIterator);
				}
			}
		}
	}
}

void NanoSpectrumChannel::StartRx(Ptr<SpectrumSignalParameters> params, Ptr<SpectrumPhy> receiver) {
	NS_LOG_FUNCTION(this << params);
	receiver->StartRx(params);
}

uint32_t NanoSpectrumChannel::GetNDevices(void) const {
	NS_LOG_FUNCTION(this);
	return m_phyList.size();
}

Ptr<NetDevice> NanoSpectrumChannel::GetDevice(uint32_t i) const {
	NS_LOG_FUNCTION(this << i);
	return m_phyList.at(i)->GetDevice()->GetObject<NetDevice>();
}

void NanoSpectrumChannel::AddPropagationLossModel(Ptr<PropagationLossModel> loss) {
	NS_LOG_FUNCTION(this << loss);
	NS_ASSERT(m_propagationLoss == 0);
	m_propagationLoss = loss;
}

void NanoSpectrumChannel::AddSpectrumPropagationLossModel(Ptr<SpectrumPropagationLossModel> loss) {
	NS_LOG_FUNCTION(this << loss);
	NS_ASSERT(m_spectrumPropagationLoss == 0);
	m_spectrumPropagationLoss = loss;
}

void NanoSpectrumChannel::SetPropagationDelayModel(Ptr<PropagationDelayModel> delay) {
	NS_LOG_FUNCTION(this << delay);
	NS_ASSERT(m_propagationDelay == 0);
	m_propagationDelay = delay;
}

Ptr<SpectrumPropagationLossModel> NanoSpectrumChannel::GetSpectrumPropagationLossModel(void) {
	NS_LOG_FUNCTION(this);
	return m_spectrumPropagationLoss;
}

void NanoSpectrumChannel::SetTransmissionRange(double r) {
	NS_LOG_FUNCTION(this << r);
	m_transmissionRange = r;
}

double NanoSpectrumChannel::GetTransmissionRange(void) {
	NS_LOG_FUNCTION(this);
	return m_transmissionRange;
}

std::vector<NanoDetail> NanoSpectrumChannel::GetNeighborss(Ptr<SpectrumPhy> phy) {				//获取的邻居节点类型包括移动性、index、节点id、节点类型
	NS_LOG_FUNCTION(this);
	//std::vector<std::pair<SimpleNanoDevice,uint32_t>>neighbors;

	std::vector<NanoDetail> neighbors;

	Ptr<MobilityModel> senderMobility = phy->GetMobility();
	double txRange = phy->GetObject<NanoSpectrumPhy>()->GetTransmissionRange();

	for (PhyList::const_iterator rxPhyIterator = m_phyList.begin(); rxPhyIterator != m_phyList.end(); ++rxPhyIterator) {
		if ((*rxPhyIterator) != phy) {
			Ptr<MobilityModel> receiverMobility = (*rxPhyIterator)->GetMobility();
			double distance = (receiverMobility->GetDistanceFrom(senderMobility));
			NS_LOG_FUNCTION(this << "check txrange" << distance << txRange);
			if (distance <= txRange) {					//根据距离判断邻居节点
				NanoDetail neighbor;
				neighbor.in_mobility = (*rxPhyIterator)->GetDevice()->GetObject<SimpleNanoDevice>()->Getmobility();
				neighbor.in_index = (*rxPhyIterator)->GetDevice()->GetObject<SimpleNanoDevice>()->index;
				neighbor.id = (*rxPhyIterator)->GetDevice()->GetNode()->GetId();
				if ((*rxPhyIterator)->GetDevice()->GetObject<SimpleNanoDevice>()->m_type == SimpleNanoDevice::NanoNode)			//将邻居节点进行分类
					neighbor.type = 0;
				else if ((*rxPhyIterator)->GetDevice()->GetObject<SimpleNanoDevice>()->m_type == SimpleNanoDevice::NanoRouter)
					neighbor.type = 1;
				else if ((*rxPhyIterator)->GetDevice()->GetObject<SimpleNanoDevice>()->m_type == SimpleNanoDevice::NanoInterface)
					neighbor.type = 2;

				neighbors.push_back(neighbor);
			}
		}
	}

	return neighbors;
}

std::vector<std::pair<uint32_t, uint32_t> > NanoSpectrumChannel::GetNeighbors(Ptr<SpectrumPhy> phy) {				//获取的邻居节点类型包括节点id与节点类型
	NS_LOG_FUNCTION(this);
	std::vector<std::pair<uint32_t, uint32_t> > neighbors;

	Ptr<MobilityModel> senderMobility = phy->GetMobility();
	double txRange = phy->GetObject<NanoSpectrumPhy>()->GetTransmissionRange();

	for (PhyList::const_iterator rxPhyIterator = m_phyList.begin(); rxPhyIterator != m_phyList.end(); ++rxPhyIterator) {
		if ((*rxPhyIterator) != phy) {
			Ptr<MobilityModel> receiverMobility = (*rxPhyIterator)->GetMobility();
			double distance = (receiverMobility->GetDistanceFrom(senderMobility));
			NS_LOG_FUNCTION(this << "check txrange" << distance << txRange);
			if (distance <= txRange) {
				std::pair<uint32_t, uint32_t> neighbor;
				neighbor.first = (*rxPhyIterator)->GetDevice()->GetNode()->GetId();
				if ((*rxPhyIterator)->GetDevice()->GetObject<SimpleNanoDevice>()->m_type == SimpleNanoDevice::NanoNode)
					neighbor.second = 1;
				else if ((*rxPhyIterator)->GetDevice()->GetObject<SimpleNanoDevice>()->m_type == SimpleNanoDevice::NanoRouter)
					neighbor.second = 2;
				else if ((*rxPhyIterator)->GetDevice()->GetObject<SimpleNanoDevice>()->m_type == SimpleNanoDevice::NanoInterface)
					neighbor.second = 3;
				else {

				}
				neighbors.push_back(neighbor);
			}
		}
	}

	return neighbors;
}

} // namespace ns3
