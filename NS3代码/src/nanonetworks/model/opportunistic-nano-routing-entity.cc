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


#include "opportunistic-nano-routing-entity.h"
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
#include <stdio.h>
#include <stdlib.h>




NS_LOG_COMPONENT_DEFINE ("OpportunisticNanoRoutingEntity");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (OpportunisticNanoRoutingEntity);

TypeId OpportunisticNanoRoutingEntity::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::OpportunisticNanoRoutingEntity").SetParent<Object> ();
  return tid;
}


OpportunisticNanoRoutingEntity::OpportunisticNanoRoutingEntity ()
{
  SetDevice(0);
  m_receivedPacketListDim = 20;
  for (int i = 0; i < m_receivedPacketListDim; i++)
	{
	  m_receivedPacketList.push_back (9999999);
	}

//  m_sentPacketListDim = 20;
//  for (int i = 0; i < m_sentPacketListDim; i++)
//    {
//	  std::pair<uint32_t, uint32_t> item;
//	  item.first = 9999999;
//	  item.second = 9999999;
//	  m_sentPacketList.push_back (item);
//    }
}


OpportunisticNanoRoutingEntity::~OpportunisticNanoRoutingEntity ()
{
  SetDevice(0);
}

void  OpportunisticNanoRoutingEntity::DoDispose (void)
{
  SetDevice (0);
}

void OpportunisticNanoRoutingEntity::SendPacket (Ptr<Packet> p)					//从MessageProcessUnit::CreteMessage中的m_device->SendPacket (p)进入
{
	NanoSeqTsHeader seqTs;					//数据包添加序列头
	p->RemoveHeader (seqTs);
	NanoL3Header header;					//数据包添加路由头,header的size为24字节
	uint32_t src = GetDevice ()->GetNode ()->GetId ();
	uint32_t id = seqTs.GetSeq ();
	uint32_t ttl = 100;
	uint32_t l3dst = 999;
	header.Setmobility(GetDevice ()->Getmobility());
	header.Setindex(0);
	header.SetSource (src);
	header.SetDestination (l3dst);
	header.SetTtl (ttl);
	header.SetPacketId (id);
	p->AddHeader (seqTs);
	p->AddHeader (header);					//包的大小为138字节，Packet为102字节，seqTs为12字节，header为24字节
	//std::cout << p << " " << p->GetSize() << std::endl;
	UpdateReceivedPacketId (p->GetUid());
	Ptr<NanoMacEntity> mac = GetDevice ()->GetMac ();
	mac->sendpacket(p);
}

void OpportunisticNanoRoutingEntity::SendtestPacket (Ptr<Packet> p)		//目的节点发送探测包,数据包添加序列头和路由头
{
	NanoL3Header header;
	NanoSeqTsHeader seqTs;
	//std::cout << "p:" << p->GetUid() << " "  << " " << p->GetSize() << " " << "seqTs2:" << seqTs << std::endl;
	p->RemoveHeader(seqTs);				//将Packet中的原seqTs数据转移到新创建的header
	//std::cout << "p:" << p->GetUid() << " " << " " << p->GetSize() << " " << "seqTs3:" << seqTs << std::endl;
	SenderTypeTag tag;
	tag.type = 4;				//在nano-routing-entity.h中定义，大小为4字节，为4代表为探测数据包
	p->AddPacketTag(tag);
	uint32_t id = seqTs.GetSeq();
	static uint32_t indexy = 0;
	GetDevice()->GetMac()->CheckForNeighborss();
	if (GetDevice()->GetMac()->m_neighborss.size() != 0) {				//判断网关节点周围是否有其它的节点
		//std::cout<<GetDevice ()->GetMac ()->m_neighborss.size()<<std::endl;
		indexy++;						//发射的探测数据包indexy周期性递增
		uint32_t ttl = 1;
		uint32_t src = 0;				//探测数据包由网关节点发出，所有源节点id为0
		uint32_t l3dst = 999;
		header.SetTtl(ttl);
		header.SetPacketId(id);
		header.SetSource(src);
		header.SetDestination(l3dst);
		header.Setmobility(0);
		header.Setindex(indexy);
		p->AddHeader(seqTs);
		p->AddHeader(header);
		Ptr<NanoMacEntity> mac = GetDevice()->GetMac();
		//std::cout << "indexy:" << indexy << " " << "GetDevice()->GetNode()->GetId(): " << GetDevice()->GetNode()->GetId() << " " << "p->GetSize(): " << p->GetSize() << std::endl;
		mac->sendtestpacket(p);
	}
}

void OpportunisticNanoRoutingEntity::ReceivePacket (Ptr<Packet> p)				//从ts-ook-based-nano-spectrum-phy.cc中的GetL3 ()->ReceivePacket (p)跳转过来的
{
	NanoL3Header header;
	NanoMacHeader Header;
	SenderTypeTag tag;
	p->RemovePacketTag(tag);
	p->RemoveHeader(Header);		//移除mac头Packet大小减8字节
	p->RemoveHeader(header);		//移除路由头Packet大小减24字节
	uint32_t from = Header.GetSource();
	bool alreadyReceived = CheckAmongReceivedPacket(p->GetUid());			//判断最近是否接收过该数据包
	if (!alreadyReceived) {
		UpdateReceivedPacketId(p->GetUid());				//接收过的数据包id放入队列，防止重复接收
		if (from != 0 && GetDevice()->m_type == SimpleNanoDevice::NanoInterface)		//接收节点为网关节点，并且发送节点不是网关节点
		{
			p->AddHeader(header);
			GetDevice()->GetMessageProcessUnit()->ProcessMessage(p);					//网关节点处理数据包
		} else if (from != 0 && tag.type == 1)		//发送节点是路由节点，并且发送的数据包是要给路由或网关节点
		{
			GetDevice()->ConsumeEnergyReceive(GetDevice()->GetPacketSize());
			p->AddHeader(header);
			Forward(p);
		} else if (from != 0 && tag.type == 2 && (header.Getmobility() < GetDevice()->Getmobility()))		//发送节点是纳米节点或者路由节点，并且发送的数据包是要给移动性优于本节点的邻居节点
		{
			GetDevice()->ConsumeEnergyReceive(GetDevice()->GetPacketSize());
			p->AddHeader(header);
			Forward(p);
		} else if (from != 0 && tag.type == 3) {				//发送节点是纳米节点或者路由节点，并且发送的数据包是要发送给移动性大于0的邻居节点
			GetDevice()->ConsumeEnergyReceive(GetDevice()->GetPacketSize());
			p->AddHeader(header);
			Forward(p);
		} else if (from == 0 && GetDevice()->m_type == SimpleNanoDevice::NanoNode) {			//接收index更新数据包，发送节点是网关节点，并且接收数据包的节点是纳米节点tag.type == 4
			//根据周期更新index
			GetDevice()->ConsumeEnergyReceive(GetDevice()->GetTestSize());				//接收网关探测包也是消耗接收50字节的能量，实际探测包为100+字节!!!!!!!!!!!!!!!!
			GetDevice()->index = header.Getindex();
		}
	}
}

void OpportunisticNanoRoutingEntity::Forward (Ptr<Packet> p)
{
	NanoL3Header header;
	p->RemoveHeader(header);
	uint32_t ttl = header.GetTtl();
	if (ttl > 0) {
		ttl = ttl - 1;
		header.Setmobility(GetDevice()->Getmobility());
		header.SetTtl(ttl);
		p->AddHeader(header);
		Ptr<NanoMacEntity> mac = GetDevice()->GetMac();
		mac->forward(p);
	}
}

void OpportunisticNanoRoutingEntity::SetReceivedPacketListDim (int m)
{
	NS_LOG_FUNCTION (this);
	m_receivedPacketListDim = m;
}

//void OpportunisticNanoRoutingEntity::UpdateSentPacketId (uint32_t id, uint32_t nextHop)
//{
//  NS_LOG_FUNCTION (this << id << nextHop);
//  m_sentPacketList.pop_front ();
//  std::pair<uint32_t, uint32_t> item;
//  item.first = id;
//  item.second = 0;
//  m_sentPacketList.push_back (item);
//}

void OpportunisticNanoRoutingEntity::UpdateReceivedPacketId (uint32_t id)
{
	NS_LOG_FUNCTION (this);
	m_receivedPacketList.pop_front ();
	m_receivedPacketList.push_back (id);
}

bool OpportunisticNanoRoutingEntity::CheckAmongReceivedPacket (uint32_t id)
{
  NS_LOG_FUNCTION (this);
  for (std::list<uint32_t>::iterator it = m_receivedPacketList.begin(); it != m_receivedPacketList.end (); it++)
    {
	  NS_LOG_FUNCTION (this << *it << id);
	  if (*it == id) return true;
    }
  return false;
}


//void OpportunisticNanoRoutingEntity::SetSentPacketListDim (int m)
//{
//  NS_LOG_FUNCTION (this);
//  m_sentPacketListDim = m;
//}
} // namespace ns3
