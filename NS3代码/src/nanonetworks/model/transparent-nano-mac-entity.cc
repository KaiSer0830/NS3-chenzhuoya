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


#include "transparent-nano-mac-entity.h"
#include <ns3/log.h>
#include <ns3/pointer.h>
#include <ns3/packet.h>
#include "simple-nano-device.h"
#include "nano-mac-queue.h"
#include "nano-spectrum-phy.h"
#include "nano-mac-header.h"
#include "ns3/seq-ts-header.h"
#include "ns3/simulator.h"

NS_LOG_COMPONENT_DEFINE ("TransparentNanoMacEntity");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (TransparentNanoMacEntity);

TypeId TransparentNanoMacEntity::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TransparentNanoMacEntity")
    .SetParent<NanoMacEntity> ();
  return tid;
}


TransparentNanoMacEntity::TransparentNanoMacEntity ()
{
  SetMacQueue (CreateObject <NanoMacQueue> ());
  SetDevice (0);
  Simulator::Schedule (Seconds(0.001), &NanoMacEntity::CheckForNeighbors, this);
}


TransparentNanoMacEntity::~TransparentNanoMacEntity ()
{
}

void TransparentNanoMacEntity::DoDispose (void)
{
  NanoMacEntity::DoDispose ();
}

void TransparentNanoMacEntity::DoSendPacket ()
{
  if (m_queue.size () > 0)
    {
		NS_LOG_FUNCTION (this);
		Ptr<NanoSpectrumPhy> phy = GetDevice ()->GetPhy ();

		Ptr<Packet> p = (m_queue.front ())->Copy ();				//从m_queue队列中复制第一个数据包
		m_queue.pop_front ();										//m_queue队列中删除第一个数据包
		phy->StartTx (p);

		if (m_queue.size () > 0)
		{
			double txtime = phy->GetTxDuration (p);					//GetTxDuration在ts-ook-based-nano-spectrum-phy.cc实现，为发送一个数据包的总时间
			Simulator::Schedule (FemtoSeconds (txtime), &TransparentNanoMacEntity::DoSendPacket, this);
		}
    }
}


void TransparentNanoMacEntity::Send (Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this << p);

	NanoMacHeader header;
	uint32_t src = GetDevice ()->GetNode ()->GetId ();
	uint32_t dst = 0;
	header.SetSource (src);
	header.SetDestination (dst);

	NS_LOG_FUNCTION (this << "mac header" << header);
	p->AddHeader (header);

	m_queue.push_back (p);

	if (m_queue.size () == 1)
		DoSendPacket ();
}

void TransparentNanoMacEntity::Send (Ptr<Packet> p, uint32_t dst)
{
	NS_LOG_FUNCTION (this << p);

	NanoMacHeader header;
	uint32_t src = GetDevice ()->GetNode ()->GetId ();
	header.SetSource (src);
	header.SetDestination (dst);

	NS_LOG_FUNCTION (this << "mac header" << header);
	p->AddHeader (header);
	GetDevice()->ConsumeEnergySend(GetDevice()->GetPacketSize());				//节点消耗发送数据包的能量
	m_queue.push_back (p);

	DoSendPacket ();
}

void TransparentNanoMacEntity::Receive (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
