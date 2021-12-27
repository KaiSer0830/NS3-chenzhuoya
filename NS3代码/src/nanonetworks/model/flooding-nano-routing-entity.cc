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


#include "flooding-nano-routing-entity.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/packet.h"
#include "simple-nano-device.h"
#include "nano-mac-queue.h"
#include "nano-l3-header.h"
#include "nano-mac-entity.h"
#include "ns3/log.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/enum.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/pointer.h"
#include "ns3/channel.h"
#include "simple-nano-device.h"
#include "nano-spectrum-phy.h"
#include "nano-mac-entity.h"
#include "nano-mac-header.h"
#include "nano-seq-ts-header.h"
#include "ns3/simulator.h"
#include "nano-routing-entity.h"
#include "message-process-unit.h"


NS_LOG_COMPONENT_DEFINE ("FloodingNanoRoutingEntity");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (FloodingNanoRoutingEntity);

TypeId FloodingNanoRoutingEntity::GetTypeId(void) {
	static TypeId tid = TypeId("ns3::FloodingNanoRoutingEntity").SetParent<Object>();
	return tid;
}


FloodingNanoRoutingEntity::FloodingNanoRoutingEntity ()
{
	SetDevice(0);
	m_receivedPacketListDim = 20;
	for (int i = 0; i < m_receivedPacketListDim; i++)
	{
		m_receivedPacketList.push_back (9999999);
	}
}


FloodingNanoRoutingEntity::~FloodingNanoRoutingEntity ()
{
	SetDevice(0);
}

void  FloodingNanoRoutingEntity::DoDispose (void)
{
	SetDevice (0);
}

void FloodingNanoRoutingEntity::SendPacket (Ptr<Packet> p)
{
	NS_LOG_FUNCTION(this << p << "size" << p->GetSize ());

	NanoSeqTsHeader seqTs;
	p->RemoveHeader(seqTs);

	NS_LOG_FUNCTION(this << p << "size" << p->GetSize () << seqTs);

	NanoL3Header header;
	uint32_t src = GetDevice()->GetNode()->GetId();
	uint32_t dst = 0;						//目的地固定为0，即网关节点
	uint32_t id = seqTs.GetSeq();
	uint32_t ttl = 100;
	header.SetSource(src);
	header.SetDestination(dst);
	header.SetTtl(ttl);
	header.SetPacketId(id);
	NS_LOG_FUNCTION(this << "l3 header" << header);

	p->AddHeader(seqTs);
	p->AddHeader(header);
	NS_LOG_FUNCTION(this << p << "size" << p->GetSize ());

	UpdateReceivedPacketId(id);				//自己发送的自己不需要接收，也放入接收过的数据包队列

	SenderTypeTag tag;

	if (GetDevice()->m_type == SimpleNanoDevice::NanoNode) {
		tag.type = 1;
	} else if (GetDevice()->m_type == SimpleNanoDevice::NanoRouter) {
		tag.type = 2;
	}

	p->AddPacketTag(tag);
	NS_LOG_FUNCTION(this << p << "size" << p->GetSize ());
	Ptr<NanoMacEntity> mac = GetDevice()->GetMac();
	mac->Send(p, dst);
}

void FloodingNanoRoutingEntity::ReceivePacket (Ptr<Packet> pkt)
{
	NS_LOG_FUNCTION(this << pkt << "size" << pkt->GetSize());

	Ptr<Packet> p = pkt->Copy();

	NanoMacHeader macHeader;
	p->RemoveHeader(macHeader);
	SenderTypeTag tag;
	p->RemovePacketTag(tag);
	NanoL3Header l3Header;
	p->RemoveHeader(l3Header);
	NS_LOG_FUNCTION(this << macHeader);
	NS_LOG_FUNCTION(this << l3Header);

	uint32_t id = l3Header.GetPacketId();
	bool alreadyReceived = CheckAmongReceivedPacket(id);		//根据数据包的id判断该数据包是否已经被接收过
	UpdateReceivedPacketId(id);					//这一句最好放在if (!alreadyReceived)里面

	SimpleNanoDevice::NodeType type = GetDevice()->m_type;

	if (!alreadyReceived) {
		NS_LOG_FUNCTION(this << "received a packet for the first time");
		if (GetDevice()->GetMessageProcessUnit() && type == SimpleNanoDevice::NanoInterface)	//如果节点有消息处理单元并且是纳米网关节点，则传输结束，进行消息处理
		{
			NS_LOG_FUNCTION(this << GetDevice()->GetNode ()->GetId () << l3Header.GetSource () << l3Header.GetDestination () << "FOR ME");
			p->AddHeader(l3Header);
			GetDevice()->GetMessageProcessUnit()->ProcessMessage(p);
		} else			//如果不是纳米网关节点
		{
			NS_LOG_FUNCTION(this << GetDevice()->GetNode ()->GetId () << l3Header.GetSource () << l3Header.GetDestination () << "NOT FOR ME");
			p->AddHeader(l3Header);

			if (tag.type == 1)			//接收来自纳米节点的数据包，进行数据包转发
			{
				NS_LOG_FUNCTION(this << "received from a sensor --> forward");
				GetDevice()->ConsumeEnergyReceive(GetDevice()->GetPacketSize());
				ForwardPacket(p);
			} else if (tag.type == 2 && type == SimpleNanoDevice::NanoRouter)		//接收来自路由节点的数据包并且自己也是路由节点，进行数据包转发
			{
				NS_LOG_FUNCTION(this << "received from a router, i'm a router --> forward");
				GetDevice()->ConsumeEnergyReceive(GetDevice()->GetPacketSize());
				ForwardPacket(p);
			} else {
				NS_LOG_FUNCTION(this << "do nothing!");
			}
		}
	} else {
		NS_LOG_FUNCTION(this << "packet already received in the past");
	}
}

void FloodingNanoRoutingEntity::ForwardPacket (Ptr<Packet> p)
{
	NS_LOG_FUNCTION(this << p << "size" << p->GetSize ());

	NanoL3Header l3Header;
	p->RemoveHeader(l3Header);
	NS_LOG_FUNCTION(this << l3Header);

	uint32_t dst = 0;
	uint32_t ttl = l3Header.GetTtl();
	if (ttl > 1) {
		if (GetDevice()->m_type == SimpleNanoDevice::NanoNode)		//如果自己是纳米节点，ttl-1,继续进行数据包转发
		{
			l3Header.SetTtl(ttl - 1);
		} else if (GetDevice()->m_type == SimpleNanoDevice::NanoRouter)			//如果自己是路由节点，ttl控制在10以内
		{
			if ((ttl - 1) > 10)
				l3Header.SetTtl(10);
			else
				l3Header.SetTtl(ttl - 1);
		}

		NS_LOG_FUNCTION(this << "new l3 header" << l3Header);
		p->AddHeader(l3Header);

		SenderTypeTag tag;
		if (GetDevice()->m_type == SimpleNanoDevice::NanoNode)		//重新设置数据包的类型
		{
			tag.type = 1;
		} else if (GetDevice()->m_type == SimpleNanoDevice::NanoRouter) {
			tag.type = 2;
		}
		p->AddPacketTag(tag);
		Ptr<NanoMacEntity> mac = GetDevice()->GetMac();
		mac->Send(p, dst);
	} else {
		NS_LOG_FUNCTION(this << "ttl expired");
	}
}

void FloodingNanoRoutingEntity::UpdateReceivedPacketId (uint32_t id)			//弹出一个最早的数据，push一个新数据
{
  NS_LOG_FUNCTION (this);
  m_receivedPacketList.pop_front ();				//pop一个又push一个，保持m_receivedPacketList的大小为20
  m_receivedPacketList.push_back (id);
}

bool FloodingNanoRoutingEntity::CheckAmongReceivedPacket (uint32_t id)
{
	NS_LOG_FUNCTION (this);
	for (std::list<uint32_t>::iterator it = m_receivedPacketList.begin(); it != m_receivedPacketList.end (); it++)
	{
		NS_LOG_FUNCTION (this << *it << id);
		if (*it == id) return true;
	}
	return false;
}

void FloodingNanoRoutingEntity::SetReceivedPacketListDim (int m)
{
	NS_LOG_FUNCTION (this);
	m_receivedPacketListDim = m;
}

} // namespace ns3
