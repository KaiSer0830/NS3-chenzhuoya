/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013,2014 TELEMATICS LAB, DEE - Politecnico di Bari
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



#include "backoff-based-nano-mac-entity.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/packet.h"
#include "simple-nano-device.h"
#include "nano-mac-queue.h"
#include "nano-spectrum-phy.h"
#include "nano-mac-header.h"
#include "ns3/seq-ts-header.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("BackoffBasedNanoMacEntity");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (BackoffBasedNanoMacEntity);

TypeId BackoffBasedNanoMacEntity::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BackoffBasedNanoMacEntity")
    .SetParent<NanoMacEntity> ();
  return tid;
}


BackoffBasedNanoMacEntity::BackoffBasedNanoMacEntity ()
{
  SetMacQueue (CreateObject <NanoMacQueue> ());
  SetDevice (0);
  Simulator::Schedule (Seconds(0.001), &NanoMacEntity::CheckForNeighbors, this);
}


BackoffBasedNanoMacEntity::~BackoffBasedNanoMacEntity ()
{
}

void BackoffBasedNanoMacEntity::DoDispose (void)
{
  NanoMacEntity::DoDispose ();
}

void BackoffBasedNanoMacEntity::DoSendPacket ()
{
	NS_LOG_FUNCTION(this);
	Ptr<NanoSpectrumPhy> phy = GetDevice()->GetPhy();

	NS_LOG_FUNCTION(this << "m_neighbors.size ()" << m_neighbors.size ());

	if (m_neighbors.size()) {
		srand(time(NULL));
		Ptr<Packet> p = (m_queue.front())->Copy();		//从m_queue队列中复制第一个数据包
		m_queue.pop_front();								//m_queue队列中删除第一个数据包

		NanoMacHeader header;
		p->RemoveHeader(header);
		uint32_t dst = header.GetDestination();
		NS_LOG_FUNCTION(this << "check dst" << header);

		if (dst == GetDevice()->GetNode()->GetId()) {			//如果目的地是自己的节点id，更换目的地
			NS_LOG_FUNCTION(this << "select new dst");
			int i = rand() % m_neighbors.size();
			dst = m_neighbors.at(i).first;
			header.SetDestination(dst);
		}
		p->AddHeader(header);
		NS_LOG_FUNCTION(this << "new header" << header);
		phy->StartTx(p);

		if (m_queue.size () > 0)			//如果m_queue队列发送后队列还有元素，则进行随机退避时间等待再发送
		{
			double backoff = rand () % 100000;
			Simulator::Schedule (PicoSeconds (backoff), &BackoffBasedNanoMacEntity::DoSendPacket, this);
		}
    }
	else if (m_queue.size () > 0)
    {
		double backoff = rand () % 10;
		NS_LOG_FUNCTION (this << "try again after (ms)" << backoff);
		Simulator::Schedule (MilliSeconds (backoff), &BackoffBasedNanoMacEntity::DoSendPacket, this);
    }
	else
    {}
}

void BackoffBasedNanoMacEntity::Send (Ptr<Packet> p)
{
	NS_LOG_FUNCTION(this << p);

	NanoMacHeader header;
	uint32_t src = GetDevice()->GetNode()->GetId();
	uint32_t dst = 0;
	header.SetSource(src);
	header.SetDestination(dst);

	NS_LOG_FUNCTION(this << "mac header" << header);
	p->AddHeader(header);

	m_queue.push_back(p);

	if (m_queue.size() == 1) {
		Simulator::Schedule(Seconds(0.0), &BackoffBasedNanoMacEntity::DoSendPacket, this);
	}
}

void BackoffBasedNanoMacEntity::Send (Ptr<Packet> p, uint32_t dst)
{
	NS_LOG_FUNCTION(this << p << dst);

	NanoMacHeader header;
	uint32_t src = GetDevice()->GetNode()->GetId();
	header.SetSource(src);
	header.SetDestination(dst);

	NS_LOG_FUNCTION(this << "mac header" << header);
	p->AddHeader(header);
	GetDevice()->ConsumeEnergySend(GetDevice()->GetPacketSize());				//节点消耗发送数据包的能量
	m_queue.push_back(p);

	if (m_queue.size() == 1) {
		Simulator::Schedule(Seconds(0.0), &BackoffBasedNanoMacEntity::DoSendPacket, this);
	}
}

void BackoffBasedNanoMacEntity::Receive (Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this);
}

} // namespace ns3
